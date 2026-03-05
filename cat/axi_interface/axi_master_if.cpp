
#include "axi_master_if.h"

axi_master_interface::axi_master_interface() {};

axi_master_interface::~axi_master_interface() {};

ac_int<SIZE_BITS, false>  axi_master_interface::encode_size(const int n)
{
   if (n == 8)    return 0;
   if (n == 16)   return 1;
   if (n == 32)   return 2;
   if (n == 64)   return 3;
   if (n == 128)  return 4;
   if (n == 256)  return 5;
   if (n == 512)  return 6;
   if (n == 1024) return 7;
   return 0;
}

ac_int<BYTE_BITS, false>  axi_master_interface::encode_strb(const int n, ac_int<BUS_BITS, false> low_bits)
{
   ac_int<BYTE_BITS, false> r = 0;
    
  #pragma hls_unroll
    for (int i=0; i<BYTE_BITS; i++) {
        if (i<(n>>3)) {
            r.set_slc(i, (ac_int<1, false>) 1);
        }
    }
    
   return r << low_bits;
}

void axi_master_interface::tb_memory_write(long long address, int count, unsigned char *data)
{
   for (int i=0; i<count; i++) memory_store_byte_write(address+i, data[i]);
}

void axi_master_interface::tb_memory_read(long long address, int count, unsigned char *data)
{
   for (int i=0; i<count; i++) data[i] = memory_store_byte_read(address+i);
}  

void axi_master_interface::memory_store_byte_write(long long address, unsigned char data)
{
   unsigned char   *memory_page;

   if ((memory_store_index[(address>>16) & 0xFFFF]) == NULL) {
     memory_store_index[(address>>16) & 0xFFFF] = (unsigned char *) malloc(0x10000 * sizeof(unsigned char *));
   }
   memory_page = memory_store_index[(address>>16) & 0xFFFF];

   memory_page[address & 0xFFFF] = data;
}

unsigned char axi_master_interface::memory_store_byte_read(long long address)
{
   unsigned char   *memory_page;

   if ((memory_store_index[(address>>16) & 0xFFFF]) == NULL) return 0;

   memory_page = memory_store_index[(address>>16) & 0xFFFF];

   return memory_page[address & 0xFFFF];
}

char *string_axi_data(axi_u1024 *data, char *buffer)
{
    int i, j;
    
    for (i=0; i<255; i++) {
        if (data->slc<4>(1024-((1+i)*4)) != 0) break;
    }

    for (j=0; j<256-i; j++) {
        buffer[j] = "0123456789ABCDEF"[data->slc<4>(1024-((1+i+j)*4))];
    }
    buffer[j] = 0;

    return buffer;
}

void axi_master_interface::csim_memory_read(axi_address_type address, long size, axi_data_type *data)
{
   unsigned char byte_value;
   const bool chatty = false;
   *data = (axi_data_type) 0;

   if (aligned) {
     for (int byte=0; byte<size; byte++) {
       byte_value = memory_store_byte_read((long) address + byte);
       for (int bit=0; bit<8; bit++) data[byte * 8 + bit] = (byte_value >> bit) & 1;
     } 
   } else {
     int offset = (int) (address & ADDR_LOW_BITS_MASK);
     for (int byte=offset; byte<offset + size; byte++) {
       byte_value = memory_store_byte_read(address - offset + byte);
         data->set_slc((axi_u16) byte*8, (axi_u8) byte_value);
     }
   }

    if (chatty) {
        char buffer[(BUS_SIZE/4)+1];
        axi_u1024 wide_data = *data;
        printf("csim_memory_read: address: %08llx size: %ld data: %s (bits) \n", address.to_int64(), size*WORD_SIZE, string_axi_data(&wide_data, buffer));
    }

   return;
}
       
void axi_master_interface::csim_memory_write(axi_address_type address, long size, axi_data_type *data, ac_int<BYTE_BITS, false> strb)
{
    const bool chatty = false;
    
    if (chatty) {
        printf("csim_memory_write: address: %08llx size: %ld (bits) ", address.to_int64(), size*WORD_SIZE);
        for (int i=size-1; i>=0; i--) {
            if (strb[i]) printf("%02x", data->slc<8>(i*8).to_int());
            else printf("--");
        }
        printf("\n");
    }
    for (int byte=0; byte<size; byte++) {
        if (strb[byte]) memory_store_byte_write(address + byte, data->slc<8>(byte*8));
    }
}


