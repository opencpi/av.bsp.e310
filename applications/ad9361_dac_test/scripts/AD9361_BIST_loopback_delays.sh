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

if [ -z "$OCPI_TOOL_DIR" ]; then
  echo OCPI_TOOL_DIR env variable must be specified before running BIST_PRBS_rates.sh
  exit 1
fi
if [ ! -d target-$OCPI_TOOL_DIR ]; then
  echo "missing binary directory: (target-$OCPI_TOOL_DIR does not exist)"
fi

. ./scripts/test_utils.sh

### ------------------------------------------------------------------------ ###

[ -n "$FOUND_PLATFORMS" ] || FOUND_PLATFORMS=$(./target-$OCPI_TOOL_DIR/get_comma_separated_ocpi_platforms)
if [ "$FOUND_PLATFORMS" == "e3xx" ]; then
  MODE="CMOS mode 2"
  APP_XML=ad9361_test_1r1t_cmos_mode_2_app.xml
else
  MODE="LVDS mode"
  APP_XML=ad9361_test_1r1t_lvds_app.xml
fi

APP_RUNTIME_SEC=1

#TWO_R_TWO_T=1
TWO_R_TWO_T=0

DISABLE_LOG=0
MAX_NUM_SAMPLES=999999
REG_SYNC_FFFF=4294967295

run_delay_tests_1r1t() {

  loop2=( 0 1 )
  clkdelays=( 0 )
  datadelays=( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 )

  touch /var/volatile/toberemoved.out
  rm /var/volatile/*out

  echo " tx_data_clock_delay   tx_data_delay->"
  echo "  |"
  echo "  v"
  echo "  	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15"
  for loop in "${loop2[@]}"
  do
  for clkdelay in "${clkdelays[@]}"
  do
  printf "$clkdelay	"
  for datadelay in "${datadelays[@]}"
  do

    LOGFILENAME=odata/delays/ocpirun_clkdel"$clkdelay"_datadel"$datadelay".log
    if [ -f $LOGFILENAME ]; then
      rm -rf $LOGFILENAME
    fi

    if [ -f $FILENAME ]; then
      rm -rf $FILENAME
    fi

    if [ ! -d odata/delays ]; then
      mkdir odata/delays
    fi

    OCPI_LIBRARY_PATH=$OCPI_LIBRARY_PATH:./assemblies/:../lib P="-pfile_write=filename=$FILENAME $@" TX_DATA_CLOCK_DELAY=$clkdelay TX_DATA_DELAY=$datadelay ./scripts/run_app.sh $APP_XML $APP_RUNTIME_SEC $twortwot > $LOGFILENAME 2>&1

    if [ ! -f $FILENAME ]; then
      printf "error"
    else
      if [ ! -d odata/delays ]; then
        mkdir odata/delays
      fi

      ./target-$OCPI_TOOL_DIR/calculate_AD9361_BIST_PRBS_RX_BER $FILENAME $DISABLE_LOG $MAX_NUM_SAMPLES $REG_SYNC_FFFF | grep BER | tr -d "estimated_BER : " | tr -d "%" | tr -d "\n"
    fi
  printf "\t"

  done
  echo
  done
  clkdelays=( 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 )
  datadelays=( 0 )
  done
}

#firenables=( $DISABLE $ENABLE ) # TODO / FIXME figure out what's wrong with this
firenables=( $DISABLE)
if [ "$FOUND_PLATFORMS" == "e3xx" ]; then
  samprates=( $AD9361_MIN_ADC_RATE 20e6 25e6 30e6 )
  samprates2r2t=( $AD9361_MIN_ADC_RATE 5e6 10e6 15360000 )
else
  samprates=( $AD9361_MIN_ADC_RATE 25e6 40e6 $AD9361_MAX_ADC_RATE )
fi

clkdelays=( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 )
datadelays=( 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 )

for firenable in "${firenables[@]}"
do
  for samprate in "${samprates[@]}"
  do
    echo "FIR enabled is $firenable : "
    echo "$samprate sps : "

    FILENAME=/var/volatile/app_"$samprate"sps_fir"$firenable"_prbs.out
    run_delay_tests_1r1t \
      -pad9361_config_proxy=rx_fir_en_dis=$firenable \
      -pad9361_config_proxy=tx_sampling_freq=$samprate \
      -pad9361_config_proxy=bist_loopback=1 \
      -pdata_src=mode=lfsr \
      -pfile_write=filename=$FILENAME
#      -pad9361_config_proxy=ad9361_init=\"reference_clk_rate $REF_CLK_RATE,frequency_division_duplex_mode_enable 1,xo_disable_use_ext_refclk_enable 0,two_t_two_r_timing_enable $TWO_R_TWO_T,pp_tx_swap_enable 0,pp_rx_swap_enable 0\"

  done
done

