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
  exit 1
fi

# sometimes app fail's because ad9361_config_proxy.rcc worker's no-os library
# (from ADI) times out when setting the tx frequency, causing the app to
# immediately exit
MAX_NUM_APP_RETRIES=5 # set to 0 to disable retries

. ./scripts/test_utils.sh

### ------------------------------------------------------------------------ ###

APP_XML=$1

dogrep() {
  # echo out import property readbacks so they get put into the log and can
  # be compared against the golden file
  grep "ad9361_config_proxy.bb_pll_is_locked " $LOGFILENAME | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.rx_pll_is_locked " $LOGFILENAME | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.tx_pll_is_locked " $LOGFILENAME | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.rx_fir_en_dis " $LOGFILENAME    | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.rx_sampling_freq " $LOGFILENAME | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.bist_prbs " $LOGFILENAME        | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.ad9361_init " $LOGFILENAME      |              sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.DATA_CLK_Delay" $LOGFILENAME    | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.Rx_Data_Delay" $LOGFILENAME     | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.FB_CLK_Delay" $LOGFILENAME      | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_config_proxy.Tx_Data_Delay" $LOGFILENAME     | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_data_sub.DATA_CLK_Delay" $LOGFILENAME        |              sed "s/[0-9]*://g"
  grep "ad9361_data_sub.RX_Data_Delay" $LOGFILENAME         |              sed "s/[0-9]*://g"
  grep "ad9361_data_sub.FB_CLK_Delay" $LOGFILENAME          |              sed "s/[0-9]*://g"
  grep "ad9361_data_sub.TX_Data_Delay" $LOGFILENAME         |              sed "s/[0-9]*://g"
  grep "qadc.overrun " $LOGFILENAME                         | tail -n +2 | sed "s/[0-9]*://g"
  grep "qdac.underrun " $LOGFILENAME                        | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_adc_sub.r1_samps_dropped" $LOGFILENAME       | tail -n +2 | sed "s/[0-9]*://g"
  grep "ad9361_adc_sub.r2_samps_dropped" $LOGFILENAME       | tail -n +2 | sed "s/[0-9]*://g"
  grep "file_write.bytesWritten" $LOGFILENAME               | tail -n +2 | sed "s/[0-9]*://g"
}

domd5sum() {
  TMP=./tmp.out
  if [ -f $TMP ]; then
    rm -rf $TMP
  fi
  tail -c 16384 $FILENAME > $TMP # copy last 4096 samples
  md5sum $TMP
  md5sum $GOLDEN
}

#firenables=( $DISABLE $ENABLE )
firenables=( $DISABLE )
twortwots=( 0 1 ) # force using 2R2T timing diagram regardless of number of enabled channels

FOUND_PLATFORMS=$(./target-$OCPI_TOOL_DIR/get_comma_separated_ocpi_platforms)
if [ "$FOUND_PLATFORMS" == "zed" ]; then
    # DATA_CLK_P rate rounded via floor = floor(1/5.712 ns) ~= 175.070028 MHz
    # for LVDS, max samp rate = DATA_CLK_P rate / 4         ~=  43.767507 Msps complex
  samprates=( $AD9361_MIN_ADC_RATE 25e6 30e6 35e6 40e6 43767507 )
elif [ "$FOUND_PLATFORMS" == "zed_ise" ]; then
  # DATA_CLK_P rate rounded via floor = floor(1/4.294 ns) ~= 232.883092 MHz
  # for LVDS, max samp rate = DATA_CLK_P rate / 4         ~=  58.220773 Msps complex
  samprates=( $AD9361_MIN_ADC_RATE 25e6 30e6 35e6 40e6 45e6 50e6 55e6 58220773 )
elif [ "$FOUND_PLATFORMS" == "ml605" ]; then
  samprates=( $AD9361_MIN_ADC_RATE 25e6 30e6 35e6 40e6 45e6 50e6 55e6 $AD9361_MAX_ADC_RATE )
elif [ "$FOUND_PLATFORMS" == "e3xx" ]; then
  if [ -n "$SINGLE_PORT" ]; then
    samprates=( $AD9361_MIN_ADC_RATE 15e6 20e6 25e6 30720000 )
    # There is a different supported range of sample rates for 2r2t timing in CMOS
    samprates2r2t=( $AD9361_MIN_ADC_RATE 5e6 10e6 15360000 )
  else
    samprates=( $AD9361_MIN_ADC_RATE 25e6 30e6 35e6 40e6 45e6 50e6 55e6 $AD9361_MAX_ADC_RATE )
    # There is a different supported range of sample rates for 2r2t timing in CMOS
    samprates2r2t=( $AD9361_MIN_ADC_RATE 15e6 20e6 25e6 30720000 )
  fi
else
  printf "platform found which is not supported: "
  echo $FOUND_PLATFORM
  echo "TEST FAILED"
  exit 1
fi

if [ ! -d odata ]; then
  mkdir odata
fi

touch odata/retries.log

