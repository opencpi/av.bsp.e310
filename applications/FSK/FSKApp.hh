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

/*! @file
 *  @brief The contents of this file are intended to be radio-agnostic.
 ******************************************************************************/

#ifndef _FSK_APP_HH
#define _FSK_APP_HH

#include <iostream>
#include <sstream> // std::stringstream
#include <unistd.h> // sleep()
#include <iomanip> // std::setprecision()
#include <math.h> // round()
#include "OcpiApi.hh"

namespace OA = OCPI::API;

struct args_t {
  const char* name;
  std::string mode;
  bool        debug_mode;
};

void throw_invalid_usage(const char *name, const std::string& error) {

  std::ostringstream oss;
  oss << error << "\n";
  oss << "Usage is: " << name << " <mode> <optional debug_mode>\n";
  oss << "    mode       # Test mode of the application. Valid ";
  oss << "modes are 'rx', 'tx', 'txrx', 'filerw', or 'bbloopback'.\n";
  oss << "    debug_mode # Optional debug mode which performs both ";
  oss << "an initial and final dump of all properties. Use 'd'.\n";
  oss << "Example: FSK filerw d\n";

  throw oss.str();
}

/// @return debug_mode
args_t parse_and_validate_args(int argc, char** argv) {

  args_t ret({argv[0], "rx", false});

  if(argc > 1) {
    bool incorrect_test_mode = true;
    ret.mode = argv[1];
    incorrect_test_mode &= (ret.mode == "rx");
    incorrect_test_mode &= (ret.mode == "tx");
    incorrect_test_mode &= (ret.mode == "txrx");
    incorrect_test_mode &= (ret.mode == "filerw");
    incorrect_test_mode &= (ret.mode == "bbloopback");
    if(incorrect_test_mode) {
      std::ostringstream oss;
      oss << "specified mode '" << ret.mode;
      oss << "' was not one of 'rx', 'tx', 'txrx', 'filerw', or 'bbloopback'";
      throw_invalid_usage(argv[0], oss.str());
    }
  }
  else {
    throw_invalid_usage(argv[0], "wrong number of arguments.\n");
  }
  // check for debug flag
  if(argc > 2) {
    std::string arg_debug_mode = argv[2];
    if(arg_debug_mode == "d") {
      ret.debug_mode = true;
    }
  }

  return ret;
}

/// @brief This class is intended to be radio-agnostic.
class FSKApp {

public:

  FSKApp(const char* file, const args_t& args) : m_app(file),
      m_mode(args.mode), m_debug_mode(args.debug_mode),
      m_runtime_sec(prompt_for_runtime_sec()) {
  }

  inline bool get_mode_requires_rx() {

    return (m_mode == "rx") || (m_mode == "txrx");
  }

  inline bool get_mode_requires_tx() {

    return (m_mode == "tx") || (m_mode == "txrx");
  }

  unsigned get_runtime_sec() {

    return m_runtime_sec;
  }

  OA::UShort get_zero_pad_num_zeros() {

    return m_app.getPropertyValue<OA::UShort>("zero_pad", "num_zeros");
  }

  /*! @return Currently configured (based on worker property values)
   *          CIC interpolation factor.
   ****************************************************************************/
  OA::UShort get_cic_interpolation_factor() {

    return m_app.getPropertyValue<OA::UShort>("cic_int", "R");
  }

  /*! @return Currently configured (based on worker property values)
   *          TX interpolation factor (combination of cic_int and zero_pad
   *          workers).
   ****************************************************************************/
  double get_tx_interpolation_factor() {

    // x0, x1, ... -> zero_padding --> x0, [0, 0, ...], x1, [0 0 ..]
    // there are num zeros+1 samples out for every sample in
    double zp_factor = ((double)get_zero_pad_num_zeros()) + 1.;

    return zp_factor * ((double)get_cic_interpolation_factor());
  }

