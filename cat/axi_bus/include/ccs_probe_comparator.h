#ifndef _INCLUDED_CCS_PROBE_COMPARATOR_
#define _INCLUDED_CCS_PROBE_COMPARATOR_

#include <systemc.h>
#include <string>
#include <tlm.h>
#include <mc_typeconv.h>

#ifndef CCS_SC_NUMREP
// The number representation used when printing sc_lv types
#define CCS_SC_NUMREP SC_HEX //SC_BIN, SC_OCT, SC_DEC, SC_HEX
#endif

// Traits class to handle extracting bitwidth of probed data
template <class T> struct ccs_scverify_probe_traits;
template <>
struct ccs_scverify_probe_traits< bool > {
  typedef sc_lv<1> data_type;
  enum {lvwidth = 1};
};
template <>
struct ccs_scverify_probe_traits< char > {
  typedef sc_lv<8> data_type;
  enum {lvwidth = 8};
};
template <>
struct ccs_scverify_probe_traits< unsigned char > {
  typedef sc_lv<8> data_type;
  enum {lvwidth = 8};
};
template <>
struct ccs_scverify_probe_traits< short > {
  typedef sc_lv<16> data_type;
  enum {lvwidth = 16};
};
template <>
struct ccs_scverify_probe_traits< unsigned short > {
  typedef sc_lv<16> data_type;
  enum {lvwidth = 16};
};
template <>
struct ccs_scverify_probe_traits< int > {
  typedef sc_lv<32> data_type;
  enum {lvwidth = 32};
};
template <>
struct ccs_scverify_probe_traits< unsigned int > {
  typedef sc_lv<32> data_type;
  enum {lvwidth = 32};
};
template <>
struct ccs_scverify_probe_traits< long > {
  typedef sc_lv<32> data_type;
  enum {lvwidth = 32};
};
template <>
struct ccs_scverify_probe_traits< unsigned long > {
  typedef sc_lv<32> data_type;
  enum {lvwidth = 32};
};
template <>
struct ccs_scverify_probe_traits< long long > {
  typedef sc_lv<64> data_type;
  enum {lvwidth = 64};
};
template <>
struct ccs_scverify_probe_traits< unsigned long long > {
  typedef sc_lv<64> data_type;
  enum {lvwidth = 64};
};
template <int Tlvwidth>
struct ccs_scverify_probe_traits< sc_lv<Tlvwidth> > {
  typedef sc_lv<Tlvwidth> data_type;
  enum {lvwidth = Tlvwidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< sc_bv<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< sc_int<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< sc_uint<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< sc_bigint<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< sc_biguint<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
#ifdef SC_INCLUDE_FX
template <int Twidth, int Ibits>
struct ccs_scverify_probe_traits< sc_fixed<Twidth,Ibits> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth, int Ibits, sc_q_mode Qmode, sc_o_mode Omode, int Nbits>
struct ccs_scverify_probe_traits< sc_fixed<Twidth,Ibits,Qmode,Omode,Nbits> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth, int Ibits>
struct ccs_scverify_probe_traits< sc_ufixed<Twidth,Ibits> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth, int Ibits, sc_q_mode Qmode, sc_o_mode Omode, int Nbits>
struct ccs_scverify_probe_traits< sc_ufixed<Twidth,Ibits,Qmode,Omode,Nbits> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
#endif

#ifdef __AC_INT_H
template <int Twidth>
struct ccs_scverify_probe_traits< ac_int<Twidth,true> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< ac_int<Twidth,false> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
#endif

#ifdef __AP_INT_H__
template <int Twidth>
struct ccs_scverify_probe_traits< ap_int<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <int Twidth>
struct ccs_scverify_probe_traits< ap_uint<Twidth> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
#endif

#ifdef __AC_FIXED_H
template <int Twidth, int Ibits, bool Signed, ac_q_mode Qmode, ac_o_mode Omode>
struct ccs_scverify_probe_traits< ac_fixed<Twidth,Ibits,Signed,Qmode,Omode> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
#endif
#ifdef __AC_FLOAT_H
template <int MTbits, int MIbits, int Ebits, ac_q_mode Qmode>
struct ccs_scverify_probe_traits< ac_float<MTbits,MIbits,Ebits,Qmode> > {
  typedef sc_lv<MTbits+Ebits> data_type;
  enum {lvwidth = MTbits+Ebits};
};
#endif

#ifdef __AC_STD_FLOAT_H
template <int Twidth, int Ebits>
struct ccs_scverify_probe_traits< ac_std_float<Twidth,Ebits> > {
  typedef sc_lv<Twidth> data_type;
  enum {lvwidth = Twidth};
};
template <>
struct ccs_scverify_probe_traits< ac::bfloat16 > {
  typedef sc_lv<16> data_type;
  enum {lvwidth = 16};
};
template <ac_ieee_float_format TFormat>
struct ccs_scverify_probe_traits< ac_ieee_float<TFormat> > {
  typedef ac_ieee_float<TFormat> dT;
  typedef sc_lv<dT::width> data_type;
  enum {lvwidth = dT::width};
};
#endif

// Compares the source ac::probe() and synthesized RTL probe
template <int Tw>
SC_MODULE(ccs_probe_comparator)
{
  sc_port<tlm::tlm_fifo_get_if<sc_lv<Tw> > > PROBE_fifo;
  sc_port<tlm::tlm_fifo_get_if<sc_lv<Tw> > > RTL_fifo;
  sc_lv<Tw>                                  PROBE_value;
  sc_lv<Tw>                                  RTL_value;
  bool                                       PROBE_value_ready;
  bool                                       RTL_value_ready;
  int                                        compare_cnt;
  int                                        error_cnt;
  sc_signal<sc_lv<Tw> >                      PROBE_value_sig; // signals used for simulator visibility
  sc_signal<sc_lv<Tw> >                      RTL_value_sig;
  sc_signal<int>                             compare_cnt_sig;
  sc_signal<int>                             error_cnt_sig;
  std::ostringstream                         _msg;
  const std::string                          probe_name;
  bool                                       summary_reported;

  SC_HAS_PROCESS(ccs_probe_comparator);
  ccs_probe_comparator(const sc_module_name& name, const std::string &prbnm)
    : sc_module(name)
    , PROBE_fifo("PROBE_fifo")
    , RTL_fifo("RTL_fifo")
    , PROBE_value_ready(false)
    , RTL_value_ready(false)
    , compare_cnt(0)
    , error_cnt(0)
    , PROBE_value_sig("PROBE_value_sig", sc_lv<Tw>('0'))
    , RTL_value_sig("RTL_value_sig", sc_lv<Tw>('0'))
    , compare_cnt_sig("compare_cnt_sig", 0)
    , error_cnt_sig("error_cnt_sig", 0)
    , probe_name(prbnm)
    , summary_reported(false)
  {
    SC_METHOD(compare);
  }
  virtual ~ccs_probe_comparator() {
    // If the end_of_simulation() callback is not triggered by the kernel
    this->summary_report();
  }
  virtual void start_of_simulation() {
  }
  virtual void end_of_simulation() {
    this->summary_report();
  }
  void summary_report() {
    if ( summary_reported ) return;
    summary_reported = true;
    _msg.str("");
    _msg << "ac::probe(" << probe_name << ") Summary: " << compare_cnt_sig.read() << " values compared, " << error_cnt_sig.read() << " mismatches detected @ " << sc_time_stamp();
    if (error_cnt) {
      SC_REPORT_WARNING(name(), _msg.str().c_str());
    } else {
      SC_REPORT_INFO(name(), _msg.str().c_str());
    }
  }
  void compare() {
    while (true) {
      if (!PROBE_value_ready) {
        PROBE_value_ready = PROBE_fifo->nb_get(PROBE_value);
      }
      if (!RTL_value_ready) {
        RTL_value_ready = RTL_fifo->nb_get(RTL_value);
      }
      if (!PROBE_value_ready || !RTL_value_ready) break; // waiting

      PROBE_value_ready = RTL_value_ready = false;
      PROBE_value_sig.write(PROBE_value);
      RTL_value_sig.write(RTL_value);
      compare_cnt_sig.write(++compare_cnt);
      if (PROBE_value != RTL_value) {
        error_cnt_sig.write(++error_cnt);
        _msg.str("");
        _msg << "Incorrect Data Detected: '" << probe_name << "' (probe=" << PROBE_value.to_string(CCS_SC_NUMREP) << ", rtl=" << RTL_value.to_string(CCS_SC_NUMREP) << ") @ " << sc_time_stamp();
        SC_REPORT_WARNING(name(), _msg.str().c_str());
      }
    }
    if      (RTL_value_ready)   next_trigger(PROBE_fifo->ok_to_get());  // waiting for PROBE data
    else if (PROBE_value_ready) next_trigger(RTL_fifo->ok_to_get());    // waiting for RTL data
    else                        next_trigger(PROBE_fifo->ok_to_get() | RTL_fifo->ok_to_get());
  }
};

// RTL probe monitor for logic vector types, sc_logic specialization below.
// This also instantiates both the source and RTL probe fifos used by the comparator.
// The source ac::probe() callbacks that write to the PROBE_fifo are auto generated by SCVerify 
//  and a global tlm_fifo pointer links the PROBE_fifo with the callback routine.
// The RTL_fifo is written by the capture_RTL method when RTL_vld is asserted.
template<bool clk_phase, int W, int fifo_sz>
class ccs_probe_monitor : public sc_module
{
public:
  sc_in_clk        clk;
  sc_in<sc_logic>  rst;
  sc_in<sc_logic>  RTL_vld;
  sc_in<sc_lv<W> > RTL_dat;
  tlm::tlm_fifo<sc_lv<W> > RTL_fifo;
  tlm::tlm_fifo<sc_lv<W> > PROBE_fifo;
  ccs_probe_comparator<W> comparator;
  const std::string probe_name;

