
#include <ac_int.h>
#include <ac_channel.h>
#include <mc_scverify.h>
#include <axi_master_if.h>

#include "defines.h"

// #pragma busifc_cfg slave_0 DataWidth=32 BaseAddress=0 Protocol=axi4lite
#pragma hls_design top
void dense(ac_channel<bool>     &start, 
           ac_channel<bool>     &done,
           bool                  use_relu, 
           param_t               addr_hi, 
           param_t               feature_addr_lo, 
           param_t               weight_addr_lo, 
           param_t               output_addr_lo, 
           axi_32                input_vector_len, 
           axi_32                output_vector_len, 
	   axi_32               &debug,
           axi_master_interface &memory)
{
// #pragma busifc start              WordOffset=0 Slave=slave_0
// #pragma busifc done               WordOffset=2 Slave=slave_0
// #pragma busifc use_relu           WordOffset=4 Slave=slave_0
// #pragma busifc addr_hi            WordOffset=5 Slave=slave_0
// #pragma busifc feature_addr_lo    WordOffset=6 Slave=slave_0
// #pragma busifc weight_addr_lo     WordOffset=7 Slave=slave_0
// #pragma busifc output_addr_lo     WordOffset=8 Slave=slave_0
// #pragma busifc input_vector_len   WordOffset=9 Slave=slave_0
// #pragma busifc output_vector_len  WordOffset=10 Slave=slave_0
// #pragma busifc debug              WordOffset=11 Slave=slave_0

   const int stride = (BUS_SIZE / WORD_BITS);
   const int burst_size = (1 << LEN_BITS);
   const int burst_bytes = burst_size * STRIDE;
   const int burst_words = burst_size * stride;
   axi_address_type weight_address;
   axi_address_type feature_address;
   axi_address_type output_address;
   axi_data_type    feature_line;
   axi_data_type    weight_line;
   axi_data_type    output_line;
   axi_data_type    feature_memory[0x8000];
   axi_data_type    weight_memory[burst_size];
   bool go;

   go = start.read();

   debug = -1;

   weight_address  = addr_hi + weight_addr_lo;
   feature_address = addr_hi + feature_addr_lo;
   output_address  = addr_hi + output_addr_lo;

   // read in all features
   
   index_t num_feature_lines = ((input_vector_len + (burst_words - 1)) >> BUS_BITS) * WORD_BYTES;
   num_feature_lines = (input_vector_len + INDEX_MASK) >> INDEX_BITS;
   index_t remaining_lines = num_feature_lines;

   for (unsigned int i=0; i<input_vector_len * WORD_BYTES; i+= burst_bytes) {
     index_t feature_burst_size = (remaining_lines < burst_size) ? remaining_lines : (index_t) burst_size;
     memory.burst_read(feature_address + i, feature_memory + i, feature_burst_size);
     remaining_lines -= feature_burst_size;
   }

   ac_fixed<WORD_BITS*2, INTEGER_BITS*2, true> sum;
   ac_fixed<WORD_BITS*2, INTEGER_BITS*2, true> sum_array[stride];

   index_t out_index = 0;
   ac_int<32, false> weight_index = 0;
   ac_int<LEN_BITS, false> weight_cache_offset;
   const ac_int<32, false> weight_mask = ((1 << LEN_BITS) - 1);

   debug = 0xa5;

   for (int i=0; i<16; i++) {
     memory.write(0x1000000 + i * 4, (axi_32) i);
   }

   debug = 0x5a;

   while (out_index<output_vector_len) {
debug = out_index;
printf("debug: %d \n", debug.to_int()); 
    #pragma hls_unroll
     for (int w=0; w<stride; w++) {
       sum_array[w] = 0;
     }
     sum = 0.0;

     for (index_t in_index=0; in_index<num_feature_lines; in_index++) {

       weight_cache_offset = weight_index & weight_mask;

       if (weight_cache_offset == 0) {
//printf("fresh weights read from address: %08x \n", weight_address + weight_index * stride * WORD_BYTES);
          memory.burst_read(weight_address + weight_index * stride * WORD_BYTES, weight_memory, burst_size);
       }

       feature_line = feature_memory[in_index];
       weight_line = weight_memory[weight_cache_offset];

      #pragma hls_unroll
       for (int w=0; w<stride; w++) {

         feature_type feature;
         weight_type weight;

         feature.set_slc(0, feature_line.slc<WORD_BITS>(w*WORD_BITS));
         weight.set_slc(0, weight_line.slc<WORD_BITS>(w*WORD_BITS)); 

// printf("HW w_index: %d f_index: %d feature: %f weight: %f \n", 
//     weight_index * 32 + w, in_index * 32 + w, (float) feature.to_double(), (float) weight.to_double());

         sum_array[w] += feature * weight;
       }

       weight_index++; 
     }

     for (int w=0; w<stride; w++) {
       sum += sum_array[w];
     }

// printf("HW sum: %f \n", (float) sum.to_double());

     feature_type sum_out = sum;
     index_t line_index = out_index & INDEX_MASK;
     output_line.set_slc(line_index*WORD_BITS, sum_out.slc<WORD_BITS>(0));    
     out_index++;

     if ((out_index & INDEX_MASK) == 0) {
       memory.write(output_address, output_line);
       output_address += BYTE_BITS;
     }
   }

   done.write(true);
   return;
}
