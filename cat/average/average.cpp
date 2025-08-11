
#include <ac_int.h>
#include <ac_channel.h>
#include <mc_scverify.h>
#include <axi_master_if.h>

typedef ac_int<23, true> num_t;
typedef axi_address_type addr_t;

#pragma busifc_cfg slave_0 DataWidth=64 BaseAddress=0 Protocol=axi4lite
#pragma hls_design top
void average(num_t count, addr_t index, num_t &result, axi_master_interface &memory)
{
#pragma busifc count    WordOffset=0 Slave=slave_0
#pragma busifc index    WordOffset=1 Slave=slave_0
#pragma busifc result   WordOffset=2 Slave=slave_0

   num_t sum;
   axi_32 n;

   sum = 0;

   for (int i=0; i<count; i++) {
     memory.read(index+i, n);
     sum += n;
   }

   // result = sum / count
   result = sum >> 5;
}

