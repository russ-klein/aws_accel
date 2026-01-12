///////////////////////////////////////////////////////////////////////////////
// Filename : catI_stdmap.h
// Created : David Nguyen Van Mau
// Date : September 23 2025                                                //
// Description : 
// Modifications :                                                           //
// Copyright (c) Mentor Graphics Corporation 1999, All Right Reserved        //
///////////////////////////////////////////////////////////////////////////////

#ifndef _catI_stdmap_h_
#define _catI_stdmap_h_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <fstream>
////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#ifdef WIN32
  #ifdef WIN32_NO_DLL
    #define ABC_DLLEXPORT
    #define ABC_DLLIMPORT
  #else
    #define ABC_DLLEXPORT __declspec(dllexport)
    #define ABC_DLLIMPORT __declspec(dllimport)
  #endif
#else  /* defined(WIN32) */
#define ABC_DLLIMPORT
#endif /* defined(WIN32) */

#ifndef ABC_DLL
#define ABC_DLL ABC_DLLIMPORT
#endif
////////////////////////////////////////////////////////////////////////

class catI_stdport;
class catI_stdcell_1o;
class catI_pin;
class catI_conn; 
class catI_inst;

class ABC_DLL catI_stdport {

public:
  catI_stdport(char* name,float rdly,float fdly, float fan_rdly, float fan_fdly, int loadinput, int loadmax, int idx);
  ~catI_stdport();

  void print();

  
  std::string d_name;
  float d_rdly;
  float d_fdly;
  float d_rfandly;
  float d_ffandly;
  int   d_loadinput;
  int   d_loadmax;
  int   d_idx;
};

class ABC_DLL catI_stdcell_1o {
public : 
  catI_stdcell_1o();
  ~catI_stdcell_1o();

  void print();

  std::string d_name;
  std::string d_equation;
  float       d_area;
  catI_stdport* d_output;
  std::vector<catI_stdport*> d_inputs;
};

class ABC_DLL catI_stdlib {
public :
  catI_stdlib();
  ~catI_stdlib();

  catI_stdcell_1o* get_cell(std::string);
  void print();

  std::unordered_map<std::string,catI_stdcell_1o*> d_cells;
};

class ABC_DLL catI_pin {
public:
  catI_pin(catI_stdport* ref, catI_inst* owner);
  ~catI_pin();

  bool is_pi() const;
  bool is_po() const;

  void print();

  catI_conn*    d_conn;
  catI_stdport* d_ref;
  catI_inst*    d_owner;
};

class ABC_DLL catI_conn {
public :
  catI_conn(int idx);
  ~catI_conn();

  bool connect_src(catI_pin* pin);
  bool connect_dst(catI_pin* pin);

  void fill_dot(std::ofstream& s_out);

  catI_pin* d_src;
  int       d_idx;
  std::vector<catI_pin*> d_dsts;
};

class ABC_DLL catI_inst {
public :
  catI_inst(catI_stdcell_1o* ref, int idx);
  ~catI_inst();

  bool is_pi() const;
  bool is_po() const;

  void print();
  void fill_dot(std::ofstream& s_out);
  std::string dot_pin_name(int idx_pin);

  catI_stdcell_1o* d_ref;

  int d_idx; // index in the network it belongs
  std::vector<catI_pin*> d_ins;
  std::vector<catI_pin*> d_outs;
};

class ABC_DLL catI_mapped_ntk {
public :

  catI_mapped_ntk();
  ~catI_mapped_ntk();

  catI_inst* create_pi();
  catI_inst* create_po();
  catI_inst* create_inst(catI_stdcell_1o* ref);

  void print();

  void print_dot(const char* fname);

  std::vector<catI_conn*> d_connections;
  std::vector<catI_inst*> d_insts;
  std::vector<catI_inst*> d_pis;
  std::vector<catI_inst*> d_pos;
  catI_stdlib d_lib;
};

extern ABC_DLL const char* Cat_Abc_LoadLibrary(catI_stdlib& lib,std::unordered_set<std::string>* elts_to_load);
extern ABC_DLL const char* Cat_Abc_LoadNtkMiniMap(int * pArray, catI_stdcell_1o* lib,catI_mapped_ntk* mapped_ntk);
extern ABC_DLL const char* Cat_Abc_NtkPrintMiniMapping( int * pArray );
#endif
