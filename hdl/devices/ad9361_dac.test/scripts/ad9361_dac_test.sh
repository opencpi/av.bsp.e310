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

FOUND_PLATFORMS=$(./target-$OCPI_TOOL_DIR/get_comma_separated_ocpi_platforms)
if [ "$FOUND_PLATFORMS" != "e3xx" ]; then
  printf "platform found which is not supported: "
  echo $FOUND_PLATFORMS
  echo "TEST FAILED"
  exit 1
fi

touch toberemoved.log
rm *log > /dev/null 2>&1

if [ -d odata ]; then
  rm -rf odata
fi

mkdir odata

# SINGLE_PORT is set to true by default because it is the simplest case for CMOS
# Remove this line to enable switching between single port and dual port
export SINGLE_PORT=true

if [ -n "$SINGLE_PORT" ]; then
  echo Single Port mode
  MODE="CMOS mode 2"
  APP_XML=ad9361_test_1r1t_cmos_mode_2_app.xml
else
  MODE="CMOS mode 5"
  APP_XML=ad9361_test_1r1t_cmos_mode_5_app.xml
fi

if [ ! -f $APP_XML ]; then
  echo "app xml not found: $APP_XML"
  echo "(pwd is: $PWD)"
  exit 1
fi

DO_PRBS=1
if [ ! -z "$1" ]; then
  if [ "$1" == "disableprbs" ]; then # ONLY DO THIS TO SAVE TIME IF YOU KNOW
                                     # PRBS IS ALREADY WORKING
    DO_PRBS=0
  fi
fi

if [ "$DO_PRBS" == "1" ]; then
  echo "Running PRBS Built-In-Self-Test across range of sample rates for $MODE"
  OCPI_LIBRARY_PATH=$OCPI_LIBRARY_PATH:./assemblies/:$OCPI_PROJECT_PATH:../lib:$OCPI_CDK_DIR/../projects/core/exports/lib/components ./scripts/AD9361_BIST_PRBS.sh $APP_XML 2>&1 | tee odata/AD9361_BIST_PRBS.log
  if [ "$?" !=  "0" ]; then
    cat odata/AD9361_BIST_PRBS.log
    echo "TEST FAILED"
    exit 1
  fi

  diff odata/AD9361_BIST_PRBS.log scripts/AD9361_BIST_PRBS.e3xx.golden
  XX=$?

  if [ "$XX" !=  "0" ]; then
    echo "TEST FAILED"
    exit 1
  fi
fi

echo "Running loopback Built-In-Self-Test across range of sample rates for $MODE"
OCPI_LIBRARY_PATH=$OCPI_LIBRARY_PATH:./assemblies/:$OCPI_PROJECT_PATH:../lib:$OCPI_CDK_DIR/../projects/core/exports/lib/components ./scripts/AD9361_BIST_loopback.sh $APP_XML 2>&1 | tee odata/AD9361_BIST_loopback.log
if [ "$?" !=  "0" ]; then
  cat odata/AD9361_BIST_loopback.log
  echo "TEST FAILED"
  exit 1
fi

diff odata/AD9361_BIST_loopback.log scripts/AD9361_BIST_loopback.delays_11_0_12_0.use_ext_refclk.golden
XX=$?

if [ "$SX" ==  "0" ]; then
  echo "TEST PASSED"
else
  echo "TEST FAILED"
  exit 1
fi

exit 0
