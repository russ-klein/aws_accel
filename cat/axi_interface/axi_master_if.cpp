
#include "axi_master_if.h"

axi_master_interface::axi_master_interface() 
{
#ifdef C_SIMULATION
  for (int i=0; i<0x10000; i++) memory_store_index[i] = (unsigned char *) NULL;
#endif
};

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
   if (n ==    8)    return ((ac_int<BYTE_BITS, false>) 0x01 << low_bits);
#if (BYTE_BITS > 1)
   if (n ==   16)    return ((ac_int<BYTE_BITS, false>) 0x03 << low_bits);
#if (BYTE_BITS > 2)
   if (n ==   32)    return ((ac_int<BYTE_BITS, false>) 0x0F << low_bits);
#if (BYTE_BITS > 4)
   if (n ==   64)    return ((ac_int<BYTE_BITS, false>) 0xFF << low_bits);
#if (BYTE_BITS > 8)
   if (n ==  128)    return ((ac_int<BYTE_BITS, false>) 0xFFFF << low_bits);
#if (BYTE_BITS > 16)
   if (n ==  256)    return ((ac_int<BYTE_BITS, false>) 0xFFFFFFFF << low_bits);
#if (BYTE_BITS > 32)
   if (n ==  512)    return ((ac_int<BYTE_BITS, false>) 0xFFFFFFFFFFFFFFFF << low_bits);
#if (BYTE_BITS > 64)
   if (n == 1024)    return ((ac_int<BYTE_BITS, false>) 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF << low_bits);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
   return 0;
}

#ifdef C_SIMULATION

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
     unsigned char *p = (unsigned char *) malloc(0x10000 * sizeof(unsigned char));
     if (p == NULL) {
       fprintf(stderr, "Out of memory, malloc failed \n");
       perror("axi_bus");
       exit(47);
     } 
     memory_store_index[(address>>16) & 0xFFFF] = p; // (unsigned char *) malloc(0x10000 * sizeof(unsigned char));
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

axi_data_type axi_master_interface::csim_memory_read(axi_address_type address, int size)
{
   unsigned char byte_value;

   assert (size <= STRIDE);

   axi_data_type ret = (axi_data_type) 0;

   bool aligned = false;

   if (aligned) {
     for (int byte=0; byte<size; byte++) {
       byte_value = memory_store_byte_read((long) address + byte);
       for (int bit=0; bit<8; bit++) ret[byte * 8 + bit] = (byte_value >> bit) & 1;
     } 
   } else {
     const unsigned long long offset = ((unsigned long long) address) & ADDR_LOW_BITS_MASK;
     for (int byte=offset; byte<offset + size; byte++) {
       byte_value = memory_store_byte_read(((unsigned long long) address) - offset + byte);
       for (int bit=0; bit<8; bit++) ret[byte * 8 + bit] = (byte_value >> bit) & 1;
     }
   }

   return(ret);
}
       

void axi_master_interface::csim_memory_write(axi_address_type address, int size, axi_data_type data)
{
   assert (size <= STRIDE);

   bool aligned = false;

   if (aligned) {
     for (int byte=0; byte<size; byte++) {
       memory_store_byte_write((long) address + byte, data.slc<8>(byte*8));
     }
   } else {
     const unsigned long long offset = ((unsigned long long) address) & ADDR_LOW_BITS_MASK;
     for (int byte=offset; byte<offset + size; byte++) {
       memory_store_byte_write(((unsigned long long) address) - offset + byte, data.slc<8>(byte*8));
     }
   }
}

#else

void axi_master_interface::tb_memory_write(long long address, int count, unsigned char *data) { }

void axi_master_interface::tb_memory_read(long long address, int count, unsigned char *data) { }

#endif

void axi_master_interface::send_ar(ar_payload &ar)
{
#ifdef C_SIMULATION
   csim_r_address = ar.address;       // byte address
   csim_r_size    = (1 << ar.size);   // bytes per beat
   csim_r_count   = ar.len + 1;       // number of beats
   csim_r_id      = ar.id;            
#else
   channels.ar_channel.write(ar);
#endif
}
   
