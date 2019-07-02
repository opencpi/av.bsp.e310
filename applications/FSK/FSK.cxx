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
 *  @brief This contents of this file are intended to include any and all
 *         radio-specific functionality, and as little as possible
 *         radio-agnostic functionality.
 ******************************************************************************/

#include <iostream>
#include <sstream> // std::stringstream
#include <unistd.h> // sleep()
#include <iomanip> // std::setprecision()
#include <math.h> // round()
#include <vector> // std::vector
#include "OcpiApi.hh"
#include "FSKApp.hh"

namespace OA = OCPI::API;

/*! @brief This method is intended to be specific to the E3xx radio.
 *         Assign the appropriate application XML file according to requested
 *         mode.
 ******************************************************************************/
std::string get_and_print_xml_name(const args_t& args) {

  std::string ret;

  /** @todo / FIXME - consider supporting digital AD9361 loopback (which is
                     *not* baseband loopback) */
  if(args.mode == "rx") {
    ret.assign("app_fsk_rx_e3xx.xml");
  }
  else if(args.mode == "tx") {
    ret.assign("app_fsk_tx_e3xx.xml");
  }
  else if(args.mode == "txrx") {
    ret.assign("app_fsk_txrx_e3xx.xml");
  }
  else if(args.mode == "bbloopback") {
    // Note that bbloopback is still "allowed" as an argument, as it is
    // printed out in usage(). This is done in order to make this executable's
    // arguments consistent with those of the FSK app in the assets project.
    std::string str("ERROR: bbloopback mode not supported ");
    str += "on e3xx frontend.n";
    throw_invalid_usage(args.name, str);
  }
  else if(args.mode == "filerw") {
    ret.assign("app_fsk_filerw.xml");
  }
  else
  {
    throw_invalid_usage(args.name, "incorrect test mode.\n");
  }

  std::cout << "Application properties are found in XML file: " << ret << "\n";

  return ret;
}

/// @brief This class is intended to be specific to the E3xx radio.
class E3xxFSKApp : public FSKApp {

public:

  /*! @param[in] file OAS filename.
   *  @param[in] args Arguments to executable.
   ****************************************************************************/
  E3xxFSKApp(const char* file, const args_t& args) : FSKApp(file, args),
      m_rx_antenna_locked(false), m_tx_antenna_locked(false) {
  }

  void validate_container_model_and_platform() {

    OA::Container *container;

    bool valid_container_found = false;
    for(unsigned n = 0; (container = OA::ContainerManager::get(n)); n++) {
      std::cout << "container is " << container->platform() << "\n";

      if(container->model() == "hdl" && container->platform() == "e3xx") {
        valid_container_found = true;
        std::cout << "FSK App for E3XX\n";
        break;
      }
    }

    if(!valid_container_found) {
      throw_invalid_container();
    }

    std::cout << "model is " << container->model().c_str() << ", ";
    std::cout << "container is " << container->platform().c_str() << "\n";
  }

  static std::string get_chans_str(std::vector<const char*> chans) {

    bool first = true;
    std::ostringstream oss;
    for(auto it = chans.begin(); it != chans.end(); ++it) {
      oss << (first ? "" : ", ") << *it;
      first = false;
    }
    return oss.str();
  }

  void throw_invalid_chan(const char* argv0, const std::string& input,
      const std::vector<const char*>& chans) const {

    // note the SMA_channel member of the config property of the
    // e3xx_rx.rcc/e3xx_tx.rcc workers is an enum, and the framework already
    // throws an exception for invalid writes to enums, but the below error
    // message is a lot easier to understand
    std::ostringstream oss;
    oss << "invalid e3xx channel of \'" << input << "\' specified, only ";
    oss << "supported values are {" << get_chans_str(chans) << "}.\n";
    throw_invalid_usage(argv0, oss.str());
  }

  /*! @param[in] argv0       First argument to executable.
   *  @param[in] input       Channel name string received from prompt.
   *  @param[in] chan        Vector of supported Channel name strings.
   ****************************************************************************/
  void validate_channel(const char* argv0, const std::string& input,
      const std::vector<const char*>& chans) const {

    bool input_is_supported = false;
    for(auto it = chans.begin(); it != chans.end(); ++it) {
      if(input == *it) {
        input_is_supported = true;
        break;
      }
    }
    if(!input_is_supported) {
      throw_invalid_chan(argv0, input, chans);
    }
  }

  /*! @param[in] argv0       First argument to executable.
   *  @param[in] description Description of channel for prompt.
   *  @param[in] chans       Vector of supported Channel name strings.
   *  @param[in] inst        Application instance name of the e3xx_rx.rcc or
   *                         e3xx_tx.rcc worker whose config property is to be
   *                         set.
   ****************************************************************************/
  void prompt_and_select_ch(const char* argv0, const char* description,
      const std::vector<const char*>& chans, const char* inst) {

    // note the SMA_channel member of the config property of the
    // e3xx_rx.rcc/e3xx_tx.rcc workers is erroneously named (it should have been
    // SMB channel)
    const char* val_start_cstr = "duplex_mode FDD,SMA_channel ";

    std::string input;

    std::cout << "Enter E3xx " << description << " channel(";
    std::cout << get_chans_str(chans) << ")\n";
    std::getline(std::cin, input);

    validate_channel(argv0, input, chans);
    m_app.setProperty(inst, "config", (val_start_cstr + input).c_str());
    if(std::string(inst) == "rx") {
      m_rx_antenna.assign(input);
      m_rx_antenna_locked = true;
    }
    else if(std::string(inst) == "tx") {
      m_tx_antenna.assign(input);
      m_tx_antenna_locked = true;
    }
  }

