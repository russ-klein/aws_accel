#ifndef __AC_COVERGROUP_H__
#define __AC_COVERGROUP_H__

#ifndef __cplusplus
#error C++ is required to include this header file
#endif

// if QuestaSim enhanced datatype viewing is enabled, include ac_int to get proper namespace scoping
#ifdef SC_INCLUDE_MTI_AC
#include <ac_int.h>
#endif

#if defined(_WIN32)
 #if !defined(_MSC_VER)
  #warning Call-stack tracking and end-of-execution cover() summary is not available on non-Microsoft Compilers
 #else
  #if (_MSC_VER < 1400) || (NTDDI_VERSION < NTDDI_WS03)
   #pragma message("Call-stack tracking and end-of-execution cover() summary requires Visual Studio 8 or newer")
  #else
   #define AC_COVERGROUP_USES_STACK
  #endif
 #endif
#else
 #define AC_COVERGROUP_USES_STACK
#endif

#ifdef AC_COVERGROUP_USES_STACK
#if defined(_WIN32)
#include <windows.h>
#include <WinNT.h>
#else
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bfd.h>
#endif
#endif

#include <stdio.h>

#ifndef __SYNTHESIS__
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <stdarg.h>
#endif

#ifdef __AC_NAMESPACE
namespace __AC_NAMESPACE {
#endif

namespace ac {
  #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)

  // Function: build_stack_key
  // Returns a string built from the concatenation of the call stack addresses
  inline void build_stack_key(std::stringstream &key, const int starting_frame=0)
  {
      #ifdef AC_COVERGROUP_USES_STACK
      void *buffer[100]; // max stack size
      char ptrstr[17];
      #if defined(_WIN32)
      unsigned short nptrs = CaptureStackBackTrace(0,100,buffer,NULL);
      #else
      int nptrs = backtrace(buffer,100)-1; // always drop outermost frame
      #endif
      if (nptrs > 0) {
        key << " STACK[";
        for (int j=starting_frame;j<nptrs;j++) { sprintf(ptrstr,"%016p",(void*)buffer[j]); key << std::string(ptrstr); if (j<nptrs-1) key << ",";}
        key << "]:";
      }
      #endif
  }

#ifdef AC_COVERGROUP_USES_BFD
  // Using BFD for locating file/line info
  static bfd* abfd = 0;
  static asymbol **syms = 0;
  static asection *text = 0;
  static void addr2fileLineFunc(char *address, std::stringstream &info) {
    info.clear();
    #if defined(_WIN32)
    return;
    #else
    if (!abfd) {
      char ename[1024];
      int l = readlink("/proc/self/exe",ename,sizeof(ename));
      if (l == -1) {
        // could not locate path to executable
        return;
      }
      ename[l] = 0;
      bfd_init();
      abfd = bfd_openr(ename, 0);
      if (!abfd) {
        // could not open binary
        return;
      }
      bfd_check_format(abfd,bfd_object);
      unsigned storage_needed = bfd_get_symtab_upper_bound(abfd);
      syms = (asymbol**)malloc(storage_needed);
      unsigned cSymbols = bfd_canonicalize_symtab(abfd, syms);
      text = bfd_get_section_by_name(abfd, ".text");
    }
    void *p=0;
    long offset = strtol(address,NULL,16) - text->vma;
    if (offset > 0) {
      const char *file;
      const char *func;
      unsigned line;
      if (bfd_find_nearest_line(abfd, text, syms, offset, &file, &func, &line) && file) {
        info << file << ":" << line;
//        info << " " << func;
      }
    }
    #endif
  }
#endif

