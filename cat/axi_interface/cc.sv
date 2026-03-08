








   typedef struct packed {
     bit open_;
     bit close_;
     bit read_;
     bit write_;
     bit [31:4] unused;
   } control_register;

   typedef struct packed {
     bit in_use;
     bit [3:1] error_code;
     bit data_ready;
     bit buffer_full;
     bit [9:6] unused1;
     bit tx_error;
     bit rx_error;
     bit [31:12] unused;
   } status_register;

   typedef enum {ST_IDLE, ST_OPEN, 
                ST_CLOSE, ST_READ, ST_WRITE} st_state_type;

   st_state_type state, next_state;

   always_comb begin
     next_state = state;
     case (state)
       ST_IDLE    : if (cr.open_)  next_state = ST_RX;
       ST_OPEN    : if (cr.read_)  next_state = ST_READ;
                    if (cr.write_) next_state = ST_WRITE; 
       ST_READ    : if (cr.write_ && read_done) next_state  = ST_WRITE;
                    if (cr.done_  && read_done) next_state  = ST_CLOSE;
       ST_WRITE   : if (cr.read_  && write_done) next_state = ST_READ;
                    if (cr.done_  && write_done) next_state = ST_CLOSE;
       ST_CLOSE   : next_state = ST_IDLE;
       default    : next_state = ST_IDLE;
     endcase
   end

   always_ff @(posedge clock, negedge reset_n) begin
     if (reset_n == 0) state = ST_IDLE;
     else state = next_state;
   end
