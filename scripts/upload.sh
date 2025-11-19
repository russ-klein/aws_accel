#!/bin/bash

#
# Copy design to S3 storage, and creates bit-file
#

#
# Setup environment
#
BUCKET=russ-bucket-1234
REGION=us-east-1
TEMP_FILE=/tmp/temp_list.txt

#
# Upload to S3
#

if [ -z "$1" ]; then
   echo "Missing design name";
   exit 1
fi

echo "Uploading $1 "

pushd /home/ubuntu/aws-fpga/hdk/cl/examples/$1/build/checkpoints

# find the newest checkpoint

ls -t *.tar | head -1 > $TEMP_FILE

file_to_upload=$(cat /tmp/temp_list.txt)

#
# Create bitfile
#

AFI_FILE=$(date +%Y%m%d_%H%M%S.$1.txt)

echo aws ec2 create-fpga-image --name $1 \
        --input-storage-location Bucket=$BUCKET,Key=$1/$file_to_upload \
        --logs-storage-location Bucket=$BUCKET,Key=logfiles \
        --region $REGION

aws ec2 create-fpga-image --name $1 \
        --input-storage-location Bucket=$BUCKET,Key=$1/$file_to_upload \
        --logs-storage-location Bucket=$BUCKET,Key=logfiles \
        --region $REGION > ~/afi_files/$AFI_FILE

echo "AFI file created: "
cat ~/afi_files/$AFI_FILE

# clean up after yourself

rm -f $TEMP_FILE
popd

echo
echo "Design $1 uploaded and bitfile created "
echo
