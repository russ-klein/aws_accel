#include "axi_master_if.h"

//=================================================
//
// Simulation output routines, makes debug easier
//
// ================================================

void cat_putc(axi_master_interface &axi_bus, char c)
{
#ifdef C_SIMULATION
   printf("%c", c);
#else
   axi_bus.write(0x60000000, (axi_u8) c);
#endif
}

void print_string(axi_master_interface &axi_bus, const char *s) 
{
   while (*s) {
      cat_putc(axi_bus, *s++);
   };
}

void print_int(axi_master_interface &axi_bus, int n)
{
   axi_u8 digits[20];
   int idx = 20;

   if (n<0) {
     cat_putc(axi_bus, '-');
     n = n * -1;
   }

   if (n==0) {
     digits[--idx] = '0';
   }

   else while (n) {
     digits[--idx] = n - ((n/10) * 10) + '0';
     n = n / 10;
   }

   for (int i=idx; i<20; i++) cat_putc(axi_bus, digits[i]);
}

void end_simulation(axi_master_interface &axi_bus)
{
#ifdef C_SIMULATION
   printf("\n\nbdy.. bdy.. bdy.. That's all, Folks! \n");
#else
   axi_bus.write(0x61FFFFF8, (axi_u32) 0x06070BED);
#endif
}


/*
void report_difference(
    axi_master_interface &axi_bus,
    int index,
    int expected,
    int actual)
{
   print_string(axi_bus, "mismatch at index: "); print_int(axi_bus, index);
   print_string(axi_bus, " expected: "); print_int(axi_bus, expected);
   print_string(axi_bus, " actual: "); print_int(axi_bus, actual);
   print_string(axi_bus, "\r\n");
}
*/

#pragma hls_design top
void dma(
   ac_channel<bool>       &go,
   axi_master_interface   &axi_bus)
{
   axi_u32 source_address;
   axi_u32 destination_address;
   axi_u32 size_of_transfer;

   axi_u8 transfer_buffer[4096];

   go.read();

   print_string(axi_bus, "\n\n\n\rAXI DMA \n\n\r");

   axi_bus.read(0x61000000, source_address);
   axi_bus.read(0x61000004, destination_address);
   axi_bus.read(0x61000008, size_of_transfer);

   axi_bus.burst_read(source_address, transfer_buffer, size_of_transfer);
   axi_bus.burst_write(destination_address, transfer_buffer, size_of_transfer);

   end_simulation(axi_bus);
}

int main()
{
   ac_channel<bool>       go;
   axi_master_interface   axi_bus;

   go.write(1);

   dma(go, axi_bus);
}