void axi_master_interface::send_ar(ar_payload &ar)
{
#ifdef C_SIMULATION
   csim_r_address = ar.address;
   csim_r_size    = (1<<ar.size);
   csim_r_count   = ar.len + 1;
   csim_r_id      = ar.id;
#else
   channels.ar_channel.write(ar);
#endif
}
   
void axi_master_interface::get_r(r_payload &r)
{
#ifdef C_SIMULATION
    axi_address_type a = csim_r_address;
   csim_memory_read(a, csim_r_size, &r.data);
   r.last = csim_r_count == 1;
   r.resp = 0;
   r.id   = csim_r_id;
   csim_r_count--;
   csim_r_address += csim_r_size;
#else
   channels.r_channel.read(r);
#endif
}
   
void axi_master_interface::send_aw(aw_payload &aw)
{
#ifdef C_SIMULATION
   csim_w_address = aw.address;
   csim_w_size    = (1<<aw.size);
   csim_w_count   = aw.len + 1;
   csim_w_id      = aw.id;
#else
   channels.aw_channel.write(aw);
#endif
}
   
void axi_master_interface::send_w(w_payload &w)
{
#ifdef C_SIMULATION
    axi_address_type a = csim_w_address;
    csim_memory_write(a, csim_w_size, &w.data, w.strb);
    w.last = csim_w_count == 1;
    csim_w_count--;
    csim_w_address += csim_w_size;
#else
    channels.w_channel.write(w);
#endif
}
   
void axi_master_interface::get_b(b_payload &b)
{
#ifdef C_SIMULATION
   b.resp = 0;
#else
   channels.b_channel.read(b);
#endif
} 

axi_address_type min(axi_address_type a, axi_address_type b, axi_address_type c)
{
   if ((a<=b) && (a<=c)) return a;
   if ((b<=a) && (b<=c)) return b;
   if ((c<=a) && (c<=b)) return c;

   return -1;
}

ac_int<BUS_BITS+1, false> axi_master_interface::ones(ac_int<BYTE_BITS, false> bytes)
{
   ac_int<BUS_BITS+1, false> sum = 0;

   for (int i=0; i<BYTE_BITS; i++) {
      sum += bytes.slc<1>(i);
   }

   return sum;
}

ac_int<BYTE_BITS, false> axi_master_interface::set_strb(
    const axi_size_type start_offset,
    const axi_size_type end_offset,
    bool first,
    bool last)
{
    if (first & last) {
        ac_int<BYTE_BITS, false> r;
        axi_size_type e = end_offset ? end_offset : (axi_size_type) BUS_BYTES;
        
        r = BYTE_MASK;
        r = r << (start_offset + (BUS_BYTES - e));
        r = r >> (BUS_BYTES - e);
        return r;
    }
    
    if (first) {
        return (BYTE_MASK << start_offset);
    }
    
    if (last) {
        axi_size_type e = end_offset ? end_offset : (axi_size_type) BUS_BYTES;
        return (BYTE_MASK >> (BUS_BYTES - e));
    }
    return(BYTE_MASK);
}