  // Encapsulate up to 2 coverpoints in a covergroup with cross
  class covergroup
  {
  public:
    covergroup(const char *cgname, const char *cp1name, unsigned cp1size, const char *cp2name, unsigned cp2size, const char *stack)
      : d_cgname(cgname)
      , d_cp1name(cp1name)
      , d_cp1size(cp1size)
      , d_cp2name(cp2name)
      , d_cp2size(cp2size)
      , d_stack(stack) {}
    ~covergroup() {}
    void print_stack(unsigned indent=0) {
      char fmt[30]; // buffer for format string
      #ifdef AC_COVERGROUP_USES_BFD
      char address[30];
      #endif
      sprintf(fmt,"%%%dsAddr: %%s",indent);
      size_t colon_s = d_stack.find("STACK[")+6;
      size_t colon_e = d_stack.find(",",colon_s);
      while (colon_e != std::string::npos) {
        printf(fmt,"",d_stack.substr(colon_s,colon_e-colon_s).c_str());
        #ifdef AC_COVERGROUP_USES_BFD
        // Using BFD (when we can find libbfd.a to link against)
        sprintf(address,"%s",d_stack.substr(colon_s,colon_e-colon_s).c_str());
        std::stringstream details;
        addr2fileLineFunc(address, details);
        printf(" %s",details.str().c_str());
        #endif
        printf("\n");
        colon_s = colon_e+1;
        colon_e = d_stack.find(",",colon_s);
      }
    }
    void report() {
      printf("\n");
      printf("  covergroup '%s'\n", d_cgname.c_str());
      printf("    Call Stack:\n");
      print_stack(6);
      printf("    coverpoint '%s'\n", d_cp1name.c_str());
      printf("      %9s %4s   %s\n", "hit count","","bin");
      printf("      %9s %4s   %s\n", "---------","","---");
      for (std::map<std::string,int>::iterator bpos = d_coverpoint1.begin(); bpos!=d_coverpoint1.end(); ++bpos) {
        printf("      %9d %4s   %s\n", (*bpos).second, (const char *)(((*bpos).second==0)?"MISS":""),(*bpos).first.c_str());
      }

      // only continue on if a second coverpoint exists
      if (d_cp2size==0) return;
      printf("    coverpoint '%s'\n", d_cp2name.c_str());
      printf("      %9s %4s   %s\n", "hit count","","bin");
      printf("      %9s %4s   %s\n", "---------","","---");
      for (std::map<std::string,int>::iterator bpos = d_coverpoint2.begin(); bpos!=d_coverpoint2.end(); ++bpos) {
        printf("      %9d %4s   %s\n", (*bpos).second, (const char *)(((*bpos).second==0)?"MISS":""),(*bpos).first.c_str());
      }
      unsigned *table = new unsigned[d_cp1size*d_cp2size];
      for (unsigned i=0;i<d_cp1size;i++)
        for (unsigned j=0;j<d_cp2size;j++)
          table[i*d_cp2size+j] = 0;

      for (std::map<std::string,int>::iterator bpos = d_cross.begin(); bpos!=d_cross.end(); ++bpos) {
        std::string binname((*bpos).first);
        size_t eqpos = 0;
        for (size_t p1 = 0; p1<d_cp1size; p1++) {
          for (size_t p2 = 0; p2<d_cp2size; p2++) {
            if ((binname[eqpos+p1]=='T') && (binname[eqpos+d_cp1size+p2]=='T')) table[p1*d_cp2size+p2]+=(*bpos).second;
          }
        }
      }
      printf("\n");
      printf("    cross %s,%s\n", d_cp1name.c_str(), d_cp2name.c_str());
      // print column headings - coverpoint2 bins across top, coverpoint1 bins down the side
      printf("      %15s %-15s\n"," ", d_cp2name.c_str());
      printf("      %-15s",d_cp1name.c_str());
      for (std::map<std::string,int>::iterator bpos = d_coverpoint2.begin(); bpos!=d_coverpoint2.end(); ++bpos) {
        printf(" %15s",(*bpos).first.c_str());
      }
      printf("\n");
      size_t startpos = 0;
      size_t endpos = d_binlist.find_first_of(":");
      for (unsigned i=0;i<d_cp1size;i++) {
        printf("      %-15s", d_binlist.substr(startpos,endpos-startpos).c_str());
        for (unsigned j=0;j<d_cp2size;j++) {
          if (table[i*d_cp2size+j]==0) {
            printf("  ** MISS ** %3d", table[i*d_cp2size+j]);
          } else {
            printf(" %15d", table[i*d_cp2size+j]);
          }
        }
        printf("\n");
        startpos = endpos+1;
        endpos = d_binlist.find(":",startpos);
      }
    }

    void cover2(unsigned cp1size, unsigned cp2size, va_list argp) {
      bool build_binlist = (d_binlist.length()==0);
      // string to keep track of cross values
      std::stringstream currentVal;
      unsigned cp1_true_count = 0;
      unsigned cp2_true_count = 0;

      // process coverpoint 1
      for (unsigned i=0; i<cp1size; i++) {
        const char *cp1_expr_str = va_arg(argp, const char *);
        //          if (strlen(cp1_expr_str)==0) {
        //            std::cerr << "Misuse of covergroup_cross2. Check the second and third parameters" << std::endl;
        //            return;
        //          }
        bool cp1_value = (bool)va_arg(argp, int);
        if (build_binlist) { d_binlist.append(cp1_expr_str); d_binlist.append(":"); }
        if (cp1_value) {currentVal << "T"; cp1_true_count++; }  // update cross value
        else currentVal << "F";

        std::map<std::string,int>::iterator cp1binpos = d_coverpoint1.find(cp1_expr_str);
        if (cp1binpos == d_coverpoint1.end()) {
          std::map<std::string,int> newcg;
          d_coverpoint1.insert(std::pair<std::string,int>(cp1_expr_str,(cp1_value?1:0)));
        } else {
          if (cp1_value) (*cp1binpos).second++; // increment count in entry
        }
      }

      // process coverpoint 2
      for (unsigned i=0; i<cp2size; i++) {
        const char *cp2_expr_str = va_arg(argp, const char *);
        //          if (strlen(cp2_expr_str)==0) {
        //            std::cerr << "Misuse of covergroup_cross2. Check the second and third parameters" << std::endl;
        //            return;
        //          }
        bool cp2_value = (bool)va_arg(argp, int);
        if (build_binlist) { d_binlist.append(cp2_expr_str); d_binlist.append(":"); }
        if (cp2_value) {currentVal << "T"; cp2_true_count++; }  // update cross value
        else currentVal << "F";

        std::map<std::string,int>::iterator cp2binpos = d_coverpoint2.find(cp2_expr_str);
        if (cp2binpos == d_coverpoint2.end()) {
          std::map<std::string,int> newcg;
          d_coverpoint2.insert(std::pair<std::string,int>(cp2_expr_str,(cp2_value?1:0)));
        } else {
          if (cp2_value) (*cp2binpos).second++; // increment count in entry
        }
      }

      // track the intersection of the two coverpoints
      if ((cp1_true_count>0) && (cp2_true_count>0)) {
        std::map<std::string,int>::iterator crosspos = d_cross.find(currentVal.str()/*expr_str*/);
        if (crosspos == d_cross.end()) {
          // create new cross entry
          d_cross.insert(std::pair<std::string,int>(currentVal.str()/*expr_str*/,1));
        } else {
          (*crosspos).second++; // increment count in entry
        }
      }
    }

