
#ifndef __AXI_MASTER_IF_H_INCLUDED__
#define __AXI_MASTER_IF_H_INCLUDED__

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_std_float.h>
#include <ac_channel.h>
#include <ac_wait.h>

#include "axi_bus_defines.h"

/* 
 * Things I always need to look up about AXI
 *
 * LEN is the length of a burst - 1:
 *      0 is a 1 beat burst, 
 *      1 is a 2 beat burst, etc
 *
 * BURST is the type of burst cycle
 *      00 = fixed (always same address)
 *      01 = increment (start at lowest address and increment)
 *      10 = wrap (start at address and wrap on cache line boundary,
 *                 delivers cache miss value first)
 *      11 = reserved (don't use it)
 *
 * SIZE is the size of the read on the bus of 8 * 2^size bits
 *      0 = 8 bits
 *      1 = 16 bits
 *      2 = 32 bits
 *      3 = 64 bits, etc
 *
 * RESP 
 *      00 = OK
 *      01 = exclusive OK 
 *      10 = slave error
 *      11 = decode error (no such address)
 */

typedef struct {
    ac_int<QOS_BITS,      false>    qos;
    ac_int<REGION_BITS,   false>    region;
    ac_int<PROT_BITS,     false>    prot;
    ac_int<CACHE_BITS,    false>    cache;
    ac_int<LOCK_BITS,     false>    lock;
    ac_int<BURST_BITS,    false>    burst;
    ac_int<SIZE_BITS,     false>    size;
    ac_int<LEN_BITS,      false>    len;
    ac_int<ADDRESS_BITS,  false>    address;
    ac_int<READ_ID_BITS,  false>    id;
} ar_payload;

typedef struct {
    ac_int<QOS_BITS,      false>    qos;
    ac_int<REGION_BITS,   false>    region;
    ac_int<PROT_BITS,     false>    prot;
    ac_int<CACHE_BITS,    false>    cache;
    ac_int<LOCK_BITS,     false>    lock;
    ac_int<BURST_BITS,    false>    burst;
    ac_int<SIZE_BITS,     false>    size;
    ac_int<LEN_BITS,      false>    len;
    ac_int<ADDRESS_BITS,  false>    address;
    ac_int<READ_ID_BITS,  false>    id;
} aw_payload;

typedef struct {
    ac_int<1,             false>    last;
    ac_int<BYTE_BITS,     false>    strb;
    ac_int<BUS_SIZE,      false>    data;
} w_payload;

typedef struct {
    ac_int<1,             false>    last;
    ac_int<RESP_BITS,     false>    resp;
    ac_int<BUS_SIZE,      false>    data;
    ac_int<READ_ID_BITS,  false>    id;
} r_payload;

typedef struct {
    ac_int<RESP_BITS,     false>    resp;
    ac_int<WRITE_ID_BITS, false>    id;
} b_payload;

typedef struct {
    ac_channel<aw_payload>  *addr_channel;
    ac_channel<w_payload>   *data_channel;
    ac_channel<b_payload>   *ack_channel;
} write_struct;

typedef struct {
    ac_channel<ar_payload>  *addr_channel;
    ac_channel<r_payload>   *data_channel;
} read_struct;

typedef struct {
    write_struct            *write;
    read_struct             *read;
} axi_bus_struct_deep;

typedef struct {
    ac_channel<aw_payload>  aw_channel;
    ac_channel<w_payload>   w_channel;
    ac_channel<b_payload>   b_channel;
    ac_channel<ar_payload>  ar_channel;
    ac_channel<r_payload>   r_channel;
} axi_bus_signals;


// type definitions to simplify code

typedef ac_int<8, true>              axi_8;
typedef ac_int<8, false>             axi_u8;
typedef ac_int<16, true>             axi_16;
typedef ac_int<16, false>            axi_u16;
typedef ac_int<32, true>             axi_32;
typedef ac_int<32, false>            axi_u32;
typedef ac_int<64, true>             axi_64;
typedef ac_int<64, false>            axi_u64;
typedef ac_ieee_float32              axi_float;
typedef ac_ieee_float64              axi_double;

typedef ac_int<WORD_SIZE, false>     axi_base_data_type;
typedef ac_int<BUS_SIZE, false>      axi_data_type;
typedef ac_int<ADDRESS_BITS, false>  axi_address_type;
typedef ac_int<RESP_BITS, false>     axi_resp_type;
typedef ac_int<16, false>            axi_size_type;   // maximum bytes to transfer on a bulk read/write
                                                      // set to arbitrary size, not axi protocol specific

// Page size = 4K, this is used to prevent bursts from crossing 4K boundary