template <typename datatype, int bit_shift>
axi_resp_type axi_master_interface::axi_burst_read_base(
       axi_address_type   address,
       datatype          *data_in,
       axi_size_type      count)
{
    ar_payload ar;
    r_payload r;

    const int size_bytes = (1 << bit_shift);
    const int size_bits  = (size_bytes << 3);
    const axi_size_type byte_count = count * size_bytes;

    const axi_address_type   address_mask  = ADDR_HIGH_BITS_MASK;
    const axi_address_type   first_line    = (address & address_mask);                       // byte address of first line to write
    const axi_address_type   last_line     = ((address + byte_count - 1) & address_mask);    // byte address of last line to write
    
    const bool start_aligned  = (address == first_line);
    const axi_size_type lines = ((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1;    // number of lines in the transfer
    const axi_size_type start_delta = address - first_line;

    // const bool chatty         = false;
    
    // const axi_size_type word_size       = size_bits >> 3;           // word size in bytes
    const axi_size_type words_per_line  = BUS_SIZE / size_bits;     // STRIDE is bytes per line
    const axi_size_type bytes_per_word  = size_bits / WORD_SIZE;
    const axi_size_type offset          = (axi_size_type) (address & ADDR_LOW_BITS_MASK);
    const axi_size_type bit_offset      = offset << 3;


    axi_size_type     bytes_recv        = 0;
    axi_size_type     lines_received    = 0;
    axi_resp_type     resp              = 0;
    axi_size_type     burst_lines_recv;
    axi_address_type  max_burst;
    axi_size_type     burst_size;
    axi_size_type     base;
    axi_data_type     old_data;
    axi_address_type  burst_end;
    axi_address_type  base_word;
    axi_address_type  burst_start;
    axi_address_type  four_k_boundary;

    bool first = true;

    r.data = 0;
   
    if (size_bits == 0) return resp;

    burst_start = first_line;
    base_word   = first_line;

    base = 0;

    do {
        four_k_boundary = (((address + bytes_recv) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
        max_burst       = (address + bytes_recv) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
        burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

        burst_size = (burst_end - burst_start) >> BUS_BITS;
       
        bool last_burst = (burst_end == last_line);
      
        // set read address payload

        ar.address  = (address + bytes_recv) & ADDR_HIGH_BITS_MASK;
        ar.id       = 4; // any number
        ar.len      = burst_size;
        ar.size     = BUS_BITS; // full bus width
        ar.burst    = 1; // incrementing
        ar.lock     = 0; // unlocked
        ar.cache    = 0; // uncached
        ar.prot     = 0; // no protection
        ar.qos      = 0; // normal qos

        send_ar(ar);

        burst_lines_recv = 0;

        axi_size_type flush_cycle = (last_burst && !start_aligned) ? 1 : 0;

       #pragma hls_pipeline_init_interval 1
        for (int i=0; i<burst_size+1+flush_cycle; i++) {
            bool last_beat = (bytes_recv + BUS_BYTES - start_delta) >= byte_count;

            if (burst_lines_recv < burst_size+1) {
                get_r(r);
                burst_lines_recv++;
                if (first) old_data = r.data;
            }
           
            bool skip_load = (first && !start_aligned && !last_beat);
           
            if (!skip_load) {
               #pragma hls_unroll
                data_load: for (int b=0; b<BUS_BYTES; b++) {  // for each byte lane
                    int bit = b<<3;
                    if ((base + b) < byte_count) { // if not past then end of the array
                        axi_size_type index = (base + b) >> bit_shift;
                        axi_size_type slice = (b & (ADDR_LOW_BITS_MASK >> (BUS_BITS - bit_shift))) << 3;
                        if (start_aligned) {
                            data_in[index].set_slc(slice, r.data.slc<8>(bit));//first parameter wrong
                        } else {
                            if (b + offset>=STRIDE) {
                                data_in[index].set_slc(slice, r.data.slc<8>(bit-BUS_SIZE+bit_offset));
                            } else {
                                data_in[index].set_slc(slice, old_data.slc<8>(bit+bit_offset));
                            }
                        }
                    }
                }
               
               base += BUS_BYTES;
            }
            if (first) bytes_recv += BUS_BYTES - offset;
            else bytes_recv += BUS_BYTES;
                              
            old_data = r.data;
               
            first = false;

            if (base >= byte_count) break;

        }
       
        lines_received += burst_lines_recv;
        burst_start += (burst_size + 1) << BUS_BITS;

    } while (lines_received < lines);

   return r.resp;
}

template <typename datatype, int bit_shift>
axi_resp_type axi_master_interface::axi_burst_write_base(
       axi_address_type   address,
       datatype          *data_out,
       axi_size_type      count)
{
    aw_payload aw;
    w_payload w;
    b_payload b;

    const int size_bytes = (1 << bit_shift);
    const int size_bits  = (size_bytes << 3);
    const axi_size_type byte_count = count * size_bytes;

    const axi_address_type   address_mask  = ADDR_HIGH_BITS_MASK;
    const axi_address_type   first_line    = (address & address_mask);                       // byte address of first line to write
    const axi_address_type   last_line     = ((address + byte_count - 1) & address_mask);    // byte address of last line to write
    
    const axi_address_type  last_byte  = address + byte_count - 1;
    
    const axi_size_type lines          = ((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1;    // number of lines in the transfer
    const axi_size_type start_delta    = address - first_line;
    const axi_size_type end_delta      = (address + byte_count - last_line);
    // const bool chatty                  = false;
    
    const axi_size_type words_per_line  = BUS_SIZE / size_bits;     // STRIDE is bytes per line
    const axi_size_type bytes_per_word  = size_bits / WORD_SIZE;
    const axi_size_type offset          = (axi_size_type) (address & ADDR_LOW_BITS_MASK);

    axi_size_type     bytes_sent        = 0;
    axi_size_type     lines_sent        = 0;
    axi_resp_type     resp              = 0;
    axi_size_type     burst_lines_sent;
    axi_address_type  max_burst;
    axi_size_type     burst_size;
    axi_size_type     bytes_loaded      = 0;
    axi_address_type  burst_end;
    axi_address_type  base_word;
    axi_address_type  burst_start;
    axi_address_type  four_k_boundary;

    bool first = true;
   
    if (size_bits == 0) return
        resp;

    burst_start = first_line;
    base_word   = first_line;

    ac_int<BUS_SIZE * 2, false> buffer;
    
    buffer = 0;
        
    for (int i=0; i<(1 << (BUS_BITS-bit_shift)); i++) {
        if ((bytes_loaded + i * size_bytes) < byte_count) {
            buffer.set_slc((i * size_bits) + (offset << 3), data_out[(bytes_sent>>bit_shift)+i]);
        }
    }
    
    bytes_loaded += BUS_BYTES;
    
    do {
        
        four_k_boundary = (((address + bytes_sent) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
        max_burst       = (address + bytes_sent) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
        burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

        burst_size = (burst_end - burst_start) >> BUS_BITS;
        
        assert (burst_size >= 0);
        
        axi_size_type beat_bytes = (last_byte + 1) - (address + bytes_sent);
        if (beat_bytes > max_burst + BUS_BYTES) beat_bytes = max_burst + BUS_BYTES;
        if (beat_bytes + address + bytes_sent > four_k_boundary + BUS_BYTES) beat_bytes = four_k_boundary + BUS_BYTES - (address + bytes_sent);
             
        // set read address payload

        aw.address  = (address + bytes_sent) & ADDR_HIGH_BITS_MASK;
        aw.id       = 4; // any number
        aw.len      = burst_size;
        aw.size     = BUS_BITS; // full bus width
        aw.burst    = 1; // incrementing
        aw.lock     = 0; // unlocked
        aw.cache    = 0; // uncached
        aw.prot     = 0; // no protection
        aw.qos      = 0; // normal qos

        send_aw(aw);

        burst_lines_sent = 0;
        
        axi_size_type burst_start_delta = (address + bytes_sent) & ADDR_LOW_BITS_MASK;
        axi_size_type burst_end_delta   = (address + bytes_sent + beat_bytes) & ADDR_LOW_BITS_MASK;

       #pragma hls_pipeline_init_interval 1
        for (int i=0; i<burst_size+1; i++) {
            bool last_beat = (i == burst_size);

            w.data = 0;
            w.strb = set_strb(burst_start_delta, burst_end_delta, first, last_beat);
            w.last = last_beat;
            w.data = buffer.slc<BUS_SIZE>(0);
            
            send_w(w);
            burst_lines_sent++;
            
            buffer = buffer >> BUS_SIZE;
            
            bytes_sent += ones(w.strb);
            
            for (int i=0; i<(1 << (BUS_BITS-bit_shift)); i++) {
                if ((bytes_loaded + i * size_bytes) < byte_count) {
                    buffer.set_slc((i * size_bits) + (offset << 3), data_out[(bytes_loaded>>bit_shift)+i]);
                }
            }

            bytes_loaded += BUS_BYTES;
                                          
            first = false;
        }

        ac::wait();
        
        get_b(b);
        
        if (b.resp != 0) return b.resp;
        
        lines_sent  += burst_lines_sent;
        burst_start += (burst_size + 1) << BUS_BITS;

    } while (lines_sent < lines);

   return b.resp;
}


axi_resp_type  axi_master_interface::read(axi_address_type address, axi_8 &data_in)
{
    return axi_burst_read_base<axi_8, 0>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u8 &data_in)
{
    return axi_burst_read_base<axi_u8, 0>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_16 &data_in)
{
    return axi_burst_read_base<axi_16, 1>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u16 &data_in)
{
    return axi_burst_read_base<axi_u16, 1>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_32 &data_in)
{
    return axi_burst_read_base<axi_32, 2>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u32 &data_in)
{
    return axi_burst_read_base<axi_u32, 2>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_64 &data_in)
{
    return axi_burst_read_base<axi_64, 3>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u64 &data_in)
{
    return axi_burst_read_base<axi_u64, 3>(address, &data_in, 1);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_float &data_in)
{
    axi_u32 float_in;
    axi_resp_type r;
    
    r = axi_burst_read_base<axi_u32, 2>(address, &float_in, 1);
    data_in.set_data(float_in.slc<32>(0));
    return r;
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_double &data_in)
{
    axi_u64 double_in;
    axi_resp_type r;
        
    r = axi_burst_read_base<axi_u64, 3>(address, &double_in, 1);
    data_in.set_data(double_in.slc<64>(0));
    return r;
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_8 data_out)
{
    return axi_burst_write_base<axi_8, 0>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u8 data_out)
{
    return axi_burst_write_base<axi_u8, 0>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_16 data_out)
{
    return axi_burst_write_base<axi_16, 1>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u16 data_out)
{
    return axi_burst_write_base<axi_u16, 1>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_32 data_out)
{
    return axi_burst_write_base<axi_32, 2>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u32 data_out)
{
    return axi_burst_write_base<axi_u32, 2>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_64 data_out)
{
    return axi_burst_write_base<axi_64, 3>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u64 data_out)
{
    return axi_burst_write_base<axi_u64, 3>(address, &data_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_float data_out)
{
    axi_u32 float_out;
    
    float_out.set_slc(0, (axi_u32) data_out.data());
    return axi_burst_write_base<axi_u32, 2>(address, &float_out, 1);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_double data_out)
{
    axi_u64 double_out;
    
    double_out.set_slc(0, (axi_u64) data_out.data());
    return axi_burst_write_base<axi_u64, 3>(address, &double_out, 1);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_8   *data, axi_size_type size)
{
   return axi_burst_write_base<axi_8, 0>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u8  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u8, 0>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_16  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_16, 1>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u16 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u16, 1>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_32  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_32, 2>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u32 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u32, 2>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_64  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_64, 3>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u64 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u64, 3>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_128  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_128, 4>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u128 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u128, 4>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_256 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_256, 5>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u256 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u256, 5>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_512 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_512, 6>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u512 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u512, 6>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_1024 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_1024, 7>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u1024 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u1024, 7>(address, data, size);
}
/*
axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_float *data, axi_size_type size)
{
   return axi_burst_write_base<axi_float, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_double *data, axi_size_type size)
{
   return axi_burst_write_base<axi_double, 64>(address, data, size);
}
*/
axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_8   *data, axi_size_type size)
{
   return axi_burst_read_base<axi_8, 0>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u8  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u8, 0>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_16  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_16, 1>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u16 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u16, 1>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_32  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_32, 2>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u32 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u32, 2>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_64  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_64, 3>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u64 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u64, 3>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_128  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_128, 4>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u128 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u128, 4>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_256  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_256, 5>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u256 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u256, 5>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_512  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_512, 6>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u512 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u512, 6>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_1024  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_1024, 7>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u1024 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u1024, 7>(address, data, size);
}

/*
axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_float *data, axi_size_type size)
{
   return axi_burst_read_base<axi_float, 2>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_double *data, axi_size_type size)
{
   return axi_burst_read_base<axi_double, 3>(address, data, size);
}
*/
