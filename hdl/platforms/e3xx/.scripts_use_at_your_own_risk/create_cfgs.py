#!/bin/python
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

"""
This script is used for generating the XML and XDC files for each desired platform configuration.
It uses template files and outputs files for each CMOS mode of the e3xx platform. Example
modes are 2, 3, 5 and 6. These are the CMOS Single/Dual Port Full Duplex DDR modes. Each mode
will correspond to multiple platform configurations, but a single XDC file. Therefore, the XDC
files are named 'e3xx_mimo_xcvr_mode_<mode-number>_cmos.xdc', and the XML platform configurations
are named 'cfg_<num-rx>rx_<num-tx>tx_mode_<mode-number>_cmos.xml', where num-rx and num-tx can
be 0, 1, or 2 for each mode.

Run as follows:
./create_cfgs.py 2 3 5 6
    where '2 3 5 6' can be replaced with a list of desired modes to generate
"""
import sys
import pdb

MODES = []
# Accept mode arguments or default to 2, 3
if len(sys.argv) > 1:
    argv = sys.argv[1:]
    if argv:
        while argv:
            MODES += argv[0]
            argv = argv[1:]
else:
  MODES = ["2", "3"]

# 1rx0tx, 1rx1tx... 2rx2tx
RX_COUNTS = ["0", "1", "2"]
TX_COUNTS = ["0", "1", "2"]
TEMPLATE_DIR = "template-configs/"
DEST_DIR = ""
PLAIN_XDCS = ["e3xx.xdc", "e3xx_timing.xdc"]
#XDC_DIR = "xdc-files/"
CARD_NAME = "e3xx_mimo_xcvr"

# Delay values for data_sub parameters
DATA_CLK_Delay = "11"
RX_Data_Delay  = "0"
FB_CLK_Delay   = "12"
TX_Data_Delay  = "0"

# Different data pins correspond to inputs/outputs based
# on which mode you are in. This dictionary maps mode #
# to parameters and lists of rx versus tx pins
#
# For now, the only supported modes are SP FD DDR, and DP FD DDR. As more are supported,
# entries must be added to this dictionary.
SIGNAL_MODES={

           "2": {
                 "rx": ["CAT_P0_D_0", "CAT_P0_D_1", "CAT_P0_D_2", "CAT_P0_D_3", "CAT_P0_D_4",  "CAT_P0_D_5" ],
                 "tx": ["CAT_P0_D_6", "CAT_P0_D_7", "CAT_P0_D_8", "CAT_P0_D_9", "CAT_P0_D_10", "CAT_P0_D_11"],
                 "lvds_p": "false",
                 "half_duplex_p": "false",
                 "single_port_p": "true",
                 "swap_ports_p": "false",
                 "data_rate_config_p": "DDR",
            },
           "3": {
                 "rx": ["CAT_P1_D_0", "CAT_P1_D_1", "CAT_P1_D_2", "CAT_P1_D_3", "CAT_P1_D_4",  "CAT_P1_D_5" ],
                 "tx": ["CAT_P1_D_6", "CAT_P1_D_7", "CAT_P1_D_8", "CAT_P1_D_9", "CAT_P1_D_10", "CAT_P1_D_11"],
                 "lvds_p": "false",
                 "half_duplex_p": "false",
                 "single_port_p": "true",
                 "swap_ports_p": "true",
                 "data_rate_config_p": "DDR",
            },
           "5": {
                 "rx": ["CAT_P0_D_0", "CAT_P0_D_1", "CAT_P0_D_2", "CAT_P0_D_3", "CAT_P0_D_4", "CAT_P0_D_5", "CAT_P0_D_6", "CAT_P0_D_7", "CAT_P0_D_8", "CAT_P0_D_9", "CAT_P0_D_10", "CAT_P0_D_11"],
                 "tx": ["CAT_P1_D_0", "CAT_P1_D_1", "CAT_P1_D_2", "CAT_P1_D_3", "CAT_P1_D_4", "CAT_P1_D_5", "CAT_P1_D_6", "CAT_P1_D_7", "CAT_P1_D_8", "CAT_P1_D_9", "CAT_P1_D_10", "CAT_P1_D_11"],
                 "lvds_p": "false",
                 "half_duplex_p": "false",
                 "single_port_p": "false",
                 "swap_ports_p": "false",
                 "data_rate_config_p": "DDR",
            },
           "6": {
                 "rx": ["CAT_P1_D_0", "CAT_P1_D_1", "CAT_P1_D_2", "CAT_P1_D_3", "CAT_P1_D_4", "CAT_P1_D_5", "CAT_P1_D_6", "CAT_P1_D_7", "CAT_P1_D_8", "CAT_P1_D_9", "CAT_P1_D_10", "CAT_P1_D_11"],
                 "tx": ["CAT_P0_D_0", "CAT_P0_D_1", "CAT_P0_D_2", "CAT_P0_D_3", "CAT_P0_D_4", "CAT_P0_D_5", "CAT_P0_D_6", "CAT_P0_D_7", "CAT_P0_D_8", "CAT_P0_D_9", "CAT_P0_D_10", "CAT_P0_D_11"],
                 "lvds_p": "false",
                 "half_duplex_p": "false",
                 "single_port_p": "false",
                 "swap_ports_p": "true",
                 "data_rate_config_p": "DDR",
            }
          }

