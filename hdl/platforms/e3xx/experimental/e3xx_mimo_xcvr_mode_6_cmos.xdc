###############################################################################
# Pin mapping
###############################################################################
## RF board connector pins

# Pin 1
set_property PACKAGE_PIN H19 [get_ports {E3XX_CONN_TX_BANDSEL_2}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_TX_BANDSEL_2}]

# Pin 2
# 3.3v DB

# Pin 3
set_property PACKAGE_PIN F19 [get_ports {E3XX_CONN_RX1B_BANDSEL_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1B_BANDSEL_0}]

#Pin 4
# 3.3v DB

#Pin 5
set_property PACKAGE_PIN G19 [get_ports {E3XX_CONN_RX1B_BANDSEL_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1B_BANDSEL_1}]

#Pin 6
set_property PACKAGE_PIN E19 [get_ports {E3XX_CONN_RX1_BANDSEL_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1_BANDSEL_0}]

#Pin 7
set_property PACKAGE_PIN E20 [get_ports E3XX_CONN_VCTXRX2_V2]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCTXRX2_V2]

#Pin 8
set_property PACKAGE_PIN G21 [get_ports {E3XX_CONN_RX1_BANDSEL_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1_BANDSEL_1}]

#Pin 9
set_property PACKAGE_PIN G22 [get_ports E3XX_CONN_TX_ENABLE1A]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_TX_ENABLE1A]

#Pin 10
set_property PACKAGE_PIN G20 [get_ports {E3XX_CONN_RX1_BANDSEL_2}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1_BANDSEL_2}]

#Pin 11
set_property PACKAGE_PIN H22 [get_ports E3XX_CONN_TX_ENABLE2A]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_TX_ENABLE2A]

#Pin 12
set_property PACKAGE_PIN F22 [get_ports {E3XX_CONN_TX_BANDSEL_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_TX_BANDSEL_0}]

#Pin 13
set_property PACKAGE_PIN A17 [get_ports E3XX_CONN_TX_ENABLE1B]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_TX_ENABLE1B]

#Pin 14
set_property PACKAGE_PIN F21 [get_ports {E3XX_CONN_TX_BANDSEL_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_TX_BANDSEL_1}]

#Pin 15
set_property PACKAGE_PIN B16 [get_ports E3XX_CONN_TX_ENABLE2B]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_TX_ENABLE2B]

#Pin 16
set_property PACKAGE_PIN J21 [get_ports E3XX_CONN_DB_SCL]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_DB_SCL]

#Pin 17
set_property PACKAGE_PIN A19 [get_ports {E3XX_CONN_RX1C_BANDSEL_0}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1C_BANDSEL_0}]

#Pin 18
set_property PACKAGE_PIN J22 [get_ports E3XX_CONN_DB_SDA]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_DB_SDA]

#Pin 19
set_property PACKAGE_PIN B15 [get_ports {E3XX_CONN_RX1C_BANDSEL_1}]
set_property IOSTANDARD LVCMOS33 [get_ports {E3XX_CONN_RX1C_BANDSEL_1}]

#Pin 20
set_property PACKAGE_PIN K21 [get_ports E3XX_CONN_TCXO_DAC_SYNCn]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_TCXO_DAC_SYNCn]

#Pin 21
set_property PACKAGE_PIN A16 [get_ports E3XX_CONN_VCTXRX2_V1]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCTXRX2_V1]

#Pin 22
set_property PACKAGE_PIN L22 [get_ports E3XX_CONN_TCXO_DAC_SCLK]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_TCXO_DAC_SCLK]

#Pin 23
set_property PACKAGE_PIN B17 [get_ports E3XX_CONN_VCTXRX1_V2]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCTXRX1_V2]

#Pin 24
set_property PACKAGE_PIN L21 [get_ports E3XX_CONN_TCXO_DAC_SDIN]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_TCXO_DAC_SDIN]