#define PAGE_SIZE     ((axi_address_type) 0x1000)
#define PAGE_MASK     ((PAGE_SIZE) - 1)

class axi_master_interface {

private:
    long long   csim_r_address;
    long        csim_r_size;
    long        csim_r_count;
    long        csim_r_id;
    long long   csim_w_address;
    long        csim_w_size;
    long        csim_w_count;
    long        csim_w_id;
    

public:
    axi_bus_signals channels;
    bool aligned;

    ac_int<64, false> debug_signal;

    axi_master_interface();
    ~axi_master_interface();

    ac_int<SIZE_BITS, false> encode_size(const int);
    ac_int<BYTE_BITS, false> encode_strb(const int, ac_int<BUS_BITS, false>);

    void send_aw(aw_payload &aw);
    void send_w (w_payload  &w);
    void get_b  (b_payload  &b);
    void send_ar(ar_payload &ar);
    void get_r  (r_payload  &r);

    void tb_memory_write(long long address, int count, unsigned char *data);
    void tb_memory_read (long long address, int count, unsigned char *data);

    axi_data_type csim_memory_read(axi_address_type address, int size);
    void csim_memory_write(axi_address_type address, int size, axi_data_type data);

    ac_int<BUS_BITS+1, false> ones(ac_int<BYTE_BITS, false> bytes);

    unsigned char *memory_store_index[0x10000];

    void memory_store_byte_write(long long address, unsigned char data);
    unsigned char memory_store_byte_read(long long address);

    template <typename datatype, int size>
    axi_resp_type axi_burst_read_base(axi_address_type address, datatype *data_out, axi_size_type count);

    template <typename datatype, int size>
    axi_resp_type axi_burst_read_real_base(axi_address_type address, datatype *data_out, axi_size_type count);

    template <typename datatype, int size>
    axi_resp_type axi_read_base(axi_address_type address, datatype &data_in);

    template <typename datatype, int size>
    axi_resp_type axi_read_real(axi_address_type address, datatype &data_in);

    template <typename datatype, int size>
    axi_resp_type axi_burst_write_base(axi_address_type address, datatype *data_out, axi_size_type count);

    template <typename datatype, int size>
    axi_resp_type axi_burst_write_real_base(axi_address_type address, datatype *data_out, axi_size_type count);

    template <typename datatype, int size>
    axi_resp_type axi_write_base(axi_address_type address, datatype data_out);

    template <typename datatype, int size>
    axi_resp_type axi_write_real(axi_address_type address, datatype data_out);

    axi_resp_type read(axi_address_type address, axi_8       &data_in);
    axi_resp_type read(axi_address_type address, axi_u8      &data_in);
    axi_resp_type read(axi_address_type address, axi_16      &data_in);
    axi_resp_type read(axi_address_type address, axi_u16     &data_in);
    axi_resp_type read(axi_address_type address, axi_32      &data_in);
    axi_resp_type read(axi_address_type address, axi_u32     &data_in);
    axi_resp_type read(axi_address_type address, axi_64      &data_in);
    axi_resp_type read(axi_address_type address, axi_u64     &data_in);
    axi_resp_type read(axi_address_type address, axi_float   &data_in);
    axi_resp_type read(axi_address_type address, axi_double  &data_in);

    axi_resp_type write(axi_address_type address, axi_8       data_out);
    axi_resp_type write(axi_address_type address, axi_u8      data_out);
    axi_resp_type write(axi_address_type address, axi_16      data_out);
    axi_resp_type write(axi_address_type address, axi_u16     data_out);
    axi_resp_type write(axi_address_type address, axi_32      data_out);
    axi_resp_type write(axi_address_type address, axi_u32     data_out);
    axi_resp_type write(axi_address_type address, axi_64      data_out);
    axi_resp_type write(axi_address_type address, axi_u64     data_out);
    axi_resp_type write(axi_address_type address, axi_float   data_out);
    axi_resp_type write(axi_address_type address, axi_double  data_out);

    axi_resp_type burst_read (axi_address_type address, axi_8   *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_u8  *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_16  *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_u16 *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_32  *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_u32 *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_64  *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_u64 *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_float *data, axi_size_type size);
    axi_resp_type burst_read (axi_address_type address, axi_double *data, axi_size_type size);

    axi_resp_type burst_write(axi_address_type address, axi_8   *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_u8  *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_16  *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_u16 *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_32  *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_u32 *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_64  *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_u64 *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_float *data, axi_size_type size);
    axi_resp_type burst_write(axi_address_type address, axi_double *data, axi_size_type size);
};

#endif