  void prompt_and_select_tx_antenna(const char* argv0) {

    if(get_mode_requires_tx()) {
      std::vector<const char*> supported_chs{"TRXA", "TRXB"};
      if(m_rx_antenna_locked) {
        for(auto it = supported_chs.begin(); it != supported_chs.end(); ++it) {
          if(m_rx_antenna == *it) {
            // prevents selection of antenna that has already been "locked" for
            // RX purposes
            supported_chs.erase(it);
            break;
          }
        }
      }
      prompt_and_select_ch(argv0, "TX SMB", supported_chs, "tx");
    }
  }

  void prompt_and_select_rx_antenna(const char* argv0) {

    if(get_mode_requires_rx()) {
      std::vector<const char*> supported_chs{"TRXA", "RX2A", "RX2B", "TRXB"};
      if(m_tx_antenna_locked) {
        for(auto it = supported_chs.begin(); it != supported_chs.end(); ++it) {
          if(m_tx_antenna == *it) {
            // prevents selection of antenna that has already been "locked" for
            // TX purposes
            supported_chs.erase(it);
            break;
          }
        }
      }
      prompt_and_select_ch(argv0, "RX SMB", supported_chs, "rx");
    }
  }

  /*! @brief Prompt the user for tx settings and verify tx inputs are within
   *         valid ranges (for AD9361 frontends, read min/max values from
   *         tx worker)
   ****************************************************************************/
  void prompt_and_configure_tx() {

    if(get_mode_requires_tx()) {
      // set defaults that are known to result in succesful runs in txrx mode on
      // E3xx
      double fs    = 4.;
      double rf_fc = 2400.;
      double rf_bw = -1.;
      double bb_bw = 4.;
      double bb_gn = -1.;
      double rf_gn = -28.;

      FSKApp::prompt_and_configure_tx(fs, rf_fc, rf_bw, bb_bw, bb_gn, rf_gn);
    }
  }

  /*! @brief Prompt the user for rx settings and verify rx inputs are within
   *         valid ranges (for AD9361 frontends, read min/max values from rx
   *         worker).
   ****************************************************************************/
  void prompt_and_configure_rx() {

    if(get_mode_requires_rx()) {
      // set defaults that are known to result in succesful runs in txrx mode on
      // E3xx
      double fs    = 4.;
      double rf_fc = 2400.;
      double rf_bw = -1.;
      double bb_bw = 4.;
      double bb_gn = -1.;
      double rf_gn = 12.;

      FSKApp::prompt_and_configure_rx(fs, rf_fc, rf_bw, bb_bw, bb_gn, rf_gn);
    }
  }

protected:
  bool m_rx_antenna_locked;
  bool m_tx_antenna_locked;
  /// @brief Value is only correct if m_rx_antenna_locked is true
  std::string m_rx_antenna;
  /// @brief Value is only correct if m_tx_antenna_locked is true
  std::string m_tx_antenna;

}; // class E3xxFSKApp

int main(int argc, char **argv) {
  // Reference https://opencpi.github.io/OpenCPI_Application_Development.pdf for
  // an explanation of the ACI.

  int ret = 0;

  bool adc_overrun_flag = false;

  try {
    args_t args(parse_and_validate_args(argc, argv));

    E3xxFSKApp app(get_and_print_xml_name(args).c_str(), args);

    app.initialize(); // all resources have been allocated
    std::cout << "App initialized.\n";

    app.prompt_and_select_rx_antenna(argv[0]);
    app.prompt_and_select_tx_antenna(argv[0]);
    app.prompt_and_configure_rx();
    app.prompt_and_configure_tx();
    app.print_properties(true); // dump initial props

    app.start();      // execution is started
    std::cout << "App started.\n";

    // Must use either wait()/finish() or stop(). The finish() method must
    // always be called after wait(). The start() method can be called
    // again after stop().
    if(args.mode != "filerw") {
      // All none-filerw modes use runtime to determine when to stop.
      unsigned timer = app.get_runtime_sec();
      std::cout << "App runs for " << timer << " seconds...\n";
      while(timer-- > 0) {
        sleep(1);
        adc_overrun_flag = app.get_adc_overrun_flag();
      }
      app.stop();
    }
    else {
      // The filerw mode waits for a ZLM for app to finish.
      std::cout << "Waiting for done signal from file_write.\n";
      app.wait();       // wait until app is "done"
      app.finish();     // do end-of-run processing like dump properties
    }
    std::cout << "App stopped/finished.\n";

    app.print_properties(false); // dump final props
    app.print_bytes_to_file(adc_overrun_flag);
    app.print_tx_peak_values();
    app.print_rx_peak_values();
    app.move_out_file();

    std::cout << "Application complete\n";

  } catch (std::string &e) {
    std::cerr << "ERROR: " << e << "\n";
    ret = 1;
  }

  return ret;
}
