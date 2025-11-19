SDK_INCLUDES = ~/aws-fpga/sdk/userspace/include

CC = gcc
CC_FLAGS  = -I $(SDK_INCLUDES) 
CC_FLAGS += -lfpga_mgmt

reg_test: reg_test.c
	$(CC) $< -o $@ $(CC_FLAGS) 

mac_test: mac_test.c
	$(CC) $< -o $@ $(CC_FLAGS) 

average_test: average_test.c
	$(CC) $< -o $@ $(CC_FLAGS) 

dense_test: dense_test.c
	$(CC) $< -o $@ $(CC_FLAGS) 
