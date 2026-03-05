
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

void print_axi_data(axi_master_interface &axi_bus, axi_u1024 data)
{
    int i;
    
    // advance to first non-zero hex digit
    for (i=0; i<254; i++) {
        if (data.slc<4>(1024-((i+1)*4)) != 0) break;
    }
    
    // print remaining digits to output stream
    
    for (int j=i; j<256; j++) {
        cat_putc(axi_bus, "0123456789ABCEDF"[data.slc<4>(1024-(i+1)*4)]);
    }
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
    axi_1024 expected,
    axi_1024 actual)
{
   print_string(axi_bus, "mismatch at index: "); print_int(axi_bus, index);
   print_string(axi_bus, " expected: "); print_axi_data(axi_bus, expected);
   print_string(axi_bus, " actual: "); print_axi_data(axi_bus, actual);
   print_string(axi_bus, "\r\n");
}

void prep_write(axi_master_interface &axi_bus, int n)
{
#ifdef C_SIMULATION

   // for C simulations, we need to have responses
   // pre-loaded into the b-channel
    
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
/*
 
   TODO: templatetize the 
 
   Does not work, need some more powerful c++ Voodoo
template <typename T>
int test_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address,
    int count)
{
   T write_array[count];
   T read_array[count];
   int errors = 0;
   const int size = write_array[0].length()/8;
   bool pass = true;
   bool verbose = false;

   print_string(axi_bus, "single transfer test...  ");
    printf("address=%08x, size=%d ", address.to_int(), size);

   for (int i=0; i<count; i++) write_array[i] = i;
    
   prep_write(axi_bus, count);

   for (int i=0; i<count; i+=size) axi_bus.write(address + i, (T) write_array[i]);

   // pass_data(axi_bus);

   for (int i=0; i<count; i+=size) axi_bus.read(address + i, read_array[i]);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}
*/

int test_u8_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u8 write_array[N];
   axi_u8 read_array[N];
   int errors = 0;
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
          errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}

int test_8_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_8 write_array[N];
   axi_8 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_u16_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u16 write_array[N];
   axi_u16 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_16_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_16 write_array[N];
   axi_16 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
   return errors;
}


int test_u32_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u32 write_array[N];
   axi_u32 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_32_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_32 write_array[N];
   axi_32 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_u64_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_u64 write_array[N];
   axi_u64 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_64_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_64 write_array[N];
   axi_64 read_array[N];
   int errors = 0;
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
         errors++;
         if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}

