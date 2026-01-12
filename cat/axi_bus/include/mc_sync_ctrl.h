#ifndef MC_SYNC_CTRL_H
#define MC_SYNC_CTRL_H
////////////////////////////////////////////////////////////////////////////////
// mc_sync_ctrl struct for user control of the start and throughput
// syncronization of IO resources that do not provide sync signals from the
// Catapult process,
// i.e. ac_channel mapped resources without an lz handshake (in.rdy, out.vld)
//
// Example usage:
// CCS_MAIN(int argc, char* argv[]) {
//   #ifdef CCS_SCVERIFY
//   testbench::<rsc_name>_sync_ctrl.start_latency = 1;
//   testbench::<rsc_name>_sync_ctrl.init_interval = 8;
//   #endif
//   ...
// }
//
// For reference the generated mc_sync_ctrl instances are declared in the 
// SCVerify testbench class, written to:
//  <project>/<solution>/scverify/ccs_testbench.h
//
// The iosync_generator is defined in:
//  $MGC_HOME/shared/include/mc_transactors.h
////////////////////////////////////////////////////////////////////////////////
#include "ac_read_env.h"

struct mc_sync_ctrl {
  int start_latency;   //Initial latency cycles after reset
  int init_interval;   //Throughput rate cycles per transaction
  int ramp_down_cyc;   //Extra ramp down cycles added to the last transaction (for non-pipelined process loop) 
  bool pause_on_stall; //Disable the iosync_generator when IO parent process stalls (experimental)

  mc_sync_ctrl()
  : start_latency(-1),
    init_interval(-1),
    ramp_down_cyc(-1),
    capture_cnt(0),
    capture_start(0)
  {
    pause_on_stall = ac_env::read_bool("SCVerify_IOSYNC_PAUSE_ON_STALL",false);
  }

  //Used by the iosync_generator to change sync rate from TB
  // written by mc_testbench capture functions.
  int capture_cnt;
  int capture_start;
  bool update(bool iodir, int capture_end) {
    int capture_cnt_prev = capture_cnt;
    capture_cnt += iodir ? capture_end - capture_start: //output delta
                           capture_start - capture_end; //input delta
    return capture_cnt > capture_cnt_prev;
  }

  //Clear the sync rate controls
  void clear() {
    start_latency = -1;
    init_interval = -1;
    ramp_down_cyc = -1;
  }
};

inline std::ostream& operator<<(std::ostream& os, const mc_sync_ctrl& sctrl) {
  os << "start_latency=" << sctrl.start_latency << ", init_interval=" << sctrl.init_interval 
     << ", ramp_down_cyc=" << sctrl.ramp_down_cyc << ", pause_on_stall=" << sctrl.pause_on_stall;
  return os;
}

#endif //MC_SYNC_CTRL_H