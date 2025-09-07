
void dense(ac_channel<bool>     &start, 
           ac_channel<bool>     &done,
           bool                  use_relu, 
           param_t               addr_hi, 
           param_t               feature_addr_lo, 
           param_t               weight_addr_lo, 
           param_t               output_addr_lo, 
           axi_32                input_vector_len, 
           axi_32                output_vector_len, 
           axi_master_interface &memory);
