#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

// Vivado does not support svGetScopeFromName
#ifndef VIVADO_SIM
//#include "svdpi.h"
#endif

//#include "sh_dpi_tasks.c"

#include "fpga_pci.h"
#include "fpga_mgmt.h"
#include "fpga_dma.h"

#define HBM_MEMORY (0x1000000000)
#define DDR_MEMORY (0)

void cat_poke(pci_bar_handle_t handle, unsigned long long addr, int data)
{
  int r;

  r = fpga_pci_poke(handle, addr, data);
printf("poke: addr: %10llx data; %08x \n", addr, data);
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
printf("peek: addr: %10llx data; %08x \n", addr, value);
  return value;
}

/*
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
    printf("writing to address %08llx value: %08x \n", address +i*2, (int) value); 
    sleep(1);
    printf("read back address  %08llx value: %08x \n", address +i*2, (int) cat_peek(mem, address + (unsigned long long) i*2));
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
*/

// int aws_dense(int num_inputs, int num_outputs, float *f, float *w, float *o) 
int main()
{
  pci_bar_handle_t csr_handle;
  pci_bar_handle_t mem_handle;
  int slot = 0;
  unsigned long long memory_type;
  //unsigned long long feature_address = 0;
  //unsigned long long weight_address = num_inputs * WORD_BYTES;
  //unsigned long long output_address = weight_address + num_inputs * num_outputs * WORD_BYTES;
  int r, i;
  int value;
  int done;
  int average;

  const bool chatty = true;

  if (chatty) printf(">>> starting... \n");

  r = fpga_pci_init();
  if (chatty) printf("pci_init: %d \n", r);
  if (r) return r;

  r = fpga_mgmt_init();
  if (chatty) printf("mgmt_init: %d \n", r);
  if (r) return r;

  // Get handle to Catapult CSRs
  r = fpga_pci_attach(slot, FPGA_MGMT_PF, MGMT_PF_BAR4, 0, &csr_handle);
  printf("pic_attach (CSR): %d \n", r);
  if (r) return r;

  // Get handle to FPGA memory (DDR, HBM)
  r = fpga_pci_attach(slot, FPGA_APP_PF, APP_PF_BAR4, 0, &mem_handle);
  if (chatty) printf("pic_attach (MEM): %d \n", r);
  if (r) return r;
  
  // select memory to use HBM is on-chip, DDR is on board
  //memory_type = HBM_MEMORY;
  printf(">>>-----> DDR memory test: \n");

  memory_type = DDR_MEMORY;

  for (i=0; i<20; i++) {
    r = fpga_pci_poke(mem_handle, DDR_MEMORY + i*4, i);
    if (r) {
      printf("bad status from poke: %d \n", r);
      exit(r);
    }
    printf("wrote: %08x @ %08x \n", i, i*4); 
  }

  for (i=0; i<20; i++) {
    r = fpga_pci_peek(mem_handle, DDR_MEMORY + i*4, &value);
    if (r) {
      printf("bad status from peek: %d \n", r);
      exit(r);
    }
    printf("read: %08x @ %08x \n", value, i*4);
  }

  printf(">>>-----> HBM memory test: \n");

  memory_type = HBM_MEMORY;

  for (i=0; i<20; i++) {
    r = fpga_pci_poke(mem_handle, HBM_MEMORY + i*4, i);
    if (r) {
      printf("bad status from poke: %d \n", r);
      exit(r);
    }
    printf("wrote: %08x @ %08x \n", i, i*4); 
  }

  for (i=0; i<20; i++) {
    r = fpga_pci_peek(mem_handle, HBM_MEMORY + i*4, &value);
    if (r) {
      printf("bad status from peek: %d \n", r);
      exit(r);
    }
    printf("read: %08x @ %08x \n", value, i*4);
  }
  printf(">>>-----> register test: \n");

  memory_type = HBM_MEMORY;

  for (i=0; i<20; i++) {
    r = fpga_pci_poke(csr_handle, i*4, i);
    if (r) {
      printf("bad status from poke: %d \n", r);
      exit(r);
    }
    printf("wrote: %08x @ %08x \n", i, i*4); 
  }

  for (i=0; i<20; i++) {
    r = fpga_pci_peek(csr_handle, i*4, &value);
    if (r) {
      printf("bad status from peek: %d \n", r);
      exit(r);
    }
    printf("read: %08x @ %08x \n", value, i*4);
  }
}
  