#Pin 25
set_property PACKAGE_PIN C15 [get_ports E3XX_CONN_VCTXRX1_V1]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCTXRX1_V1]

#Pin 26
set_property PACKAGE_PIN R18 [get_ports {E3XX_CONN_DB_EXP_1_8V_5}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_5}]

#Pin 27
set_property PACKAGE_PIN E18 [get_ports E3XX_CONN_VCRX1_V1]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCRX1_V1]

#Pin 28
set_property PACKAGE_PIN T18 [get_ports {E3XX_CONN_DB_EXP_1_8V_6}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_6}]

#Pin 29
set_property PACKAGE_PIN F18 [get_ports E3XX_CONN_VCRX1_V2]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCRX1_V2]

#Pin 30
set_property PACKAGE_PIN M20 [get_ports E3XX_CONN_TCXO_CLK]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_TCXO_CLK]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets E3XX_CONN_TCXO_CLK]

#Pin 31
set_property PACKAGE_PIN F17 [get_ports E3XX_CONN_VCRX2_V1]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCRX2_V1]

#Pin 32
set_property PACKAGE_PIN M15 [get_ports {E3XX_CONN_DB_EXP_1_8V_8}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_8}]

#Pin 33
set_property PACKAGE_PIN G17 [get_ports E3XX_CONN_VCRX2_V2]
set_property IOSTANDARD LVCMOS33 [get_ports E3XX_CONN_VCRX2_V2]

#Pin 34
set_property PACKAGE_PIN J18 [get_ports {E3XX_CONN_DB_EXP_1_8V_9}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_9}]

#Pin 35
set_property PACKAGE_PIN U5 [get_ports {E3XX_CONN_CAT_CTRL_IN_2}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_IN_2}]

#Pin 36
set_property PACKAGE_PIN J20 [get_ports {E3XX_CONN_DB_EXP_1_8V_10}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_10}]

#Pin 37
set_property PACKAGE_PIN U6 [get_ports {E3XX_CONN_CAT_CTRL_IN_3}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_IN_3}]

#Pin 38
set_property PACKAGE_PIN K19 [get_ports {E3XX_CONN_DB_EXP_1_8V_11}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_11}]

#Pin 39
set_property PACKAGE_PIN AB5 [get_ports {E3XX_CONN_CAT_CTRL_OUT_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_0}]

#Pin 40
set_property PACKAGE_PIN K20 [get_ports {E3XX_CONN_CAT_CTRL_OUT_4}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_4}]

#Pin 41
set_property PACKAGE_PIN AB6 [get_ports {E3XX_CONN_CAT_CTRL_OUT_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_1}]

#Pin 42
set_property PACKAGE_PIN L19 [get_ports {E3XX_CONN_CAT_CTRL_OUT_5}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_5}]

#Pin 43
set_property PACKAGE_PIN AB7 [get_ports {E3XX_CONN_CAT_CTRL_OUT_2}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_2}]

#Pin 44
set_property PACKAGE_PIN V12 [get_ports {E3XX_CONN_CAT_CTRL_OUT_6}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_6}]

#Pin 45
set_property PACKAGE_PIN AA4 [get_ports {E3XX_CONN_CAT_CTRL_OUT_3}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_3}]

#Pin 46
set_property PACKAGE_PIN W12 [get_ports {E3XX_CONN_CAT_CTRL_OUT_7}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_OUT_7}]

#Pin 47
set_property PACKAGE_PIN T6 [get_ports {E3XX_CONN_DB_EXP_1_8V_31}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_31}]

#Pin 48
set_property PACKAGE_PIN U11 [get_ports E3XX_CONN_CAT_RESET]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_RESET]

#Pin 49
# 1.8V

#Pin 50
set_property PACKAGE_PIN W6 [get_ports E3XX_CONN_CAT_CS]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_CS]

#Pin 51
# 1.8V
#Pin 52
set_property PACKAGE_PIN W5 [get_ports E3XX_CONN_CAT_SCLK]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_SCLK]

