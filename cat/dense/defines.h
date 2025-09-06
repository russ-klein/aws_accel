
#define WORD_SHIFT 1
#define WORD_BYTES (1 << WORD_SHIFT)
#define WORD_BITS (8 * WORD_BYTES)
#define INTEGER_BITS 8
#define FRACTIONAL_BITS (WORD_BITS - INTEGER_BITS)

#define INDEX_BITS (BUS_BITS - WORD_SHIFT)
#define INDEX_MASK ((1 << INDEX_BITS) - 1)

typedef ac_fixed<WORD_BITS, INTEGER_BITS, true, AC_RND, AC_SAT> feature_type;
typedef ac_fixed<WORD_BITS, INTEGER_BITS, true, AC_RND, AC_SAT> weight_type;

typedef ac_int<32, true> param_t;
typedef ac_int<16, false> index_t;


