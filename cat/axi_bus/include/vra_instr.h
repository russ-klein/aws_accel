////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
//
// Unpublished work. Copyright 2022 Siemens
//       All Rights Reserved
//
// This document contains information that is proprietary to Mentor Graphics
// Corp. The original recipient of this document may duplicate this
// document in whole or in part for internal business purposes only, provided
// that this entire notice appears in all copies. In duplicating any part of
// this document, the recipient agrees to make every reasonable effort to
// prevent the unauthorized use and distribution of the proprietary information.
//
////////////////////////////////////////////////////////////////////////////////

/*
//   Source:           vra_instr.h
//   Description:      Contains various classes and functions for VRA
//                     instrumentation, including the base class used to implement
//                     the instrumentation hooks.
//   Author:           Niramay Sanghvi
//
*/

//==================================================================================

#ifndef _INCLUDED_VRA_INSTR_H_
#define _INCLUDED_VRA_INSTR_H_

#if __cplusplus < 201103L
#error "The standard C++11 (201103L) or newer is required"
#endif

#include <iostream>
#include <fstream>
#include <ac_assert.h>
#include <string>

#include <cstdint>

#include <ac_stacktrace.h>
#include <math.h>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <regex>
#include <utility>
#include <ctime>
#include <stdarg.h>
#include <locale>
#include <clocale>
#include <sys/resource.h>

#define VRA_UNPROCESSED_PREFIX      "vra_unprocessed"
#define METADATA_CSV_FILE_PATH_KEY  "$target_output_csv"
#define METADATA_VRA_TYPE_KEY       "$type_analyzed"

#ifdef __SANITIZE_ADDRESS__
#warning VRA support for Address Sanitizer is still experimental. You might not get the output you expect.
#endif

enum ac_q_mode { AC_TRN, AC_RND, AC_TRN_ZERO, AC_RND_ZERO, AC_RND_INF, AC_RND_MIN_INF, AC_RND_CONV, AC_RND_CONV_ODD };
enum ac_o_mode { AC_WRAP, AC_SAT, AC_SAT_ZERO, AC_SAT_SYM };

// Provide a forward declaration of ac_int and ac_fixed to help facilitate the establisment
// of an interface.

template<int W, bool S> class ac_int;
template<int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2> class ac_fixed;

// Forward declaration of ac_q_mode, ac_omode  to facilitate the establishment of an interface.

// This specialization of std::hash uses the Murmur2 hash algorithm.
template <>
struct std::hash<std::vector<std::uintptr_t> > {
  typedef std::vector<std::uintptr_t> key_type;

  std::size_t operator() (const key_type &key) {
    constexpr int uip_dig = std::numeric_limits<std::uintptr_t>::digits;
    constexpr int st_dig = std::numeric_limits<std::size_t>::digits;
    constexpr bool uip_sign = std::numeric_limits<std::uintptr_t>::is_signed;
    constexpr bool st_sign = std::numeric_limits<std::size_t>::is_signed;

    static_assert((uip_dig == 32 || uip_dig == 64) && !uip_sign, "std::uintptr_t must be unsigned 32-bit or 64-bit.");
    static_assert(uip_dig == st_dig, "std::uintptr_t and size_t have different number of digits.");
    static_assert(uip_sign == st_sign, "std::uintptr_t and size_t have different signedness.");

    constexpr bool is_arch_32b = (uip_dig == 32);

    constexpr std::size_t m = is_arch_32b ? 0x5bd1e995ULL : 0xc6a4a7935bd1e995ULL;
    constexpr int r = is_arch_32b ? 24 : 47;

    const std::size_t len = key.size()*sizeof(std::size_t);
    std::size_t h = (len * m);

    for (const std::uintptr_t key_elem : key) {
      std::size_t k = reinterpret_cast<std::size_t>(key_elem);

      k *= m;
      k ^= k >> r;
      k *= m;
      if (is_arch_32b) {
        h *= m;
        h ^= k;
      } else {
        h ^= k;
        h *= m;
      }
    }

    if (is_arch_32b) {
      h ^= h >> 13;
      h *= m;
      h ^= h >> 15;
    } else {
      h ^= h >> r;
      h *= m;
      h ^= h >> r;
    }

    return h;
  }
};

namespace ac_vra_ns {
  inline std::vector<std::string> sep_string(const std::string &in, const char delim) {
    std::stringstream in_ss(in);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(in_ss, segment, delim)) {
      if (!segment.empty()) { seglist.push_back(segment); }
    }