  private:
    std::string d_cgname;
    std::string d_cp1name;
    unsigned d_cp1size;
    std::string d_cp2name;
    unsigned d_cp2size;
    std::string d_stack; // call stack of this occurrence of the covergroup
    std::string d_binlist; // populated during first call to cover2
    // maps for each coverpoint and for the cross
    std::map<std::string, int> d_coverpoint1; // bins for cp1
    std::map<std::string, int> d_coverpoint2; // bins for cp2
    std::map<std::string, int> d_cross; // sparse bins for cp1 x cp2
  };

  // class ac::ac_covergroup_mgr : public std::map<std::string,std::map<std::string,int> >
  class ac_covergroup_mgr : public std::map<std::string, ac::covergroup >
  {
  public:
    ac_covergroup_mgr() {}
    ~ac_covergroup_mgr() {
      if (!empty()) {
        // locate this executable's location for symbol lookup
        printf("Covergroup Report\n");
        for (std::map<std::string,ac::covergroup>::iterator cgpos=begin(); cgpos!=end(); ++cgpos) {
          (*cgpos).second.report();
        }
      } else {
        printf("WARNING - NO covergroup() CALLS EXECUTED\n");
      }
    }
    void cross2(const char *filename, unsigned lineno, const char *cgname, const char *cp1name, unsigned cp1size, const char *cp2name, unsigned cp2size, va_list argp) {
      // build signature of the requested cover-group (based on stack, filename and cgname)
      std::stringstream cgnamestr;
      cgnamestr << cgname << ":[" << cp1size << "," << cp2size << "]:"  << filename << ":" << lineno;
      std::stringstream stack;
      build_stack_key(stack,3); // 3
      cgnamestr << stack;
      // locate entry if it exists already
      std::map<std::string,ac::covergroup>::iterator cgpos = find(cgnamestr.str());
      if (cgpos == end()) {
        // does not exist, create a new entry for covergroup cross data
        ac::covergroup newcg(cgname,cp1name,cp1size,cp2name,cp2size,stack.str().c_str());
        insert(std::pair<std::string,ac::covergroup>(cgnamestr.str(),newcg));
      }
      // again get pointer to entry
      cgpos = find(cgnamestr.str());
      (*cgpos).second.cover2(cp1size,cp2size,argp);
    }
  };
#endif

  // Executable covergroup cross "ac_covergroup_cross2" (not synthesizable)
  inline void ac_covergroup_cross2(const char *filename, unsigned lineno, const char *cgname, const char *cp1name, unsigned cp1size, const char *cp2name, unsigned cp2size, ...)
  {
    #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)
    static ac::ac_covergroup_mgr s_cgc2m;
    va_list argp;
    va_start(argp,cp2size);
    s_cgc2m.cross2(filename,lineno,cgname,cp1name,cp1size,cp2name,cp2size,argp);
    va_end(argp);
    #endif // endif __SYNTHESIS__
  }

} // end namespace ac

#ifdef __AC_NAMESPACE
}
#endif

// COVERPOINT/COVERGROUP
#define covbin(x) #x,x
#define coverpoint(cgname,cp1name,cp1size,...)                        ac::ac_covergroup_cross2(__FILE__,__LINE__,#cgname,#cp1name,cp1size,"",0,__VA_ARGS__,"",false)
#define covergroup_cross2(cgname,cp1name,cp1size,cp2name,cp2size,...) ac::ac_covergroup_cross2(__FILE__,__LINE__,#cgname,#cp1name,cp1size,#cp2name,cp2size,__VA_ARGS__,"",false)

// special macro for ac_channel size coverage
#define cover_channel(scope,chname,available) coverpoint(scope,chname,3,covbin((chname.debug_size()==0)),covbin((chname.debug_size()==available)),covbin((chname.debug_size()>available)))

#endif // endif __AC_COVERGROUP_H__

