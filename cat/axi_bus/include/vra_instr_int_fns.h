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
//   Source:           vra_int_instr_fns.h
//   Description:      implementation of VRA instrumentation functions that must occur after ac_int.h is compiled
*/

#ifndef _INCLUDED_VRA_INT_INSTR_FNS_H_
#define _INCLUDED_VRA_INT_INSTR_FNS_H_

#ifndef __SYNTHESIS__

template <int W2, bool S2>
int ac_vra_ns::calc_int_bits(const ac_int<W2, S2> &op2) {
  constexpr int num_digits_double = std::numeric_limits<double>::digits;
  if ((W2 - int(S2)) <= (num_digits_double - 1)) {
    // Value is small enough that there won't be any precision loss when
    // converting it to a double.
    return 0;
  }

  double op2_d = op2.to_double();

  double thresh_max = ((1L << num_digits_double) - 1);
  double thresh_min = -thresh_max;
  // If x is in the range of [thresh_min, thresh_max], the ac_quant_info
  // class will handle the bitwidth calculations.
  if (op2_d >= thresh_min && op2_d <= thresh_max) {
    return 0;
  }

  constexpr int N2 = ac_int<W2, S2>::N;
  bool op2_is_neg = op2.is_neg();
  typedef ac_private::iv<N2> Base2;
  int num_bits = (32*N2) - int(op2.Base2::leading_bits(op2_is_neg)) + op2_is_neg;
  return num_bits;
}

template <class T>
int ac_vra_ns::calc_int_bits(const T op2) {
  constexpr bool double_input = std::is_same<T, double>::value || std::is_same<T, double&>::value;
  static_assert(!double_input, "Input must not be a double.");

  constexpr bool S2 = std::is_signed<T>::value;
  constexpr int W2 = std::numeric_limits<T>::digits + int(S2);
  constexpr int N2 = ac_int<W2, S2>::N;
  typedef ac_private::iv<N2> Base2;
  ac_int<W2, S2> op2_(AC_VRA_STACK_NOT_TRACED);
  op2_.Base2::operator =(op2);
  return calc_int_bits(op2_);
}

// Complete the implementation of the instrumented base class now that ac_int has been defined
template<int W, bool S>
const ac_int<W,S> &ac_int_numeric_base<W,S>::value() const {
  return (const ac_int<W,S> &) *this;
}

template<int W, bool S> std::string ac_int_numeric_base<W,S>::type_name() {
  return ac_int<W,S>::type_name();
}

template<int W, bool S>
double ac_int_numeric_base<W,S>::s_max_val = ::value<AC_VAL_MAX>(ac_int<W,S>()).to_double();

template<int W, bool S>
double ac_int_numeric_base<W,S>::s_min_val = ::value<AC_VAL_MIN>(ac_int<W,S>()).to_double();

#endif

#endif