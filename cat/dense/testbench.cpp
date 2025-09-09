
#include <stdlib.h>

#include "ac_int.h"
#include "ac_fixed.h"
#include "ac_channel.h"

#include "axi_master_if.h"

#include "defines.h"

#include "dense.h"
#include "timer.h"

#define INPUT_VECTOR_LENGTH  (32 * 16)
#define OUTPUT_VECTOR_LENGTH (64 * 16)
#define WEIGHT_SIZE (INPUT_VECTOR_LENGTH * OUTPUT_VECTOR_LENGTH)

/*
template<typename T>
ac_int<T::width, false> bits(T x)
{
   int r;
   r = x.slc<x.width>(0);
   return r;
}

template<typename T>
T fixed(int x)
{
   T r;
   r = (float) x / (float) (1 << r.i_width);
   return r;
}
*/

float random_value(int n)
{
   static int count = 0;
   static float seed = 1.0 / 256.0;

   int mask = (1 << (n - 6)) - 1;
   int denom = (1 << (WORD_BITS - INTEGER_BITS));
   int value = rand() & mask;
 
   if (rand() & 1) value = value * -1;

//return (((float) count++) * seed);
   return (float) ((float) value/ (float) denom);
}  

template<typename T>
void random_fill(int n, T *array)
{
   for (int i=0; i<n; i++) {
     array[i] = (T) random_value(WORD_BITS);
   }
}

void sw_dense(int inputs, int outputs, float *f, float *w, float *out)
{
   float sum;

   timer_start();
   for (int o=0; o<outputs; o++) {
     sum = 0;
     for (int i=0; i<inputs; i++) {
       sum += f[i] * w[o*inputs+i];
     }
     out[o] = sum;
   }
   printf("Time for software: %d milliseconds \n", timer_stop());
}


void cat_dense(int inputs, int outputs, float *f, float *w, float *out)
{
   ac_fixed<WORD_BITS*2, INTEGER_BITS*2, true> sum;
   feature_type cat_f[INPUT_VECTOR_LENGTH];
   feature_type cat_o[OUTPUT_VECTOR_LENGTH];
   weight_type  cat_w[WEIGHT_SIZE];

   for (int i=0; i<inputs; i++) cat_f[i] = f[i];
   for (int i=0; i<inputs * outputs; i++) cat_w[i] = w[i];

   timer_start();
   for (int o=0; o<outputs; o++) {
     sum = 0.0;
     for (int i=0; i<inputs; i++) {
       sum += cat_f[i] * cat_w[o*inputs+i];

//printf("CAT w_index: %d f_index: %d feature: %f weight: %f \n",
//    o*inputs+i, i, (float) cat_f[i].to_double(), (float) cat_w[o*inputs+i].to_double());

     }
//printf("CAT sum: %f \n", (float) sum.to_double());
     cat_o[o] = sum;
   }
   printf("Time for quantized: %d milliseconds \n", timer_stop());
   for (int i=0; i<outputs; i++) out[i] = cat_o[i].to_double();
}


void hw_dense(int inputs, int outputs, float *f, float *w, float *out)
{
   feature_type sum;
   axi_master_interface axi_bus;

   ac_channel<bool> start;
   ac_channel<bool> done;
   axi_16 r;
   bool done_bit;
   axi_32 debug;

   static feature_type cat_f[INPUT_VECTOR_LENGTH];
   static feature_type cat_o[OUTPUT_VECTOR_LENGTH];
   static weight_type  cat_w[WEIGHT_SIZE];

   const param_t use_relu = 0;
   const param_t hi_mem   = 0x00;
   const param_t feature_addr = 0x0;
   const param_t weights_addr = inputs * WORD_BYTES;
   const param_t outputs_addr = weights_addr + inputs * outputs * WORD_BYTES;
   const axi_32  input_count = inputs;
   const axi_32  output_count = outputs;

   // copy to cat format
   for (int i=0; i<inputs; i++) cat_f[i] = f[i];
   for (int i=0; i<inputs * outputs; i++) cat_w[i] = w[i];
  
   // load features
   for (int i=0; i<inputs; i++) {
     axi_bus.write(feature_addr + i * WORD_BYTES, cat_f[i].slc<WORD_BITS>(0));
   }

   // load weights
   for (int i=0; i<inputs*outputs; i++) axi_bus.write(weights_addr + i * WORD_BYTES, cat_w[i].slc<WORD_BITS>(0));

   // start processing
   start.write(true);

   timer_start();
   dense(
      start,
      done,
      use_relu,
      hi_mem,
      feature_addr,
      weights_addr,
      outputs_addr,
      input_count,
      output_count,
      debug,
      axi_bus);

   printf("Time for architected: %d milliseconds \n", timer_stop());
   done_bit = done.read();

   // read outputs
   for (int i=0; i<outputs; i++) {
     feature_type out_value;
     ac_int<WORD_BITS, false> r;
     axi_bus.read(outputs_addr + (i * WORD_BYTES), r);
     out_value.set_slc(0, r);  
     out[i] = out_value.to_double();
   } 
}

bool close(float a, float b, float margin)
{
   float delta = a - b;

   if (delta < 0) delta = delta * -1.0;

   if (delta < margin) return true;
   else                return false;
}


int compare(int n, float *a, float *b, char *s)
{
   int errors = 0;
   float margin = 0.66;

   for (int i=0; i<n; i++) {
      if (!close(a[i], b[i], margin)) {
         errors++;
         printf("Error: %s[%d]: expected: %f received %f \n", s, i, a[i], b[i]);
      }
   }

   return errors;
}


int main()
{
   int i;
   int errors = 0;

   // static so we do not blow up the stack
   static float features[INPUT_VECTOR_LENGTH];
   static float weights[WEIGHT_SIZE];
   static float sw_outputs[OUTPUT_VECTOR_LENGTH];
   static float hw_outputs[OUTPUT_VECTOR_LENGTH];
   static float cat_outputs[OUTPUT_VECTOR_LENGTH];
   static float aws_outputs[OUTPUT_VECTOR_LENGTH];

   const int num_inputs  = INPUT_VECTOR_LENGTH;
   const int num_outputs = OUTPUT_VECTOR_LENGTH;
   const int num_weights = WEIGHT_SIZE;

   random_fill(num_weights, weights);
   random_fill(num_inputs, features);

   printf("Computing reference values \n");
   sw_dense(num_inputs, num_outputs, features, weights, sw_outputs);

   printf("Computing quantized values \n");
   cat_dense(num_inputs, num_outputs, features, weights, cat_outputs);
   
   printf("Computing architected values \n");
   hw_dense(num_inputs, num_outputs, features, weights, hw_outputs);

   // aws_dense(num_inputs, num_outputs, features, weights, aws_outputs);

   // compare results
  
   errors += compare(num_outputs, sw_outputs, cat_outputs, (char *) "cat");
   printf("Quantized values: %d errors \n", errors);

   errors += compare(num_outputs, sw_outputs, hw_outputs, (char *) "hw");
   printf("Architected values: %d errors \n", errors);

   // errors += compare(num_outputs, sw_outputs, aws_outputs, (char *) "aws");
   // printf("FPGA values: %d errors \n", errors);

   if (errors > 0) {
     printf("test failed: %d errors of %d values \n", errors, num_outputs); 
   } else {
     printf("test passed: %d errors \n", errors);
   }
}