/*
  // copy weights to PCI card
  copy_to_cat(mem_handle, memory_type + weight_address, num_inputs * num_outputs, w);

  // copy features to PCI card
  copy_to_cat(mem_handle, memory_type + feature_address, num_inputs, f);

  // clear the output memory
  for (int i=0; i<num_outputs; i+=2) cat_poke(mem_handle, memory_type + output_address, 0);

  if (chatty) printf("HBM memory initialized \n");

  // set inputs
  cat_poke(csr_handle, CSR_USE_RELU, 0);
  cat_poke(csr_handle, CSR_ADDR_HI, memory_type >> 32);
  cat_poke(csr_handle, CSR_FEATURE_ADDR, feature_address);
  cat_poke(csr_handle, CSR_WEIGHT_ADDR,  weight_address);
  cat_poke(csr_handle, CSR_OUTPUT_ADDR,  output_address);
  cat_poke(csr_handle, CSR_IN_VECTOR_LENGTH, num_inputs);
  cat_poke(csr_handle, CSR_OUT_VECTOR_LENGTH, num_outputs);

  printf("checking parameters: \n");

  printf("use_relu: %d (%d) \n",         (int) cat_peek(csr_handle, CSR_USE_RELU), 0);
  printf("addr_hi: %d (%ld) \n",          (int) cat_peek(csr_handle, CSR_ADDR_HI), (unsigned long) (memory_type >> 32));
  printf("feature_address: %d (%ld) \n",  (int) cat_peek(csr_handle, CSR_FEATURE_ADDR), (unsigned long) (feature_address & 0xFFFFFFFF));
  printf("weight_address:: %d (%ld) \n",  (int) cat_peek(csr_handle, CSR_WEIGHT_ADDR), (unsigned long) (weight_address & 0xFFFFFFFF));
  printf("output_address: %d (%ld) \n",   (int) cat_peek(csr_handle, CSR_OUTPUT_ADDR), (unsigned long) (output_address & 0xFFFFFFFF));
  printf("num_inputs: %d (%d) \n",       (int) cat_peek(csr_handle, CSR_IN_VECTOR_LENGTH), num_inputs);
  printf("num_outputs: %d (%d) \n",      (int) cat_peek(csr_handle, CSR_OUT_VECTOR_LENGTH), num_outputs);

  if (chatty) printf("accelerator inputs set \n");

  printf(">>>> check inputs: ===========================================\n");
	  printf("inputs: "); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + feature_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + feature_address) >> 16));
	  printf("\n");

	  printf("weights:"); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + weight_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + weight_address) >> 16));
	  printf("\n");

	  printf("outputs:"); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + output_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + output_address) >> 16));
	  printf("\n");
  printf("=============================================================\n");

  // start computation
  while (0 == cat_peek(csr_handle, CSR_GO_READY));

  if (chatty) printf("accelerator ready \n");

  cat_poke(csr_handle, CSR_GO, 1);

  if (chatty) printf("accelerator engaged \n");

  // wait for completion
  while (0 == cat_peek(csr_handle, CSR_DONE_VALID)) {
	  printf("inputs: "); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + feature_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + feature_address) >> 16));
	  printf("\n");

	  printf("weights:"); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + weight_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + weight_address) >> 16));
	  printf("\n");

	  printf("outputs:"); 
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + output_address) & 0xFFFF));
	  printf("%8.4f ", (float) cat_to_float(cat_peek(mem_handle, memory_type + output_address) >> 16));
	  printf("\n");
	  
	  sleep(1);
  }

  if (chatty) printf("accelerator operation completed \n");

  done = cat_peek(csr_handle, CSR_DONE);

  if (chatty) printf("accelerator completion acknowledged \n");

  // copy features to PCI card
  copy_from_cat(mem_handle, memory_type + output_address, num_outputs, o);

  if (chatty) printf("accelerator results retrieved \n");

  if (chatty) printf(">>> aws_dense done \n");

  return 0;
}
*/