#Pin 53
# 5V

#Pin 54
set_property PACKAGE_PIN V7 [get_ports E3XX_CONN_CAT_MOSI]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_MOSI]

#Pin 55
# 5V

#Pin 56
set_property PACKAGE_PIN W7 [get_ports E3XX_CONN_CAT_MISO]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_MISO]

#Pin 57
# 5V

#Pin 58
set_property PACKAGE_PIN V4 [get_ports {E3XX_CONN_CAT_CTRL_IN_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_IN_0}]

#Pin 59
# 5V

#Pin 60
set_property PACKAGE_PIN V5 [get_ports {E3XX_CONN_CAT_CTRL_IN_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_CTRL_IN_1}]

#Pin 61
# 1.8V

#Pin 62
set_property PACKAGE_PIN U4 [get_ports {E3XX_CONN_DB_EXP_1_8V_33}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_33}]

#Pin 63
# 1.8V

#Pin 64
set_property PACKAGE_PIN T4 [get_ports {E3XX_CONN_DB_EXP_1_8V_34}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_34}]
#Pin 65
# GND

#Pin 66
set_property PACKAGE_PIN R6 [get_ports {E3XX_CONN_DB_EXP_1_8V_32}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_32}]

#Pin 67
set_property PACKAGE_PIN AB1 [get_ports E3XX_CONN_CAT_TXNRX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_TXNRX]

#Pin 68
# GND

#Pin 69
set_property PACKAGE_PIN AB4 [get_ports E3XX_CONN_CAT_ENABLE]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_ENABLE]

#Pin 70
set_property PACKAGE_PIN M19 [get_ports E3XX_CONN_CAT_BBCLK_OUT]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_BBCLK_OUT]

#Pin 71
set_property PACKAGE_PIN AB2 [get_ports E3XX_CONN_CAT_ENAGC]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_ENAGC]

#Pin 72
# GND

#Pin 73
# GND

#Pin 74
set_property PACKAGE_PIN T16 [get_ports E3XX_CONN_CAT_SYNC]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_SYNC]

#Pin 78
set_property PACKAGE_PIN N15 [get_ports {E3XX_CONN_CAT_P1_D_11}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_11}]

#Pin 76
# GND

#Pin 100
set_property PACKAGE_PIN N22 [get_ports {E3XX_CONN_CAT_P1_D_10}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_10}]

#Pin 93
set_property PACKAGE_PIN M17 [get_ports {E3XX_CONN_CAT_P0_D_11}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_11}]

#Pin 96
set_property PACKAGE_PIN T17 [get_ports {E3XX_CONN_CAT_P1_D_9}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_9}]

#Pin 95
set_property PACKAGE_PIN N17 [get_ports {E3XX_CONN_CAT_P0_D_10}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_10}]

#Pin 98
set_property PACKAGE_PIN M22 [get_ports {E3XX_CONN_CAT_P1_D_8}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_8}]

#Pin 81
set_property PACKAGE_PIN K15 [get_ports {E3XX_CONN_CAT_P0_D_9}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_9}]

#Pin 92
set_property PACKAGE_PIN P21 [get_ports {E3XX_CONN_CAT_P1_D_7}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_7}]

#Pin 97
set_property PACKAGE_PIN N20 [get_ports {E3XX_CONN_CAT_P0_D_8}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_8}]

#Pin 94
set_property PACKAGE_PIN R20 [get_ports {E3XX_CONN_CAT_P1_D_6}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_6}]

#Pin 77
set_property PACKAGE_PIN J16 [get_ports {E3XX_CONN_CAT_P0_D_7}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_7}]

#Pin 86
set_property PACKAGE_PIN P18 [get_ports {E3XX_CONN_CAT_P1_D_5}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_5}]

#Pin 85
set_property PACKAGE_PIN K16 [get_ports {E3XX_CONN_CAT_P0_D_6}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_6}]

