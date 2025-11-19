#!/bin/bash


if [ -z "$1" ]; then
  echo "Missing design name"
  exit 1
fi

echo "Building $1"

export CL_DIR=/home/ubuntu/aws-fpga/hdk/cl/examples/$1

if [ -d $CL_DIR ]; then
  echo "Found directory: $CL_DIR"
else
  echo "Could not find: $CL_DIR"
  exit 2
fi

pushd $CL_DIR/build/scripts

echo
echo "Go get some coffee, this is going to take a while... "
echo

./aws_build_dcp_from_cl.py -c $1

echo "Build complete "

# clean up after yourself
#
popd