void axi_master_interface::get_r(r_payload &r)
{
#ifdef C_SIMULATION
   r.data = csim_memory_read(csim_r_address, csim_r_size);
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
   csim_memory_write(csim_w_address, csim_w_size, w.data);
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


template <typename datatype, int size> 
axi_resp_type  axi_master_interface::axi_read_real(axi_address_type address, datatype &data_in)
{
   ar_payload ar;
   r_payload  r;

   ac_int<BUS_BITS, false> low_bits = address.slc<BUS_BITS>(0); 
   static ac_int<WRITE_ID_BITS, false> id = 0;

   ar.address  = address;
   ar.id       = id++;              // expected to overflow
   ar.len      = 0;                 // single read
   ar.size     = encode_size(size); // size
   ar.burst    = 1;                 // incrementing
   ar.lock     = 0;                 // unlocked
   ar.cache    = 0;                 // uncached
   ar.prot     = 0;                 // no protection
   ar.qos      = 0;                 // normal qos

   // channels.ar_channel.write(ar);
   send_ar(ar);

   ac::wait();

   // r = channels.r_channel.read();
   get_r(r);

   data_in.d = ((ac_int<size, false>) (r.data >> (((ac_int<BUS_BITS + 3, false>) low_bits) << 3)));

   return r.resp;
}


template <typename datatype, int size> 
axi_resp_type  axi_master_interface::axi_read_base(axi_address_type address, datatype &data_in)
{
   ar_payload ar;
   r_payload  r;

   ac_int<BUS_BITS, false> low_bits = address.slc<BUS_BITS>(0); 
   static ac_int<WRITE_ID_BITS, false> id = 0;

   ar.address  = address;
   ar.id       = id++;              // expected to overflow
   ar.len      = 0;                 // single read
   ar.size     = encode_size(size); // size from template
   ar.burst    = 1;                 // incrementing
   ar.lock     = 0;                 // unlocked
   ar.cache    = 0;                 // uncached
   ar.prot     = 0;                 // no protection
   ar.qos      = 0;                 // normal qos

   // channels.ar_channel.write(ar);
   send_ar(ar);

   ac::wait();

   // r = channels.r_channel.read();
   get_r(r);

   data_in = r.data >> (((ac_int<BUS_BITS + 3, false>) low_bits) << 3); // (8 * low_bits); this eliminates a multiplier and replaces it with a shift
   return r.resp;
}

template <typename datatype, int size> 
axi_resp_type  axi_master_interface::axi_write_real(axi_address_type address, datatype data_out)
{
   aw_payload aw;
   w_payload  w;
   b_payload  b;

   ac_int<BUS_BITS, false> low_bits = address.slc<BUS_BITS>(0); 
   static ac_int<READ_ID_BITS, false> id = 0;

   aw.address  = address;
   aw.id       = id++;              // expected to overflow
   aw.len      = 0;                 // single read
   aw.size     = encode_size(size); // size
   aw.burst    = 1;                 // incrementing
   aw.lock     = 0;                 // unlocked
   aw.cache    = 0;                 // uncached
   aw.prot     = 0;                 // no protection
   aw.qos      = 0;                 // normal qos

   // channels.aw_channel.write(aw);
   send_aw(aw);

   w.data      = ((ac_int<BUS_SIZE, false>) data_out.data()) << (((ac_int<BUS_BITS + 3, false>) low_bits) << 3);  // data
   w.last      = 1;                 // single write operation
   w.strb      = encode_strb(size, low_bits); // strobe bits (todo: check alignment)

   // channels.w_channel.write(w);
   send_w(w); 

   ac::wait();

   // b = channels.b_channel.read();
   get_b(b);

   return b.resp;
}

template <typename datatype, int size> 
axi_resp_type  axi_master_interface::axi_write_base(axi_address_type address, datatype data_out)
{
   aw_payload aw;
   w_payload  w;
   b_payload  b;

   ac_int<BUS_BITS, false> low_bits = address.slc<BUS_BITS>(0); 
   static ac_int<READ_ID_BITS, false> id = 0;

   aw.address  = address;
   aw.id       = 0x0;               // was: id++; // expected to overflow
   aw.len      = 0x0;                 // was: 0; // single read
   aw.size     = encode_size(size); // size from template
   aw.burst    = 1;                 // incrementing
   aw.lock     = 1;                 // unlocked
   aw.cache    = 0;                 // uncached
   aw.prot     = 0;                 // no protection
   aw.qos      = 0;                 // normal qos

   w.data      = ((ac_int<BUS_SIZE, false>) data_out) << (((ac_int<BUS_BITS + 3, false>) low_bits) << 3);  // data
   w.last      = 1;                 // single write operation
   w.strb      = encode_strb(size, low_bits); // strobe bits (todo: check alignment)

   // channels.aw_channel.write(aw);
   send_aw(aw);
   // channels.w_channel.write(w);
   send_w(w); 

   ac::wait();

   // b = channels.b_channel.read();
   get_b(b);

   return b.resp;
}

int min(int a, int b, int c)
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

template <typename datatype, int size>
axi_resp_type axi_master_interface::axi_burst_write_base(
       axi_address_type   address,
       datatype          *data_out,
       axi_size_type      count)
{
   // simple and fast
   // must be fully aligned
   // datatype must match buswidth
   // caller responsible for 4k boundary avoidance

   aw_payload  aw;
   w_payload   w;
   b_payload   b;

   ac_int<16, false> beat;
   axi_resp_type    resp = 0;

   // set write address payload
   
   if (size == 0) return resp;

   aw.address  = address;
   aw.id       = 0; // any number 
   aw.len      = count-1; 
   aw.size     = BUS_BITS; // full buswidth
   aw.burst    = 1; // incrementing
   aw.lock     = 0; // unlocked
   aw.cache    = 0; // uncached
   aw.prot     = 0; // no protection
   aw.qos      = 0; // normal qos
  
   send_aw(aw);
 
   beat = 0;
   w.strb = -1; // 0xFFFFFFFF;

  #pragma hls_pipeline_init_interval 1
   data_beat_loop: for (int beat=0; ; beat++) {
     w.last = (beat == aw.len);
     w.data = data_out[beat];
     send_w(w);
     if (w.last) break;
   }
   // ac::wait();

   get_b(b);

   return b.resp;
}

/*
template <typename datatype, int size> 
axi_resp_type axi_master_interface::axi_burst_write_base_complete(
       axi_address_type   address,
       datatype          *data_out,
       axi_size_type      count)
{
   aw_payload  aw;
   w_payload   w;
   b_payload   b;

   const axi_address_type address_mask  = ADDR_HIGH_BITS_MASK;
   const ac_int<LEN_BITS, false> max_burst_len = (((ac_int<LEN_BITS, false>) 0) - 1);  // defines maximum possible bust size
                                                          // if count is greater transfer is broken up into multiple bursts
                                              
   const axi_address_type first_line      = (address & address_mask);                                    // byte address of first line to write
   const axi_address_type last_line       = ((address + (count * (size/WORD_SIZE)) - 1) & address_mask);                      // byte address of last line to write
   const bool start_aligned  = (address == first_line);
   const bool end_aligned    = (((address + count) & ADDR_LOW_BITS_MASK) == 0);
   const int lines           = (((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1);  // number of lines in the transfer             
   const int start_delta     = ((address - first_line) / (size/WORD_SIZE));                                   
   const int end_delta       = ((1 << BUS_BITS) - 1) - ((address + (count * (size/WORD_SIZE)) - 1) & ADDR_LOW_BITS_MASK);

   axi_size_type burst_count = 0;
   axi_size_type sent        = 0;
   axi_size_type lines_sent  = 0;
   axi_size_type delta       = start_delta;
   axi_address_type max_burst;
   axi_size_type burst_size;
   axi_address_type burst_end;
   axi_address_type base_word;
   axi_resp_type    resp = 0;

   bool first = true;
   bool last;

   axi_address_type burst_start;
   axi_address_type four_k_boundary;

   w.data = 0;

   // set write address payload
   
   if (size == 0) return resp;

   burst_start = first_line;   
   base_word   = first_line;

   do {

      four_k_boundary = (((address + sent) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
      max_burst       = (address + sent) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
      burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

      burst_size = (burst_end - burst_start) >> BUS_BITS;

      aw.address  = address + sent;
      aw.id       = 0; // any number 
      aw.len      = burst_size; 
      aw.size     = BUS_BITS; // full buswidth
      aw.burst    = 1; // incrementing
      aw.lock     = 0; // unlocked
      aw.cache    = 0; // uncached
      aw.prot     = 0; // no protection
      aw.qos      = 0; // normal qos
  
      send_aw(aw);
 
     #pragma hls_pipeline_init_interval 1
      data_beat_loop: for (int beat=0; beat<burst_size+1; beat++) {
         last = (sent + STRIDE/(size/WORD_SIZE)) >= count;

         w.data = 0;
         w.strb = (first && last) ? (BYTE_MASK << (start_delta + end_delta)) >> end_delta :
                  (first)         ?  BYTE_MASK << start_delta :
                  (last)          ?  BYTE_MASK >> end_delta   :
                                     BYTE_MASK;
         w.strb = 0xFF;
         w.last = (beat == burst_size);

         delta = first ? start_delta : 0;

        #pragma hls_unroll 
         data_load: for (ac_int<16, false> i=0; i<(BUS_SIZE/size); i++) {
             
             // todo: promote constant sub-expressions
             
             if (((base_word +  i * (size/WORD_SIZE)) >= address) && 
                 ((base_word +  i * (size/WORD_SIZE)) < (address + (count * (size/WORD_SIZE))))) {
                     w.data.set_slc(size * i, data_out[i + sent - delta]);
             }

         }
         send_w(w);
      
         lines_sent++;
         sent += ones(w.strb)/(size/WORD_SIZE); // todo: get rid of these division operations!!  Add 7 clocks to each loop iteration
         //delta += ones(w.strb)/(size/WORD_SIZE);
         base_word += STRIDE;
         first = false;
      }

      // ac::wait();

      get_b(b);

      if (b.resp != 0) return b.resp;

      burst_start += burst_size + 1;

   } while (lines_sent < lines);  
   
   return b.resp;
}
*/

template <typename datatype, int size> 
axi_resp_type axi_master_interface::axi_burst_write_real_base(
       axi_address_type   address,
       datatype          *data_out,
       axi_size_type      count)
{
   aw_payload  aw;
   w_payload   w;
   b_payload   b;

   const axi_address_type address_mask  = ADDR_HIGH_BITS_MASK;
   const ac_int<LEN_BITS, false> max_burst_len = (((ac_int<LEN_BITS, false>) 0) - 1);  // defines maximum possible bust size
                                                          // if count is greater transfer is broken up into multiple bursts
                                              
   const axi_address_type first_line  = (address & address_mask);                                    // byte address of first line to write
   const axi_address_type last_line   = ((address + (count * (size/WORD_SIZE)) - 1) & address_mask);                      // byte address of last line to write
   const bool start_aligned           = (address == first_line);
   const bool end_aligned             = (((address + count) & ADDR_LOW_BITS_MASK) == 0);
   const axi_size_type lines          = (axi_size_type) (((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1);  // number of lines in the transfer             
   const axi_size_type start_delta    = (axi_size_type) (address - first_line);                                   
   const axi_size_type end_delta      = (axi_size_type) (((1 << BUS_BITS) - 1) - ((address + (count * (size/WORD_SIZE)) - 1) & ADDR_LOW_BITS_MASK));

   axi_size_type burst_count = 0;
   axi_size_type sent        = 0;
   axi_size_type lines_sent  = 0;
   axi_size_type delta       = 0;
   axi_address_type max_burst;
   axi_size_type burst_size;

   axi_address_type burst_end;
   axi_address_type base_word;
   axi_resp_type    resp = 0;

   bool first = true;
   bool last;

   axi_address_type burst_start;
   axi_address_type four_k_boundary;

   w.data = 0;

   // set write address payload
   
   if (size == 0) return resp;

   burst_start = first_line;   
   base_word   = first_line;

   do {

      four_k_boundary = (((address + sent) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
      max_burst       = (address + sent) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
      burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

      burst_size = (burst_end - burst_start) >> BUS_BITS;

      aw.address  = address + sent;
      aw.id       = 0; // any number 
      aw.len      = burst_size; 
      aw.size     = BUS_BITS; // full buswidth
      aw.burst    = 1; // incrementing
      aw.lock     = 0; // unlocked
      aw.cache    = 0; // uncached
      aw.prot     = 0; // no protection
      aw.qos      = 0; // normal qos
  
      // channels.aw_channel.write(aw);
      send_aw(aw);
 
     #pragma hls_pipeline_init_interval 1
      for (int i=0; i<burst_size+1; i++) {
         last = (sent + STRIDE/(size/WORD_SIZE)) >= count;

         w.data = 0;
         w.strb = (first && last) ? (BYTE_MASK << (start_delta + end_delta)) >> end_delta :
                  (first)         ?  BYTE_MASK << start_delta :
                  (last)          ?  BYTE_MASK >> end_delta   :
                                     BYTE_MASK;
         w.last = last;
   
        #pragma hls_unroll 
         data_load: for (ac_int<16, false> i=0; i<(BUS_SIZE/size); i++) {
             
             // todo: promote constant sub-expressions
             
             if (((base_word +  i * (size/WORD_SIZE)) >= address) && 
                 ((base_word +  i * (size/WORD_SIZE)) < (address + (count * (size/WORD_SIZE))))) 
                     w.data.set_slc(size * i, (ac_int<size, false>) data_out[delta + i].data());

         }
         // channels.w_channel.write(w);
         send_w(w);
      
         lines_sent++;
         sent += ones(w.strb)/(size/WORD_SIZE); // todo: get rid of these division operations!!
         delta += ones(w.strb)/(size/WORD_SIZE);
         base_word += STRIDE;
         first = false;
      }

      ac::wait();

      // b = channels.b_channel.read();
      get_b(b);

      if (b.resp != 0) return b.resp;

      burst_start += burst_size + 1;

   } while (lines_sent < lines);  
   
   return b.resp;
}


template <typename datatype, int size>
axi_resp_type axi_master_interface::axi_burst_read_base(
       axi_address_type   address,
       datatype          *data_in,
       axi_size_type      count)
{
   ar_payload ar;
   r_payload r;

   axi_resp_type    resp = 0;
   ac_int<16, false> beat;

   if (count == 0) return resp;

   ar.address  = address;
   ar.id       = 4; // any number 
   ar.len      = count - 1;
   ar.size     = BUS_BITS; // full bus width
   ar.burst    = 1; // incrementing
   ar.lock     = 0; // unlocked
   ar.cache    = 0; // uncached
   ar.prot     = 0; // no protection
   ar.qos      = 0; // normal qos

   send_ar(ar);
   ac::wait();

  #pragma hls_pipeline_init_interval 1
   for (int beat=0; beat<count; beat++) {
     get_r(r);
     data_in[beat] = r.data;
     if (r.resp) return r.resp;
     // if (beat == ar.len) break;
   }

   ac::wait();
   return r.resp;
}

/*
template <typename datatype, int size> 
axi_resp_type axi_master_interface::axi_burst_read_base_complete(
       axi_address_type   address,
       datatype          *data_in,
       axi_size_type      count)
{
   ar_payload ar;
   r_payload r;

   const axi_address_type address_mask  = ADDR_HIGH_BITS_MASK;
   const ac_int<LEN_BITS, false> max_burst_len = (((ac_int<LEN_BITS, false>) 0) - 1);  // defines maximum possible bust size
                                                          // if count is greater transfer is broken up into multiple bursts
                                              
   const axi_address_type first_line      = (address & address_mask);                                    // byte address of first line to write
   const axi_address_type last_line       = ((address + (count * (size/WORD_SIZE)) - 1) & address_mask);                      // byte address of last line to write
   const bool start_aligned  = (address == first_line);
   const bool end_aligned    = (((address + count) & ADDR_LOW_BITS_MASK) == 0);
   const int lines           = (((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1);  // number of lines in the transfer             
   const int start_delta     = ((address - first_line) / (size/WORD_SIZE));                                   
   const int end_delta       = STRIDE - (((1 << BUS_BITS) - 1) - ((address + (count * (size/WORD_SIZE)) - 1) & ADDR_LOW_BITS_MASK));

   const int all_words       = count;
   const int first_words     = (STRIDE - start_delta) / (size/WORD_SIZE);
   const int last_words      = (end_delta / (size/WORD_SIZE)==0) ? (STRIDE) / (size/WORD_SIZE) : end_delta / (size/WORD_SIZE);
   const int full_bus        = (STRIDE) / (size/WORD_SIZE);

   axi_size_type words_moved;
   axi_size_type burst_count = 0;
   axi_size_type received    = 0;
   axi_size_type lines_received  = 0;
   axi_size_type delta       = 0;
   axi_address_type max_burst;
   axi_size_type burst_size;

   axi_address_type burst_end;
   axi_address_type base_word;
   axi_resp_type    resp = 0;

   bool first = true;
   bool last;

   axi_address_type burst_start;
   axi_address_type four_k_boundary;

   r.data = 0;

   // set write address payload
   
   if (count == 0) return resp;

   burst_start = first_line;   
   base_word   = first_line;

   do {

      four_k_boundary = (((address + received) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
      max_burst       = (address + received) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
      burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

      burst_size = (burst_end - burst_start) >> BUS_BITS;

      // set read address payload

      ar.address  = address + received;
      ar.id       = 4; // any number 
      ar.len      = burst_size;
      ar.size     = BUS_BITS; // full bus width
      ar.burst    = 1; // incrementing
      ar.lock     = 0; // unlocked
      ar.cache    = 0; // uncached
      ar.prot     = 0; // no protection
      ar.qos      = 0; // normal qos

      // channels.ar_channel.write(ar);
      send_ar(ar);
printf("burst_size: %d \n", burst_size);
     #pragma hls_pipeline_init_interval 1
      for (int beat=0; beat<burst_size+1; beat++) {

         datatype t[(BUS_SIZE/size)*2];
printf("bus_size; %d size: %d size of t: %d \n", BUS_SIZE, size, (BUS_SIZE/size)*2);
         // r = channels.r_channel.read();
         get_r(r);

         delta = first ? start_delta : 0;
         axi_size_type offset = (received - delta) & (axi_size_type) 0xFFF8;
printf("offset after init: %d \n", offset);
        #pragma hls_unroll
         data_load: for (ac_int<16, false> i=0; i<(BUS_SIZE/size); i++) {
            if (((base_word + i * (size/WORD_SIZE)) >= address) &&
                ((base_word + i * (size/WORD_SIZE)) < address + (count * (size/WORD_SIZE)))) {
                  // data_in[i + offset] = r.data.slc<size>(i * size);
printf("i>=start_delta: %s \n", (i>=start_delta) ? "true" : "false"); 
printf("beat: %d i: %d start_delta: %d index_into_t: %d \n", beat, i, start_delta, beat+i-start_delta);
                  (i >= start_delta) ? t[beat + i - start_delta] = r.data.slc<size>(i * size) :
                                      t[beat + i - start_delta + (BUS_SIZE/size)] = r.data.slc<size>(i * size);
            }
         }

		    // problem:  loop above has "beat", which exceeds the size of the array, core dump!!
printf("offset[1] = %d \n", offset);
         if (start_delta > 0) {
            if (!first || (first && !last)) {
               // write data array
              #pragma hls_unroll
               for (int i=0; i<(BUS_SIZE/size); i++) {
                  data_in[i + offset] = t[i];
               }
            }

           #pragma hls_unroll
            for (int i=0; i<(BUS_SIZE/size); i++) {
               t[i] = t[i+(BUS_SIZE)/size];
            }
         } else {
            // write data array
           #pragma hls_unroll
            for (int i=0; i<(BUS_SIZE/size); i++) {
printf("offset[2] = %d \n", offset);
               data_in[beat + i + offset] = t[i + beat];
            }
         }

         last = (received + STRIDE/(size/WORD_SIZE)) >= count;
         words_moved = (first && last) ? all_words   :
                       (first)         ? first_words :
                       (last)          ? last_words  :
                                         full_bus;
                       
         lines_received++;
         received += words_moved;
         delta += words_moved;
         base_word += STRIDE;
         first = false;
      }
   } while (lines_received < lines);  

   return r.resp;
}
*/



template <typename datatype, int size> 
axi_resp_type axi_master_interface::axi_burst_read_real_base(
       axi_address_type   address,
       datatype          *data_in,
       axi_size_type      count)
{
   ar_payload ar;
   r_payload r;

   const axi_address_type address_mask  = ADDR_HIGH_BITS_MASK;
   const ac_int<LEN_BITS, false> max_burst_len = (((ac_int<LEN_BITS, false>) 0) - 1);  // defines maximum possible bust size
                                                          // if count is greater transfer is broken up into multiple bursts
                                              
   const axi_address_type first_line   = (address & address_mask);                                    // byte address of first line to write
   const axi_address_type last_line    = ((address + (count * (size/WORD_SIZE)) - 1) & address_mask);                      // byte address of last line to write
   const bool start_aligned            = (address == first_line);
   const bool end_aligned              = (((address + count) & ADDR_LOW_BITS_MASK) == 0);
   const axi_size_type lines           = (axi_size_type) (((last_line >> BUS_BITS) - (first_line >> BUS_BITS)) + 1);  // number of lines in the transfer             
   const axi_size_type start_delta     = (axi_size_type) (address - first_line);                                   
   const axi_size_type end_delta       = (axi_size_type) (((1 << BUS_BITS) - 1) - ((address + (count * (size/WORD_SIZE)) - 1) & ADDR_LOW_BITS_MASK));

   const axi_size_type all_words       = count;
   const axi_size_type first_words     = (STRIDE - start_delta) / (size/WORD_SIZE);
   const axi_size_type last_words      = (end_delta / (size/WORD_SIZE)==0) ? (axi_size_type) (( STRIDE) / (size/WORD_SIZE)) : (axi_size_type) (end_delta / (size/WORD_SIZE));
   const axi_size_type full_bus        = (STRIDE) / (size/WORD_SIZE);

   axi_size_type words_moved;
   axi_size_type burst_count = 0;
   axi_size_type received    = 0;
   axi_size_type lines_received  = 0;
   axi_size_type delta       = 0;
   axi_address_type max_burst;
   axi_size_type burst_size;

   axi_address_type burst_end;
   axi_address_type base_word;
   axi_resp_type    resp = 0;

   bool first = true;
   bool last;

   axi_address_type burst_start;
   axi_address_type four_k_boundary;

   r.data = 0;

   // set write address payload
   
   if (size == 0) return resp;

   burst_start = first_line;   
   base_word   = first_line;

   do {

      four_k_boundary = (((address + received) & (~(PAGE_MASK))) + (PAGE_SIZE) - (STRIDE));   // byte address of last line in 4K page
      max_burst       = (address + received) + (((1 << LEN_BITS) - 1) << (BUS_BITS));         // byte address of last line in max burst size
      burst_end       = min(last_line, four_k_boundary, max_burst);                       // smallest of remaining words, 4K boundary, or max_burst_len

      burst_size = (burst_end - burst_start) >> BUS_BITS;

      // set read address payload

      ar.address  = address + received;
      ar.id       = 4; // any number 
      ar.len      = burst_size;
      ar.size     = BUS_BITS; // full bus width
      ar.burst    = 1; // incrementing
      ar.lock     = 0; // unlocked
      ar.cache    = 0; // uncached
      ar.prot     = 0; // no protection
      ar.qos      = 0; // normal qos

      // channels.ar_channel.write(ar);
      send_ar(ar);

     #pragma hls_pipeline_init_interval 1
      for (int i=0; i<burst_size+1; i++) {

         // r = channels.r_channel.read();
         get_r(r);

        #pragma hls_unroll
         data_load: for (ac_int<16, false> i=0; i<(BUS_SIZE/size); i++) {

            if (((base_word + i) >= address) &&
                ((base_word + i) < address + (count * (size/WORD_SIZE)))) {
                  data_in[delta + i].d =  r.data.slc<size>(i * size);
            }
         }
         last = (received + STRIDE/(size/WORD_SIZE)) >= count;
         words_moved = (first && last) ? all_words   :
                       (first)         ? first_words :
                       (last)          ? last_words  :
                                         full_bus;
                       
         lines_received++;
         received += words_moved;
         delta += words_moved;
         base_word += STRIDE;
         first = false;
      }
   } while (lines_received < lines);  

   return r.resp;
}


axi_resp_type  axi_master_interface::read(axi_address_type address, axi_8 &data_in)
{
  return axi_read_base<axi_8, 8>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u8 &data_in)
{
   return axi_read_base<axi_u8, 8>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_16 &data_in)
{
   return axi_read_base<axi_16, 16>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u16 &data_in)
{
   return axi_read_base<axi_u16, 16>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_32 &data_in)
{
   return axi_read_base<axi_32, 32>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u32 &data_in)
{
   return axi_read_base<axi_u32, 32>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_64 &data_in)
{
   return axi_read_base<axi_64, 64>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u64 &data_in)
{
   return axi_read_base<axi_u64, 64>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_128 &data_in)
{
   return axi_read_base<axi_128, 128>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u128 &data_in)
{
   return axi_read_base<axi_u128, 128>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_256 &data_in)
{
   return axi_read_base<axi_256, 256>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u256 &data_in)
{
   return axi_read_base<axi_u256, 256>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_512 &data_in)
{
   return axi_read_base<axi_512, 512>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u512 &data_in)
{
   return axi_read_base<axi_u512, 512>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_1024 &data_in)
{
   return axi_read_base<axi_1024, 1024>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_u1024 &data_in)
{
   return axi_read_base<axi_u1024, 1024>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_float &data_in)
{
   return axi_read_real<axi_float, 32>(address, data_in);
}

axi_resp_type  axi_master_interface::read(axi_address_type address, axi_double &data_in)
{
   return axi_read_real<axi_double, 64>(address, data_in);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_8 data_out)
{
   return axi_write_base<axi_8, 8>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u8 data_out)
{
   return axi_write_base<axi_u8, 8>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_16 data_out)
{
   return axi_write_base<axi_16, 16>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u16 data_out)
{
   return axi_write_base<axi_u16, 16>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_32 data_out)
{
   return axi_write_base<axi_32, 32>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u32 data_out)
{
   return axi_write_base<axi_u32, 32>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_64 data_out)
{
   return axi_write_base<axi_64, 64>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u64 data_out)
{
   return axi_write_base<axi_u64, 64>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_128 data_out)
{
   return axi_write_base<axi_128, 128>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u128 data_out)
{
   return axi_write_base<axi_u128, 128>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_256 data_out)
{
   return axi_write_base<axi_256, 256>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u256 data_out)
{
   return axi_write_base<axi_u256, 256>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_512 data_out)
{
   return axi_write_base<axi_512, 512>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u512 data_out)
{
   return axi_write_base<axi_u512, 512>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_1024 data_out)
{
   return axi_write_base<axi_1024, 1024>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_u1024 data_out)
{
   return axi_write_base<axi_u1024, 1024>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_float data_out)
{
   return axi_write_real<axi_float, 32>(address, data_out);
}

axi_resp_type  axi_master_interface::write(axi_address_type address, axi_double data_out)
{
   return axi_write_real<axi_double, 64>(address, data_out);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_8   *data, axi_size_type size)
{
   return axi_burst_write_base<axi_8, 8>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u8  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u8, 8>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_16  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_16, 16>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u16 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u16, 16>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_32  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_32, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u32 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u32, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_64  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_64, 64>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u64 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u64, 64>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_128  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_128, 128>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u128 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u128, 128>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_256  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_256, 256>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u256 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u256, 256>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_512  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_512, 512>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u512 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u512, 512>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_1024  *data, axi_size_type size)
{
   return axi_burst_write_base<axi_1024, 1024>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_u1024 *data, axi_size_type size)
{
   return axi_burst_write_base<axi_u1024, 1024>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_float *data, axi_size_type size)
{
   return axi_burst_write_real_base<axi_float, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_write(axi_address_type address, axi_double *data, axi_size_type size)
{
   return axi_burst_write_real_base<axi_double, 64>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_8   *data, axi_size_type size)
{
   return axi_burst_read_base<axi_8, 8>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u8  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u8, 8>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_16  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_16, 16>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u16 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u16, 16>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_32  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_32, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u32 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u32, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_64  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_64, 64>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u64 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u64, 64>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_128  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_128, 128>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u128 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u128, 128>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_256  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_256, 256>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u256 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u256, 256>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_512  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_512, 512>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u512 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u512, 512>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_1024  *data, axi_size_type size)
{
   return axi_burst_read_base<axi_1024, 1024>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_u1024 *data, axi_size_type size)
{
   return axi_burst_read_base<axi_u1024, 1024>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_float *data, axi_size_type size)
{
   return axi_burst_read_real_base<axi_float, 32>(address, data, size);
}

axi_resp_type axi_master_interface::burst_read(axi_address_type address, axi_double *data, axi_size_type size)
{
   return axi_burst_read_real_base<axi_double, 64>(address, data, size);
}