#Pin 90
set_property PACKAGE_PIN P17 [get_ports {E3XX_CONN_CAT_P1_D_4}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_4}]

#Pin 75
set_property PACKAGE_PIN J15 [get_ports {E3XX_CONN_CAT_P0_D_5}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_5}]

#Pin 82
set_property PACKAGE_PIN P15 [get_ports {E3XX_CONN_CAT_P1_D_3}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_3}]

#Pin 91
set_property PACKAGE_PIN M16 [get_ports {E3XX_CONN_CAT_P0_D_4}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_4}]

#Pin 88
set_property PACKAGE_PIN P20 [get_ports {E3XX_CONN_CAT_P1_D_2}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_2}]

#Pin 79
set_property PACKAGE_PIN J17 [get_ports {E3XX_CONN_CAT_P0_D_3}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_3}]

#Pin 80
set_property PACKAGE_PIN M21 [get_ports {E3XX_CONN_CAT_P1_D_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_1}]

#Pin 89
set_property PACKAGE_PIN L17 [get_ports {E3XX_CONN_CAT_P0_D_2}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_2}]

#Pin 84
set_property PACKAGE_PIN N19 [get_ports {E3XX_CONN_CAT_P1_D_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P1_D_0}]

#Pin 83
set_property PACKAGE_PIN K18 [get_ports {E3XX_CONN_CAT_P0_D_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_1}]

#Pin 102
set_property PACKAGE_PIN P22 [get_ports E3XX_CONN_CAT_TX_FRAME]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_TX_FRAME]

#Pin 87
set_property PACKAGE_PIN L16 [get_ports {E3XX_CONN_CAT_P0_D_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_CAT_P0_D_0}]

#Pin 104
set_property PACKAGE_PIN R21 [get_ports E3XX_CONN_CAT_FB_CLK]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_FB_CLK]

#Pin 99
set_property PACKAGE_PIN N18 [get_ports E3XX_CONN_CAT_RX_FRAME]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_RX_FRAME]

#Pin 103
# GND

#Pin 101
set_property PACKAGE_PIN L18 [get_ports E3XX_CONN_CAT_DATA_CLK]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_CAT_DATA_CLK]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets E3XX_CONN_CAT_DATA_CLK]

#Pin 105
# 1.8V

#Pin 106
# GND

#Pin 107
set_property PACKAGE_PIN AA8 [get_ports {E3XX_CONN_RX2_BANDSEL_2}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2_BANDSEL_2}]

#Pin 108
set_property PACKAGE_PIN Y11 [get_ports E3XX_CONN_LED_TXRX1_TX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_TXRX1_TX]

#Pin 109
set_property PACKAGE_PIN AA9 [get_ports {E3XX_CONN_RX2_BANDSEL_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2_BANDSEL_1}]

#Pin 110
set_property PACKAGE_PIN AB10 [get_ports E3XX_CONN_LED_TXRX1_RX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_TXRX1_RX]

#Pin 111
set_property PACKAGE_PIN AB9 [get_ports {E3XX_CONN_RX2_BANDSEL_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2_BANDSEL_0}]

#Pin 112
set_property PACKAGE_PIN AA12 [get_ports E3XX_CONN_LED_RX1_RX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_RX1_RX]

#Pin 113
set_property PACKAGE_PIN U10 [get_ports {E3XX_CONN_RX2C_BANDSEL_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2C_BANDSEL_1}]

#Pin 114
set_property PACKAGE_PIN U12 [get_ports E3XX_CONN_LED_TXRX2_TX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_TXRX2_TX]

#Pin 115
set_property PACKAGE_PIN Y10 [get_ports {E3XX_CONN_RX2C_BANDSEL_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2C_BANDSEL_0}]

#Pin 116
set_property PACKAGE_PIN AB11 [get_ports E3XX_CONN_LED_TXRX2_RX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_TXRX2_RX]