  SC_HAS_PROCESS(ccs_probe_monitor);
  ccs_probe_monitor(const sc_module_name &nm, const std::string &prbnm, tlm::tlm_fifo<sc_lv<W> > **g_PROBE_fifo_p)
    : sc_module(nm)
    , clk("clk")
    , rst("rst")
    , RTL_vld("RTL_vld")
    , RTL_dat("RTL_dat")
    , RTL_fifo("RTL_fifo", fifo_sz)
    , PROBE_fifo("PROBE_fifo", fifo_sz)
    , comparator("comparator", prbnm)
    , probe_name(prbnm)
  {
    SC_METHOD(capture_RTL); // method to capture RTL signal changes
    sensitive << (clk_phase? clk.pos(): clk.neg());
    dont_initialize();

    *g_PROBE_fifo_p = &PROBE_fifo;
    comparator.RTL_fifo(RTL_fifo);
    comparator.PROBE_fifo(PROBE_fifo);
  }

  void capture_RTL() {
    if ( rst.read() == SC_LOGIC_0 && RTL_vld.read() == SC_LOGIC_1 ) {
      #ifdef SCVERIFY_VERBOSE_AC_PROBE
      std::cout << "CAPTURE RTL PROBE '" << probe_name << "' VALUE '" << RTL_dat.read().to_string(CCS_SC_NUMREP) << "' @ " << sc_time_stamp() << std::endl;
      #endif
      RTL_fifo.nb_put(RTL_dat.read());
    }
  }
};

// Partial specialization to handle single bit sc_logic data types
template<bool clk_phase, int fifo_sz>
class ccs_probe_monitor<clk_phase, 1, fifo_sz> : public sc_module
{
public:
  sc_in_clk        clk;
  sc_in<sc_logic>  rst;
  sc_in<sc_logic>  RTL_vld;
  sc_in<sc_logic>  RTL_dat;
  tlm::tlm_fifo<sc_lv<1> > RTL_fifo;
  tlm::tlm_fifo<sc_lv<1> > PROBE_fifo;
  ccs_probe_comparator<1> comparator;
  std::string probe_name;

