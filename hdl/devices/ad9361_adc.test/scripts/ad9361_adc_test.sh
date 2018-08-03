#!/bin/bash
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

# (for zed platform only) run PRBS test twice - once for zed bitstream, once
# for zed_ise bitstream
for run in {1..2}
do

FOUND_PLATFORMS=$(./target-$OCPI_TOOL_DIR/get_comma_separated_ocpi_platforms)
if [ "$FOUND_PLATFORMS" == "zed" ]; then
  if [ "$run" == "1" ]; then
    # force test 1 of 2 to test zed     bitstream (and NOT zed_ise bitstream)
    ocpihdl -d PL:0 load assemblies/ad9361_1r1t_test_adc_asm/container-ad9361_1r1t_test_adc_asm_zed_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed/target-zynq/ad9361_1r1t_test_adc_asm_zed_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed.bitz
  elif [ "$run" == "2" ]; then
    # force test 2 of 2 to test zed_ise bitstream (and NOT zed     bitstream)
    ocpihdl -d PL:0 load assemblies/ad9361_1r1t_test_adc_asm/container-ad9361_1r1t_test_adc_asm_zed_ise_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed/target-zynq_ise/ad9361_1r1t_test_adc_asm_zed_ise_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed.bitz
  fi
elif [ "$FOUND_PLATFORMS" == "zed_ise" ]; then
  if [ "$run" == "1" ]; then
    # force test 1 of 2 to test zed     bitstream (and NOT zed_ise bitstream)
    ocpihdl -d PL:0 load assemblies/ad9361_1r1t_test_adc_asm/container-ad9361_1r1t_test_adc_asm_zed_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed/target-zynq/ad9361_1r1t_test_adc_asm_zed_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed.bitz
  elif [ "$run" == "2" ]; then
    # force test 2 of 2 to test zed_ise bitstream (and NOT zed     bitstream)
    ocpihdl -d PL:0 load assemblies/ad9361_1r1t_test_adc_asm/container-ad9361_1r1t_test_adc_asm_zed_ise_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed/target-zynq_ise/ad9361_1r1t_test_adc_asm_zed_ise_cfg_1rx_0tx_fmcomms_2_3_lpc_lvds_cnt_1rx_0tx_thruasm_fmcomms_2_3_lpc_LVDS_zed.bitz
  fi
elif [ "$FOUND_PLATFORMS" == "ml605" -o "$FOUND_PLATFORMS" == "e3xx" ]; then
  if [ "$run" == "2" ]; then
    continue
  fi
else
  printf "platform found which is not supported: "
  echo $FOUND_PLATFORM
  echo "TEST FAILED"
  exit 1
fi

touch toberemoved.log
rm *log > /dev/null 2>&1

if [ -d odata ]; then
  rm -rf odata
fi

mkdir odata

if [ "$FOUND_PLATFORMS" == "e3xx" ]; then
  # SINGLE_PORT is set to true by default because it is the simplest case for CMOS
  # Remove this line to enable switching between single port and dual port
  export SINGLE_PORT=true

  if [ -n "$SINGLE_PORT" ]; then
    echo Single Port mode
    MODE="CMOS mode 2"
    APP_XML=ad9361_adc_test_1r1t_cmos_mode_2.xml
  else
    MODE="CMOS mode 5"
    APP_XML=ad9361_adc_test_1r1t_cmos_mode_5.xml
  fi
else
  MODE="LVDS mode"
  APP_XML=ad9361_adc_test_1r1t_lvds_app.xml
fi

if [ ! -z "$1" ]; then
  APP_XML=$1
fi

if [ ! -f $APP_XML ]; then
  echo "app xml not found: $APP_XML"
  echo "(pwd is: $PWD)"
  exit 1
fi

echo "Running PRBS Built-In-Self-Test across range of sample rates for 1R1T $MODE"

OCPI_LIBRARY_PATH=$OCPI_LIBRARY_PATH:$OCPI_CDK_DIR/../projects/core/exports:./assemblies/:$OCPI_PROJECT_PATH:../lib ./scripts/AD9361_BIST_PRBS.sh $APP_XML 2>&1 | tee odata/AD9361_BIST_PRBS.log
if [ "$?" !=  "0" ]; then
  echo "TEST FAILED"
  exit 1
fi

FOUND_PLATFORMS=$(./target-$OCPI_TOOL_DIR/get_comma_separated_ocpi_platforms)
XX="1"
if [ "$FOUND_PLATFORMS" == "zed" ]; then
  diff odata/AD9361_BIST_PRBS.log scripts/AD9361_BIST_PRBS.zed.golden
  XX=$?
elif [ "$FOUND_PLATFORMS" == "zed_ise" ]; then
  diff odata/AD9361_BIST_PRBS.log scripts/AD9361_BIST_PRBS.zed_ise.golden
  XX=$?
elif [ "$FOUND_PLATFORMS" == "ml605" ]; then
  diff odata/AD9361_BIST_PRBS.log scripts/AD9361_BIST_PRBS.ml605.golden
  XX=$?
elif [ "$FOUND_PLATFORMS" == "e3xx" ]; then
  diff odata/AD9361_BIST_PRBS.log scripts/AD9361_BIST_PRBS.e3xx.golden
  XX=$?
else
  printf "platform found which is not supported: "
  echo $FOUND_PLATFORM
  echo "TEST FAILED"
  exit 1
fi
X=$XX

if [ "$X" ==  "0" ]; then
  echo "TEST PASSED"
else
  echo "TEST FAILED"
  exit 1
fi

done # for run in {1..2}

#echo "Running additional reports: (PRBS Built-In-Self-Test across range of clock and data delays)"
#./scripts/AD9361_BIST_PRBS_delays.sh $APP_XML > odata/AD9361_BIST_PRBS_delays.log
exit 0
