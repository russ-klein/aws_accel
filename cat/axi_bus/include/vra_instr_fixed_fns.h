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
//   Source:           vra_instr_fixed_fns.h
//   Description:      implementation of VRA instrumentation functions that must occur after ac_fixed.h is compiled
*/

#ifndef _INCLUDED_VRA_INSTR_FIXED_FNS_H_
#define _INCLUDED_VRA_INSTR_FIXED_FNS_H_

#ifndef __SYNTHESIS__

template <int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2>
int ac_vra_ns::calc_int_bits(const ac_fixed<W2, I2, S2, Q2, O2> &op2) {
  constexpr int num_digits_double = std::numeric_limits<double>::digits;
  if ((I2 - int(S2)) <= (num_digits_double - 1)) {
    // Value is small enough that there won't be any precision loss when
    // converting the integer part to a double.
    return 0;
  }

  double thresh_max = ((1L << num_digits_double) - 1);
  double thresh_min = -thresh_max;
  double op2_d = op2.to_double();

  if (op2_d >= thresh_min && op2_d <= thresh_max) {
    return 0;
  }

  constexpr int N2 = ac_fixed<W2, I2, S2, Q2, O2>::N;
  constexpr int F2 = W2 - I2;
  bool op2_is_neg = op2.is_neg();
  typedef ac_private::iv<N2> Base2;
  int num_bits = (32*N2) - int(op2.Base2::leading_bits(op2_is_neg)) + op2_is_neg - F2;
  return AC_MAX(num_bits, 0); // Avoid negative integer widths.
}

template <int W2, int I2, bool S2, ac_q_mode Q2, ac_o_mode O2>
double ac_vra_ns::calc_frac_value(const ac_fixed<W2, I2, S2, Q2, O2> &op2) {
  if (W2 <= I2) {
    return 0.0;
  }

  constexpr int F2 = AC_MAX(W2 - I2, 1);

  ac_int<F2, false> frac_bits = op2.template slc<F2>(0);
  // Offset for conversion if input is negative.
  ac_int<F2 + 1, false> offset = 0;
  offset[F2] = 1;
  if (op2.is_neg()) {
    frac_bits = offset - frac_bits;
  }

  double frac_part_d = frac_bits.to_double()*exp2(double(-F2));

  if (frac_part_d == 1.0) {
    // Making absolutely sure that frac_part_d < 1
    frac_part_d = 0.0;
  }

  return frac_part_d;
}

// Complete the implementation of the instrumented base class now that ac_fixed has been defined
template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
const ac_fixed<W,I,S,Q,O> &ac_fixed_numeric_base<W,I,S,Q,O>::value() const
{
  return (const ac_fixed<W,I,S,Q,O> &) *this;
}

template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O> std::string ac_fixed_numeric_base<W,I,S,Q,O>::type_name()
{
  return ac_fixed<W,I,S,Q,O>::type_name();
}

template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
double ac_fixed_numeric_base<W,I,S,Q,O>::s_max_val = ::value<AC_VAL_MAX>(ac_fixed<W,I,S,Q,O>()).to_double();

template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
double ac_fixed_numeric_base<W,I,S,Q,O>::s_min_val = ::value<AC_VAL_MIN>(ac_fixed<W,I,S,Q,O>()).to_double();

#endif

#endif