  SC_HAS_PROCESS(ccs_probe_monitor);
  ccs_probe_monitor(const sc_module_name &nm, const std::string &prbnm, tlm::tlm_fifo<sc_lv<1> > **g_PROBE_fifo_p)
    : sc_module(nm)
    , clk("clk")
    , rst("rst")
    , RTL_vld("RTL_vld")
    , RTL_dat("RTL_dat")
    , RTL_fifo("RTL_fifo", fifo_sz)
    , PROBE_fifo("PROBE_fifo", fifo_sz)
    , comparator("comparator", prbnm)
    , probe_name(prbnm)
  {
    SC_METHOD(capture_RTL); // method to capture RTL signal changes
    sensitive << (clk_phase? clk.pos(): clk.neg());
    dont_initialize();

    *g_PROBE_fifo_p = &PROBE_fifo;
    comparator.RTL_fifo(RTL_fifo);
    comparator.PROBE_fifo(PROBE_fifo);
  }

  void capture_RTL() {
    if ( rst.read() == SC_LOGIC_0 && RTL_vld.read() == SC_LOGIC_1 ) {
      #ifdef SCVERIFY_VERBOSE_AC_PROBE
      std::cout << "CAPTURE RTL PROBE '" << probe_name << "' VALUE '" << RTL_dat.read() << "' @ " << sc_time_stamp() << std::endl;
      #endif
      sc_lv<1> RTL_dat_tmp;
      RTL_dat_tmp[0] = RTL_dat.read();
      RTL_fifo.nb_put(RTL_dat_tmp);
    }
  }
};
#endif