    return seglist;
  }

  inline bool regex_matches(const std::vector<std::string> &filters, const std::string &target_str) {
    for (const std::string& filter : filters) {
      std::regex filter_regex(filter);
      if (std::regex_match(target_str, filter_regex)) { return true; }
    }
    return false;
  }

  inline std::string get_fname_from_stack(const std::string &stack_top) {
    std::size_t eidx = stack_top.find(':');
    std::string fname = stack_top.substr(0, eidx);
    return fname;
  }

  inline std::string get_apath_from_stack(const std::string &stack) {
    // Find the starting index of the path.
    std::size_t start_apath_idx = stack.find('/');
    // Find the ending index of the path.
    std::size_t end_apath_idx = stack.find(':', start_apath_idx + 1);
    std::string apath = stack.substr(start_apath_idx, end_apath_idx - start_apath_idx);
    return apath;
  }

  inline void get_filters(
    const std::string &filters_as_str,
    std::vector<std::string> &apath_filters,
    std::vector<std::string> &fname_filters
  ) {
    // Filters are semicolon separated.
    std::vector<std::string> filters_as_vec = sep_string(filters_as_str, ',');

    for (const std::string& filter : filters_as_vec) {
      if (filter.find("/") != std::string::npos) { apath_filters.push_back(filter); }
      else { fname_filters.push_back(filter); }
    }
  }

  inline std::string str_vec_to_str(const std::vector<std::string> &str_vec) {
    std::stringstream ss_obj;
    for (const std::string& str : str_vec) { ss_obj << "\"" << str << "\", "; }
    std::string out_str = ss_obj.str();
    // Remove the last ", " before returning the string.
    return out_str.substr(0, out_str.size() - 2);
  }

  inline void print_if_non_empty(const char* starting_msg, const std::string& str) {
    if (!str.empty()) {
      std::cout << starting_msg << str << std::endl;
    }
  }

  inline void print_if_non_empty(const char* starting_msg, const std::vector<std::string>& str_vec) {
    if (!str_vec.empty()) {
      std::cout << starting_msg << str_vec_to_str(str_vec) << std::endl;
    }
  }

  // Calculate number of bits required to represent double value as an integer.
  inline int calc_int_bits(const double x, bool threshold_check = false) {
    if (threshold_check) {
      constexpr int num_digits_double = std::numeric_limits<double>::digits;
      // Calculate thresholds beyond which you'll use the externally calculated max bits.
      static const double thresh_max = (1L << num_digits_double) - 1;
      static const double thresh_min = -thresh_max;

      if (x >= thresh_min && x <= thresh_max) {
        return 0;
      }
    }

    if (x <= 0.0 && x >= -1.0) { // x is in range [-1.0, 0.0]
      return 1;
    }

    if (x > 0.0 && x < 1.0) { // x is in range (0.0, 1.0)
      return 0;
    }

    double flr_x = floor(x);
    int flrx_exp;
    double flx_nmant = frexp(flr_x, &flrx_exp);
    bool flx_is_npo2 = (flx_nmant == -0.5);
    return (flrx_exp + int(x < 0.0) - int(flx_is_npo2));
  }

  // Calculate number of bits required to represent a fractional part. ac_fixed only.
  inline void calc_frac_bits(const double epsilon, const double d_min_frac, const double quanterr, double &min_frac, int &frac_bits, int &frac_bits_min) {
    min_frac = (d_min_frac < std::numeric_limits<double>::max()) ? d_min_frac : 0.0;

    // for fraction values close to 1, subtract it to get better resolution.
    // This may happen with variables that are assigned once, e.g. coefficients.
    if (min_frac > 0.5) { min_frac = 1.0 - min_frac; }

    double frac_remain = min_frac; // start with smallest fractional value seen

    // adjust smallest fractional value down if quanterr is smaller
    if ( (frac_remain==0.0) && (abs(quanterr)>0.0) && (abs(quanterr)<min_frac) ) {
      frac_remain = abs(quanterr);
    }

    if (frac_remain == 0.0) {
      frac_bits = 0;
      frac_bits_min = 0;
    } else if (frac_remain > epsilon) {
      frac_bits_min = (int)ceil(abs(log2(min_frac)));
      while (frac_remain > epsilon) {
        frac_bits = (int)ceil(abs(log2(frac_remain)));
        frac_remain = abs(frac_remain - double( 1.0 / double(pow(2,frac_bits)) ) );
      }
    } else {
      frac_bits_min = frac_bits = (int)ceil(abs(log2(epsilon)));
    }
  }

  enum type_modes {
    VRA_WITH_INT,
    VRA_WITH_FIXED
  };

  template <type_modes TYPE_MODE>
  void stream_decl_info (int int_bits, int frac_bits_min, std::string sign_str, std::stringstream &mdecl) {
    if (TYPE_MODE == VRA_WITH_INT) {
      if (int_bits == 0) {
        mdecl << "(use ac_fixed)";
        return ;
      }
      mdecl << "ac_int<" << int_bits << "," << sign_str << ">";
      return ;
    }
    mdecl << "ac_fixed<" << (int_bits + frac_bits_min) << "," << int_bits << ",";
    mdecl << sign_str << ",AC_TRN,AC_WRAP>";
  }

  template <type_modes TYPE_MODE>
  class ac_quant_info {
  public:
    typedef std::vector<std::uintptr_t> StraceType;
    typedef std::vector<std::string> StrVec;
    typedef std::pair<StrVec, StrVec> StrVecPair;

    ac_quant_info(const StraceType &stacktrace, const std::string &desc)
      : d_stacktrace(stacktrace)
      , d_desc(desc)
    {
      reset();
    }

    ac_quant_info() {
      reset();
    }

    ac_quant_info(const ac_quant_info &)                = default;
    ac_quant_info(ac_quant_info &&) noexcept            = default;
    ac_quant_info &operator=(ac_quant_info const &)     = default;
    ac_quant_info &operator=(ac_quant_info &&) noexcept = default;
    ~ac_quant_info() = default;

    bool was_changed() const { return (d_valcnt != 0); }

    bool get_enable() const { return d_enabled; }

    void set_enable(bool enable) { d_enabled = enable; }
    
    // Set enable to default value.
    void set_default_enable() {
      d_enabled = true;

      #ifdef AC_INT_VRA_FILTER_VARS
      if (TYPE_MODE == VRA_WITH_INT) { d_enabled = false; }
      #endif
      #ifdef AC_FIXED_VRA_FILTER_VARS
      if (TYPE_MODE == VRA_WITH_FIXED) { d_enabled = false; }
      #endif
    }

    void set_type_min_max(double type_min_val, double type_max_val) {
      d_type_min_val = type_min_val;
      d_type_max_val = type_max_val;
    }

    // INSTRUMENTATION FUNCTIONS

    // Forbid updates with another ac_quant_info object as input.
    void update(const ac_quant_info &dat) {
      assert(0);
    }

    void update(bool overflow, double op2, double quant, int int_bits, double frac_value = 0.0) {
      // Update counters.
      d_valcnt++;
      d_overflowcnt += int(overflow || isinf(op2));

      // Update min/max values.
      if (op2 < d_min) { d_min = op2; }
      if (op2 > d_max) { d_max = op2; }
      
      if (op2 < 0.0) {
        d_signed = true;
        if (int_bits > d_extern_m_ibits_s)  { d_extern_m_ibits_s  = int_bits; }
      } else {
        if (int_bits > d_extern_m_ibits_us) { d_extern_m_ibits_us = int_bits; }
      }

      double quanterr = op2 - quant;
      d_sum += quanterr;
      d_sum2 += quanterr*quanterr;

      if (isnan(op2)) { d_nan_obs = true; }

      if (TYPE_MODE == VRA_WITH_FIXED) {
        if ((frac_value > 0.0) && (frac_value < d_min_frac)) {
          d_min_frac = frac_value;
        }

        double abs_quanterr = abs(quanterr);
        if (abs_quanterr != 0.0) {
          if (d_quanterr == -1.0) { d_quanterr = abs_quanterr; }
          if (abs_quanterr < d_quanterr) { d_quanterr = abs_quanterr; } // keep smallest quant error
        }
      }
    }

    void merge(const ac_quant_info &other) {
      assert(d_enabled);
      d_valcnt += int(other.was_changed());
      d_sum += other.d_sum;
      d_sum2 += other.d_sum2;
      if (other.d_min < d_min) { d_min = other.d_min; }
      if (other.d_max > d_max) { d_max = other.d_max; }
      if (other.d_extern_m_ibits_s  > d_extern_m_ibits_s)  { d_extern_m_ibits_s  = other.d_extern_m_ibits_s; }
      if (other.d_extern_m_ibits_us > d_extern_m_ibits_us) { d_extern_m_ibits_us = other.d_extern_m_ibits_us; }
      d_signed = d_signed || other.d_signed;
      d_type_min_val = other.d_type_min_val;
      d_type_max_val = other.d_type_max_val;
      
      d_overflowcnt += other.d_overflowcnt;
      d_nan_obs = d_nan_obs || other.d_nan_obs;

      if (TYPE_MODE == VRA_WITH_FIXED) {
        if (other.d_min_frac < d_min_frac) { d_min_frac = other.d_min_frac; }
        if (d_quanterr == -1.0) d_quanterr = other.d_quanterr;
        if (d_quanterr > other.d_quanterr) d_quanterr = other.d_quanterr;
      }
    }

    void copy_merged_data(const ac_quant_info &other) {
      assert(d_enabled);
      d_min = other.d_min;
      d_max = other.d_max;
      d_extern_m_ibits_s = other.d_extern_m_ibits_s;
      d_extern_m_ibits_us = other.d_extern_m_ibits_us;
      d_signed = other.d_signed;
      d_overflowcnt = other.d_overflowcnt;
      d_type_min_val = other.d_type_min_val;
      d_type_max_val = other.d_type_max_val;
      d_valcnt = other.d_valcnt;
      d_sum = other.d_sum;
      d_sum2 = other.d_sum2;
      d_nan_obs = other.d_nan_obs;

      if (TYPE_MODE == VRA_WITH_FIXED) {
        d_min_frac = other.d_min_frac;
        d_quanterr = other.d_quanterr;
      }
      // The following will not be copied, as they don't need to change once set:
      // 1. d_stracktrace
      // 2. d_desc
      // Not copying these data members makes the algorithm faster and more efficient.
      // Copying the stacktrace in particular is a fairly expensive operation and
      // should be avoided as much as possible.
    }

    // REPORTING FUNCTIONS
    double rms_quant_err() const {
      double ret = d_sum < 0 ? -d_sum : d_sum;
      if (d_valcnt > 1) {
        double avg = d_sum/double(d_valcnt);
        // std::max is used to make sure that var is always non-negative.
        double var = std::max((d_sum2 - d_sum*avg)/double(d_valcnt - 1), 0.0);
        ret = sqrt(var);
      }
      return ret;
    }

    std::string report(const StrVecPair& filter_pair, double epsilon, bool flat = false, char delim = ',', bool d_write_to_intermediary_file = false) const {
      constexpr int num_digits_double = std::numeric_limits<double>::digits;
      // Calculate thresholds beyond which you'll use the externally calculated max bits.
      const double thresh_max = (1L << num_digits_double) - 1;
      const double thresh_min = -thresh_max;
      int int_bits_min = 0;
      if (d_min < 0.0) {
        if (d_min >= thresh_min) {
          int_bits_min = calc_int_bits(d_min);
        } else {
          int_bits_min = d_extern_m_ibits_s;
        }
      }

      int int_bits_max = 0;
      if (d_max >= 0.0) {
        if (d_max <= thresh_max) {
          int_bits_max = calc_int_bits(d_max);
        } else {
          int_bits_max = d_extern_m_ibits_us;
        }
        // Add extra bit to accommodate signed input, if seen.
        int_bits_max += int(d_signed);
      }

      int int_bits = std::max(int_bits_max, int_bits_min);

      double min_frac = 0.0, min_frac_printed = 0.0;
      int frac_bits = 0, frac_bits_min = 0;
      double quanterr = (d_quanterr==-1.0) ? 0.0 : d_quanterr;
      if (TYPE_MODE == VRA_WITH_FIXED) {
        calc_frac_bits(epsilon, d_min_frac, quanterr, min_frac, frac_bits, frac_bits_min);
        min_frac_printed = min_frac;
      }
      double obs_min_printed = d_min, obs_max_printed = d_max;

      std::string sign_str = d_signed ? "true" : "false";

      std::stringstream mdecl;
      mdecl.imbue(std::locale());

      if (!was_changed()) {
        mdecl << "(no value change)";
      } else if (isinf(d_min) || isinf(d_max) || d_nan_obs) {
        if (d_nan_obs) {
          // Observed minimum and maximum, as well as the minimal fractional part, are both set to NaN to make sure that that's
          // what gets printed in the VRA report.
          obs_min_printed = obs_max_printed = min_frac_printed = std::numeric_limits<double>::quiet_NaN();
        }
        mdecl << "(undefined value(s) seen)";
      } else {
        stream_decl_info<TYPE_MODE>(int_bits, frac_bits_min, sign_str, mdecl);
      }

      bool ignore_no_debug_info = false;
      std::string stack(ac_debug::format_stack_trace(d_stacktrace, flat, true, true, ignore_no_debug_info));

      std::size_t first_q = 0;
      while (first_q < stack.size() && stack.at(first_q) == ' ') {
        ++first_q; 
      }
      if (stack.at(first_q) == '\"') { // Skip quote if it exists
        ++first_q; 
      }
      std::size_t first_sp  = stack.find(" ", first_q);
      std::string stack_top = stack.substr(first_q, first_sp - first_q);

      // Filter out "internal" ac_int/ac_fixed variables, i.e. variables inside various AC Datatype and VRA headers, with
      // the exception of ac_complex.h, as that has variables which might be of interest.
      static const std::vector<std::string> excluded_files = {
        std::string("ac_int.h"),
        std::string("ac_fixed.h"),
        std::string("ac_float.h"),
        std::string("ac_std_float.h"),
        std::string("vra_instr.h"),
        std::string("vra_instr_int_fns.h"),
        std::string("vra_instr_fixed_fns.h")
      };
      
      bool is_internal_var = false;

      for (const auto& excluded_file : excluded_files) {
        if (stack_top.find(excluded_file) == 0) {
          is_internal_var = true;
          break;
        }
      }

      if (is_internal_var) { return ""; }
      
      StrVec d_fname_filters = filter_pair.first;
      StrVec d_apath_filters = filter_pair.second;

      std::string fname = get_fname_from_stack(stack_top);
      std::string apath = get_apath_from_stack(stack);

      if (regex_matches(d_fname_filters, fname)) { return ""; }
      if (regex_matches(d_apath_filters, apath)) { return ""; }

      // We only need to print the stacktrace for unchanged variables if writing to an intermediary file
      // that will later be consumed by DA. If that is not the case, return an empty string for all unchanged vars.
      if (!d_write_to_intermediary_file && !was_changed()) {
        return "";
      }

      std::stringstream outs;
      outs.imbue(std::locale());

      if (flat) {
        outs
             << "\"" << d_desc << "\""             << delim
             << stack_top                          << delim
             << ""                                 << delim
             << d_valcnt                           << delim
             << d_overflowcnt                      << delim
             << "\"" << mdecl.str() << "\""        << delim
             << d_type_min_val                     << delim
             << d_type_max_val                     << delim
             << obs_min_printed                    << delim
             << obs_max_printed                    << delim;
        if (TYPE_MODE == VRA_WITH_FIXED) {
          outs
             << min_frac_printed                   << delim
             << frac_bits_min << ".." << frac_bits << delim;
        } else {
          outs
             << ""                                 << delim
             << ""                                 << delim;
        }
        outs << (d_signed ? "signed" : "unsigned") << delim;
        auto ff = outs.flags(); // capture flags
        outs << std::fixed; // enhance precision of next 2 values
        if (TYPE_MODE == VRA_WITH_FIXED) {
          outs << quanterr                         << delim;
        } else {
          outs << ""                               << delim;
        }
        outs << rms_quant_err()                    << delim;
        outs.flags(ff); // restore flags
        outs << stack;
      } else {
        outs << "DECLARATION: " << d_desc << std::endl;
        outs << "  Available Value Range (min " << d_type_min_val << " : max " << d_type_max_val << ")" << std::endl;
        outs << "  Observed Value Range  (min " << obs_min_printed << " : max " << obs_max_printed;
        if (TYPE_MODE == VRA_WITH_FIXED) {
          outs << " : min_frac " << min_frac_printed;
        }
        outs << ")" << std::endl;
        outs << "  Signed                = " << sign_str << std::endl;
        if (TYPE_MODE == VRA_WITH_FIXED) {
          outs << "  Fractional bits       = " << frac_bits_min << ".." << frac_bits << std::endl;
        }
        outs << "  Value Change Count    = " << d_valcnt << std::endl;
        std::ios_base::fmtflags ff = outs.flags(); // capture flags
        outs << std::fixed; // enhance precision of next 3 values
        outs << "  Overflow Count        = " << d_overflowcnt << std::endl;
        if (TYPE_MODE == VRA_WITH_FIXED) {
          outs << "  Smallest Quant Err    = " << quanterr << std::endl;
        }
        outs << "  RMS Quant Err         = " << rms_quant_err() << std::endl;
        outs.flags(ff); // restore flags
        outs << "  Proposed declaration  = " << mdecl.str() << std::endl;
        outs << "  CALL STACK:" << std::endl;
        outs << stack << std::endl;
      }
      
      return outs.str();
    }
    
    private:
    void reset() {
      d_min = std::numeric_limits<double>::infinity();
      d_max = -std::numeric_limits<double>::infinity();
      d_min_frac = std::numeric_limits<double>::max();
      d_extern_m_ibits_s = d_extern_m_ibits_us = 0;
      d_signed = false;
      d_overflowcnt = 0;
      d_type_min_val = d_type_max_val = 0.0;
      d_quanterr = -1.0;
      d_valcnt = 0;
      d_sum = d_sum2 = 0.0;
      set_default_enable();
      d_nan_obs = false;
    }

    // DATA MEMBERS

    // Specify the stacktrace, and a textual description of
    // the type. This information doesn't change during regular instrumentation.
    StraceType     d_stacktrace;
    std::string    d_desc;

    // Store other important instrumentation/reporting data.
    double        d_min;               // Minimum observed value.
    double        d_max;               // Maximum observed value.
    double        d_min_frac;          // Minimum observed fractional value. ac_fixed only.
    int           d_extern_m_ibits_s;  // Max. number of integer bits for signed inputs, calculated externally.
    int           d_extern_m_ibits_us; // Min. number of integer bits for signed inputs, calculated externally.
    bool          d_signed;            // Set to true if signed input was seen.
    std::uint64_t d_overflowcnt;       // Number of overflows observed.
    double        d_type_min_val;      // Minimum value which can be stored.
    double        d_type_max_val;      // Maximum value which can be stored.
    double        d_quanterr;          // Minimum observed quantization error. ac_fixed only.
    std::uint64_t d_valcnt;            // Value change count.
    double        d_sum;               // Sum of quantization errors.
    double        d_sum2;              // Sum of squares of quantization errors.
    bool          d_enabled;           // Set to true when VRA tracking is enabled for a given variable.
    bool          d_nan_obs;           // Set to true if NaN was observed.
  };

  template <type_modes TYPE_MODE>
  class num_ovf_manager
  {
    public:
    typedef std::vector<std::uintptr_t> StraceType;

    static num_ovf_manager &getInstance() {
      static num_ovf_manager s;
      return s;
    }

    // Create (or locate, if it already exists) an ac_quant_info object for ac_int/ac_fixed.
    ac_quant_info<TYPE_MODE> &create_ac_quant_info(const StraceType &key, const std::string &desc, std::size_t &aqi_idx) {
      std::size_t keyhash = std::hash<StraceType> {}(key);

      // If the key is found, return the existing ac_quant_info corresponding to it.
      auto it = d_key_to_quant_info.find(keyhash);
      if (it != d_key_to_quant_info.end()) {
        aqi_idx = it->second;
        // Make sure that enable value is reset to the default.
        d_ac_quant_infos.at(aqi_idx).set_default_enable();
        return d_ac_quant_infos.at(aqi_idx);
      }

      // If the key isn't found, create a new ac_quant_info corresponding to it.
      d_key_to_quant_info[keyhash] = aqi_idx = d_ac_quant_infos.size();
      d_ac_quant_infos.emplace_back(key, desc);
      return d_ac_quant_infos.back();
    }

    // Locate an ac_quant_info object.
    ac_quant_info<TYPE_MODE> &get_ac_quant_info(std::size_t aqi_idx) {
      return d_ac_quant_infos.at(aqi_idx);
    }

    // (Re-)Disable VRA analysis for ac_quant_info object.
    void disable_ac_quant_info(std::size_t aqi_idx) {
      d_ac_quant_infos.at(aqi_idx).set_enable(false);
    }

    // (Re-)Enable VRA analysis for ac_quant_info object.
    void enable_ac_quant_info(std::size_t aqi_idx) {
      d_ac_quant_infos.at(aqi_idx).set_enable(true);
    }

    // this is a Meyers Singleton
    private: // hide ctor
    num_ovf_manager() {
      parse_options(); // parse options at the beginning in case of error
    }

    std::string trim(const std::string &str, const std::string &whitespace = " \t") {
      const auto start = str.find_first_not_of(whitespace);
      if (start == std::string::npos) { return ""; }

      const auto end = str.find_last_not_of(whitespace);
      const auto range = end - start + 1;
      return str.substr(start, range);
    }

    void parse_options() {
      // defaults:
      d_output_fname = ""; // default is no CSV file
      d_orig_csv_fname = "";
      d_delim = ','; // default is comma
      d_epsilon = 0.000001;
      d_opts_set = false;
      d_write_to_intermediary_file = false;
      struct rlimit rl;
      int getrl_result = getrlimit(RLIMIT_STACK, &rl);
      d_old_stack_size = -1;
      if (getrl_result == 0) {
        d_old_stack_size = rl.rlim_cur;
      }
      d_new_stack_size = d_old_stack_size;

      std::string vra_opts_var = (TYPE_MODE == VRA_WITH_INT) ? "AC_INT_VRA_OPTS" : "AC_FIXED_VRA_OPTS";
      // argument parsing from option variable
      char *opt_string = getenv(vra_opts_var.c_str());
      if (opt_string != NULL) {
        d_opts_set = true;
        std::string line(opt_string);
        std::stringstream temp(line);
        std::string token;

        while (getline(temp,token,' ')) { // tokenize by space
          std::size_t pos = std::string::npos;

          // -f<csv_filename>
          pos = token.find("-f");
          if (pos != std::string::npos) {
            d_orig_csv_fname = trim(token.substr(pos+2));
          }

          // -e<epsilon_value>
          pos = token.find("-e");
          if ((TYPE_MODE == VRA_WITH_FIXED) && (pos != std::string::npos)) {
            std::string token_substr = token.substr(pos+2);
            std::istringstream temp_iss(token_substr);
            //double eps = atof(token.substr(pos+2).c_str());
            double eps;
            // Using the operation below rather than atof makes sure that you can use
            // scientific notation to specify the epsilon value. e.g. -e1e-10
            temp_iss >> eps;
            if (eps > 0.0) {
              d_epsilon = eps;
            } else {
              std::cout << vra_opts_var << ": Invalid epsilon value '" << token.substr(pos+2) << "'" << std::endl;
            }
          }

          // -d<csv_delimiter>
          pos = token.find("-d");
          if (pos != std::string::npos) {
            char tmp_delim = atoi(token.substr(pos+2).c_str());
            switch (tmp_delim) { // , ; <TAB> <SPACE> |
              case ',':
              case ';':
              case '\t':
              case ' ':
              case '|':  {
                d_delim = tmp_delim;
                break;
              }
              default: {
                std::cout << vra_opts_var << ": Invalid CSV delimiter character '" << token.substr(pos+2) << "'" << std::endl;
                break;
              }
            }
          }
          
          // -s<stack_size>
          pos = token.find("-s");
          if (pos != std::string::npos) {
            std::int64_t new_stack_size = atoll(token.substr(pos+2).c_str());

            if ((getrl_result == 0) && (new_stack_size > 0)) {
              if (rl.rlim_cur < std::uint64_t(new_stack_size)) {
                rl.rlim_cur = new_stack_size;
                if (setrlimit(RLIMIT_STACK, &rl) == 0) {
                  d_new_stack_size = rl.rlim_cur;
                }
              }
            }
          }

          // -x<file_exclusion_filters>
          pos = token.find("-x");
          if (pos != std::string::npos) {
            std::string filters_as_str = trim(token.substr(pos+2));
            get_filters(filters_as_str, d_apath_filters, d_fname_filters);
          }
        }
      }

      // If we are writing to a file, we create a temporary file of
      // unpostprocessed VRA data.  That temp file is written to the solution
      // folder, assuming that we have the SOLUTION_DIR env variable defined.
      // Otherwise, it gets written to the project home.
      //
      // The unprocessed intermediary file contains metadata in its CSV
      // columns to inform Design Analyzer of where to write the
      // postprocessed, final CSV file.  The filename for this CSV file is
      // defined by the env variable AC_INT_VRA_FILE_NAME/AC_FIXED_VRA_FILE_NAME.
      //
      // The final CSV is written to the solution directory unless an absolute
      // path is provided for env variable mentioned above. The latter support is
      // in case the user prefers a special location for the output; any
      // relative pathing is stripped from the file name otherwise, and the
      // CSV is placed in the root of the solution directory.
      std::string vra_fname_opts_var = (TYPE_MODE == VRA_WITH_INT) ? "AC_INT_VRA_FILE_NAME" : "AC_FIXED_VRA_FILE_NAME";
      char *opt_file_name = getenv(vra_fname_opts_var.c_str());
      if (opt_file_name != NULL) {
        d_opts_set = true;
        d_orig_csv_fname = opt_file_name;
        d_write_to_intermediary_file = true;
        d_orig_csv_fname = trim(d_orig_csv_fname);
        if (d_orig_csv_fname.size()) {
          d_output_fname = std::string(VRA_UNPROCESSED_PREFIX);

          bool is_absolute_path = false;
          if ('/' == d_orig_csv_fname.front()) { // This is an absolute path, so preserve it
            is_absolute_path = true; 
          } else {
            d_orig_csv_fname = d_orig_csv_fname.substr(d_orig_csv_fname.find_last_of("/") + 1); // strip everything except filename
            if (d_orig_csv_fname.size() == 0) { // If somehow stripping slashes gave us a blank file name
              d_orig_csv_fname = "vra.csv"; 
            }
          }

          char *opt_solution_dir = getenv("SOLUTION_DIR");
          if (opt_solution_dir != NULL) {
            std::string solution_dir = opt_solution_dir;
            // Remove any trailing slash
            while (solution_dir.size() && '/' == solution_dir.back()) { 
              solution_dir.pop_back(); 
            }
            if (!solution_dir.empty()) {
              solution_dir = solution_dir + "/";
              if (!is_absolute_path) { 
                d_orig_csv_fname = solution_dir + d_orig_csv_fname; 
              }
              d_output_fname = solution_dir + d_output_fname; // otherwise it just appears in the Project Home
            }
          }
        }
      } else { 
        d_output_fname = d_orig_csv_fname; // fallback to writing directly to target CSV
      }
    }

    void report_options() {
      if (d_opts_set) {
        // Report options
        std::cout << "  Runtime options:" << std::endl;
        if (d_new_stack_size != d_old_stack_size) {
          std::cout << "    Stack size increased   : " << d_old_stack_size << " -> " << d_new_stack_size << std::endl;
        }
        print_if_non_empty("    CSV filename           : ", d_orig_csv_fname);
        print_if_non_empty("    Path exclusion filters : ", d_apath_filters);
        print_if_non_empty("    File exclusion filters : ", d_fname_filters);
        std::cout << "    CSV Delimiter          : '" << d_delim << "'" << std::endl;
        if (TYPE_MODE == VRA_WITH_FIXED) {
          std::cout << "    Epsilon                : " << d_epsilon << std::endl;
        }
      }
    }

    std::string gen_unique_out_fname() {
      std::string unique_fname = d_output_fname;
      int unique_id = 0; // ID to make sure that output filename is unique.
      bool file_exists = true;
      
      while (file_exists) {
        std::string type_token = (TYPE_MODE == VRA_WITH_INT) ? ".ac_int." : ".ac_fixed.";
        // Uniquifying with the ID might not be needed as we're adding ".ac_int" / ".ac_fixed."
        // to the filename, but it doesn't hurt to be safe.
        std::string test_fname = unique_fname + type_token + std::to_string(unique_id);
        std::ifstream test_fs(test_fname);
        file_exists = test_fs.good();
        test_fs.close();
        // If file doesn't exist already, that means that the generated file will be unique.
        if (!file_exists) { unique_fname = test_fname; }
        else { ++unique_id; } // This value should lend a unique ID after incrementing.
      }

      return unique_fname;
    }
    
    ~num_ovf_manager() {
      // Ensure that numbers are written out using
      // US/British decimal notation:
      std::setlocale(LC_ALL, "en_US.UTF-8");
      std::cout.imbue(std::locale());

      std::string vra_report_type = (TYPE_MODE == VRA_WITH_INT) ? "AC_INT_VRA" : "AC_FIXED_VRA";
      std::string vra_type_str = (TYPE_MODE == VRA_WITH_INT) ? "ac_int" : "ac_fixed";
      std::cout << "============================================================" << std::endl;
      std::cout << vra_report_type << " (Beta) - Tracked " << d_key_to_quant_info.size() << " " << vra_type_str << " variable(s)" << std::endl;

      report_options(); // report user overrides if present
      std::pair<StrVec, StrVec> filter_pair = std::make_pair(d_fname_filters, d_apath_filters);

      unsigned actual_count = 0;

      if (d_key_to_quant_info.size() > 0) {
        bool flat = false;

        if (!d_output_fname.empty()) {
          std::string final_output_fname = "";
          if (d_write_to_intermediary_file) {
            final_output_fname = gen_unique_out_fname();
            std::cout << "  Writing data to intermediary output file: " << final_output_fname << " with delimiter '" << d_delim << "'" << std::endl;
          } else {
            final_output_fname = d_output_fname;
            std::cout << "  Writing data to output file: " << final_output_fname << " with delimiter '" << d_delim << "'" << std::endl;
          }

          std::ofstream of(final_output_fname);
          of.imbue(std::locale());
          if (of.is_open()) {
            flat = true;
            if (d_write_to_intermediary_file) {
              // Instructions and metadata used by DA's postprocessing
              of << "# Note: This is an intermediate file containing raw, unmapped, VRA data intended for post-processing by Design Analyzer.";
              of << "Please open the containing solution in Design Analyzer to have this file processed into its final, usable form." << std::endl;
              of << METADATA_VRA_TYPE_KEY << d_delim << vra_type_str << std::endl;
              of << METADATA_CSV_FILE_PATH_KEY << d_delim << d_orig_csv_fname << std::endl;
            }

            // write out header (must maintain order used in report())
            of << "Declaration"        << d_delim
               << "Location"           << d_delim
               << "Variable"           << d_delim
               << "Value Change Count" << d_delim
               << "Overflow Count"     << d_delim
               << "Proposed Decl"      << d_delim
               << "Allowed Min"        << d_delim
               << "Allowed Max"        << d_delim
               << "Min"                << d_delim
               << "Max"                << d_delim
               << "MinFrac"            << d_delim
               << "Frac Bits"          << d_delim
               << "Signed"             << d_delim
               << "Smallest QuantErr"  << d_delim
               << "RMS QuantErr"       << d_delim
               << "Call Stack"         << std::endl;
            
            for (auto const &aqi : d_ac_quant_infos) {
              std::string row = aqi.report(filter_pair, d_epsilon, flat, d_delim, d_write_to_intermediary_file);

              if (row.size()) {
                ++actual_count;
                of << row << std::endl;
              }
            }
            of.close();
          } else {
            std::cerr << "Could not open output file" << std::endl;
            return;
          }
        } else {
          for (auto const &aqi : d_ac_quant_infos) {
            std::string row = aqi.report(filter_pair, d_epsilon, flat, d_delim, d_write_to_intermediary_file);

            if (row.size()) {
              ++actual_count;
              std::cout << "------------------------------------------------------------" << std::endl;
              std::cout << row;
            }
          }
        }

        if (actual_count < d_key_to_quant_info.size()) {
          std::cout << "  Non-user ";
          if (!d_write_to_intermediary_file) {
            std::cout << "and unchanged ";
          }
          std::cout << vra_type_str << " variables pruned from results" << std::endl;
          std::cout << "  " << actual_count << " / " << d_key_to_quant_info.size() << " " << vra_type_str << " variable(s) written" << std::endl;
        }
      }

      std::cout << "============================================================" << std::endl;
    }

    // DATA MEMBERS
    typedef std::vector<ac_quant_info<TYPE_MODE> > QuantInfos;
    typedef std::unordered_map<std::size_t, unsigned> KeyToIndex;
    typedef std::vector<std::string> StrVec;
    QuantInfos   d_ac_quant_infos;    // List of quantization objects with instrumentation data.
    KeyToIndex   d_key_to_quant_info; // Contains info to index into d_ac_quant_infos based on the callstack.
    std::string  d_output_fname;      // Name of file written by this class
    std::string  d_orig_csv_fname;    // Name of the final CSV file after postprocessing
    char         d_delim;             // Delimiter for CSV files.
    double       d_epsilon;           // Epsilon value. ac_fixed only.
    bool         d_opts_set;          // Did we set any options with the relevant environment variable?
    bool         d_write_to_intermediary_file; // If we're writing the unpostprocessed intermediary file
    std::int64_t d_old_stack_size;    // Original stack size, before being modified with -x.
    std::int64_t d_new_stack_size;    // Contains new stack size, if it's modified with -x. If not, it has the old stack size.
    StrVec       d_fname_filters;     // List of patterns used to filter out filenames in the VRA report.
    StrVec       d_apath_filters;     // List of patterns used to filter out paths in the VRA report.
  };

  template <type_modes TYPE_MODE>
  inline ac_quant_info<TYPE_MODE> &create_ac_quant_info(const std::vector<std::uintptr_t> &key, const std::string &desc, std::size_t &aqi_idx)
  {
    #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)
    num_ovf_manager<TYPE_MODE> &s = s.getInstance();
    return s.create_ac_quant_info(key, desc, aqi_idx);
    #endif
  }

  template <type_modes TYPE_MODE>
  inline ac_quant_info<TYPE_MODE> &get_ac_quant_info(std::size_t aqi_idx)
  {
    #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)
    num_ovf_manager<TYPE_MODE> &s = s.getInstance();
    return s.get_ac_quant_info(aqi_idx);
    #endif
  }

  template <type_modes TYPE_MODE>
  inline void disable_ac_quant_info(std::size_t aqi_idx)
  {
    #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)
    num_ovf_manager<TYPE_MODE> &s = s.getInstance();
    s.disable_ac_quant_info(aqi_idx);
    #endif
  }

  template <type_modes TYPE_MODE>
  inline void enable_ac_quant_info(std::size_t aqi_idx)
  {
    #if !defined(CCS_SCVERIFY) && !defined(__SYNTHESIS__)
    num_ovf_manager<TYPE_MODE> &s = s.getInstance();
    s.enable_ac_quant_info(aqi_idx);
    #endif
  }

  // The following functions are declared here because they're used in the
  // numeric base classes (for lvalue -> lvalue assignment) and use data private
  // to the ac_int/ac_fixed classes (as applicable) for their computations:
  // 1. calc_int_bits(const ac_int<W2, S2> &op2)
  // 2. calc_int_bits(const T op2)
  // 3. calc_int_bits(const ac_fixed<W2, I2, S2, Q2, O2> &op2)
  // 4. calc_frac_value(const ac_fixed<W2, I2, S2, Q2, O2> &op2)
  //
  // These will also be declared as friend functions to ac_int/ac_fixed
  // and finally defined in vra_instr_int_fns.h/vra_instr_fixed_fns.h, once the
  // ac_int/ac_fixed classes are fully fleshed out.

  template <int W2, bool S2>
  int calc_int_bits(const ac_int<W2, S2> &op2);

  template <class T>
  int calc_int_bits(const T op2);

  // ac_fixed only.
  template <int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2>
  int calc_int_bits(const ac_fixed<W2, I2, S2, Q2, O2> &op2);

  // ac_fixed only.
  template <int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2>
  inline double calc_frac_value(const ac_fixed<W2, I2, S2, Q2, O2> &op2);

  // ac_fixed only. Does not need to be defined later as it doesn't use
  // data private to ac_fixed.
  inline double calc_frac_value(const double x) {
    double abs_x = abs(x);
    return (abs_x - floor(abs_x));
  }

  template <type_modes type_mode>
  class common_base {
    protected:
    typedef ac_vra_ns::ac_quant_info<type_mode> AQI_class;
    static constexpr std::size_t invalid_aqi_idx = std::numeric_limits<std::size_t>::max();
    #ifdef __SANITIZE_ADDRESS__
    static constexpr bool discard_asan_frame = true;
    #else
    static constexpr bool discard_asan_frame = false;
    #endif
    AQI_class d_data;
    std::size_t aqi_idx;

    explicit common_base(const std::string &type_name_str, bool build_stacktrace = true) {
      if (build_stacktrace) {
        std::vector<std::uintptr_t> key;
        // Discard the first 4 stack frames since they reflect how we got to this line from the actual
        // call to the ac_int constructor:
        //     frame[-4] build_stack_key
        //     frame[-3] common_base() ctor
        //     frame[-2] ac_int_numeric_base/ac_fixed_numeric_base ctor
        //     frame[-1] ac_int/ac_fixed ctor
        //     frame[0] ... user's code
        //
        // If ASAN is used, we discard an extra frame because ASAN instrumentation adds a frame to the callstack.
        ac_debug::build_stack_key(key, 4 + int(discard_asan_frame));
        d_data = create_ac_quant_info<type_mode>(key, type_name_str, aqi_idx);
      } else {
        aqi_idx = invalid_aqi_idx;
      }
    }
  
    public:
    void disable_vra() {
      if (aqi_idx != invalid_aqi_idx) {
        disable_ac_quant_info<type_mode>(aqi_idx);
        d_data.set_enable(false);
      }
    }

    void enable_vra() {
      if (aqi_idx != invalid_aqi_idx) {
        enable_ac_quant_info<type_mode>(aqi_idx);
        d_data.set_enable(true);
      }
    }
  };
}

