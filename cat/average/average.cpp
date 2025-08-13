
#include <ac_int.h>
#include <ac_channel.h>
#include <mc_scverify.h>
#include <axi_master_if.h>

typedef ac_int<32, true> param_t;
typedef axi_address_type addr_t;

#pragma busifc_cfg slave_0 DataWidth=32 BaseAddress=0 Protocol=axi4lite
#pragma hls_design top
void average(axi_32 count, param_t index_hi, param_t index_lo, axi_32 &result, axi_master_interface &memory)
{
#pragma busifc count    WordOffset=0 Slave=slave_0
#pragma busifc index_hi WordOffset=1 Slave=slave_0
#pragma busifc index_lo WordOffset=2 Slave=slave_0
#pragma busifc result   WordOffset=3 Slave=slave_0

   axi_32 sum;
   axi_32 n;
   axi_address_type addr;
   axi_address_type base;

   base = index_hi << 32 + index_lo;

   sum = 0;

   for (int i=0; i<count; i++) {
     addr = base + i * 4;
     memory.read(addr, n);
     sum += n;
   }

   addr = base + count * 4 + 64;

   // result = sum / count

   result = sum >> 5;
   memory.write(addr, result);
}

