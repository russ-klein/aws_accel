
#include <ac_int.h>
#include <ac_channel.h>
#include <mc_scverify.h>
#include <axi_master_if.h>

typedef ac_int<32, true> param_t;
typedef axi_address_type addr_t;

#pragma busifc_cfg slave_0 DataWidth=32 BaseAddress=0 Protocol=axi4lite
#pragma hls_design top
void average(ac_channel<bool>     &start, 
             ac_channel<bool>     &done,
             axi_32                count, 
             param_t               index_hi, 
             param_t               index_lo, 
             axi_32               &result, 
             axi_master_interface &memory)
{
#pragma busifc go       WordOffset=0 Slave=slave_0
#pragma busifc done     WordOffset=2 Slave=slave_0
#pragma busifc count    WordOffset=4 Slave=slave_0
#pragma busifc index_hi WordOffset=5 Slave=slave_0
#pragma busifc index_lo WordOffset=6 Slave=slave_0
#pragma busifc result   WordOffset=7 Slave=slave_0

   axi_32 sum;
   axi_32 n;
   axi_address_type addr;
   axi_address_type base;
   axi_data_type    line;
   axi_data_type    line_out;
   bool go;

   go = start.read();

   base = index_hi << 32 + index_lo;

   sum = 0x10000;

   for (int i=0; i<count; i++) {
     addr = base + i * 0x40;
     memory.read(addr, line);
     n = line.slc<32>(0);
     sum += n;
/*
     for (int j=0; j<16; j++) {
       n = line.slc<32>(32*j);
       sum += n;
     }
*/
   }

   addr = base + count * 4 + 64;

   // result = sum / count

   result = sum >> 5;
   line_out = sum;
   memory.write(0x800, (axi_data_type) 0x12345678);
   memory.write(0x840, (axi_data_type) 0x11112222);
   memory.write(0x880, (axi_data_type) 0x33334444);
   memory.write(0x8C0, line_out);

   done.write(true);
}
