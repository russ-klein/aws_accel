#pragma once

#include "mgc_ioport_trans_rsc_v1.h"

//------------------------------------------------------------------------------
// For abstract netlist simulation purpose only
//------------------------------------------------------------------------------
template<int streamcnt, int width>
class mgc_inout_prereg_en_trans_rsc_v1_abs
    : public mc_wire_trans_rsc_base<width,streamcnt>
{
public:
    typedef mc_wire_trans_rsc_base<width,streamcnt> base;
    MC_EXPOSE_NAMES_OF_BASE(base);
    typedef typename base::data_type data_type;

    sc_in<bool>             clk;
    sc_out<data_type>       zin;
    sc_in<data_type>        zout;
    sc_in<sc_dt::sc_logic>  lzout;

    enum { COLS = base::COLS };
    enum { DRV = streamcnt}; // driving value is stored at row position streamcnt of data
    
    virtual bool is_combinational()
    {
        return true;
    }

    virtual void require_driving_value_adjustments(int RH, int BaseAddr, int CH, int BaseBit)
    {
        const int row = this->get_current_in_row();
        if (BaseAddr <= row && row <= RH)
        {
            this->_value_changed.notify(SC_ZERO_TIME);
        }
    }
    
    virtual void adjust_driving_value(int row, int idx_lhs, int vwidth, sc_lv_base& rhs, int rhs_idx)
    {
        if (row == this->get_current_in_row())
        {
            this->set_value(DRV, idx_lhs, vwidth, rhs, rhs_idx);
        }
    }

    SC_HAS_PROCESS(mgc_inout_prereg_en_trans_rsc_v1_abs);
    mgc_inout_prereg_en_trans_rsc_v1_abs(const sc_module_name& name, bool phase, double clk_skew_delay=0.0) 
        : base(name,phase,clk_skew_delay)
        , clk("clk")
        , zin("zin")
        , zout("zout")
        , lzout("lzout")
    {
        MC_METHOD(my_at_active_clk);
        this->sensitive << (phase ? this->clk.pos() : this->clk.neg()); //active edge
        this->dont_initialize();

        SC_METHOD(update_z);
        this->sensitive << this->_value_changed << zout << lzout;
        this->dont_initialize();

        MC_METHOD(clk_skew_delay);
        this->sensitive << this->_clk_skew_event;
        this->dont_initialize();
    }

    void clk_skew_delay()
    {
        this->exchange_value();
        if (this->lzout.read() == SC_LOGIC_1)
        {
            //std::cout<<sc_time_stamp()<<"Value of zout:"<<zout<<endl;
            auto tout = zout.read();
            auto tin  = zin.read();
            for(int i= 0; i < tout.length(); ++i)
            {
                if(tout[i] != SC_LOGIC_Z)
                {
                    tin[i] = tout[i];
                }
            }
          
            this->write_row(this->get_current_out_row(), tin);
            this->incr_current_out_row();
            this->_value_changed.notify(SC_ZERO_TIME); // display next row
        }
    }

    void update_z()
    {
        const int row = this->get_current_in_row();
        this->write_row(DRV, this->read_row(row));
        if (this->is_combinational()) 
            this->initiate_driving_value_adjustments(row, row, COLS - 1, 0);
        auto t = this->read_row(DRV);
        zin = t; //this->read_row(DRV);
        //std::cout<<sc_time_stamp()<<"Value of new zin:"<<t<<" old zin: "<<zin.read()<<endl;
    }

    void my_at_active_clk()
    {
        base::at_active_clk();
    }
};