# Generate a single XML property with a name and value
def xml_property(name, value, xml_type="property"):
    return "  <" + xml_type + " name=\"" + name + "\" value=\"" + value + "\"/>\n"

# Get the platform-config XML for a given mode. Do this for each property
# for the given mode
def get_mode_xml(mode, include_swap=False):
    properties = ["lvds_p", "half_duplex_p", "single_port_p"]
    if include_swap is True:
        properties.append("swap_ports_p")
    mode_xml =  ["  " + xml_property(prop, SIGNAL_MODES[mode][prop]) for prop in properties]
    if include_swap is True:
        mode_xml += ["  " + xml_property("DATA_CLK_Delay", DATA_CLK_Delay)]
        mode_xml += ["  " + xml_property("RX_Data_Delay",  RX_Data_Delay)]
        mode_xml += ["  " + xml_property("FB_CLK_Delay",   FB_CLK_Delay)]
        mode_xml += ["  " + xml_property("TX_Data_Delay",  TX_Data_Delay)]
    return mode_xml

# Generate a device XML line that has no properties set
def get_device_xml(device):
    return ["  <device name=\"" + device + "\" card=\"" + CARD_NAME + "\"/>\n"]

# Generate a device XML block and include any property settings for the given mode
def get_device_xml_with_mode(device, mode):
    device_xml = ["  <device name=\"" + device + "\" card=\"" + CARD_NAME + "\">\n"]
    if device == "ad9361_data_sub":
        include_swap = True
    else:
        include_swap = False
    return device_xml + get_mode_xml(mode, include_swap) + ["  </device>\n"]

# Collect all of the platform-config XML for most ad9361 devices for a given mode
# This includes data_sub, config, adc/dac_sub, adc/dac
def get_devices_xml(mode, rx_count, tx_count):
    devices_xml = get_device_xml_with_mode("ad9361_data_sub", mode)
    devices_xml += get_device_xml("ad9361_config")
    if int(rx_count) > 0:
        devices_xml += get_device_xml_with_mode("ad9361_adc_sub", mode)
    if int(tx_count) > 0:
        devices_xml += get_device_xml_with_mode("ad9361_dac_sub", mode)
    for i in range(0, int(rx_count)):
        devices_xml += get_device_xml("ad9361_adc" + str(i))
    for i in range(0, int(tx_count)):
        devices_xml += get_device_xml("ad9361_dac" + str(i))
    return devices_xml

# For a given mode and direction, generate input/output delays for the pins mapped to this mode
def get_constr_text(mode, rx_or_tx):
    constrs = []

    if rx_or_tx == "rx":
        delay_mode = "input"
        clock = "CAT_DATA_CLK"
    else:
        delay_mode = "output"
        clock = "CAT_FB_CLK"

    for signal in SIGNAL_MODES[mode][rx_or_tx]:
        constrs += ["set_" + delay_mode + "_delay -clock [get_clocks {E3XX_CONN_" + clock + "}] -clock_fall -min -add_delay $min_" + rx_or_tx + "_delay [get_ports {E3XX_CONN_" + signal + "}]\n",
                    "set_" + delay_mode + "_delay -clock [get_clocks {E3XX_CONN_" + clock + "}] -clock_fall -max -add_delay $max_" + rx_or_tx + "_delay [get_ports {E3XX_CONN_" + signal + "}]\n",
                    "set_" + delay_mode + "_delay -clock [get_clocks {E3XX_CONN_" + clock + "}]             -min -add_delay $min_" + rx_or_tx + "_delay [get_ports {E3XX_CONN_" + signal + "}]\n",
                    "set_" + delay_mode + "_delay -clock [get_clocks {E3XX_CONN_" + clock + "}]             -max -add_delay $max_" + rx_or_tx + "_delay [get_ports {E3XX_CONN_" + signal + "}]\n"]
    return constrs

# Every config starts with this header and points to an XDC file
def get_header_text(mode):
    return "<HdlConfig Constraints=\"" + CARD_NAME + "_mode_" + mode + "_cmos\">\n"

print >> sys.stderr, "Generating Platform Configs and XDC files for modes: " + str(MODES)

# Loop through the chosen modes and through the various #rx#tx combinations
# and generate the platform-config XML and XDC files for each
for mode in MODES:
    for rx_count in RX_COUNTS:
        for tx_count in TX_COUNTS:
            if rx_count != "0" or tx_count != "0":
                # Collect any common XML and then generate the config-specific XMl
                xml_text = [get_header_text(mode)]
                with open(TEMPLATE_DIR + "common.xml", "r") as fd:
                    xml_text += fd.readlines()

                xml_text += get_devices_xml(mode, rx_count, tx_count)
                xml_text += ["</HdlConfig>"]

                with open(DEST_DIR + "cfg_" + rx_count + "rx_" + tx_count + "tx_mode_" + mode + 
                          "_cmos.xml", "w") as fd:
                    fd.writelines(xml_text)

    # Collect the common XDC file constraints and then generate the config-specific constraints
    xdc_text = []
    for xdc_file in PLAIN_XDCS:
        with open(xdc_file, "r") as fd:
            xdc_text += fd.readlines()

    # For this config mode, there will be unique constraints for the rx/tx data pins
    for rx_or_tx in ["rx", "tx"]:
        xdc_text += get_constr_text(mode, rx_or_tx)

    with open(DEST_DIR + CARD_NAME + "_mode_" + mode + "_cmos.xdc", "w") as fd:
        fd.writelines(xdc_text)
