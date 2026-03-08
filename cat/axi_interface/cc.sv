








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
     bit [31:12] unused2;
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


   
int driver_open(int *drv_handle) 
{
   unsigned int status_reg = readl(ST_REG_ADDR);

   if (status_reg.in_use) return E_BUSY;
   
   writel(PH_OPEN, CTRL_REG_ADDR);

   status_reg = readl(ST_REG_ADDR);
   if (status_reg.error_code) return E_ERROR;

   *drv_handle = next_handle();
   return SUCCESS;
}

int driver_write(const int drv_handle, 
                 const unsigned int *data, 
                 const unsigned int count)
{
   if (!valid_hnd(drv_handle)) return E_NOT_OPEN;

   writel(PH_WRITE, CTRL_REG_ADDR);

   for (int i=0; i<count; i++) writel(data[i], PH_DATA_REG);

   status_reg = readl(ST_REG_ADDR);
   if (status_reg.tx_error) return E_TX_ERROR;

   return SUCCESS;
}

int driver_close(const int drv_handle)
{
   if (!valid_hnd(drv_handle)) return E_NOT_OPEN;

   writel(PH_DONE, CTRL_REG_ADDR);

   return SUCCESS;
}