  /*! @return Currently configured (based on worker property values)
   *          TX sample rate in MHz.
   ****************************************************************************/
  double get_tx_sample_rate_MHz() {

    return m_app.getPropertyValue<double>("tx", "sample_rate_MHz");
  }

  /*! @return Currently configured (based on worker property values)
   *          TX bit rate in bits/sec.
   ****************************************************************************/
  double get_tx_bit_rate_bps() {

    double tx_sample_rate_MHz = get_tx_sample_rate_MHz();
    double tx_sample_rate_Hz = tx_sample_rate_MHz *1000000.;

    return tx_sample_rate_Hz / get_tx_interpolation_factor();
  }

  bool get_adc_overrun_flag() {

    bool ret = false;

    if(get_mode_requires_rx()) {
      if(m_app.getPropertyValue<bool>("qadc","overrun")) {
        ret = true;
      }
    }

    return ret;
  }

  void initialize() {

    m_app.initialize();
  }

  void start() {

    m_app.start();
  }

  void wait() {

    m_app.wait();
  }

  void finish() {

    m_app.finish();
  }

  void stop() {

    m_app.stop();
  }

  void print_properties(bool is_initial) {

    if(m_debug_mode) {
      std::string name, value;
      bool is_param, hex = false;
      std::string msg = (is_initial ? "initial" : "final");
      std::cout << "Dump of all " << msg << "property values:\n";
      for(unsigned n = 0; m_app.getProperty(n, name, value, hex, &is_param); n++) {
        std::cout << "Property " << std::setw(2) << n << ": ";
        std::cout << name << " = \"" << value << "\"";
        std::cout << (is_param? " (parameter)" : "") << "\n";
      }
    }
  }

  void print_tx_peak_values() {

    if(get_mode_requires_tx()) {
      double rate = get_tx_bit_rate_bps();
      std::cout << "TX Bit Rate            = " << rate << " bps\n";
    }
    if(m_mode == "tx" || m_mode == "txrx" || m_mode == "filerw") {
      std::string value;
      m_app.getProperty("tx_fir_real","peak", value);
      std::cout << "TX FIR Real Peak       = " << value << "\n";
      m_app.getProperty("phase_to_amp_cordic","magnitude", value);
      std::cout << "Phase to Amp Magnitude = " << value << "\n";
    }
  }

  void print_rx_peak_values() {

    if(m_mode == "rx" || m_mode == "txrx" || m_mode == "filerw") {
      std::string value;
      if(m_mode != "filerw") {
        m_app.getProperty("dc_offset_filter","peak", value);
        std::cout << "DC Offset Peak         = " << value << "\n";
        m_app.getProperty("iq_imbalance_fixer","peak", value);
        std::cout << "IQ Imbalance Peak      = " << value << "\n";
        m_app.getProperty("complex_mixer","peak", value);
        std::cout << "Complex Mixer Peak     = " << value << "\n";
        m_app.getProperty("qadc","overrun", value);
        std::cout << "ADC Samples Dropped    = " << value << "\n";
      }
      m_app.getProperty("rp_cordic","magnitude", value);
      std::cout << "RP Cordic Magnitude    = " << value << "\n";
      m_app.getProperty("rx_fir_real","peak", value);
      std::cout << "RX FIR Real Peak       = " << value << "\n";
    }
  }

  void print_bytes_to_file(bool adc_overrun_flag) {

    // mode tx is the only one that doesn't write to a file
    if(m_mode == "rx" || m_mode == "txrx" || m_mode == "filerw") {
      std::string value;
      m_app.getProperty("file_write", "bytesWritten", value);
      std::cout << "Bytes to file : " << value << "\n";
      if(adc_overrun_flag) {
        std::cout << "WARNING: RX sample rate was high enough that data could ";
        std::cout << "not be written to file fast enough, resulting in samples ";
        std::cout << "being dropped, try a lower RX sample rate\n";
      }
    }
  }

