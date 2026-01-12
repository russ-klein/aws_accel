//==================================================================
// File: ccs_dw_int_lib.h
// Description: provides a C++ interface to Designware(r) integer blocks.
//
// BETA version
//==================================================================

#pragma once

#include <ac_ipl/ac_packed_vector.h>

#include <iostream>
#include <stdlib.h>

//-------------------------------------------------------------------------------------------
// DW_minmax
// Simulation Model
template <class base_type, class index_type>
void minmax(
  base_type new_val,
  ac_int<1, false> min_max,
  index_type new_index,
  base_type &value,
  index_type &index
)
{
  if (min_max == 0 && new_val < value) {
    value = new_val;
    index = new_index;
  }

  if (min_max == 1 && new_val >= value) {
    value = new_val;
    index = new_index;
  }
}

template <int num_inputs, int width, int idx_width>
void ccs_dw_minmax_sim_model(
  ac_int<num_inputs*width, false> a,
  ac_int<1, false> tc,
  ac_int<1, false> min_max,
  ac_int<width, false> &value,
  ac_int<idx_width, false> &index
)
{
  ac_packed_vector<ac_int<width, false>, num_inputs> a_pv_us;
  ac_packed_vector<ac_int<width, true>, num_inputs> a_pv_s;

  a_pv_us.set_data(a);
  a_pv_s.set_data(a);

  typedef ac_int<width, false> base_type_us;
  typedef ac_int<width, true> base_type_s;

  base_type_us value_us = a_pv_us[0];
  base_type_s value_s = a_pv_s[0];

  typedef ac_int<idx_width, false> idx_type;
  idx_type index_temp = 0;

  for (int i = 1; i < num_inputs; i++) {
    if (tc == 0) {
      base_type_us new_val = a_pv_us[i];
      minmax(new_val, min_max, idx_type(i), value_us, index_temp);
    } else {
      base_type_s new_val = a_pv_s[i];
      minmax(new_val, min_max, idx_type(i), value_s, index_temp);
    }
  }

  value = (tc == 0) ? value_us : base_type_us(value_s);
  index = index_temp;
  
}

// Mapped to the operator using ac_int
#pragma map_to_operator ccs_dw_minmax
template<int num_inputs, int width, int idx_width>
void ccs_dw_minmax(
  const ac_int<num_inputs*width, false> &a,
  const ac_int<1, false> &tc,
  const ac_int<1, false> &min_max,
  ac_int<width, false> &value,
  ac_int<idx_width, false> &index
)
{
  ccs_dw_minmax_sim_model<num_inputs>(a, tc, min_max, value, index);
}

// Top-level API
template<int num_inputs, int width, int idx_width>
void minmax(
  const ac_int<num_inputs*width, false> &a,
  const ac_int<1, false> &tc,
  const ac_int<1, false> &min_max,
  ac_int<width, false> &value,
  ac_int<idx_width, false> &index
)
{
  ccs_dw_minmax<num_inputs>(a, tc, min_max, value, index);
}

//-------------------------------------------------------------------------------------------
// DW_sqrt
// Simulation Model
template <int width, int tc_mode>
void ccs_dw_sqrt_sim_model(const ac_int<width, false> &a, ac_int<(width + 1)/2, false> &root) {
  ac_int<width, false> a_us = a;
  if (tc_mode == 1) {
    ac_int<width, true> a_s = a;
    if (a_s < 0) {
      a_us = -a_s;
    }
  }
  
  constexpr int root_width = root.width;
  
  ac_int<root_width, false> root_temp = 0;
  
  for (int i = root_width - 1; i >= 0; i--) {
    ac_int<root_width, false> root_temp_2 = root_temp;
    root_temp_2[i] = 1;
    ac_int<2*root_width, false> root_sqr = root_temp_2*root_temp_2;
    
    if (root_sqr <= a_us) {
      root_temp = root_temp_2;
      
      if (root_sqr == a_us) {
        break;
      }
    }
  }
  
  root = root_temp;
}

// Mapped to the operator using ac_int
#pragma map_to_operator ccs_dw_sqrt
template<int width, int tc_mode>
void ccs_dw_sqrt(
  const ac_int<width, false> &a,
  ac_int<(width+1)/2, false> &root
)
{
  ccs_dw_sqrt_sim_model<width,tc_mode>(a, root);
}

// Mapped to the operator using ac_int
#pragma map_to_operator ccs_dw_lp_piped_sqrt
template<int width, int tc_mode>
void ccs_dw_lp_piped_sqrt(
  const ac_int<width, false> &a,
  ac_int<(width+1)/2, false> &root
)
{
  ccs_dw_sqrt_sim_model<width,tc_mode>(a, root);
}

// Top-level API
template<int width, bool S>
ac_int<(width+1)/2, false> dw_sqrt(const ac_int<width, S> &a)
{
  ac_int<width, false> a_us = a;
  ac_int<(width + 1)/2, false> root;
  constexpr int tc_mode = S;
  ccs_dw_sqrt<width, tc_mode>(a_us, root);
  return root;
}

// Top-level API
template<int width, bool S>
ac_int<(width+1)/2, false> dw_lp_piped_sqrt(const ac_int<width, S> &a)
{
  ac_int<width, false> a_us = a;
  ac_int<(width + 1)/2, false> root;
  constexpr int tc_mode = S;
  ccs_dw_lp_piped_sqrt<width, tc_mode>(a_us, root);
  return root;
}