int test_float_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_float write_array[N];
   axi_float read_array[N];
   int errors = 0;
   bool pass = true;
   // bool verbose = false;

   print_string(axi_bus, "float single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_float) i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 4, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 4, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         errors++;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}


int test_double_single_transfer(
    axi_master_interface  &axi_bus,
    axi_address_type address)
{
   axi_double write_array[N];
   axi_double read_array[N];
   int errors = 0;
   bool pass = true;
   // bool verbose = false;

   print_string(axi_bus, "double (precision float) single transfer test...  ");

   for (int i=0; i<N; i++) write_array[i] = (axi_double) i;
   prep_write(axi_bus, N);

   for (int i=0; i<N; i++) axi_bus.write(address + i * 8, write_array[i]);

   pass_data(axi_bus); 

   for (int i=0; i<N; i++) axi_bus.read(address + i * 8, read_array[i]);

   for (int i=0; i<N; i++) {
      if (write_array[i] != read_array[i]) {
         pass = false;
         errors++;
         // if (verbose) report_difference(axi_bus, i, write_array[i], read_array[i]);
      }
   }
 
   print_string(axi_bus, pass ? "Passed \n\r" : "Failed \n\r");
    
   return errors;
}

//------------------------
// burst transfer tests
//------------------------

template <typename datatype>
int burst_transfer_test(
    axi_master_interface &axi_bus,
    axi_address_type address,
    int count,
    char *testname)
{
   datatype write_array[count];
   datatype read_array[count];
   bool verbose = true;
   int errors = 0;

   print_string(axi_bus, testname);
   print_string(axi_bus, " burst transfer test...  ");

   for (int i=0; i<count; i++) write_array[i] = i;
   prep_write(axi_bus, count);

   axi_bus.burst_write(address, write_array, count);

   // pass_data(axi_bus);

   axi_bus.burst_read(address, read_array, count);

   for (int i=0; i<count; i++) {
      if (write_array[i] != read_array[i]) {
          printf("as ints: expected: %x read: %x \n", write_array[i].to_int(), read_array[i].to_int());
          ac_int<1024, false> r, w;
          r = read_array[i];
          w = write_array[i];
          errors++;
          if (verbose) report_difference(axi_bus, i, w, r);
      }
   }

   print_string(axi_bus, (errors==0) ? "Passed \n\r" : "Failed \n\r");

   return errors;
}

/*
 
 TODO: enable burst transactions for float and double arrays
 
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
*/


#pragma hls_design top
void counter(
   ac_channel<bool>       &go,
   axi_master_interface   &axi_bus)
{
    go.read();

    int errors = 0;
    
    //print_string(axi_bus, "\n\n\n\rAXI master interface test program \n\n\r");
    
    // single access operation tests

    errors += test_u8_single_transfer (axi_bus, 0x61000000);
    errors += test_8_single_transfer  (axi_bus, 0x61000400);
    errors += test_u16_single_transfer(axi_bus, 0x61000800);
    errors += test_16_single_transfer (axi_bus, 0x61000C00);
    errors += test_u32_single_transfer(axi_bus, 0x61001000);
    errors += test_32_single_transfer (axi_bus, 0x61001400);
    errors += test_u64_single_transfer(axi_bus, 0x61001800);
    errors += test_64_single_transfer (axi_bus, 0x61001C00);

    errors += test_float_single_transfer  (axi_bus, 0x61002000);
    errors += test_double_single_transfer (axi_bus, 0x61002400);

    // burst transfer operation tests
       
    for (int i=1; i<100; i++) {
        for (int addr=0; addr<100; addr++) {
            printf("================================\n");
            printf(" Test: %d address: %d \n", i, addr);
            printf("================================\n");
                              errors += burst_transfer_test<axi_u8>   (axi_bus, addr, i, (char *)    "8 test");
            if (BUS_BITS > 0) errors += burst_transfer_test<axi_u16>  (axi_bus, addr, i, (char *)   "16 test");
            if (BUS_BITS > 1) errors += burst_transfer_test<axi_u32>  (axi_bus, addr, i, (char *)   "32 test");
            if (BUS_BITS > 2) errors += burst_transfer_test<axi_u64>  (axi_bus, addr, i, (char *)   "64 test");
            if (BUS_BITS > 3) errors += burst_transfer_test<axi_u128> (axi_bus, addr, i, (char *)  "128 test");
            if (BUS_BITS > 4) errors += burst_transfer_test<axi_u256> (axi_bus, addr, i, (char *)  "256 test");
            if (BUS_BITS > 5) errors += burst_transfer_test<axi_u512> (axi_bus, addr, i, (char *)  "512 test");
            if (BUS_BITS > 6) errors += burst_transfer_test<axi_u1024>(axi_bus, addr, i, (char *) "1024 test");
            if (errors > 0) break;
        }
        printf("================================\n");
        printf(" errors: %d \n", errors);
        printf("================================\n");
        if (errors>0)
            printf("we got problems \n");
    }
//    errors += burst_transfer_test<axi_u8>    (axi_bus, 0x00000007, 1, (char *) "axi_u8 (1) ");
//    errors += burst_transfer_test<axi_u8>    (axi_bus, 0x00000007, 2, (char *) "axi_u8 (2) ");
//    errors += burst_transfer_test<axi_u8>    (axi_bus, 0x00000007, 3, (char *) "axi_u8 (3) ");
//    errors += burst_transfer_test<axi_u16>   (axi_bus, 0x00000006, 1, (char *) "axi_u16 (1) ");
//    errors += burst_transfer_test<axi_u16>   (axi_bus, 0x00000006, 2, (char *) "axi_u16 (2) ");
//    errors += burst_transfer_test<axi_u16>   (axi_bus, 0x00000006, 3, (char *) "axi_u16 (3) ");
//    errors += burst_transfer_test<axi_u32>   (axi_bus, 0x00000004, 1, (char *) "axi_u32 (1) ");
//    errors += burst_transfer_test<axi_u32>   (axi_bus, 0x00000004, 2, (char *) "axi_u32 (2) ");
//    errors += burst_transfer_test<axi_u32>   (axi_bus, 0x00000004, 3, (char *) "axi_u32 (3) ");

//   errors += burst_transfer_test<axi_8>     (axi_bus, 0x61000000, 256, (char *) "axi_8");
//   if (BUS_BITS > 0) errors += burst_transfer_test<axi_u16>   (axi_bus, 0x61000000, 256, (char *) "axi_u16");
//   if (BUS_BITS > 0) errors += burst_transfer_test<axi_16>    (axi_bus, 0x61000000, 256, (char *) "axi_16");
//   if (BUS_BITS > 1) errors += burst_transfer_test<axi_u32>   (axi_bus, 0x61000000, 256, (char *) "axi_u32");
//   if (BUS_BITS > 1) errors += burst_transfer_test<axi_32>    (axi_bus, 0x61000000, 256, (char *) "axi_32");
//   if (BUS_BITS > 2) errors += burst_transfer_test<axi_u64>   (axi_bus, 0x61000000, 256, (char *) "axi_u64");
//   if (BUS_BITS > 2) errors += burst_transfer_test<axi_64>    (axi_bus, 0x61000000, 256, (char *) "axi_64");
//   if (BUS_BITS > 3) errors += burst_transfer_test<axi_u128>  (axi_bus, 0x61000000, 256, (char *) "axi_u128");
//   if (BUS_BITS > 3) errors += burst_transfer_test<axi_128>   (axi_bus, 0x61000000, 256, (char *) "axi_128");
//   if (BUS_BITS > 4) errors += burst_transfer_test<axi_u256>  (axi_bus, 0x61000000, 256, (char *) "axi_u256");
//   if (BUS_BITS > 4) errors += burst_transfer_test<axi_256>   (axi_bus, 0x61000000, 256, (char *) "axi_256");
//   if (BUS_BITS > 5) errors += burst_transfer_test<axi_u512>  (axi_bus, 0x61000000, 256, (char *) "axi_u512");
//   if (BUS_BITS > 5) errors += burst_transfer_test<axi_512>   (axi_bus, 0x61000000, 256, (char *) "axi_512");
//   if (BUS_BITS > 6) errors += burst_transfer_test<axi_u1024> (axi_bus, 0x61000000, 256, (char *) "axi_u1024");
//   if (BUS_BITS > 6) errors += burst_transfer_test<axi_1024>  (axi_bus, 0x61000000, 256, (char *) "axi_1024");

   printf("errors: %d ", errors);
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
    
   printf("test complete\n");
}