enum ac_vra_stack_trace_modes { AC_VRA_STACK_TRACED, AC_VRA_STACK_NOT_TRACED };

#ifdef AC_INT_VRA
// Base class providing instrumentation hooks into ac_int class
template<int W, bool S>
class ac_int_numeric_base : public ac_vra_ns::common_base<ac_vra_ns::VRA_WITH_INT>
{
  static constexpr ac_vra_ns::type_modes type_mode = ac_vra_ns::VRA_WITH_INT;
  typedef ac_vra_ns::common_base<type_mode> cBase;
  const ac_vra_stack_trace_modes strace_mode;
public:
  template<int W2, bool S2> friend class ac_int_numeric_base;

  ac_int_numeric_base() : cBase(type_name()), strace_mode(AC_VRA_STACK_TRACED) {
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }
  
  ac_int_numeric_base(const ac_int_numeric_base &other) : cBase(type_name()), strace_mode(AC_VRA_STACK_TRACED) {
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }

  ac_int_numeric_base(ac_vra_stack_trace_modes strace_mode_)
    : cBase("", false), strace_mode(strace_mode_)
  {
    assert(strace_mode_ == AC_VRA_STACK_NOT_TRACED);
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }
  
  ~ac_int_numeric_base() { }
  
  // This operator is called when assigning an ac_int<W, S> lvalue to another ac_int<W, S>
  // lvalue. We do not take the quantization history of the former lvalue into account as
  // it may not reflect the values actually being stored in the latter lvalue. Instead, we
  // update the VRA tracking for the latter lvalue with the value currently being stored.
  ac_int_numeric_base &operator =(const ac_int_numeric_base &a) {
    if (cBase::d_data.get_enable() && (cBase::aqi_idx != cBase::invalid_aqi_idx)) {
      typename cBase::AQI_class &this_quant_data = ac_vra_ns::get_ac_quant_info<type_mode>(cBase::aqi_idx);
      // Might need to update min/max values.
      this_quant_data.set_type_min_max(s_min_val, s_max_val);
      int int_bits = ac_vra_ns::calc_int_bits(a.value());
      // The quantized value will be the same as the input, since the precision doesn't change.
      double a_val_d = a.value().to_double();
      this_quant_data.update(false, a_val_d, a_val_d, int_bits);
      // This is technically not a data merge, but using the copy_merged_data method should still work.
      cBase::d_data.copy_merged_data(this_quant_data);
    }
    return *this;
  }
  
