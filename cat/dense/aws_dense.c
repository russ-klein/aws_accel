#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "defines.h"

#include "timer.h"

// Vivado does not support svGetScopeFromName
#ifndef VIVADO_SIM
//#include "svdpi.h"
#endif

//#include "sh_dpi_tasks.c"

#include "fpga_pci.h"
#include "fpga_mgmt.h"
//#include "fpga_dma.h"

// #define WRD_SIZE 16
// #define INTEGER_BITS 8
//
// #define INPUT_VECTOR_LENGTH 32 * 16
// #define OUTPUT_VECTOR_LENGTH 64 *16
//
#define CSR_GO                     0
#define CSR_GO_READY               4
#define CSR_DONE                   8
#define CSR_DONE_VALID            12
#define CSR_USE_RELU              16
#define CSR_ADDR_HI               20
#define CSR_FEATURE_ADDR          24
#define CSR_WEIGHT_ADDR           28
#define CSR_OUTPUT_ADDR           32
#define CSR_IN_VECTOR_LENGTH      36
#define CSR_OUT_VECTOR_LENGTH     40
#define CSR_DEBUG                 44


#define HBM_MEMORY (0x1000000000)
#define DDR_MEMORY (0)

void cat_poke(pci_bar_handle_t handle, unsigned long long addr, int data)
{
  int r;

  r = fpga_pci_poke(handle, addr, data);
//printf("poke: addr: %10llx data; %08x \n", addr, data);
  if (r) {
     printf("bad status from poke: %d \n", r);
     exit(r);
  }
}

unsigned long cat_peek(pci_bar_handle_t handle, unsigned long long addr)
{
  int r;
  unsigned int value;

  r = fpga_pci_peek(handle, addr, &value);
  if (r) {
     printf("bad status from peek: %d \n", r);
     exit(r);
  }
//printf("peek: addr: %10llx data; %08x \n", addr, value);
  return value;
}


short float_to_cat(float x)
{
  short r;
  r = (x * (1 << FRACTIONAL_BITS));
  //printf("%04x = %8.4f \n", r, x);
  return (short) (x * (1 << FRACTIONAL_BITS));
}

float cat_to_float(short x)
{
  return (float) ((float) x / (float) (1 << FRACTIONAL_BITS));
}

void copy_to_cat(pci_bar_handle_t mem, unsigned long long address, int n, float *array)
{
  unsigned long value;

  for (int i=0; i<n; i+=2) {
    value = float_to_cat(array[i]);
    value += float_to_cat(array[i+1]) << 16;
    cat_poke(mem, address + i * 2, value);
  }
}


void copy_from_cat(pci_bar_handle_t mem, unsigned long long address, int n, float *array)
{
  unsigned long value;

  for (int i=0; i<n; i+=2) {
    value = cat_peek(mem, address + i * 4);
    array[i] = cat_to_float(value & 0xFFFF);
    array[i+1] += cat_to_float(value >> 16);
  }
}


