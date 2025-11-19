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

int main() {

  // Register Bank Test

  pci_bar_handle_t handle;
  int r, i;
  int value;
  int slot = 0;

  printf(">>> starting... \n");

  r = fpga_pci_init();
  printf("pci_init: %d \n", r);
  if (r) return r;
  

  r = fpga_mgmt_init();
  printf("mgmt_init: %d \n", r);
  if (r) return r;

  r = fpga_pci_attach(slot, FPGA_MGMT_PF, MGMT_PF_BAR4, 0, &handle);
  printf("pic_attach: %d \n", r);
  if (r) return r;

  for (int i=0; i<16; i++) {
    r = fpga_pci_peek(handle, i*4, &value);
    printf("pci_peek: %d \n", r);
    if (r) return r;
    printf("register read at %d received 0x%08x \n", i, value);
  }

  for (int i=8; i<16; i++) {
    r = fpga_pci_poke(handle, i*4, 0x100 + i);
    printf("pci_poke: %d \n", r);
    if (r) return r;
    printf("register read at %d received 0x%08x \n", i, value);
  }

  for (int i=8; i<16; i++) {
    r = fpga_pci_peek(handle, i*4, &value);
    printf("pci_peek: %d \n", r);
    if (r) return r;
    printf("register read at %d received 0x%08x \n", i, value);
  }

  printf(">>> ending \n");
  return 0;
}
