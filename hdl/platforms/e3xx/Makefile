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