int aws_dense(int num_inputs, int num_outputs, float *f, float *w, float *o) 
{
  pci_bar_handle_t csr_handle;
  pci_bar_handle_t mem_handle;
  int slot = 0;
  unsigned long long memory_type;
  unsigned long long feature_address = 0;
  unsigned long long weight_address = num_inputs * WORD_BYTES;
  unsigned long long output_address = weight_address + num_inputs * num_outputs * WORD_BYTES;
  int r, i;
  int value;
  int done;
  int average;

  typedef unsigned long long pci_address_t;

  const pci_address_t hbm_memory = 0x1000000000;
  const pci_address_t ddr_memory = 0;

  const bool chatty = true;

  if (chatty) printf(">>> starting... \n");

  r = fpga_pci_init();
  if (chatty) printf("pci_init: %d \n", r);
  if (r) return r;

  r = fpga_mgmt_init();
  if (chatty) printf("mgmt_init: %d \n", r);
  if (r) return r;

  // Get handle to Catapult CSRs
  if (chatty) r = fpga_pci_attach(slot, FPGA_MGMT_PF, MGMT_PF_BAR4, 0, &csr_handle);
  printf("pci_attach (CSR): %d \n", r);
  if (r) return r;

  // Get handle to FPGA memory (DDR, HBM)
  r = fpga_pci_attach(slot, FPGA_APP_PF, APP_PF_BAR4, 0, &mem_handle);
  if (chatty) printf("pci_attach (MEM): %d \n", r);
  if (r) return r;
  
  // select memory to use HBM is on-chip, DDR is on board
  // memory_type = HBM_MEMORY;
  memory_type = DDR_MEMORY;

  weight_address  += memory_type;
  feature_address += memory_type;
  output_address  += memory_type;

  unsigned long long test_addr = output_address;
  for (int i=0; i<16; i++) cat_poke(mem_handle, test_addr + i * 4, i);

  /*
    printf(">>>>>-----> ");
    for (int i=0; i<16; i++) printf("%08x ", cat_peek(mem_handle, test_addr + i * 4));
    printf("\n");
  */

  // copy weights to PCI card
  copy_to_cat(mem_handle, weight_address, num_inputs * num_outputs, w);

  // copy features to PCI card
  copy_to_cat(mem_handle, feature_address, num_inputs, f);

  // read pattern from DDR memory
  // for (int i=0; i<10; i++) printf("%08x ", cat_peek(mem_handle, memory_type + i*4));
  // printf("\n");

  // clear the output memory
  for (int i=0; i<num_outputs; i+=2) cat_poke(mem_handle, output_address, 0);

  if (chatty) printf("HBM memory initialized \n");

  // printf("output area before: ");
  // for (int i=0; i<16; i++) printf("%08x ", cat_peek(mem_handle, output_address + i*4));
  // printf("\n");

  // set inputs
  cat_poke(csr_handle, CSR_USE_RELU, 0);
  cat_poke(csr_handle, CSR_ADDR_HI, memory_type >> 32);
  cat_poke(csr_handle, CSR_FEATURE_ADDR, feature_address);
  cat_poke(csr_handle, CSR_WEIGHT_ADDR,  weight_address);
  cat_poke(csr_handle, CSR_OUTPUT_ADDR,  output_address);
  cat_poke(csr_handle, CSR_IN_VECTOR_LENGTH, num_inputs);
  cat_poke(csr_handle, CSR_OUT_VECTOR_LENGTH, num_outputs);

  printf("checking parameters: \n");

  printf("use_relu: %d (%d) \n",          (int) cat_peek(csr_handle, CSR_USE_RELU), 0);
  printf("addr_hi: %d (%ld) \n",          (int) cat_peek(csr_handle, CSR_ADDR_HI), (unsigned long) (memory_type >> 32));
  printf("feature_address: %d (%ld) \n",  (int) cat_peek(csr_handle, CSR_FEATURE_ADDR), (unsigned long) (feature_address & 0xFFFFFFFF));
  printf("weight_address:: %d (%ld) \n",  (int) cat_peek(csr_handle, CSR_WEIGHT_ADDR), (unsigned long) (weight_address & 0xFFFFFFFF));
  printf("output_address: %d (%ld) \n",   (int) cat_peek(csr_handle, CSR_OUTPUT_ADDR), (unsigned long) (output_address & 0xFFFFFFFF));
  printf("num_inputs: %d (%d) \n",        (int) cat_peek(csr_handle, CSR_IN_VECTOR_LENGTH), num_inputs);
  printf("num_outputs: %d (%d) \n",       (int) cat_peek(csr_handle, CSR_OUT_VECTOR_LENGTH), num_outputs);
  printf("debug: %x \n",                  (int) cat_peek(csr_handle, CSR_DEBUG));

  if (chatty) printf("accelerator inputs set \n");

  // start computation

  cat_poke(csr_handle, CSR_GO, 1);
 
  if (chatty) printf("waiting for GO ready... \n");

  timer_start();

  while (0 == cat_peek(csr_handle, CSR_GO_READY));

  printf("Time for FPGA: %8.3f milliseconds \n", (float) timer_stop()/1000.0);

  if (chatty) printf("accelerator ready \n");

  cat_poke(csr_handle, CSR_GO, 1);

  if (chatty) printf("accelerator engaged \n");

  // wait for completion
  while (0 == cat_peek(csr_handle, CSR_DONE_VALID)) {
	  printf("debug = %d \n", cat_peek(csr_handle, CSR_DEBUG)); 
	  sleep(1);
  }

  if (chatty) printf("accelerator operation completed \n");

  done = cat_peek(csr_handle, CSR_DONE);

  if (chatty) printf("accelerator completion acknowledged \n");
  
  // copy features to PCI card
  copy_from_cat(mem_handle, output_address, num_outputs, o);

  if (chatty) printf("accelerator results retrieved \n");

  if (chatty) printf(">>> aws_dense done \n");

  for (int i=0; i<16; i++) printf("%08x ", cat_peek(mem_handle, output_address + i*4));
  printf("\n");

  return 0;
}
