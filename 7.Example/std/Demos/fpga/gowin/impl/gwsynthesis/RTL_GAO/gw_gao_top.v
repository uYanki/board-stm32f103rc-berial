module gw_gao(
    ram_clk,
    dat,
    \ram_test/addr[7] ,
    \ram_test/addr[6] ,
    \ram_test/addr[5] ,
    \ram_test/addr[4] ,
    \ram_test/addr[3] ,
    \ram_test/addr[2] ,
    \ram_test/addr[1] ,
    \ram_test/addr[0] ,
    \ram_test/ramout[7] ,
    \ram_test/ramout[6] ,
    \ram_test/ramout[5] ,
    \ram_test/ramout[4] ,
    \ram_test/ramout[3] ,
    \ram_test/ramout[2] ,
    \ram_test/ramout[1] ,
    \ram_test/ramout[0] ,
    \ram_test/ram_inst/dout[7] ,
    \ram_test/ram_inst/dout[6] ,
    \ram_test/ram_inst/dout[5] ,
    \ram_test/ram_inst/dout[4] ,
    \ram_test/ram_inst/dout[3] ,
    \ram_test/ram_inst/dout[2] ,
    \ram_test/ram_inst/dout[1] ,
    \ram_test/ram_inst/dout[0] ,
    \ram_test/dout ,
    \ram_test/cs_i ,
    \ram_test/cstate[4] ,
    \ram_test/cstate[3] ,
    \ram_test/cstate[2] ,
    \ram_test/cstate[1] ,
    \ram_test/cstate[0] ,
    \ram_test/nstate[4] ,
    \ram_test/nstate[3] ,
    \ram_test/nstate[2] ,
    \ram_test/nstate[1] ,
    \ram_test/nstate[0] ,
    clk_i,
    tms_pad_i,
    tck_pad_i,
    tdi_pad_i,
    tdo_pad_o
);

input ram_clk;
input dat;
input \ram_test/addr[7] ;
input \ram_test/addr[6] ;
input \ram_test/addr[5] ;
input \ram_test/addr[4] ;
input \ram_test/addr[3] ;
input \ram_test/addr[2] ;
input \ram_test/addr[1] ;
input \ram_test/addr[0] ;
input \ram_test/ramout[7] ;
input \ram_test/ramout[6] ;
input \ram_test/ramout[5] ;
input \ram_test/ramout[4] ;
input \ram_test/ramout[3] ;
input \ram_test/ramout[2] ;
input \ram_test/ramout[1] ;
input \ram_test/ramout[0] ;
input \ram_test/ram_inst/dout[7] ;
input \ram_test/ram_inst/dout[6] ;
input \ram_test/ram_inst/dout[5] ;
input \ram_test/ram_inst/dout[4] ;
input \ram_test/ram_inst/dout[3] ;
input \ram_test/ram_inst/dout[2] ;
input \ram_test/ram_inst/dout[1] ;
input \ram_test/ram_inst/dout[0] ;
input \ram_test/dout ;
input \ram_test/cs_i ;
input \ram_test/cstate[4] ;
input \ram_test/cstate[3] ;
input \ram_test/cstate[2] ;
input \ram_test/cstate[1] ;
input \ram_test/cstate[0] ;
input \ram_test/nstate[4] ;
input \ram_test/nstate[3] ;
input \ram_test/nstate[2] ;
input \ram_test/nstate[1] ;
input \ram_test/nstate[0] ;
input clk_i;
input tms_pad_i;
input tck_pad_i;
input tdi_pad_i;
output tdo_pad_o;

