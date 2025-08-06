// Bus and word size definitions
//
// BUS_BITS 0 =    8 bits wide
//          1 =   16 bits wide
//          2 =   32 bits wide
//          3 =   64 bits wide
//          4 =  128 bits wide
//          5 =  256 bits wide
//          6 =  512 bits wide
//          7 = 1024 bits wide
//
#define BUS_BITS       3
#define WORD_SIZE      8
#define STRIDE        (1 << BUS_BITS)
#define BUS_SIZE      ((STRIDE) * (WORD_SIZE))

// AXI field size constants

#define M_BITS         4
#define READ_ID_BITS   4
#define WRITE_ID_BITS  (READ_ID_BITS)
#define ADDRESS_BITS  32
#define LEN_BITS       8
#define DATA_BITS      (BUS_SIZE)
#define SIZE_BITS      3
#define BURST_BITS     2
#define LOCK_BITS      1
#define CACHE_BITS     4
#define PROT_BITS      3
#define RESP_BITS      2
#define REGION_BITS    4
#define RUSER_BITS     7
#define WUSER_BITS     9
#define QOS_BITS       4
#define BYTE_BITS     ((BUS_SIZE)/(WORD_SIZE))

#define ADDR_LOW_BITS_MASK  ((1 << (BUS_BITS)) - 1)
#define ADDR_HIGH_BITS_MASK ((ac_int<ADDRESS_BITS, false>) ((((ac_int<ADDRESS_BITS, false>) 0) - 1) ^ (ADDR_LOW_BITS_MASK)))
#define BYTE_MASK           ((ac_int<BYTE_BITS, false>) ((1 << (1 << (BUS_BITS))) - 1))
