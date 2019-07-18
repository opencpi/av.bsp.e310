/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of OpenCPI <http://www.opencpi.org>
 *
 * OpenCPI is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * OpenCPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cmath>  //std::abs(), round()
#include <string> //std::string()

#include "OcpiApi.hh"

namespace OA = OCPI::API;
using namespace std;


#define CIC_DECIMATION_FACTOR 8

static void checkAndWarnIfAnalogFilterBandwidthIsAboveTheNyquistFreq(double basebandAnalogFilterBandwidth_Hz,
                                                                     double RFAnalogFilterBandwidth_Hz,
                                                                     bool RFAnalogFilterIsUsed,
                                                                     double ADCSampleRate_sps ) {
  double maxAnalogFilterBandwidthAppliedToADCInput_Hz = basebandAnalogFilterBandwidth_Hz;
  if(RFAnalogFilterIsUsed)
  {
    maxAnalogFilterBandwidthAppliedToADCInput_Hz = std::min(basebandAnalogFilterBandwidth_Hz,
                                                            RFAnalogFilterBandwidth_Hz);
  }
  double NyquistFreq_Hz = ADCSampleRate_sps / 2.;
  if(maxAnalogFilterBandwidthAppliedToADCInput_Hz > NyquistFreq_Hz)
  {
    if(RFAnalogFilterIsUsed)
    {
      std::cout << std::endl <<
          "WARNING: regarding input parameters, at least one of rf_bw (" << RFAnalogFilterBandwidth_Hz <<
          " Hz) or bb_bw (" << basebandAnalogFilterBandwidth_Hz <<
          " Hz) are above the Nyquist frequency set by data_bw (Nyquist frequency is " << CIC_DECIMATION_FACTOR << "*data_bw/2, which is " <<
          NyquistFreq_Hz << " Hz) - aliasing may occur!! - set at least one of rf_bw or bb_bw to below " << CIC_DECIMATION_FACTOR << "*data_bw/2 in the future" << std::endl << std::endl;
    }
    else
    {
      std::cout << std::endl <<
          "WARNING: regarding input parameters, bb_bw (" << basebandAnalogFilterBandwidth_Hz <<
          " Hz) is above the Nyquist frequency set by data_bw (Nyquist frequency is " << CIC_DECIMATION_FACTOR << "*data_bw/2, which is " <<
          NyquistFreq_Hz << " Hz) - aliasing may occur!! - set bb_bw to below " << CIC_DECIMATION_FACTOR << "*data_bw/2 in the future" << std::endl << std::endl;
    }
  }
}

static void usage(const char *name, const char *error_message) {
  fprintf(stderr,
	  "%s\n"
	  "Usage is: %s <rf_tune_freq> <data_bw> <rf_bw> <rf_gain> <bb_bw> <bb_gain> <if_tune_freq> <runtime> <enable_timestamps> <frontend> <smb_channel>\n"
	  "    rf_tune_freq       # RF tuning frequency in MHz\n"
	  "    data_bw            # Bandwidth of the data being written to file in MS/s\n"
	  "    rf_bw              # RF bandwidth in MHz\n"
	  "    rf_gain            # RF gain in dB\n"
	  "    bb_bw              # Baseband bandwidth in MHz\n"
	  "    bb_gain            # Baseband gain in dB (5 - 60)\n"
	  "    if_tune_freq       # IF tuning frequency in MHz. 0 disables IF tuning)\n"
	  "    runtime            # Runtime of app in seconds (1 - 5)\n"
  	  "    enable_timestamps  # Enable timestamps (1 or 0)\n"
    "    frontend           # Must be e3xx\n"
    "    smb_channel        # specify which SMB is used\n"
         "Example: rx_gps_timing_app 2400 2.5 -1 24 2.5 -1 0.1 1 1 e3xx TRXA\n",
                 error_message,name);
  exit(1);
}