  // This operator is called when assigning an ac_int<W, S> rvalue to an ac_int<W, S>
  // lvalue. The quantization history of the rvalue is actually of interest here and we
  // call the merge() function instead of the update() function to preserve it.
  ac_int_numeric_base &operator =(ac_int_numeric_base &&a) {
    if (cBase::d_data.get_enable() && (cBase::aqi_idx != cBase::invalid_aqi_idx)) {
      typename cBase::AQI_class &this_quant_data = ac_vra_ns::get_ac_quant_info<type_mode>(cBase::aqi_idx);
      this_quant_data.merge(a.d_data);
      cBase::d_data.copy_merged_data(this_quant_data);
    }
    return *this;
  }

  void update(double op2, bool overflow, int int_bits = 0) {
    // Even if the stack is not traced, we might need to update the quantization info for rvalues
    // in case they are assigned to another variable which is under analysis.
    if (cBase::aqi_idx != cBase::invalid_aqi_idx || strace_mode == AC_VRA_STACK_NOT_TRACED) {
      cBase::d_data.update(overflow, op2, value().to_double(), int_bits);
    }
  }

private:
  // These functions and values are defined in vra_instr_int_fns.h
  const ac_int<W, S> &value() const;
  static std::string type_name();
  static double s_max_val;
  static double s_min_val;
};

