// ============================================================================
// Amazon FPGA Hardware Development Kit
//
// Copyright 2024 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License"). You may not use
// this file except in compliance with the License. A copy of the License is
// located at
//
//    http://aws.amazon.com/asl/
//
// or in the "license" file accompanying this file. This file is distributed on
// an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, express or
// implied. See the License for the specific language governing permissions and
// limitations under the License.
// ============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Vivado does not support svGetScopeFromName
#ifndef VIVADO_SIM
//#include "svdpi.h"
#endif

//#include "sh_dpi_tasks.c"

#include "fpga_pci.h"
#include "fpga_mgmt.h"
#include "fpga_dma.h"

#define WRD_SIZE 16
#define INTEGER_BITS 8

#define INPUT_VECTOR_LENGTH 32 * 16
#define OUTPUT_VECTOR_LENGTH 64 *16

#define CSR_GO                 0
#define CSR_GO_READY           4
#define CSR_DONE               8
#define CSR_DONE_VALID        12
#define USE_RELU              16
#define ADDR_HI               20
#define FEATURE_ADDR          24
#define WEIGHT_ADDR           28
#define OUTPUT_ADDR           32
#define IN_VECTOR_LENGTH      36
#define OUT_VECTOR_LENGTH     40

#define HBM_MEMORY (1000000000)
#define DDR_MEMORY (0)

void cat_poke(pci_bar_handle_t handle, unsigned long long addr, int data)
{
  int r;

  r = fpga_pci_poke(handle, addr, data);
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
  return value;
}


float random_value(int n)
{
  int mask = (1 << (n - 4)) - 1;
  int denom = (1 << (WRD_SIZE - INTEGER_BITS));
  int value = rand() & mask;

  if (rand() & 1) value = value * -1;

  return (float) value/denom;
}

void random_fill(int n, float *array)
{
  for (int i=0; i<n; i++) array[i] = random_value(WRD_SIZE);
}
   
short float_to_cat(float x)
{
  return (short) (x * (1 << INTEGER_BITS));
}

void copy_to_cat(pci_bar_handle_t mem, long long address, int n, float *array)
{
  uint_32 value;

  for (int i=0; i<n; i+=2) {
    value = float_to_cat(array[i]);
    value += float_to_cat(array[i+1]) << 16;
    cat_write(mem, address + i * 4, value);
  }
} 


int main() {

  // Register Bank Test

  pci_bar_handle_t csr_handle;
  pci_bar_handle_t mem_handle;
  int slot = 0;
  long long address;
  int r, i;
  int value;
  int done;
  int average;

  static float weights[INPUT_VECTOR_LENGTH * OUTPUT_VECTOR_LENGTH];
  static float features[INPUT_VECTOR_LENGTH];
  static float outputs[OUTPUT_VECTOR_LENGTH];

  random_fill(INPUT_VECTOR_LENGTH * OUTPUT_VECTOR_LENGTH, weights);
  random_fill(INPUT_VECTOR_LENGTH, features);

  printf(">>> starting... \n");

  r = fpga_pci_init();
  printf("pci_init: %d \n", r);
  if (r) return r;
  

  r = fpga_mgmt_init();
  printf("mgmt_init: %d \n", r);
  if (r) return r;

  // Get handle to Catapult CSRs
  r = fpga_pci_attach(slot, FPGA_MGMT_PF, MGMT_PF_BAR4, 0, &csr_handle);
  printf("pic_attach (CSR): %d \n", r);
  if (r) return r;

  // Get handle to FPGA memory (DDR, HBM)
  r = fpga_pci_attach(slot, FPGA_APP_PF, APP_PF_BAR4, 0, &mem_handle);
  printf("pic_attach (MEM): %d \n", r);
  if (r) return r;

  address = HBM_MEMORY;
  //address = DDR_MEMORY;

  // copy weights to PCI card
  copy_to_cat(mem_handle, weight_address, weights, INPUT_VECTOR_SIZE * OUTPUT_VECTOR_SIZE);

  // copy features to PCI card
  copy_to_cat(mem_handle, feature_address, features, INPUT_VECTOR_SIZE);

  printf("HBM memory initialized \n");

  for (i=0; i<0x10; i++) {
    printf("hbm[%3d] = %08x \n", i, (unsigned int) cat_peek(mem_handle, address + i * 4));
  }

  cat_poke(mem_handle, 0, 32);

  value = 0;

  cat_poke(csr_handle, CSR_COUNT, 1);

  printf("accelerator inputs set \n");

  while (0 == cat_peek(csr_handle, CSR_GO_READY));

  printf("accelerator ready \n");

  cat_poke(csr_handle, CSR_GO, 1);

  printf("accelerator engaged \n");

  while (0 == cat_peek(csr_handle, CSR_DONE_VALID));

  printf("accelerator operation completed \n");

  done = cat_peek(csr_handle, CSR_DONE);

  printf("accelerator completion acknowledged \n");

  average = cat_peek(csr_handle, CSR_RESULT);

  printf("accelerator results retrieved \n");

  printf("average of 1 through 10 is: %d \n", average);

  printf(">>> ending \n");
  return 0;
}
