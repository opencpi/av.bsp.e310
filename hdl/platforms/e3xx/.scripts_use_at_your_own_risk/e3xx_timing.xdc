# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of OpenCPI <http://www.opencpi.org>
#
# OpenCPI is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# OpenCPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.

###############################################################################
# Timing Constraints for E310 daughter board signals
###############################################################################

# E3XX_CONN_CAT_DATA_CLK is the data clock from AD9361, sample rate dependent with a max rate of 61.44 MHz
set cat_data_clk_period             16.276;

create_clock -period $cat_data_clk_period -name E3XX_CONN_CAT_DATA_CLK [get_ports E3XX_CONN_CAT_DATA_CLK]

# Generate DAC output clock
#
# Modified for our design:
#   E3XX_MIMO_XCVR TX_FB_CLK_P (forwarded version of DATA_CLK_P)
create_generated_clock -name E3XX_CONN_CAT_FB_CLK -source [get_pins {ftop/pfconfig_i/E3XX_CONN_ad9361_data_sub_i/worker/dac_clock_forward/C}] -divide_by 1 -invert [get_ports {E3XX_CONN_CAT_FB_CLK}]

# TCXO clock 40 MHz
create_clock -period 25.000 -name E3XX_CONN_TCXO_CLK [get_nets E3XX_CONN_TCXO_CLK]
set_input_jitter E3XX_CONN_TCXO_CLK 0.100

# Asynchronous clock domains
set_clock_groups -asynchronous \
  -group [get_clocks -include_generated_clocks E3XX_CONN_CAT_DATA_CLK] \
  -group [get_clocks -include_generated_clocks clk_fpga_1]

# Setup ADC (AD9361) interface constraints.

# DATA_CLK_Delay-Rx_Data_Delay = 11, period/2 - 11*0.3 = 4.838
#
# t_DDRx_min = 0
set min_rx_delay 4.838
# t_DDRx_max = 1.5 (Table 49 of UG-570 for 1.8V supply - Rx Data Delay from DATA_CLK to data pins)
set max_rx_delay [expr $min_rx_delay + 1.5]
# t_DDRv_min = 0
set min_rx_frame_delay $min_rx_delay
# t_DDRv_max = 1.0 (Rx Data Delay from DATA_CLK to RX_FRAME)
set max_rx_frame_delay [expr $min_rx_delay + 1]

set_input_delay -clock [get_clocks E3XX_CONN_CAT_DATA_CLK] -max $max_rx_frame_delay [get_ports {E3XX_CONN_CAT_RX_FRAME}]
set_input_delay -clock [get_clocks E3XX_CONN_CAT_DATA_CLK] -min $min_rx_frame_delay [get_ports {E3XX_CONN_CAT_RX_FRAME}]
set_input_delay -clock [get_clocks E3XX_CONN_CAT_DATA_CLK] -max $max_rx_frame_delay [get_ports {E3XX_CONN_CAT_RX_FRAME}] -clock_fall -add_delay
set_input_delay -clock [get_clocks E3XX_CONN_CAT_DATA_CLK] -min $min_rx_frame_delay [get_ports {E3XX_CONN_CAT_RX_FRAME}] -clock_fall -add_delay

# because RX_FRAME_P is sampled on the DATA_CLK_P falling edge (we use DDR primitive as a sample-in-the-middle), the rising edge latched output is unconnected and therefore should not be used in timing analysis
set_false_path -from [get_ports E3XX_CONN_CAT_RX_FRAME] -rise_to [get_pins ftop/pfconfig_i/E3XX_CONN_ad9361_adc_sub_i/worker/supported_so_far.rx_frame_p_ddr/D]

# From observation, I am guessing that cat_fb_data_prog_dly is equivalent to
#   (FB_CLK_Delay-TX_Data_Delay)*0.3 = 3.6
#     ==> (FB_CLK_Delay-TX_Data_Delay) = 12
# If we set TX_Data_Delay to 0, this leads to an FB_CLK_Delay of 12
set cat_fb_data_prog_dly            3.6;  # Programmable skew set to delay TX data by 3.6 ns
# t_STx_max=1 (Table 49 of UG-570)
set cat_fb_data_setup               1.0;
# t_HTx_max=0 (Table 49 of UG-570)
set cat_fb_data_hold                0;

set min_tx_delay [expr $cat_fb_data_prog_dly - $cat_fb_data_hold ]
set max_tx_delay [expr $cat_fb_data_prog_dly + $cat_fb_data_setup]

set_output_delay -clock E3XX_CONN_CAT_FB_CLK -max $max_tx_delay [get_ports {E3XX_CONN_CAT_TX_FRAME}] -add_delay
set_output_delay -clock E3XX_CONN_CAT_FB_CLK -min $min_tx_delay [get_ports {E3XX_CONN_CAT_TX_FRAME}] -add_delay
set_output_delay -clock E3XX_CONN_CAT_FB_CLK -max $max_tx_delay [get_ports {E3XX_CONN_CAT_TX_FRAME}] -clock_fall -add_delay;
set_output_delay -clock E3XX_CONN_CAT_FB_CLK -min $min_tx_delay [get_ports {E3XX_CONN_CAT_TX_FRAME}] -clock_fall -add_delay;

# TCXO DAC SPI
# 12 MHz SPI clock rate
set_max_delay -datapath_only -to [get_ports E3XX_CONN_TCXO_DAC*] -from [all_registers -edge_triggered] 40
set_min_delay                -to [get_ports E3XX_CONN_TCXO_DAC*] -from [all_registers -edge_triggered] 1