#ifdef __AC_INT_NUMERICAL_ANALYSIS_BASE
#undef __AC_INT_NUMERICAL_ANALYSIS_BASE
#endif

// Hook up the ac_int_numeric_base class into the derivation of ac_int
#define __AC_INT_NUMERICAL_ANALYSIS_BASE ac_int_numeric_base<W,S>

#endif // #ifdef AC_INT_VRA

#ifdef AC_FIXED_VRA

// Base class providing instrumentation hooks into ac_fixed class
template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
class ac_fixed_numeric_base : public ac_vra_ns::common_base<ac_vra_ns::VRA_WITH_FIXED>
{
  static constexpr ac_vra_ns::type_modes type_mode = ac_vra_ns::VRA_WITH_FIXED;
  typedef ac_vra_ns::common_base<type_mode> cBase;
  const ac_vra_stack_trace_modes strace_mode;

public:
  template<int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2> friend class ac_fixed_numeric_base;

  ac_fixed_numeric_base() : cBase(type_name()), strace_mode(AC_VRA_STACK_TRACED) {
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }

  ac_fixed_numeric_base(const ac_fixed_numeric_base &other) : cBase(type_name()), strace_mode(AC_VRA_STACK_TRACED)  {
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }

  ac_fixed_numeric_base(ac_vra_stack_trace_modes strace_mode_)
    : cBase("", false), strace_mode(strace_mode_)
  {
    assert(strace_mode_ == AC_VRA_STACK_NOT_TRACED);
    cBase::d_data.set_type_min_max(s_min_val, s_max_val);
  }