wire ram_clk;
wire dat;
wire \ram_test/addr[7] ;
wire \ram_test/addr[6] ;
wire \ram_test/addr[5] ;
wire \ram_test/addr[4] ;
wire \ram_test/addr[3] ;
wire \ram_test/addr[2] ;
wire \ram_test/addr[1] ;
wire \ram_test/addr[0] ;
wire \ram_test/ramout[7] ;
wire \ram_test/ramout[6] ;
wire \ram_test/ramout[5] ;
wire \ram_test/ramout[4] ;
wire \ram_test/ramout[3] ;
wire \ram_test/ramout[2] ;
wire \ram_test/ramout[1] ;
wire \ram_test/ramout[0] ;
wire \ram_test/ram_inst/dout[7] ;
wire \ram_test/ram_inst/dout[6] ;
wire \ram_test/ram_inst/dout[5] ;
wire \ram_test/ram_inst/dout[4] ;
wire \ram_test/ram_inst/dout[3] ;
wire \ram_test/ram_inst/dout[2] ;
wire \ram_test/ram_inst/dout[1] ;
wire \ram_test/ram_inst/dout[0] ;
wire \ram_test/dout ;
wire \ram_test/cs_i ;
wire \ram_test/cstate[4] ;
wire \ram_test/cstate[3] ;
wire \ram_test/cstate[2] ;
wire \ram_test/cstate[1] ;
wire \ram_test/cstate[0] ;
wire \ram_test/nstate[4] ;
wire \ram_test/nstate[3] ;
wire \ram_test/nstate[2] ;
wire \ram_test/nstate[1] ;
wire \ram_test/nstate[0] ;
wire clk_i;
wire tms_pad_i;
wire tck_pad_i;
wire tdi_pad_i;
wire tdo_pad_o;
wire tms_i_c;
wire tck_i_c;
wire tdi_i_c;
wire tdo_o_c;
wire [9:0] control0;
wire gao_jtag_tck;
wire gao_jtag_reset;
wire run_test_idle_er1;
wire run_test_idle_er2;
wire shift_dr_capture_dr;
wire update_dr;
wire pause_dr;
wire enable_er1;
wire enable_er2;
wire gao_jtag_tdi;
wire tdo_er1;

IBUF tms_ibuf (
    .I(tms_pad_i),
    .O(tms_i_c)
);

IBUF tck_ibuf (
    .I(tck_pad_i),
    .O(tck_i_c)
);

IBUF tdi_ibuf (
    .I(tdi_pad_i),
    .O(tdi_i_c)
);

OBUF tdo_obuf (
    .I(tdo_o_c),
    .O(tdo_pad_o)
);

GW_JTAG  u_gw_jtag(
    .tms_pad_i(tms_i_c),
    .tck_pad_i(tck_i_c),
    .tdi_pad_i(tdi_i_c),
    .tdo_pad_o(tdo_o_c),
    .tck_o(gao_jtag_tck),
    .test_logic_reset_o(gao_jtag_reset),
    .run_test_idle_er1_o(run_test_idle_er1),
    .run_test_idle_er2_o(run_test_idle_er2),
    .shift_dr_capture_dr_o(shift_dr_capture_dr),
    .update_dr_o(update_dr),
    .pause_dr_o(pause_dr),
    .enable_er1_o(enable_er1),
    .enable_er2_o(enable_er2),
    .tdi_o(gao_jtag_tdi),
    .tdo_er1_i(tdo_er1),
    .tdo_er2_i(1'b0)
);

gw_con_top  u_icon_top(
    .tck_i(gao_jtag_tck),
    .tdi_i(gao_jtag_tdi),
    .tdo_o(tdo_er1),
    .rst_i(gao_jtag_reset),
    .control0(control0[9:0]),
    .enable_i(enable_er1),
    .shift_dr_capture_dr_i(shift_dr_capture_dr),
    .update_dr_i(update_dr)
);

ao_top_0  u_la0_top(
    .control(control0[9:0]),
    .trig0_i(ram_clk),
    .data_i({ram_clk,dat,\ram_test/addr[7] ,\ram_test/addr[6] ,\ram_test/addr[5] ,\ram_test/addr[4] ,\ram_test/addr[3] ,\ram_test/addr[2] ,\ram_test/addr[1] ,\ram_test/addr[0] ,\ram_test/ramout[7] ,\ram_test/ramout[6] ,\ram_test/ramout[5] ,\ram_test/ramout[4] ,\ram_test/ramout[3] ,\ram_test/ramout[2] ,\ram_test/ramout[1] ,\ram_test/ramout[0] ,\ram_test/ram_inst/dout[7] ,\ram_test/ram_inst/dout[6] ,\ram_test/ram_inst/dout[5] ,\ram_test/ram_inst/dout[4] ,\ram_test/ram_inst/dout[3] ,\ram_test/ram_inst/dout[2] ,\ram_test/ram_inst/dout[1] ,\ram_test/ram_inst/dout[0] ,\ram_test/dout ,\ram_test/cs_i ,\ram_test/cstate[4] ,\ram_test/cstate[3] ,\ram_test/cstate[2] ,\ram_test/cstate[1] ,\ram_test/cstate[0] ,\ram_test/nstate[4] ,\ram_test/nstate[3] ,\ram_test/nstate[2] ,\ram_test/nstate[1] ,\ram_test/nstate[0] }),
    .clk_i(clk_i)
);

endmodule
