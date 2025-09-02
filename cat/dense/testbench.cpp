
#include <stdlib.h>

#include "ac_int.h"
#include "ac_fixed.h"
#include "ac_channel.h"

#include "axi_master_if.h"
#include "dense.h"

#define INPUT_VECTOR_LENGTH 32 * 16
#define OUTPUT_VECTOR_LENGTH 64 *16

#define WRD_SIZE 16
#define INTEGER_BITS 8

typedef ac_fixed<WRD_SIZE, INTEGER_BITS, true> feature_type;
typedef ac_fixed<WRD_SIZE, INTEGER_BITS, true> weight_type;

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
   int mask = (1 << (n - 4)) - 1;
   int denom = (1 << (WRD_SIZE - INTEGER_BITS));
   int value = rand() & mask;
 
   if (rand() & 1) value = value * -1;

   return (float) value/denom;
}  

template<typename T>
void random_fill(int n, T *array)
{
   for (int i=0; i<n; i++) {
     array[i] = (T) random_value(WRD_SIZE);
   }
}

void sw_dense(int inputs, int outputs, feature_type *f, weight_type *w, feature_type *out)
{
   feature_type sum;

   for (int o=0; o<outputs; o++) {
     sum = 0;
     for (int i=0; i<inputs; i++) {
       sum += f[i] * w[o*inputs+i];
     }
     out[o] = sum;
   }
}


void hw_dense(int inputs, int outputs, feature_type *f, weight_type *w, feature_type *out)
{
   feature_type sum;
   axi_master_interface axi_bus;

   ac_channel<bool> start;
   ac_channel<bool> done;
   axi_16 r;

   bool done_bit;
  
   // load features
   for (int i=0; i<inputs; i++) axi_bus.write(i*(WRD_SIZE/8), f[i].slc<WRD_SIZE>(0));

   // load weights
   for (int i=0; i<inputs*outputs; i++) axi_bus.write(0x1000 + i * (WRD_SIZE/8), w[i].slc<WRD_SIZE>(0));

   // start processing
   start.write(true);

   const param_t use_relu = 0;
   const param_t hi_mem   = 0x00;
   const param_t feature_addr = 0x0;
   const param_t weights_addr = 0x1000;
   const param_t outputs_addr = 0x2000;
   const axi_32  input_count = inputs;
   const axi_32  output_count = outputs;

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
      axi_bus);

   done_bit = done.read();

   // read outputs
   for (int i=0; i<outputs; i++) {
      axi_bus.read(0x2000 + i * (WRD_SIZE/8), r);
      out[i] = r.to_double() / 255.0;
   } 
}

bool close(feature_type a, feature_type b)
{
  feature_type margin = 0.5;
  feature_type delta = a - b;

  if (delta < 0) delta = delta * (feature_type) -1.0;

  if (delta < margin) return true;
  else                return false;
}

int main()
{
   int i;
   int errors = 0;

   feature_type features[INPUT_VECTOR_LENGTH];
   weight_type  weights[INPUT_VECTOR_LENGTH * OUTPUT_VECTOR_LENGTH];
   feature_type sw_outputs[OUTPUT_VECTOR_LENGTH];
   feature_type hw_outputs[OUTPUT_VECTOR_LENGTH];

   random_fill<feature_type>(INPUT_VECTOR_LENGTH, features);
   random_fill<weight_type>(INPUT_VECTOR_LENGTH * OUTPUT_VECTOR_LENGTH, weights);

   sw_dense(INPUT_VECTOR_LENGTH, OUTPUT_VECTOR_LENGTH, features, weights, sw_outputs);
   
   hw_dense(INPUT_VECTOR_LENGTH, OUTPUT_VECTOR_LENGTH, features, weights, hw_outputs);

   // compare results
  
   for (i=0; i<OUTPUT_VECTOR_LENGTH; i++) {
     if (!close(sw_outputs[i], hw_outputs[i])) {
       printf("error: sw result: %f hw result: %f \n", sw_outputs[i].to_double(), hw_outputs[i].to_double());
       errors++;
     }
   }

   if (errors > 0) {
     printf("test failed: %d errors of %d values \n", errors, OUTPUT_VECTOR_LENGTH); 
   } else {
     printf("test passed \n");
   }
}