  ~ac_fixed_numeric_base() { }

  // This operator is called when assigning an ac_fixed<W, I, S, Q, O> lvalue to another
  // ac_fixed<W, I, S, Q, O> lvalue. We do not take the quantization history of the former
  // lvalue into account as it may not reflect the values actually being stored in the
  // latter lvalue. Instead, we update the VRA tracking for the latter lvalue with the value
  // currently being stored.
  ac_fixed_numeric_base &operator =(const ac_fixed_numeric_base &a) {
    if (cBase::d_data.get_enable() && (cBase::aqi_idx != cBase::invalid_aqi_idx)) {
      typename cBase::AQI_class &this_quant_data = ac_vra_ns::get_ac_quant_info<type_mode>(cBase::aqi_idx);
      // Might need to update min/max values.
      this_quant_data.set_type_min_max(s_min_val, s_max_val);
      // The quantized value will be the same as the input, since the precision doesn't change.
      double a_val_d = a.value().to_double();
      int int_bits = ac_vra_ns::calc_int_bits(a.value());
      double frac_value = ac_vra_ns::calc_frac_value(a.value());
      this_quant_data.update(false, a_val_d, a_val_d, int_bits, frac_value);
      // This is technically not a data merge, but using the copy_merged_data method should still work.
      cBase::d_data.copy_merged_data(this_quant_data);
    }
    return *this;
  }
  
