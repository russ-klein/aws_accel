
#ifndef _INCLUDED_MC_TESTBENCH_UTIL_H_
#define _INCLUDED_MC_TESTBENCH_UTIL_H_

namespace mc_testbench_util {

static void process_wait_ctrl(
   const sc_string &var, // variable name
   mc_wait_ctrl &var_wait_ctrl, // user testbench::<var>_wait_ctrl control variable
   tlm::tlm_fifo_put_if< mc_wait_ctrl > *ccs_wait_ctrl_fifo_if, // FIFO for wait_ctrl objects
   const int var_capture_count,
   const int var_stopat,
   const bool is_channel,
   const bool capture_empty_hold_count
) {
#ifdef MC_DEFAULT_TRANSACTOR_LOG
   const bool log_event = (MC_DEFAULT_TRANSACTOR_LOG & MC_TRANSACTOR_WAIT);
   const bool log_empty = (MC_DEFAULT_TRANSACTOR_LOG & MC_TRANSACTOR_EMPTY);
#else
   const bool log_event = true;
   const bool log_empty = false;
#endif
   if (var_capture_count < var_stopat) {
      var_wait_ctrl.ischannel = is_channel;
      var_wait_ctrl.iteration = var_capture_count;
      var_wait_ctrl.stopat = var_stopat;
      if (var_wait_ctrl.cycles != 0) {
         if (var_wait_ctrl.cycles < 0) {
            if (log_event) {
               std::ostringstream msg; msg.str("");
               msg << "Ignoring negative value (" << var_wait_ctrl.cycles << ") for testbench control testbench::" << var << "_wait_ctrl.cycles.";
               SC_REPORT_WARNING("User testbench", msg.str().c_str());
            }
            var_wait_ctrl.cycles = 0;
         }
         if (var_wait_ctrl.interval < 0) {
            if (log_event) {
               std::ostringstream msg; msg.str("");
               msg << "Ignoring negative value (" << var_wait_ctrl.interval << ") for testbench control testbench::" << var << "_wait_ctrl.interval.";
               SC_REPORT_WARNING("User testbench", msg.str().c_str());
            }
            var_wait_ctrl.interval = 0;
         }
         if (var_wait_ctrl.cycles > 0) {
            if (log_event && !var_wait_ctrl.quiet) {
               std::ostringstream msg; msg.str("");
               msg << "Captured " << var << "_wait_ctrl request " << var_wait_ctrl << " @ " << sc_time_stamp();
               SC_REPORT_INFO("User testbench", msg.str().c_str());
            }
            if (var_wait_ctrl.hold_count >= 0) {
               if (log_event && !var_wait_ctrl.quiet && capture_empty_hold_count) {
                  std::ostringstream msg; msg.str("");
                  msg << "Ignoring " << var << "_wait_ctrl.hold_count = " << var_wait_ctrl.hold_count << " with cycles > 0";
                  SC_REPORT_WARNING("User testbench", msg.str().c_str());
               }
               var_wait_ctrl.hold_count = -1;
            }
            ccs_wait_ctrl_fifo_if->put(var_wait_ctrl);
         }
      }
   } else {
      // No input data captured for non-blocking channel reads, track empty state
      if (capture_empty_hold_count && (var_capture_count == var_stopat)) {
         var_wait_ctrl.ischannel = is_channel;
         var_wait_ctrl.iteration = var_capture_count;
         var_wait_ctrl.stopat = var_stopat;
         var_wait_ctrl.cycles = 0;
         if (var_wait_ctrl.hold_count < 0) {
            var_wait_ctrl.hold_count = 0;
         }
         if (log_empty && !var_wait_ctrl.quiet) {
            std::ostringstream msg; msg.str("");
            msg << "Captured " << var << " hold count cycles " << var_wait_ctrl.hold_count << " for empty channel read request, stopped at capture count " << var_stopat << " @ " << sc_time_stamp();
            SC_REPORT_INFO("User testbench", msg.str().c_str());
         }
         ccs_wait_ctrl_fifo_if->put(var_wait_ctrl);
      }
   }
   var_wait_ctrl.clear(); // reset wait_ctrl
}

} // end namespace mc_testbench_util

#endif