#Pin 117
set_property PACKAGE_PIN U9 [get_ports {E3XX_CONN_RX2B_BANDSEL_1}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2B_BANDSEL_1}]

#Pin 118
set_property PACKAGE_PIN AA11 [get_ports E3XX_CONN_LED_RX2_RX]
set_property IOSTANDARD LVCMOS18 [get_ports E3XX_CONN_LED_RX2_RX]

#Pin 119
set_property PACKAGE_PIN Y4 [get_ports {E3XX_CONN_RX2B_BANDSEL_0}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_RX2B_BANDSEL_0}]

#Pin 120
set_property PACKAGE_PIN AB12 [get_ports {E3XX_CONN_DB_EXP_1_8V_24}]
set_property IOSTANDARD LVCMOS18 [get_ports {E3XX_CONN_DB_EXP_1_8V_24}]

### Other I/O
set_property PACKAGE_PIN A22 [get_ports AVR_CS_R]
set_property IOSTANDARD LVCMOS33 [get_ports AVR_CS_R]
set_property PACKAGE_PIN B22 [get_ports AVR_IRQ]
set_property IOSTANDARD LVCMOS33 [get_ports AVR_IRQ]
set_property PACKAGE_PIN C22 [get_ports AVR_MISO_R]
set_property IOSTANDARD LVCMOS33 [get_ports AVR_MISO_R]
set_property PACKAGE_PIN A21 [get_ports AVR_MOSI_R]
set_property IOSTANDARD LVCMOS33 [get_ports AVR_MOSI_R]
set_property PACKAGE_PIN D22 [get_ports AVR_SCK_R]
set_property IOSTANDARD LVCMOS33 [get_ports AVR_SCK_R]

set_property PACKAGE_PIN E21 [get_ports ONSWITCH_DB]
set_property IOSTANDARD LVCMOS33 [get_ports ONSWITCH_DB]
set_property PULLUP TRUE [get_ports {ONSWITCH_DB}]

set_property PACKAGE_PIN Y9 [get_ports GPS_PPS]
set_property IOSTANDARD LVCMOS18 [get_ports GPS_PPS]

set_property PACKAGE_PIN D18 [get_ports PPS_EXT_IN]
set_property IOSTANDARD LVCMOS33 [get_ports PPS_EXT_IN]

