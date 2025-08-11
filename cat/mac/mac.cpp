#include <ac_int.h>
#include <ac_channel.h>
#include <mc_scverify.h>

typedef ac_int<12, true> factor_t;
typedef ac_int<24, true> product_t;
typedef ac_int<24, true> addend_t;
typedef ac_int<25, true> sum_t;
 
#pragma busifc_cfg slave_0 DataWidth=64 BaseAddress=0 Protocol=axi4lite
#pragma hls_design top
void mac(factor_t f1, factor_t f2, addend_t a1, sum_t &result)
{
#pragma busifc f1      WordOffset=0 Slave=slave_0
#pragma busifc f2      WordOffset=1 Slave=slave_0
#pragma busifc a1      WordOffset=2 Slave=slave_0
#pragma busifc result  WordOffset=3 Slave=slave_0

   product_t p;
   sum_t s;

   p = f1 * f2;
   s = p + a1;
   result = s;
}