  /// @brief move file to local location from ram for rx and txrx modes
  void move_out_file() {

    if(get_mode_requires_rx()) {
      std::string file_name;
      m_app.getProperty("file_write","fileName",file_name);

      std::ostringstream oss;
      oss << "mv " << file_name << " odata/";
      oss << ((m_mode == "rx") ? "out_app_fsk_rx.bin" : "out_app_fsk_txrx.bin");
      std::string str = oss.str();
      const char* cmd = str.c_str();

      int ret = system(str.c_str());
      if(ret != 0) {
        const char* cmd1 = " returned non-zero value of ";
        std::ostringstream oss;
        oss << cmd << cmd1 << ret << "\n";
        throw oss.str();
      }
    }
  }

  /// @brief Child class implements radio-specific functionality
  virtual void prompt_and_select_rx_antenna(const char* argv0) = 0;
  /// @brief Child class implements radio-specific functionality
  virtual void prompt_and_select_tx_antenna(const char* argv0) = 0;

  /// @brief Prompt and query/configure tx_spec worker.
  void prompt_and_configure_tx(double tx_sample_rate, double tx_rf_center_freq,
      double tx_rf_bw, double tx_bb_bw, double tx_bb_gain, double tx_rf_gain) {

    if(get_mode_requires_tx()) {
      const char* inst = "tx";

#define P(i,v,p,u) prc(v,#v,"invalid "#v".\n",i,#p"_min_"#u,#p"_max_"#u,#p"_"#u)
      // prompt w/ inst, var, prefix, unit
      P(inst, tx_sample_rate,    sample_rate,         MHz);
      P(inst, tx_rf_center_freq, frequency,           MHz);
      P(inst, tx_rf_bw,          rf_cutoff_frequency, MHz);
      P(inst, tx_rf_gain,        rf_gain,             dB);
      P(inst, tx_bb_bw,          bb_cutoff_frequency, MHz);
      P(inst, tx_bb_gain,        bb_gain,             dB);
    }
  }

  /// @brief Prompt and query/configure rx_spec worker.
  void prompt_and_configure_rx(double rx_sample_rate, double rx_rf_center_freq,
      double rx_rf_bw, double rx_bb_bw, double rx_bb_gain, double rx_rf_gain) {

    if(get_mode_requires_rx()) {
      const char* inst = "rx";

      P(inst, rx_sample_rate,    sample_rate,         MHz);
      P(inst, rx_rf_center_freq, frequency,           MHz);
      P(inst, rx_rf_bw,          rf_cutoff_frequency, MHz);
      P(inst, rx_rf_gain,        rf_gain,             dB);
      P(inst, rx_bb_bw,          bb_cutoff_frequency, MHz);
      P(inst, rx_bb_gain,        bb_gain,             dB);

      // re-read prop value before using it again, since it is volatile
      rx_sample_rate = m_app.getPropertyValue<double>(inst, "sample_rate_MHz");

      double if_fc_min = rx_sample_rate * -0.5;
      double if_fc_max = rx_sample_rate * 32767. / 65536.;
      double rx_if_center_freq = 0.; // set default for prompt

      const char* var = "rx_if_center_freq";
      const char* msg = "invalid rx_if_center_freq.\n";
      prompt_with_range(rx_if_center_freq, var, msg, if_fc_min, if_fc_max);

      // It is desired that setting a + IF freq results in mixing *down*.
      // Because complex_mixer's NCO mixes *up* for + freqs (see complex mixer
      // datasheet), IF tune freq must be negated in order to achieve the
      // desired effect.
      double nco_output_freq = -rx_if_center_freq;

      // from complex mixer datasheet, nco_output_freq =
      // sample_freq * phs_inc / 2^phs_acc_width, phs_acc_width is fixed at 16
      OA::Short phs_inc = round(nco_output_freq / rx_sample_rate * 65536.);

      if(phs_inc == 0) {
        m_app.setPropertyValue<bool>("complex_mixer", "enable", false);
      }
      else {
        m_app.setPropertyValue<OA::Short>("complex_mixer", "phs_inc", phs_inc);
      }
    }
  }

protected:

  OA::Application   m_app;
  const std::string m_mode;
  const bool        m_debug_mode;
  const unsigned    m_runtime_sec;

  virtual void validate_container_model_and_platform() {

    throw_invalid_container();
  }

  void throw_invalid_container() {
    throw std::string("Error: not on a valid platform (no containers found)\n");
  }

  void throw_outside_range(const char *error_message,
      double current_value, double lower_limit, double upper_limit) {

    std::ostringstream oss;
    oss << error_message << "Value given of " << current_value << " ";
    oss << "is outside the range of " << lower_limit << " to ";
    oss << upper_limit << ".\n";

    throw oss.str();
  }

  /*! @brief Compare value x against min and max values and throw std::string
   *         when limits are invalid.
   ****************************************************************************/
  template<typename T>
  void validate_limits(const T val, const char* error_message, T min,
      T max) {

    if((val < min) || (val > max)) {
      throw_outside_range(error_message, val, min, max);
    }
  }

  template<typename T>
  void prompt_with_range(T& val, const char* val_c_str,
      const char* error_message, T& min, T& max) {

    std::cout << std::setprecision(15) << "Enter " << val_c_str << " [range (";
    std::cout << min << " - " << max << ") default = ";
    std::cout << val << "]\n";
    std::string input;
    std::getline(std::cin, input);
    if(!input.empty()) {
      std::istringstream stream(input);
      stream >> val;
    }
    validate_limits(val, error_message, min, max);
  }

  /*! @brief Prompt the user for the amount of time to run for all modes except
   *         filerw.
   ****************************************************************************/
  unsigned prompt_for_runtime_sec() {

    unsigned ret;

    if(m_mode == "rx" || m_mode == "tx" || m_mode == "txrx") {
      ret = 20;

      const char* msg = "Enter run time in seconds [default = ";
      std::cout << std::setprecision(5) << msg << ret << "]\n";
      std::string input;
      std::getline(std::cin, input);
      if(!input.empty()) {
        std::istringstream stream(input);
        stream >> ret;
      }
    }

    if(m_mode == "rx" || m_mode == "tx" || m_mode == "txrx") {
      if(ret < 1 || ret > 60) {
        const char* msg = "invalid runtime in seconds.\n";
        throw_outside_range(msg, ret, 1, 60);
      }
    }

    return ret;
  }

  /*! @brief Prompt and query/configure rx_spec or tx_spec worker.
   *  @param[in] val           Reference to variable which will hold value read
   *                           from prompt. Value passed in represents the
   *                           default value which will be used when user
   *                           presses enter at the prompt.
   *  @param[in] val_c_str     C-style string description of the value being
   *                           prompted, e.g. rx_sample_rate.
   *  @param[in] error_message Message to be conveyed for when prop is outside
   *                           the min/max values.
   *  @param[in] prop_min      Name of property which conveys minimum allowable
   *                           value of prop, and will be read form the rx_spec
   *                           or tx_spec worker.
   *  @param[in] prop_min      Name of property which conveys maximum allowable
   *                           value of prop, and will be read form the rx_spec
   *                           or tx_spec worker.
   *  @param[in] prop          Name of property to be set on the rx_spec or
   *                           tx_spec worker.
   ****************************************************************************/
  template<typename T>
  void prc(T& val, const char* val_c_str, const char* error_message,
      const char* inst, const char* prop_min, const char* prop_max,
      const char* prop) {

    T min = m_app.getPropertyValue<T>(inst, prop_min);
    T max = m_app.getPropertyValue<T>(inst, prop_max);

    prompt_with_range(val, val_c_str, error_message, min, max);

    m_app.setPropertyValue<double>(inst, prop, val);
  }

}; // class FSKApp

#endif // _FSK_APP_HH
