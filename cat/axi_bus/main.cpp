
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

void prep_write(axi_master_interface &axi_bus, int n)
{
#ifdef C_SIMULATION

   b_payload b;

   b.resp = 0;
   b.id = 0;

   for (int i=0; i<n; i++) axi_bus.channels.b_channel.write(b);

#endif
}

void pass_data(axi_master_interface &axi_bus)
{
// in logic simulation, the memory will pass the data from the writes to the reads
// in C simulation, this routine moves the data from the write channel to the read
// data should be accessed in the test in the same order for the writes followed by reads
//
// WARNING: DO NOT SYNTHESIZE THIS FUNCITON
//
#ifdef C_SIMULATION

   while (axi_bus.channels.w_channel.available(1)) {
      r_payload r;
      w_payload w;

      w = axi_bus.channels.w_channel.read();
      r.id = 0;
      r.resp = 0;
      r.last = 0;
      r.data = w.data;

      axi_bus.channels.r_channel.write(r);
   }
#endif 
}


//=================================================
//
// Test routines for AXI master interface
//
//=================================================

//------------------------
// single transfer tests
//------------------------

// N = number of values to read and write

#define N 128

void test_u8_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u8 write_array[N];
   axi_u8 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "u8 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 1, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 1, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_8_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_8 write_array[N];
   axi_8 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "8 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 1, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 1, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_u16_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u16 write_array[N];
   axi_u16 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "u16 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 2, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 2, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_16_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_16 write_array[N];
   axi_16 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "16 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 2, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 2, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_u32_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u32 write_array[N];
   axi_u32 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "u32 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 4, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 4, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_32_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_32 write_array[N];
   axi_32 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "32 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 4, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 4, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_u64_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u64 write_array[N];
   axi_u64 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "u64 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 8, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 8, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_64_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_64 write_array[N];
   axi_64 read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "64 single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 8, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 8, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_float_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_float write_array[N];
   axi_float read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "float single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_float) i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 4, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 4, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_double_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_double write_array[N];
   axi_double read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "double (precision float) single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_double) i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 8, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 8, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

//------------------------
// burst transfer tests
//------------------------

void test_u8_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_u8 write_array[N];
   axi_u8 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "u8 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   // pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_8_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_8 write_array[N];
   axi_8 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "8 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_u16_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_u16 write_array[N];
   axi_u16 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "u16 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_16_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_16 write_array[N];
   axi_16 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "16 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_u32_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_u32 write_array[N];
   axi_u32 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "u32 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_32_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_32 write_array[N];
   axi_32 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "32 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_u64_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_u64 write_array[N];
   axi_u64 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "u64 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_64_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_64 write_array[N];
   axi_64 read_array[N];
   bool pass = true;
   bool verbose = true;

   print_string(axi_bus, "64 burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }

   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}

void test_float_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_float write_array[N];
   axi_float read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "float burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_float) i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}


void test_double_burst_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   axi_double write_array[N];
   axi_double read_array[N];
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "double burst transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_double) i;
   prep_write(axi_bus, N);

   axi_bus.burst_write(address, write_array, count);

   pass_data(axi_bus); 

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
}



#pragma hls_design top
void counter(
   ac_channel<bool>       &go,
   axi_master_interface   &axi_bus)
{
   go.read();

   //print_string(axi_bus, "\n\n\n\rAXI master interface test program \n\n\r");

   // single access operation tests

   //test_u8_single_transfer (axi_bus, 0x61000000);
/*
   test_8_single_transfer  (axi_bus, 0x61000400);
   test_u16_single_transfer(axi_bus, 0x61000800);
   test_16_single_transfer (axi_bus, 0x61000C00);
   test_u32_single_transfer(axi_bus, 0x61001000);
   test_32_single_transfer (axi_bus, 0x61001400);
   test_u64_single_transfer(axi_bus, 0x61001800);
   test_64_single_transfer (axi_bus, 0x61001C00);

   test_float_single_transfer  (axi_bus, 0x61002000);
   test_double_single_transfer (axi_bus, 0x61002400);
*/
   // burst transfer operation tests

   test_u8_burst_transfer   (axi_bus, 0x61002800, 64);
   
/*
   test_8_burst_transfer    (axi_bus, 0x61002C00, 64);
   test_u16_burst_transfer  (axi_bus, 0x61003000, 64);
   test_16_burst_transfer   (axi_bus, 0x61003400, 64);
   test_u32_burst_transfer  (axi_bus, 0x61003800, 64);
   test_32_burst_transfer   (axi_bus, 0x61003C00, 64);
   test_u64_burst_transfer  (axi_bus, 0x61004000, 64);
   test_64_burst_transfer   (axi_bus, 0x61004400, 64);

   test_float_burst_transfer  (axi_bus, 0x61004800, 64);
   test_double_burst_transfer (axi_bus, 0x61004C00, 64);
*/
   // burst corner cases
   
   // test_u8_burst_transfer (axi_bus, 0x61005001, 15); // misaligned start
/*
   test_u8_burst_transfer (axi_bus, 0x61005400, 15); // misaligned end
   test_u8_burst_transfer (axi_bus, 0x61005801, 14); // both ends misaligned
   test_u8_burst_transfer (axi_bus, 0x61005C01, 6);  // misaligned short burst

   test_u16_burst_transfer (axi_bus, 0x61006002, 15); // misaligned start
   test_u16_burst_transfer (axi_bus, 0x61006400, 15); // misaligned end
   test_u16_burst_transfer (axi_bus, 0x61006802, 14); // both ends misaligned
   test_u16_burst_transfer (axi_bus, 0x61006C02, 2);  // misaligned short burst

   test_u32_burst_transfer (axi_bus, 0x61007004, 15); // misaligned start
   test_u32_burst_transfer (axi_bus, 0x61007400, 15); // misaligned end
   test_u32_burst_transfer (axi_bus, 0x61007804, 14); // both ends misaligned
   test_u32_burst_transfer (axi_bus, 0x61007C04, 1);  // misaligned short burst
*/
   end_simulation(axi_bus);
}

int main()
{
   ac_channel<bool>       go;
   axi_master_interface   axi_bus;

   go.write(1);

   counter(go, axi_bus);
}
