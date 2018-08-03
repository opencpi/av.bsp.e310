$(if $(realpath $(OCPI_CDK_DIR)),,\
  $(error The OCPI_CDK_DIR environment variable is not set correctly.))
# This is an HDL platform Makefile for the "e3xx" platform

Libraries=axi zynq sdp
ExportFiles=e3xx.xdc e3xx.mk
ComponentLibraries=devices

# Append the per-mode configurations to our list
Configurations=\
	base \
	cfg_0rx_1tx_mode_2_cmos \
	cfg_0rx_1tx_mode_3_cmos \
	cfg_0rx_2tx_mode_2_cmos \
	cfg_0rx_2tx_mode_3_cmos \
	cfg_1rx_0tx_mode_2_cmos \
	cfg_1rx_0tx_mode_3_cmos \
	cfg_1rx_1tx_mode_2_cmos \
	cfg_1rx_1tx_mode_3_cmos \
	cfg_1rx_2tx_mode_2_cmos \
	cfg_1rx_2tx_mode_3_cmos \
	cfg_2rx_0tx_mode_2_cmos \
	cfg_2rx_0tx_mode_3_cmos \
	cfg_2rx_1tx_mode_2_cmos \
	cfg_2rx_1tx_mode_3_cmos \
	cfg_2rx_2tx_mode_2_cmos \
	cfg_2rx_2tx_mode_3_cmos

include $(OCPI_CDK_DIR)/include/hdl/hdl-platform.mk