  // This operator is called when assigning an ac_fixed<W, I, S, Q, O> rvalue to an
  // ac_fixed<W, I, S, Q, O> lvalue. The quantization history of the rvalue is actually of
  // interest here and we call the merge() function instead of the update() function to preserve it.
  ac_fixed_numeric_base &operator =(ac_fixed_numeric_base &&a) {
    if (cBase::d_data.get_enable() && (cBase::aqi_idx != cBase::invalid_aqi_idx)) {
      typename cBase::AQI_class &this_quant_data = ac_vra_ns::get_ac_quant_info<type_mode>(cBase::aqi_idx);
      this_quant_data.merge(a.d_data);
      cBase::d_data.copy_merged_data(this_quant_data);
    }
    return *this;
  }

  // Called when THIS <W,I,S,Q,O> is constructed from <W2,I2,S2,Q2,O2>
  // IMPORTANT: This needs to stay around as the signature matches that of a function used for CCOV overflow detection.
  template<int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2>
  void update(bool overflow, bool neg, const ac_fixed_numeric_base<W2,I2,S2,Q2,O2> &op2, int int_bits, double frac_value) {
    update(overflow, neg, op2.value().to_double(), int_bits, frac_value);
  }

  // Called when THIS <W,I,S,Q,O> is constructed from double
  void update(bool overflow, bool neg, double op2, int int_bits, double frac_value) {
    // Even if the stack is not traced, we might need to update the quantization info for rvalues
    // in case they are assigned to another variable which is under analysis.
    if (cBase::aqi_idx != cBase::invalid_aqi_idx || strace_mode == AC_VRA_STACK_NOT_TRACED) {
      cBase::d_data.update(overflow, op2, value().to_double(), int_bits, frac_value);
    }
  }

private:
  // These functions and values are defined in vra_instr_fixed_fns.h
  const ac_fixed<W,I,S,Q,O> &value() const;
  static std::string type_name();
  static double s_max_val;
  static double s_min_val;
};

#ifdef __AC_FIXED_NUMERICAL_ANALYSIS_BASE
#undef __AC_FIXED_NUMERICAL_ANALYSIS_BASE
#endif

// Hook up the ac_fixed_numeric_base class into the derivation of ac_fixed
#define __AC_FIXED_NUMERICAL_ANALYSIS_BASE ac_fixed_numeric_base<W,I,S,Q,O>

#endif // #ifdef AC_FIXED_VRA

#undef VRA_UNPROCESSED_PREFIX
#undef METADATA_CSV_FILE_PATH_KEY
#undef METADATA_VRA_TYPE_KEY

#endif

