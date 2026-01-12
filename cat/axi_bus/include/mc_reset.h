#ifndef MC_RESET_H
#define MC_RESET_H

#include <sstream>
#include <iomanip>
#include <systemc.h>

class mc_programmable_reset : public sc_module
{
public:
	sc_out< sc_logic > reset_out;

   SC_HAS_PROCESS(mc_programmable_reset);
   mc_programmable_reset(const sc_module_name& name, double duration, bool phaseneg)
      : sc_module(name)
      , reset_out("reset_out")
      , d_duration(duration)
      , d_phaseneg(phaseneg)
      , d_assert_reset(false)
      , d_do_once(true)
      , d_enable_event(0)
   {
      SC_METHOD(reset_driver);
      sensitive << reset_deactivation_event;
      //dont_initialize();
   }

   void reset_driver()
   {
      if (d_assert_reset) {
         d_assert_reset = false;
         reset_out = (d_phaseneg ? SC_LOGIC_0 : SC_LOGIC_1);
         reset_deactivation_event.notify(d_duration, SC_NS);
         //set enable_event as pointer to mc_programmable_enable.enable_event
         //to turn on enable toggling for this reset driver
         if (d_enable_event && d_do_once) {
            d_enable_event->notify(d_duration * 1.25, SC_NS);
            d_do_once = false;
         }
      } else {
         d_assert_reset = true;
         reset_out = (d_phaseneg ? SC_LOGIC_1 : SC_LOGIC_0);
      }
   }

   void set_enable_event(sc_event* new_event) {
      d_enable_event = new_event;
   }

private:
   double     d_duration;
   bool       d_phaseneg;
   bool       d_assert_reset;
   bool       d_do_once;
   sc_event*  d_enable_event;
   sc_event   reset_deactivation_event;
};

class mc_programmable_enable : public sc_module
{
public:
	sc_out< sc_logic > enable_out;
   sc_event enable_event;

   SC_HAS_PROCESS(mc_programmable_enable);
   mc_programmable_enable(const sc_module_name& name, double duration, bool phaseneg)
      : sc_module(name)
      , enable_out("enable_out")
      , d_duration(duration)
      , d_phaseneg(phaseneg)
      , d_assert_enable(true)
   {
      SC_METHOD(enable_driver);
      sensitive << enable_event;
      dont_initialize();
   }

   void enable_driver()
   {
      if (d_assert_enable) {
         d_assert_enable = false;
         enable_out = (d_phaseneg ? SC_LOGIC_1 : SC_LOGIC_0);
         enable_event.notify(d_duration * 0.5, SC_NS);
      } else {
         d_assert_enable = true;
         enable_out = (d_phaseneg ? SC_LOGIC_0 : SC_LOGIC_1);
      }
   }

private:
   double     d_duration;
   bool       d_phaseneg;
   bool       d_assert_enable;
};

class mc_sync_timer : public sc_module
{
public:
	sc_in_clk clk;
	sc_in< sc_logic > rst;
   sc_out< sc_logic > sync_out;

   SC_HAS_PROCESS(mc_sync_timer);
   mc_sync_timer(const sc_module_name& name, unsigned int reset_length, unsigned int offset, unsigned int duration, sc_logic active_edge)
      : sc_module(name)
      , clk("clk")
      , rst("rst")
      , d_active_edge(active_edge)
      , d_reset_length(reset_length?reset_length-1:0) // reset_length-1 downto 0 OR ELSE 0 (no reset loop)
      , d_offset(reset_length?(offset?offset-1:0):0)
      , d_duration(duration-1) // duration-1 downto 0
      , d_count(0)
   {
      SC_METHOD(sync_driver);
      sensitive << clk << rst;
   }

   void sync_driver()
   {
      if (rst.read() == SC_LOGIC_1) {
         sync_out.write(SC_LOGIC_0);
         d_count = d_offset;
      } else {
         if (clk.read() == d_active_edge) {
            if (d_reset_length) {
               d_reset_length--; // reset loop still executing
            } else {
               if (d_count) {
                  sync_out.write(SC_LOGIC_0);
                  d_count--;
               } else {
                  sync_out.write(SC_LOGIC_1);
                  d_count = d_duration;
               }
            }
         }
      }
   }

private:
   sc_logic        d_active_edge;
   unsigned int    d_reset_length;
   unsigned int    d_offset;
   unsigned int    d_duration;
   unsigned int    d_count;
};

#endif

