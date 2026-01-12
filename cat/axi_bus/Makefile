GCC = g++
GCC_FLAGS = -std=c++11 -I ./include 
LD = g++

main: main.o axi_master_if.o
	$(LD) -o main main.o axi_master_if.o

main.o: main.cpp axi_master_if.h
	$(GCC) $(GCC_FLAGS) -c -o main.o main.cpp

axi_master_if.o: axi_master_if.cpp axi_master_if.h
	$(GCC) $(GCC_FLAGS) -c -o axi_master_if.o axi_master_if.cpp

clean:
	rm -rf main main.o axi_master_if.o