set_property PACKAGE_PIN E16 [get_ports {PL_GPIO[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[0]}]
set_property PACKAGE_PIN C18 [get_ports {PL_GPIO[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[1]}]
set_property PACKAGE_PIN D17 [get_ports {PL_GPIO[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[2]}]
set_property PACKAGE_PIN D16 [get_ports {PL_GPIO[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[3]}]
set_property PACKAGE_PIN D15 [get_ports {PL_GPIO[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[4]}]
set_property PACKAGE_PIN E15 [get_ports {PL_GPIO[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {PL_GPIO[5]}]
set_property PULLDOWN TRUE [get_ports {PL_GPIO[*]}]
create_clock -name clk_fpga_0 -period 10.000 [get_nets {ftop/pfconfig_metadata_out*[clk]}]
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
create_generated_clock -name E3XX_CONN_CAT_FB_CLK -divide_by 1 -source [get_pins ftop/pfconfig_i/E3XX_CONN_ad9361_dac_sub_i/worker/data_mode_cmos.dac_clock_forward/C] [get_ports E3XX_CONN_CAT_FB_CLK] -invert

##### commented this out because:
##### https://www.xilinx.com/support/answers/62488.html
##### For Clock Modifying Blocks (CMB) such as MMCMx, PLLx,IBUFDS_GTE2, BUFR and PHASER_x primitives, you do not need to manually create the generated clocks. 
#####
##### # TODO: Note that this line should be different for different modes. E.g. this is correct for SP Full Duplex,
##### #       but for DP Full Duplex, the source should be from 'full_speed_dacs' instead of 'slow_down_dacs'
##### create_generated_clock -name dacd2_clk -divide_by 2 -source [get_pins ftop/pfconfig_i/E3XX_CONN_ad9361_dac_sub_i/worker/data_mode_cmos.slow_down_dacs.BUFR_inst/I] [get_pins ftop/pfconfig_i/E3XX_CONN_ad9361_dac_sub_i/worker/data_mode_cmos.slow_down_dacs.BUFR_inst/O]
#####

# TCXO clock 40 MHz
create_clock -period 25.000 -name E3XX_CONN_TCXO_CLK [get_nets E3XX_CONN_TCXO_CLK]
set_input_jitter E3XX_CONN_TCXO_CLK 0.100

# Asynchronous clock domains
set_clock_groups -asynchronous \
  -group [get_clocks -include_generated_clocks E3XX_CONN_CAT_DATA_CLK] \
  -group [get_clocks -include_generated_clocks clk_fpga_0]

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

###############################################################################
## Asynchronous paths
###############################################################################
set_false_path -from [get_ports E3XX_CONN_CAT_CTRL_OUT]
set_false_path -to   [get_ports E3XX_CONN_CAT_RESET]
set_false_path -to   [get_ports E3XX_CONN_RX*_BANDSEL*]
set_false_path -to   [get_ports E3XX_CONN_TX_BANDSEL*]
set_false_path -to   [get_ports E3XX_CONN_TX_ENABLE*]
set_false_path -to   [get_ports E3XX_CONN_LED_*]
set_false_path -to   [get_ports E3XX_CONN_VCRX*]
set_false_path -to   [get_ports E3XX_CONN_VCTX*]

###############################################################################
# Timing Constraints for E300 mother board
###############################################################################

# 10MHz / PPS References
create_clock -period 100.000 -name PPS_EXT_IN [get_nets PPS_EXT_IN]
create_clock -period 100.000 -name GPS_PPS [get_nets GPS_PPS]

# Asynchronous clock domains
set_clock_groups -asynchronous \
  -group [get_clocks -include_generated_clocks PPS_EXT_IN] \
  -group [get_clocks -include_generated_clocks GPS_PPS]

set_clock_groups -asynchronous \
  -group [get_clocks -include_generated_clocks PPS_EXT_IN] \
  -group [get_clocks -include_generated_clocks GPS_PPS]

###############################################################################
## Asynchronous paths
###############################################################################
set_false_path -from [get_ports ONSWITCH_DB]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_0}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_0}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_0}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_0}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_1}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_1}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_1}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_1}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_2}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_2}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_2}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_2}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_3}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_3}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_3}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_3}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_4}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_4}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_4}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_4}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_5}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_5}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_5}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_5}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_6}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_6}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_6}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_6}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_7}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_7}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_7}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_7}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_8}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_8}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_8}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_8}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_9}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_9}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_9}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_9}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_10}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_10}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_10}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_10}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_11}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}] -clock_fall -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_11}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -min -add_delay $min_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_11}]
set_input_delay -clock [get_clocks {E3XX_CONN_CAT_DATA_CLK}]             -max -add_delay $max_rx_delay [get_ports {E3XX_CONN_CAT_P1_D_11}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_0}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_0}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_0}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_0}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_1}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_1}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_1}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_1}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_2}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_2}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_2}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_2}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_3}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_3}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_3}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_3}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_4}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_4}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_4}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_4}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_5}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_5}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_5}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_5}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_6}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_6}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_6}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_6}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_7}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_7}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_7}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_7}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_8}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_8}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_8}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_8}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_9}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_9}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_9}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_9}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_10}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_10}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_10}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_10}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_11}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}] -clock_fall -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_11}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -min -add_delay $min_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_11}]
set_output_delay -clock [get_clocks {E3XX_CONN_CAT_FB_CLK}]             -max -add_delay $max_tx_delay [get_ports {E3XX_CONN_CAT_P0_D_11}]
