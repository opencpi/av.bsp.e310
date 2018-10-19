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

REF_CLK_RATE=40e6
AD9361_MIN_ADC_RATE=2.083334e6
AD9361_MAX_ADC_RATE=61.44e6

# these are from ad9361_config_proxy-spec, which is derived from ADI's no-os headers
ENABLE=1
DISABLE=0
ENSM_MODE_FDD=3
BIST_INJ_RX=2
FIR_RX1=129 #0x81

rmfiles() {
  if [ -f $FILENAME ]; then
    rm -rf $FILENAME
  fi
  if [ -f $LOGFILENAME ]; then
    rm -rf $LOGFILENAME
  fi
}

mvfiles() {
  mv $FILENAME odata
  mv $LOGFILENAME odata
}

runtest() {
  echo $TEST_ID
  for try in $(seq 0 $MAX_NUM_APP_RETRIES); do
    rmfiles
            ./scripts/run_app.sh $APP_XML $APP_RUNTIME_SEC $twortwot > $LOGFILENAME 2>&1
    if [ -f $FILENAME ]; then
      break
    fi
    printf "retry $TEST_ID " >> odata/retries.log
    echo "firenable=$firenable samprate=$samprate twortwot=$twortwot APP_RUNTIME_SEC=$APP_RUNTIME_SEC" >> odata/retries.log
  done
}

