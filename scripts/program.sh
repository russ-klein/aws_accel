#!/bin/bash

if [-z "$1"]; then
  echo "Missing design name"
  exit 1
fi

if [-z "$2"]; then
  SLOT=0
else
  SLOT=$2
fi

ls -t ~/afgi_files/*.$1.txt | head -1 > $TEMP_FILE
agfi_line = $(grep temp_file agfi)
AGFI = $(echo $agfi_line | sed 's/.*\://')

echo "AFGI for $1 is $AFGI"

echo "Programming $1 to FPGA device in slot $SLOT"

sudo fpga-clear-local-image -S $SLOT

sudo fpga-load-local-image -S $SLOT -I $AGFI