static void print_limits(const char *error_message, double current_value, double lower_limit, double upper_limit) {
  fprintf(stderr,
  "%s"
  "Value given of %f is outside the range of %f to %f.\n",
  error_message, current_value, lower_limit, upper_limit);
  exit(1);
}

void warn_max_rf_rx_input_power(const char* frontend_name,
                                double absolute_max_power_dBm)
{
  std::ostringstream oss;
  oss << "\n***** WARNING: " << frontend_name;
  oss << " absolute maxmimum RF RX input power is ";
  oss << absolute_max_power_dBm << " dBm *****\n\n";
  std::cout << oss.str().c_str();
}

int main(int argc, char **argv) {
  OA::Container *container;
  std::string xml_name;

  std::string smb_channel("TRXA"); // default for E3xx platform

  //Check what platform we are on
  bool validContainerFound = false;
  for (unsigned n = 0; (container = OA::ContainerManager::get(n)); n++)
  {
    if (container->platform() == "e3xx")
    {
      if(argc == 12) {
        smb_channel = std::string(argv[11]);

        if (smb_channel != "TRXA" &&
            smb_channel != "RX2A" &&
            smb_channel != "RX2B" &&
            smb_channel != "TRXB") {
          usage(argv[0],"Error: invalid e3xx channel specified: only supported values are {TRXA, RX2A, RX2B, TRXB}).\n");
        }
      }
      validContainerFound = true;
    }
    if (validContainerFound)
    {
      break;
    }
  }

  if (!validContainerFound)
  {
    fprintf(stderr,"Error: not on a valid platform (no containers found)\n");
    return 1;
  }

  //Assign the appropriate application XML file
  xml_name = "rx_gps_timestamping_e3xx_app.xml";

  const char *argv0 = strrchr(argv[0], '/');
  argv0 = argv[0];
  bool overrunFlag = false;
  if ((argc != 10) and (argc != 11) and (argc != 12))//Check number of inputs
    usage(argv0,"Error: wrong number of arguments.\n");

  //Verify inputs are within valid ranges
  double rf_tune_freq = strtod(argv[1],NULL);
  double data_bw = strtod(argv[2],NULL);
  double rf_bw = strtod(argv[3],NULL);
  double rf_gain = strtod(argv[4],NULL);
  double bb_bw = strtod(argv[5],NULL);
  double if_tune_freq = strtod(argv[7],NULL);
  double bb_gain = strtod(argv[6],NULL);
  uint8_t runtime = atoi(argv[8]);
  uint8_t timestamps = atoi(argv[9]);
  if(timestamps == 0) {
    printf("ERROR: timestamps must be 1 since they cannot yet be disabled in this app\n");
    exit(1);
  }
  char rx_sample_rate_str [25];
  OA::Short phase_inc;

  // multiply to compensate for cic decimate
  double rx_sample_rate_MHz = data_bw * CIC_DECIMATION_FACTOR;

  std::string value;

  const double rx_sample_rate_max_MHz_e3xx = 30.72;
  const double data_bw_max_MHz_e3xx = rx_sample_rate_max_MHz_e3xx / CIC_DECIMATION_FACTOR;
  double data_bw_max_MHz;
  data_bw_max_MHz = data_bw_max_MHz_e3xx;

  //Check arguments NOT associated with RF frontend

  // sample rate is lower-limited by IF freq. complex mixer valid range of IF
  // freqs is [-0.5*samprate 32767/32768*samprate], so valid sample rate must
  // be greater than the largest IF magnitude, i.e. samp rate must be >=
  // abs(IFfreq)/0.5
  if (data_bw < std::abs(if_tune_freq)/0.5)
  {
    fprintf(stderr,
    "data_bw value given of %.15f is below the minimum data_bw of %0.15f which is required by the specified IF tune freq, either lower IF tune freq or increase data_bw\n",
    data_bw, std::abs(if_tune_freq/0.5));
    exit(1);
  }

  if (runtime < 1 || runtime > 5)
  {
    print_limits("Error: invalid runtime.\n", runtime, 1, 5);
  }
  if (timestamps != 1 && timestamps != 0)
  {
    print_limits("Error: invalid enable_timestamps.\n", timestamps, 0, 1);
  }

  // IF freq range is limited by sample rate. complex mixer valid range of IF
  // freqs is [-0.5*samprate 32767/32768*samprate]
  double min = -0.5         *rx_sample_rate_MHz;
  double max = 32767./32768.*rx_sample_rate_MHz;
  if((if_tune_freq < min) or (if_tune_freq > max))
  {
    print_limits("Error: invalid if_tune_freq.\n", if_tune_freq, min, max);
  }

  try {
  printf("RX GPS Timestamping App\n");
  printf("ADC->Complex Mixer TS->CIC Decimator TS->File Write\n");
  printf("Other application properties in XML file: %s\n", xml_name.c_str());

  OA::Application app(xml_name.c_str(), NULL);
  app.initialize();
  app.setProperty("rx", "config", ("duplex_mode FDD,SMA_channel " + smb_channel).c_str());

  //Check arguments associated with RF frontend
  double rx_sample_rate_min_MHz, rx_sample_rate_max_MHz;
  double rx_frequency_min_MHz, rx_frequency_max_MHz;
  double rx_rf_cutoff_frequency_min_MHz, rx_rf_cutoff_frequency_max_MHz;
  double rx_rf_gain_min_dB, rx_rf_gain_max_dB;
  double rx_bb_cutoff_frequency_min_MHz, rx_bb_cutoff_frequency_max_MHz;
  double rx_bb_gain_min_dB, rx_bb_gain_max_dB;

  app.getProperty("rx","sample_rate_min_MHz", value);
  rx_sample_rate_min_MHz = atof(value.c_str());
  app.getProperty("rx","sample_rate_max_MHz", value);
  rx_sample_rate_max_MHz = atof(value.c_str());

  if (rx_sample_rate_MHz < rx_sample_rate_min_MHz || rx_sample_rate_MHz > rx_sample_rate_max_MHz)
    {
      const double data_bw_min_MHz = rx_sample_rate_min_MHz / CIC_DECIMATION_FACTOR;
      print_limits("Error: invalid data_bw.\n", data_bw, data_bw_min_MHz, data_bw_max_MHz);
    }

  app.getProperty("rx","frequency_min_MHz", value);
  rx_frequency_min_MHz = atof(value.c_str());
  app.getProperty("rx","frequency_max_MHz", value);
  rx_frequency_max_MHz = atof(value.c_str());
  if (rf_tune_freq < rx_frequency_min_MHz || rf_tune_freq > rx_frequency_max_MHz)
    {
      print_limits("Error: invalid rx_rf_center_freq.\n", rf_tune_freq, rx_frequency_min_MHz, rx_frequency_max_MHz);
    }

  app.getProperty("rx","rf_cutoff_frequency_min_MHz", value);
  rx_rf_cutoff_frequency_min_MHz = atof(value.c_str());
  app.getProperty("rx","rf_cutoff_frequency_max_MHz", value);
  rx_rf_cutoff_frequency_max_MHz = atof(value.c_str());
  if (rf_bw < rx_rf_cutoff_frequency_min_MHz || rf_bw > rx_rf_cutoff_frequency_max_MHz)
    {
      print_limits("Error: invalid rf_bw.\n", rf_bw, rx_rf_cutoff_frequency_min_MHz, rx_rf_cutoff_frequency_max_MHz);
    }

  app.getProperty("rx","rf_gain_min_dB", value);
  rx_rf_gain_min_dB = atof(value.c_str());
  app.getProperty("rx","rf_gain_max_dB", value);
  rx_rf_gain_max_dB = atof(value.c_str());
  if (rf_gain < rx_rf_gain_min_dB || rf_gain > rx_rf_gain_max_dB)
    {
      print_limits("Error: invalid rf_gain.\n", rf_gain, rx_rf_gain_min_dB, rx_rf_gain_max_dB);
    }

  app.getProperty("rx","bb_cutoff_frequency_min_MHz", value);
  rx_bb_cutoff_frequency_min_MHz = atof(value.c_str());
  app.getProperty("rx","bb_cutoff_frequency_max_MHz", value);
  rx_bb_cutoff_frequency_max_MHz = atof(value.c_str());
  if (bb_bw < rx_bb_cutoff_frequency_min_MHz || bb_bw > rx_bb_cutoff_frequency_max_MHz)
    {
      print_limits("Error: invalid bb_bw.\n", bb_bw, rx_bb_cutoff_frequency_min_MHz, rx_bb_cutoff_frequency_max_MHz);
    }

  app.getProperty("rx","bb_gain_min_dB", value);
  rx_bb_gain_min_dB = atof(value.c_str());
  app.getProperty("rx","bb_gain_max_dB", value);
  rx_bb_gain_max_dB = atof(value.c_str());
  if (bb_gain < rx_bb_gain_min_dB || bb_gain > rx_bb_gain_max_dB)
    {
      print_limits("Error: invalid bb_gain.\n", bb_gain, rx_bb_gain_min_dB, rx_bb_gain_max_dB);
    }

  //Setup front end
  app.setProperty("rx","frequency_Mhz", argv[1]);

  sprintf(rx_sample_rate_str,  "%f", rx_sample_rate_MHz);
  app.setProperty("rx","sample_rate_Mhz", rx_sample_rate_str);

  // It is desired that setting a + IF freq results in mixing *down*.
  // Because complex_mixer's NCO mixes *up* for + freqs (see complex mixer
  // datasheet), IF tune freq must be negated in order to achieve the
  // desired effect.
  double nco_output_freq = -if_tune_freq;

  // todo this math might be better off in a small proxy that sits on top of complex_mixer
  // from complex mixer datasheet, nco_output_freq =
  // sample_freq * phs_inc / 2^phs_acc_width, phs_acc_width is fixed at 16
  phase_inc = round(nco_output_freq/rx_sample_rate_MHz*65536.);

  if (phase_inc == 0)
  {
    app.setProperty("complex_mixer_ts","enable", "false");
  }
  else
  {
     //std::cout << "setting complex mixer phase_inc = " << phase_inc << "\n";
     app.setPropertyValue<OA::Short>("complex_mixer_ts","phs_inc", phase_inc);
  }

  app.setProperty("rx","rf_cutoff_frequency_Mhz", argv[3]);
  app.setProperty("rx","rf_gain_dB", argv[4]);
  app.setProperty("rx","bb_cutoff_frequency_Mhz", argv[5]);
  app.setProperty("rx","bb_gain_dB", argv[6]);

  app.getProperty("rx","rf_cutoff_frequency_Mhz", value);
  double rf_cutoff_frequency_Mhz = atof(value.c_str());
  app.getProperty("rx","bb_cutoff_frequency_Mhz", value);
  double bb_cutoff_frequency_Mhz = atof(value.c_str());
  bool currentFrontendUses_rx_rf_cutoff_frequency_MHz = (rf_cutoff_frequency_Mhz < 0.);
  checkAndWarnIfAnalogFilterBandwidthIsAboveTheNyquistFreq( bb_cutoff_frequency_Mhz*1000000,
                                                            rf_cutoff_frequency_Mhz*1000000,
                                                            currentFrontendUses_rx_rf_cutoff_frequency_MHz,
                                                            rx_sample_rate_MHz*1000000 );

  
  {
  OA::Property p(app, "rx", "frequency_MHz");
  double RF_tune_frequency = p.getValue<double>();
  printf("RF tune frequency     : %0.15f\tMHz (nominal)\n", RF_tune_frequency);
  }

  {
  OA::Property p(app, "rx", "sample_rate_MHz");
  double sample_rate_MHz = p.getValue<double>();
  printf("Data bandwidth to file: %0.15f\tMS/s complex (nominal)\n",sample_rate_MHz / CIC_DECIMATION_FACTOR);
  }

  {
  OA::Property p(app, "rx", "rf_cutoff_frequency_MHz");
  double RF_cutoff_frequency = p.getValue<double>();
  printf("RF cutoff frequency   : %0.15f \tMHz (nominal)\n", RF_cutoff_frequency);
  }

  {
  OA::Property p(app, "rx", "rf_gain_dB");
  double RF_gain = p.getValue<double>();
  printf("RF gain               : %0.15f \tdB (nominal)\n", RF_gain);
  }

  {
  OA::Property p(app, "rx", "bb_cutoff_frequency_MHz");
  double BB_cutoff_frequency = p.getValue<double>();
  printf("BB cutoff frequency   : %0.15f\tMHz (nominal)\n", BB_cutoff_frequency);
  }

  {
  double BB_gain = app.getPropertyValue<double>("rx", "bb_gain_dB");
  printf("BB gain               : %0.15f\tdB (nominal)\n",  BB_gain);
  }

  {
  OA::Property p(app, "rx", "sample_rate_MHz");
  double sample_rate_MHz = p.getValue<double>();
  double phs_inc = app.getPropertyValue<OA::Short>("complex_mixer_ts", "phs_inc");

  // todo this math might be better off in a small proxy that sits on top of complex_mixer.hdl
  // from complex mixer datasheet, nco_output_freq =
  // sample_freq * phs_inc / 2^phs_acc_width, phs_acc_width is fixed at 16
  double nco_output_freq = sample_rate_MHz * phs_inc / 65536.;

  // It is desired that setting a + IF freq results in mixing *down*. Because
  // complex_mixer's NCO mixes *up* for + freqs (see complex mixer datasheet),
  // IF frequency is accurately reported as the negative of the NCO freq.
  double IF_tune_frequency = -nco_output_freq;
  printf("IF tune frequency     : %.15f\tMHz\n", IF_tune_frequency);
  }

  printf("Runtime               : %s s\n",   argv[8]);
  printf("Timestamps enabled    : %s \n",    timestamps ? "true" : "false");
  std::string value;
  app.getProperty("time_server","status",value);
  uint32_t valid1pps=atol(value.c_str())&0x08000000;
  printf("Valid 1 PPS           : %s \n", valid1pps ? "true" : "false");

  app.start();

  printf("App runs for %i seconds...\n",runtime);
  while (runtime > 0)
  {
    sleep(1);
    runtime --;
    app.getProperty("qadc_ts","overrun", value);
    if (strcmp("true", value.c_str()) == 0)
    {
      overrunFlag  = true;
    }
  }

  app.stop();
  app.getProperty("file_write","bytesWritten", value);
  printf("Bytes to file : %s\n", value.c_str());
  if (overrunFlag)
  {
    printf("WARNING: data_bw was high enough that data could not be written to file fast enough, resulting in samples being dropped, try a lower data_bw\n");
  }

  //Query Peak values
  printf("Peak values observed by RX app components\n");
  app.getProperty("complex_mixer_ts","peak", value);
  printf("Complex Mixer Peak    : %s\n", value.c_str());

  printf("SMB channel           : %s\n", smb_channel.c_str());

  // copy file to local location from ram
  system("mv /var/volatile/rx_gps_timestamping_app.out odata/rx_gps_timestamping_app_raw.out");

  /*std::string nname, vvalue;
  bool isParameter, hex = false;
  for (unsigned n = 0; app.getProperty(n, nname, vvalue, hex, &isParameter); n++)
  {
    fprintf(stdout, "Property %2u: %s = \"%s\"%s\n", n, nname.c_str(), vvalue.c_str(), isParameter ? " (parameter)" : "");
  }*/

  printf("Application complete\n");

  } catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
  }


  return 0;
}