APP_RUNTIME_SEC=1
for twortwot in "${twortwots[@]}"
do
  for firenable in "${firenables[@]}"
  do
    # If in 2r2t timing, and there is a different supported range of
    # sample rates for 2r2t timing, use that range instead
    if [[ "$twortwot" == "1" && -n "${samprates2r2t[@]}" ]]; then
      samprates=( ${samprates2r2t[@]} )
    fi
    for samprate in "${samprates[@]}"
    do
      echo "FIR enabled      : $firenable"
      if [ "$twortwot" == "0" ]; then
        echo "Data port config : 1R1T"
      else
        echo "Data port config : 2R2T"
      fi
      echo "sample rate      : $samprate sps"
      echo "runtime          : $APP_RUNTIME_SEC sec"


      # only TX 64 samples which will cause and underrun since the TX duration
      # of 64 samples for any possible AD9361 DAC sample rate is less than the
      # minimum ocpirun runtime of 1 second
      TEST_ID="test forcing DAC FIFO underrun"
      FILENAME="$PREFIX"_underrun.out
      LOGFILENAME="$PREFIX"_underrun.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=num_samples=64 \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep
      mvfiles
      echo --------------------------------------------------------------------------------

      TEST_ID="loopback fixed value of 0x000"
      FILENAME="$PREFIX"_loopback0.out
      LOGFILENAME="$PREFIX"_loopback0.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=fixed \
         -pdata_src=fixed_value=0,0,0,0,0,0,0,0,0,0,0,0 \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep
      GOLDEN=./scripts/data_src_fixed0_4096samps.out.golden domd5sum
      mvfiles
      echo --------------------------------------------------------------------------------

      TEST_ID="loopback fixed value of 0xfff"
      FILENAME="$PREFIX"_loopbackf.out
      LOGFILENAME="$PREFIX"_loopbackf.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=fixed \
         -pdata_src=fixed_value=1,1,1,1,1,1,1,1,1,1,1,1 \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep
      GOLDEN=./scripts/data_src_fixedf_4096samps.out.golden domd5sum
      mvfiles
      echo --------------------------------------------------------------------------------

      TEST_ID="loopback fixed value of 0x555"
      FILENAME="$PREFIX"_loopback5.out
      LOGFILENAME="$PREFIX"_loopback5.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=fixed \
         -pdata_src=fixed_value=0,1,0,1,0,1,0,1,0,1,0,1 \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep
      GOLDEN=./scripts/data_src_fixed5_4096samps.out.golden domd5sum
      mvfiles
      echo --------------------------------------------------------------------------------

      TEST_ID="loopback fixed value of 0xaaa"
      FILENAME="$PREFIX"_loopbacka.out
      LOGFILENAME="$PREFIX"_loopbacka.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=fixed \
         -pdata_src=fixed_value=1,0,1,0,1,0,1,0,1,0,1,0 \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep
      GOLDEN=./scripts/data_src_fixeda_4096samps.out.golden domd5sum
      mvfiles
      echo --------------------------------------------------------------------------------

      # This can be useful for testing patterns that are not just alternating bits 
      # E.g bit patterns with period > 2
      #PREFIX=/var/volatile/app_"$samprate"sps_fir"$firenable"_"$twortwot"_"$APP_RUNTIME_SEC"sec
      #TEST_ID="loopback fixed value of 0x1100"
      #FILENAME="$PREFIX"_loopback12.out
      #LOGFILENAME="$PREFIX"_loopback12.log
      #P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
      #   -pad9361_config_proxy=tx_sampling_freq=$samprate \
      #   -pad9361_config_proxy=bist_loopback=1 \
      #   -pdata_src=mode=fixed \
      #   -pdata_src=fixed_value=1,0,1,0,1,0,1,1,1,1,0,0 \
      #   -pfile_write=filename=$FILENAME" \
      #  runtest
      #dogrep
      ##GOLDEN=./scripts/data_src_fixed12_4096samps.out.golden domd5sum
      #mvfiles
      #echo --------------------------------------------------------------------------------

      TEST_ID="loopback walking ones"
      FILENAME="$PREFIX"_loopbackwalking.out
      LOGFILENAME="$PREFIX"_loopbackwalking.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=walking \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep

      ## skipping md5sum since we don't have a sync-up operation like
      ## we do for calculate_AD9361_BIST_PRBS_RX_BER

      mvfiles
      echo --------------------------------------------------------------------------------

      TEST_ID="loopback LFSR"
      FILENAME="$PREFIX"_loopbacklfsr.out
      LOGFILENAME="$PREFIX"_loopbacklfsr.log
      P="-pad9361_config_proxy=rx_fir_en_dis=$firenable \
         -pad9361_config_proxy=tx_sampling_freq=$samprate \
         -pad9361_config_proxy=bist_loopback=1 \
         -pdata_src=mode=lfsr \
         -pfile_write=filename=$FILENAME" \
        runtest
      dogrep

      #no md5sum, relying on calculate_AD9361_BIST_PRBS_RX_BER

      DISABLE_DEBUG=0
      MAX_NUM_SAMPS=9999999 # just need something to pass to this parameter
      SEED_FFFFFFFF=4294967295

      BER_FILENAME=$FILENAME
      ./target-$OCPI_TOOL_DIR/calculate_AD9361_BIST_PRBS_RX_BER $BER_FILENAME $DISABLE_DEBUG $MAX_NUM_SAMPS $SEED_FFFFFFFF | grep -e BER -e ERROR
      mvfiles
      echo --------------------------------------------------------------------------------

    done
  done
done

