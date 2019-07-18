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

/*
 * THIS FILE WAS ORIGINALLY GENERATED ON Thu Mar  7 11:09:22 2019 EST
 * BASED ON THE FILE: e3xx_rx_ts.xml
 * YOU *ARE* EXPECTED TO EDIT IT
 *
 * This file contains the implementation skeleton for the e3xx_rx_ts worker in C++
 */

#include <map>     // std::map
#include <cmath>   // round(), std::log, std::abs()
#include <cstdlib> // strtol(), strtod()
#include <sstream> // std::ostringstream
#include <string>  // std::string
#include <iomanip> // std::setprecision()
#include <cstdint> // uint32_t, uint64_t
#include <utility> // std::pair
#include <cinttypes> // PRIu64

#include "e3xx_rx_ts-worker.hh"
#include <sstream>
#include "ad9361.h"   // for RFPLL_MODULUS
#include "OcpiApi.hh" // OCPI::API::Application
#include "OcpiOsDebugApi.hh" // OCPI_LOG_DEBUG
#include "ocpi_component_prop_type_helpers.h" // ocpi_*_t types
#include "ad9361_common.h" // AD9361_RX_port_t, AD9361_duplex_mode_t
#include "worker_prop_parsers_ad9361_config_proxy.h" // AD9361_InitParam_ad9361_config_proxy type, parse()
#include "readers_ad9361_cfg.h" // get_AD9361_duplex_mode()
#include "readers_ad9361_rx_gain.h"   // get_AD9361_rx_gain_...() functions
#include "readers_ad9361_rf_rx_pll.h" // get_AD9361_Rx_RFPLL_LO_freq_Hz()
#include "readers_ad9361_bb_rx_adc.h" // get_AD9361_CLKRF_FREQ_step_Hz()
#include "readers_ad9361_bb_rx_filters_analog.h" // get_AD9361_Rx_BBBW_Hz()
#include "writers_ad9361_rx_gain.h"   // set_AD9361_rx_gain_...() functions
#include "writers_ad9361_rf_rx_pll.h" // set_AD9361_Rx_RFPLL_LO_freq_Hz()
#include "writers_ad9361_bb_rx_adc.h" // set_AD9361_CLKRF_FREQ_Hz()
#include "writers_ad9361_bb_rx_filters_analog.h" // set_AD9361_rx_rf_bandwidth()

#include "ad9361_api.h" // RX1, RX2 macros

// it is possible for this worker to assign the AD9361 to one of the RXB or RXC ports, none of which are connctedwhich would result in an invalid settings
// it is possible for the AD9361 to have the setting assigned where one of the
// RXB or RXC ports are used instead of the RXA ports, and none of the B or C
// ports are connected to an FMCOMMS2/3 SMA connector, which is why we
// include "invalid" here as a possible settings
//enum class worker_E3XX_SMA_port_RF_RX_t {TRXA, RX2A, RX2B, TRXB};
//enum class worker_E3XX_SMA_port_RF_RX_t {RX1A, RX2A, invalid};

// See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet
enum class ad9361_adc_sub_devsignal_channel_t {zero, one};

#define PROP_STR_RF_GAIN_DB              "rf_gain_dB"
#define PROP_STR_BB_GAIN_DB              "bb_gain_dB"
#define PROP_STR_FREQUENCY_MHZ           "frequency_MHz"
#define PROP_STR_SAMPLE_RATE_MHZ         "sample_rate_MHz"
#define PROP_STR_RF_CUTOFF_FREQUENCY_MHZ "rf_cutoff_frequency_MHz"
#define PROP_STR_BB_CUTOFF_FREQUENCY_MHZ "bb_cutoff_frequency_MHz"

#define LOG_WRITTEN_START() \
  log_trace("start of %s_written()", prop.c_str())

#define LOG_WRITTEN_END() \
  log_trace("end   of %s_written()", prop.c_str())

#define LOG_READ_START() \
  log_trace("start of %s_read()", prop.c_str())

#define LOG_READ_END() \
  log_trace("end   of %s_read()", prop.c_str())

#define HZ_TO_MHZ(x) ((x)/1000000)

using namespace OCPI::RCC; // for easy access to RCC data types and constants
using namespace E3xx_rx_tsWorkerTypes;
namespace OA=OCPI::API;

class E3xx_rx_tsWorker : public E3xx_rx_tsWorkerBase {

private:
  /*! @todo / FIXME - replace this with type defined in RadioCtrlrNoOSTuneResamp
   *                  class, or better yet, one in AD9361ConfigProxy class.
   ****************************************************************************/
  enum class rx_timing_diagram_channel_t {R1, R2};

  const std::string m_worker_str;

  uint8_t m_two_t_two_r_timing_enable;

  const double m_No_OS_API_precision_rx_sampling_freq;

  ad9361_adc_sub_devsignal_channel_t
      m_ad9361_adc_sub_devsignal_channel_controlled_by_this_worker;

  bool m_app_inst_name_ad9361_config_proxy_initialized;
  bool m_app_inst_name_ad9361_data_sub_initialized;
  bool m_app_inst_name_ad9361_adc_sub_initialized;
  bool m_app_inst_name_e3xx_mimo_xcvr_filter_proxy_initialized;
  bool m_app_inst_name_ad9361_config_initialized;
  bool m_config_initialized;

  std::string previousSmaPort;

  // property writes may be delayed until after initial OWD props are
  // initialized, the purpose of this vector is to queue up pending writes to
  // allow them to be executed in the same original order they would have been
  // executed in had they not been delayed (why do we care about execution
  // order? - because some property's allowable values are dependent on the
  // current value of other properties, it's the old "batch processing of
  // frontend resources" problem)
  std::vector<std::string> m_pending_prop_write_queue;

  // first is string representation of this worker's prop name, second is
  // pointer to this worker's prop
  std::map<std::string, double*> m_map_val;

  // first is string representation of this worker's min/max/step prop name
  // whose value is expected to always be that of a parameter, second is the
  // expected value
  std::map<std::string, double> m_map_fixedval;

public:

  E3xx_rx_tsWorker() : E3xx_rx_tsWorkerBase(),
      m_worker_str("e3xx_rx_ts.rcc"),
      m_No_OS_API_precision_rx_sampling_freq(1.),
      m_ad9361_adc_sub_devsignal_channel_controlled_by_this_worker(ad9361_adc_sub_devsignal_channel_t::zero),
      m_app_inst_name_ad9361_config_proxy_initialized(false),
      m_app_inst_name_ad9361_data_sub_initialized(false),
      m_app_inst_name_ad9361_adc_sub_initialized(false),
      m_app_inst_name_e3xx_mimo_xcvr_filter_proxy_initialized(false),
      m_app_inst_name_ad9361_config_initialized(false),
      m_config_initialized(false),
      previousSmaPort("")
  {
    // rf_gain_dB is not fixed
    // rf_gain_max_dB is not fixed
    // rf_gain_min_dB is not fixed
    m_map_fixedval["rf_gain_step_dB"             ] = E3XX_RX_TS_RF_GAIN_STEP_DB_P;

    m_map_fixedval[PROP_STR_BB_GAIN_DB           ] = E3XX_RX_TS_NOT_SUPPORTED_P;
    m_map_fixedval["bb_gain_max_dB"              ] = E3XX_RX_TS_BB_GAIN_MAX_DB_P;
    m_map_fixedval["bb_gain_min_dB"              ] = E3XX_RX_TS_BB_GAIN_MIN_DB_P;
    m_map_fixedval["bb_gain_step_dB"             ] = E3XX_RX_TS_BB_GAIN_STEP_DB_P;

    // frequency_MHz is not fixed
    m_map_fixedval["frequency_max_MHz"           ] = E3XX_RX_TS_FREQUENCY_MAX_MHZ_P;
    m_map_fixedval["frequency_min_MHz"           ] = E3XX_RX_TS_FREQUENCY_MIN_MHZ_P;
    // frequency_step_MHz is not fixed

    // sample_rate_MHz is not fixed
    m_map_fixedval["sample_rate_min_MHz"         ] = E3XX_RX_TS_SAMPLE_RATE_MIN_MHZ_P;
    // sample_rate_step_MHz is not fixed

    m_map_fixedval[PROP_STR_RF_CUTOFF_FREQUENCY_MHZ] = E3XX_RX_TS_NOT_SUPPORTED_P;
    m_map_fixedval["rf_cutoff_frequency_max_MHz" ] = E3XX_RX_TS_RF_CUTOFF_FREQUENCY_MAX_MHZ_P;
    m_map_fixedval["rf_cutoff_frequency_min_MHz" ] = E3XX_RX_TS_RF_CUTOFF_FREQUENCY_MIN_MHZ_P;
    m_map_fixedval["rf_cutoff_frequency_step_MHz"] = E3XX_RX_TS_RF_CUTOFF_FREQUENCY_STEP_MHZ_P;

    // bb_cutoff_frequency_MHz is not fixed
    m_map_fixedval["bb_cutoff_frequency_max_MHz" ] = E3XX_RX_TS_BB_CUTOFF_FREQUENCY_MAX_MHZ_P;
    m_map_fixedval["bb_cutoff_frequency_min_MHz" ] = E3XX_RX_TS_BB_CUTOFF_FREQUENCY_MIN_MHZ_P;
    // bb_cutoff_frequency_step_MHz is not fixed

    m_map_val["rf_gain_max_dB"              ] = &m_properties.rf_gain_max_dB;
    m_map_val["rf_gain_min_dB"              ] = &m_properties.rf_gain_min_dB;
    m_map_val["rf_gain_step_dB"             ] = &m_properties.rf_gain_step_dB;
    m_map_val["bb_gain_max_dB"              ] = &m_properties.bb_gain_max_dB;
    m_map_val["bb_gain_min_dB"              ] = &m_properties.bb_gain_min_dB;
    m_map_val["bb_gain_step_dB"             ] = &m_properties.bb_gain_step_dB;
    m_map_val["frequency_max_MHz"           ] = &m_properties.frequency_max_MHz;
    m_map_val["frequency_min_MHz"           ] = &m_properties.frequency_min_MHz;
    m_map_val["sample_rate_max_MHz"         ] = &m_properties.sample_rate_max_MHz;
    m_map_val["sample_rate_min_MHz"         ] = &m_properties.sample_rate_min_MHz;
    m_map_val["rf_cutoff_frequency_max_MHz" ] = &m_properties.rf_cutoff_frequency_max_MHz;
    m_map_val["rf_cutoff_frequency_min_MHz" ] = &m_properties.rf_cutoff_frequency_min_MHz;
    m_map_val["rf_cutoff_frequency_step_MHz"] = &m_properties.rf_cutoff_frequency_step_MHz;
    m_map_val["bb_cutoff_frequency_max_MHz" ] = &m_properties.bb_cutoff_frequency_max_MHz;
    m_map_val["bb_cutoff_frequency_min_MHz" ] = &m_properties.bb_cutoff_frequency_min_MHz;

    m_map_val[PROP_STR_RF_GAIN_DB             ] = &m_properties.rf_gain_dB;
    m_map_val[PROP_STR_BB_GAIN_DB             ] = (double*) &m_properties.bb_gain_dB;
    m_map_val[PROP_STR_FREQUENCY_MHZ          ] = &m_properties.frequency_MHz;
    m_map_val[PROP_STR_SAMPLE_RATE_MHZ        ] = &m_properties.sample_rate_MHz;
    m_map_val[PROP_STR_RF_CUTOFF_FREQUENCY_MHZ] = (double*) &m_properties.rf_cutoff_frequency_MHz;
    m_map_val[PROP_STR_BB_CUTOFF_FREQUENCY_MHZ] = &m_properties.bb_cutoff_frequency_MHz;

    // important - this is the only value supported for now
    m_two_t_two_r_timing_enable = 0;
  }

private:

  void log_info(const char* msg, ...)
#ifdef __GNUC__
  __attribute__((format(printf, 2, 3)))
#endif
  {
    if(!m_properties.enable_log_info) { return; }
    printf("INFO : %s worker: ",m_worker_str.c_str());
    va_list arg;
    va_start(arg, msg);
    vprintf(msg, arg);
    va_end(arg);
    printf("\n");
  };

  void log_debug(const char* msg, ...)
#ifdef __GNUC__
  __attribute__((format(printf, 2, 3)))
#endif
  {
    if(!m_properties.enable_log_debug) { return; }
    printf("DEBUG: %s worker: ",m_worker_str.c_str());
    va_list arg;
    va_start(arg, msg);
    vprintf(msg, arg);
    va_end(arg);
    printf("\n");
  };

  void log_trace(const char* msg, ...)
#ifdef __GNUC__
  __attribute__((format(printf, 2, 3)))
#endif
  {
    if(!m_properties.enable_log_trace) { return; }
    printf("TRACE: %s worker: ",m_worker_str.c_str());
    va_list arg;
    va_start(arg, msg);
    vprintf(msg, arg);
    va_end(arg);
    printf("\n");
  };

  std::string strip_unit(std::string prop) {
    return prop.substr(prop.find_last_of("_")+1);
  } ///

  ad9361_adc_sub_devsignal_channel_t
    get_ad9361_adc_sub_devsignal_channel_for_timing_diagram_R1() {

    // See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet

    OCPI::API::Application& app = getApplication();
    const char* adc_sub_inst = m_properties.app_inst_name_ad9361_adc_sub;
    OCPI::API::Property p(app, adc_sub_inst, "channels_are_swapped");
    ocpi_bool_t channels_are_swapped = p.getBoolValue();

    return channels_are_swapped ? ad9361_adc_sub_devsignal_channel_t::one : ad9361_adc_sub_devsignal_channel_t::zero;
  }

  ad9361_adc_sub_devsignal_channel_t
    get_ad9361_adc_sub_devsignal_channel_for_timing_diagram_R2() {

    // See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet

    OCPI::API::Application& app = getApplication();
    const char* adc_sub_inst = m_properties.app_inst_name_ad9361_adc_sub;
    OCPI::API::Property p(app, adc_sub_inst, "channels_are_swapped");
    ocpi_bool_t channels_are_swapped = p.getBoolValue();

    return channels_are_swapped ? ad9361_adc_sub_devsignal_channel_t::zero : ad9361_adc_sub_devsignal_channel_t::one;
  }

  const char* get_AD9361_RX_port_controlled_by_this_worker(
    OCPI::API::Application& app, AD9361_RX_port_t& val)
  {
    // See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet

    typedef ad9361_adc_sub_devsignal_channel_t chan_t;

    chan_t ad9361_adc_sub_devsignal_channel_for_timing_diagram_R1;
    chan_t& idxR1 = ad9361_adc_sub_devsignal_channel_for_timing_diagram_R1;
    idxR1 =  get_ad9361_adc_sub_devsignal_channel_for_timing_diagram_R1();

    chan_t ad9361_adc_sub_devsignal_channel_for_timing_diagram_R2;
    chan_t& idxR2 = ad9361_adc_sub_devsignal_channel_for_timing_diagram_R2;
    idxR2 =  get_ad9361_adc_sub_devsignal_channel_for_timing_diagram_R2();

    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    if(m_ad9361_adc_sub_devsignal_channel_controlled_by_this_worker == idxR1)
    {
      const char* err = get_AD9361_RX_RF_port_for_timing_diagram_R1(app, inst, val);
      if(err != 0) { return err; }
    }
    else // (m_ad9361_adc_sub_devsignal_channel_controlled_by_this_worker == idxR2)
    {
      const char* err = get_AD9361_RX_RF_port_for_timing_diagram_R2(app, inst, val);
      if(err != 0) { return err; }
    }
    return 0;
  }
#if 0
  worker_E3XX_SMA_port_RF_RX_t get_E3XX_SMA_port_controlled_by_this_worker() {
    AD9361_RX_port_t AD9361_RX_port;

    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    get_AD9361_RX_port_controlled_by_this_worker(app, inst, AD9361_RX_port);
    if(AD9361_RX_port == AD9361_RX_port_t::RX1A)
    {
      return worker_E3XX_SMA_port_RF_RX_t::RX1A;
    }
    else if(AD9361_RX_port == AD9361_RX_port_t::RX2A)
    {
      return worker_E3XX_SMA_port_RF_RX_t::RX2A;
    }
    else
    {
      return worker_E3XX_SMA_port_RF_RX_t::invalid;
    }
  }
#endif
  //
  void disable_RX_FIR_filter() {
    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    std::string DISABLE_str;
    app.getProperty(inst, "DISABLE",       DISABLE_str);
    app.setProperty(inst, "rx_fir_en_dis", DISABLE_str.c_str());
  }

  /*! @brief Get value from worker array property read. This is just a
   *         convenience function to reduce lines of code.
   *  @param[in] w            application's worker instance name of the worker
   *                          whose property is to be accessed
   *  @param[in] p            property name
   *  @param[in] array_idx    index of array
   ****************************************************************************/
  template<typename T>
  T get_prop_val(const std::string& w, const std::string& p, size_t array_idx) {

    OA::AccessList list({array_idx});
    return getApplication().getPropertyValue<T>(w, p, list);
  }

  /*! @brief Set worker array property value for a single array index. This is
   *         just a convenience function to reduce lines of code
   *  @param[in] w            application's worker instance name of the worker
   *                          whose property is to be accessed
   *  @param[in] p            property name
   *  @param[in] val          value to assign
   *  @param[in] array_idx    index of array
   ****************************************************************************/
  template<typename T>
  void set_prop_val(const std::string& w, const std::string& p, size_t array_idx,
                    T val) {
    OA::AccessList list({array_idx});
    getApplication().setPropertyValue<T>(w, p, val, list);
  }

  /*! @brief Get a single channel's tx_attenuation in mdB from the AD9361. Note
   *         that this uses the No-OS API call to retrieve the value, which
   *         returns the theoretical value based on register settings.
   *  @param[in] chan         channel forwarded onto No-OS call (0 or 1)
   ****************************************************************************/
  int32_t get_tx_attenuation_mdB(size_t chan) {

    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    return get_prop_val<int32_t>(inst, "tx_attenuation", chan);
  }

  /*! @brief Get the nominal tx_rf_bandwidth in Hz from the AD9361. Note that
   *         this uses the No-OS API call to retrieve the value.
   ****************************************************************************/
  uint32_t get_tx_rf_bandwidth_Hz() {

    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    return getApplication().getPropertyValue<uint32_t>(inst, "tx_rf_bandwidth");
  }

  /*! @brief Get nominal tx_lo_freq in Hz from the AD9361. Note this is the
   *         No-OS API call with nominal (integer) precision, which is less
   *         precise than get_AD9361_Tx_RFPLL_LO_freq_Hz().
   ****************************************************************************/
  uint64_t get_tx_lo_freq_Hz() {

    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    return getApplication().getPropertyValue<uint64_t>(inst, "tx_lo_freq");
  }

  /*! @brief Set a single channel's tx_attenuation in mdB on the AD9361.
   *  @param[in] chan               channel forwarded onto No-OS call (0 or 1)
   *  @param[in] tx_attenuation_mdB attenuation
   ****************************************************************************/
  void set_tx_attenuation_mdB(size_t chan, int32_t tx_attenuation_mdB) {

    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    set_prop_val(inst, "tx_attenuation", chan, tx_attenuation_mdB);
  }

  /*! @brief Set tx_rf_bandwidth in Hz on the AD9361.
   ****************************************************************************/
  void set_tx_rf_bandwidth_Hz(uint32_t tx_rf_bandwidth_Hz) {

    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    app.setPropertyValue<uint32_t>(inst, "tx_rf_bandwidth", tx_rf_bandwidth_Hz);
  }

  /*! @brief Set tx_lo_freq in Hz on the AD9361.
   ****************************************************************************/
  void set_tx_lo_freq_Hz(uint64_t tx_lo_freq_Hz) {

    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    app.setPropertyValue<uint64_t>(inst, "tx_lo_freq", tx_lo_freq_Hz);
  }

  void set_ad9361_init(ad9361_config_proxy_ad9361_init& ad9361_init) {

    const size_t chan = 0;

    // save property values which may change during write to ad9361_init

    /// @TODO/FIXME - chan==0 assumes 1R1T
    const int32_t tx_attenuation_mdB = get_tx_attenuation_mdB(chan);

    const uint32_t tx_rf_bandwidth_Hz = get_tx_rf_bandwidth_Hz();
    const uint64_t tx_lo_freq_Hz      = get_tx_lo_freq_Hz();

    // write ad9361_init
    {
      const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
      std::string ad9361_init_str = to_string(ad9361_init);
      getApplication().setProperty(inst, "ad9361_init",ad9361_init_str.c_str());
    }

    // re-apply saved properties property values which may have changed during
    // write to ad9361_init

    /// @TODO/FIXME - chan==0 assumes 1R1T
    if(not(get_tx_attenuation_mdB(chan) == tx_attenuation_mdB)) {
      set_tx_attenuation_mdB(chan, tx_attenuation_mdB);
    }

    if(not(get_tx_rf_bandwidth_Hz() == tx_rf_bandwidth_Hz)) {
      set_tx_rf_bandwidth_Hz(tx_rf_bandwidth_Hz);
    }
    if(not(get_tx_lo_freq_Hz() == tx_lo_freq_Hz)) {
      set_tx_lo_freq_Hz(tx_lo_freq_Hz);
    }

    // we do not set ad9361_config_proxy's tx_sampling_freq because this
    // worker's sample_rate_MHz's value will affect both the
    // ad9361_config_proxy's rx_sampling_freq and tx_sampling_freq properties
  }

  /*! @brief calc_difference_between_desired_and_in_situ_after_hw_write
   ****************************************************************************/
  const char* calc_difference_between_desired_and_in_situ_after_hw_write(
      const std::string& prop,
      const std::pair<double, std::string>& desired,
      std::pair<double, std::string>& in_situ_after_hw_write,
      double& diff) {
    const std::string unit_desired = strip_unit(prop);
    convert_value(in_situ_after_hw_write, unit_desired);

    const double& val_desired          =                desired.first;
    double& val_in_situ_after_hw_write = in_situ_after_hw_write.first;

    diff = val_in_situ_after_hw_write - val_desired;
    return 0;
  }

  void do_log_info_desired_vs_in_situ(const std::string& prop,
      const std::pair<double, std::string>& desired,
      std::pair<double, std::string>& in_situ_after_hw_write) {

    double diff;
    const char* err = calc_difference_between_desired_and_in_situ_after_hw_write(prop, desired, in_situ_after_hw_write, diff);
    if(err == 0)
    {
      log_info("attempted property write: prop=%s,\tdesired value = %.15f %s,\tcurrent in-situ nominal value = %.15f %s,\t(difference=%.15f %s)", prop.c_str(), desired.first, desired.second.c_str(), in_situ_after_hw_write.first, in_situ_after_hw_write.second.c_str(), diff, desired.second.c_str());
    }
    else
    {
      log_info("attempted property write: prop=%s,\tdesired value=%.15f,\terror determining in-situ values: %s", prop.c_str(), desired.first, err);
    }
  }

  RCCResult
  error_if_fixed_prop_val_is_not_prop_fixedval(const std::string& prop)
  {
    double diff = ((*(m_map_val.at(prop))) - m_map_fixedval.at(prop));

    // ideally the only acceptable difference would be 0. but we have to give
    // some wiggle room due to double precision rounding 0.00000000000001 was
    // arbitrarily chosen to be "very little wiggle room"
    if(std::abs(diff) > 0.00000000000001)
    {
      RCCResult res = setError("attempted to overwrite fixed property value of %s (%.19f) with a value of %.19f", prop.c_str(), m_map_fixedval.at(prop), *(m_map_val.at(prop)));
      return res;
    }
    return RCC_OK;
  }

  RCCResult
  error_if_step_prop_val_is_not_prop_fixedval(const std::string& prop)
  {
    double diff = ((*(m_map_val.at(prop))) - m_map_fixedval.at(prop));

    // ideally the only acceptable difference would be 0. but we have to give
    // some wiggle room due to double precision rounding 0.00000000000001 was
    // arbitrarily chosen to be "very little wiggle room"
    if(std::abs(diff) > 0.00000000000001)
    {
      RCCResult res = setError("attempted to overwrite fixed property value of %s (%.19f) with a value of %.19f", prop.c_str(), m_map_fixedval.at(prop), *(m_map_val.at(prop)));
      return res;
    }
    return RCC_OK;
  }

  RCCResult
  error_if_min_prop_val_is_not_prop_fixedval(const std::string& prop)
  {
    double diff = ((*(m_map_val.at(prop))) - m_map_fixedval.at(prop));

    // ideally the only acceptable difference would be 0. but we have to give
    // some wiggle room due to double precision rounding 0.00000000000001 was
    // arbitrarily chosen to be "very little wiggle room"
    if(std::abs(diff) > 0.00000000000001)
    {
      RCCResult res = setError("attempted to overwrite fixed property value of %s (%.19f) with a value of %.19f", prop.c_str(), m_map_fixedval.at(prop), *(m_map_val.at(prop)));
      return res;
    }
    if((*(m_map_val.at(prop))) < m_map_fixedval.at(prop))
    {
      // increase number just a tad to make it in bounds
      log_debug("adjusting attempted write of minimum value from %.19f to %.19f", *(m_map_val.at(prop)), m_map_fixedval.at(prop));
      *(m_map_val.at(prop)) = m_map_fixedval.at(prop);
    }
    return RCC_OK;
  }

  RCCResult
  error_if_max_prop_val_is_not_prop_fixedval(const std::string& prop)
  {
    double diff = ((*(m_map_val.at(prop))) - m_map_fixedval.at(prop));

    // ideally the only acceptable difference would be 0. but we have to give
    // some wiggle room due to double precision rounding 0.00000000000001 was
    // arbitrarily chosen to be "very little wiggle room"
    if(std::abs(diff) > 0.00000000000001)
    {
      RCCResult res = setError("attempted to overwrite fixed property value of %s (%.19f) with a value of %.19f", prop.c_str(), m_map_fixedval.at(prop), *(m_map_val.at(prop)));
      return res;
    }
    if((*(m_map_val.at(prop))) > m_map_fixedval.at(prop))
    {
      // decrease number just a tad to make it in bounds
      log_debug("adjusting attempted write of minimum value from %.19f to %.19f", *(m_map_val.at(prop)), m_map_fixedval.at(prop));
      *(m_map_val.at(prop)) = m_map_fixedval.at(prop);
    }
    return RCC_OK;
  }

  const char* get_min_allowed_val(const std::string prop, double& min) {
    if(prop == PROP_STR_RF_GAIN_DB)
    {
      try
      {
        OCPI::API::Application& app = getApplication();
        const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

        const char* err = get_AD9361_rx_gain_min_dB(app, inst, min);
        if(err != 0) { return err; }
      }
      catch(const std::exception& e)
      {
        std::string err;
        err = "Exception when setting ad9361_config_proxy property: ";
        err += e.what();
        return err.c_str();
      }
    }
    else if(prop == PROP_STR_BB_GAIN_DB)
    {
      min =   E3XX_RX_TS_BB_GAIN_MIN_DB_P;
    }
    else if(prop == PROP_STR_FREQUENCY_MHZ)
    {
      min =   E3XX_RX_TS_FREQUENCY_MIN_MHZ_P;
    }
    else if(prop == PROP_STR_SAMPLE_RATE_MHZ)
    {
      min =   E3XX_RX_TS_SAMPLE_RATE_MIN_MHZ_P;
    }
    else if(prop == PROP_STR_RF_CUTOFF_FREQUENCY_MHZ)
    {
      min =   E3XX_RX_TS_RF_CUTOFF_FREQUENCY_MIN_MHZ_P;
    }
    else if(prop == PROP_STR_BB_CUTOFF_FREQUENCY_MHZ)
    {
      min =   E3XX_RX_TS_BB_CUTOFF_FREQUENCY_MIN_MHZ_P;
    }
    else
    {
      std::string err = "requested min for invalid property named " + prop;
      return err.c_str();
    }
    return 0;
  }

  const char* get_max_allowed_val(const std::string prop, double& max) {
    if(prop == PROP_STR_RF_GAIN_DB)
    {
      try
      {
        OCPI::API::Application& app = getApplication();
        const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

        const char* err = get_AD9361_rx_gain_max_dB(app, inst, max);
        if(err != 0) { return err; }
      }
      catch(const std::exception& e)
      {
        std::string err;
        err = "Exception when setting ad9361_config_proxy property: ";
        err += e.what();
        return err.c_str();
      }
    }
    else if(prop == PROP_STR_BB_GAIN_DB)
    {
      max =   E3XX_RX_TS_BB_GAIN_MAX_DB_P;
    }
    else if(prop == PROP_STR_FREQUENCY_MHZ)
    {
      max =   E3XX_RX_TS_FREQUENCY_MAX_MHZ_P;
    }
    else if(prop == PROP_STR_SAMPLE_RATE_MHZ)
    {
      OCPI::API::Application& app = getApplication();
      const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
      /// @todo / FIXME - read from ad9361_config_proxy's ad9361_phy.rx2tx2 property instead of m_two_t_two_r_timing_enable
      if (m_two_t_two_r_timing_enable &&
          !app.getPropertyValue<bool>(inst, "LVDS") &&
          app.getPropertyValue<bool>(inst, "single_port") &&
          !app.getPropertyValue<bool>(inst, "half_duplex")) {
        max = 15.36;
      } else {
        max = 30.72;
      }
    }
    else if(prop == PROP_STR_RF_CUTOFF_FREQUENCY_MHZ)
    {
      max =   E3XX_RX_TS_RF_CUTOFF_FREQUENCY_MAX_MHZ_P;
    }
    else if(prop == PROP_STR_BB_CUTOFF_FREQUENCY_MHZ)
    {
      max =   E3XX_RX_TS_BB_CUTOFF_FREQUENCY_MAX_MHZ_P;
    }
    else
    {
      std::string err = "requested max for invalid property named " + prop;
      return err.c_str();
    }
    return 0;
  }

  /*! @brief E.g., if prop is "rf_gain_dB", return RCC_OK if
   *         m_properties_rf_gain_min_dB <= m_properties.rf_gain_dB <= 
   *         m_properties_rf_gain_max_dB, and return setError(...) otherwise.
   ****************************************************************************/
  RCCResult check_bounds(const std::string& prop, double desired_val) {
    double min, max;
    const char* errmin = get_min_allowed_val(prop, min);
    if(errmin != 0) { return setError(errmin); }
    const char* errmax = get_max_allowed_val(prop, max);
    if(errmax != 0) { return setError(errmax); }
    const bool too_low  = (desired_val < min);
    const bool too_high = (desired_val > max);
    if(too_high || too_low)
    {
      std::ostringstream oss;
      oss << std::setprecision(17) << prop << " too " << (too_high ? "high" : "low") << " (\"";
      oss << desired_val << "\") can only be in the range [ ";
      oss << std::setprecision(17) << min << " to ";
      oss  << std::setprecision(17) << max << " ]";
      
      return setError(oss.str().c_str());
    }
    return RCC_OK;
  }

  void get_adjusted_val_to_be_applied_to_hw(
      const std::string& prop, double val_desired,
      std::pair<ocpi_ulonglong_t, std::string>& adjusted) {
    if(prop.compare(PROP_STR_FREQUENCY_MHZ) == 0)
    {
      ocpi_ulonglong_t val = (ocpi_ulonglong_t) round(val_desired*1e6);
      adjusted = std::make_pair(val, "Hz");
    }
    else
    {
      std::string err = "invalid property: " + prop;
      throw err.c_str();
    }
    log_info("attempted property write: %s=%.15f, value to be written (after potential adjustment due to No-OS API precision limitations) is %" PRIu64 " %s", prop.c_str(), val_desired, adjusted.first, adjusted.second.c_str());
  }

  void get_adjusted_val_to_be_applied_to_hw(
      const std::string& prop, double val_desired,
      std::pair<ocpi_long_t, std::string>& adjusted) {
    if(prop.compare(PROP_STR_RF_GAIN_DB) == 0)
    {
      ocpi_long_t val = (ocpi_long_t) round(val_desired);
      adjusted = std::make_pair(val, "dB");
    }
    else
    {
      std::string err = "invalid property: " + prop;
      throw err.c_str();
    }
    log_info("attempted property write: %s=%.15f, write to be written (after potential adjustment due to No-OS API precision limitations) is %i %s", prop.c_str(), val_desired, adjusted.first, adjusted.second.c_str());
  }

  void get_adjusted_val_to_be_applied_to_hw(
      const std::string& prop, double val_desired,
      std::pair<ocpi_ulong_t, std::string>& adjusted) {
    if(prop.compare(PROP_STR_SAMPLE_RATE_MHZ) == 0)
    {
      ocpi_ulong_t val = (ocpi_ulong_t) round(val_desired*1e6); // Msps to sps
      adjusted = std::make_pair(val, "Hz");
    }
    else if(prop.compare(PROP_STR_BB_CUTOFF_FREQUENCY_MHZ) == 0)
    {
      //! @todo TODO/FIXME - calc value to write to rx_rf_bandwidth that will result in actual nominal 3dB freq instead of assuming No-OS/ad9361_config_proxy's 'rx_rf_bandwidth' corresponds to the 3dB freq
      ocpi_ulong_t val = (ocpi_ulong_t) round(val_desired*1e6); // MHz to Hz

      // AD9361_Reference_Manual_UG-570.pdf Rev. A pg. 9 - 
      // "... is normally calibrated to 1.4x the baseband channel bandwidth (BBBW)"
      //ocpi_ulong_t val = (ocpi_ulong_t) round(val_desired/1.4*1e6);

      adjusted = std::make_pair(val, "Hz");

    }
    else
    {
      std::string err = "invalid property: " + prop;
      throw err.c_str();
    }
    log_info("attempted property write: prop%s,\tdesired value=%.15f,\tvalue to be written (after potential adjustment due to No-OS API precision limitations) is %u %s", prop.c_str(), val_desired, adjusted.first, adjusted.second.c_str());
  }

  RCCResult set_in_situ_val(const std::string& prop,
      ocpi_ulonglong_t adjusted_val_to_be_applied_to_hw) {
    OCPI::API::Application& app = getApplication();
    ocpi_ulonglong_t& val_adjusted = adjusted_val_to_be_applied_to_hw;
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    if(prop.compare(PROP_STR_FREQUENCY_MHZ) == 0)
    {
      const char* err = set_AD9361_Rx_RFPLL_LO_freq_Hz(app, inst, val_adjusted);
      if(err != 0) { return setError(err); }

      const char* filter_proxy_inst = m_properties.app_inst_name_e3xx_mimo_xcvr_filter_proxy;
      OCPI::API::Property filter_freq(app, filter_proxy_inst, "rx_frequency_MHz");
      double val_MHz = HZ_TO_MHZ(val_adjusted);
      filter_freq.setDoubleValue(val_MHz);    // Set the proper analog filterbank/switch settings

      if (m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_TRXA) {
        if (previousSmaPort != "" && previousSmaPort != "trxa_mode") {
            app.setProperty(filter_proxy_inst, previousSmaPort.c_str(), "off");
        }
        if (previousSmaPort != "trxa_mode") {
            app.setProperty(filter_proxy_inst, "trxa_mode", "rx");
            previousSmaPort = "trxa_mode";
        }
      } else if (m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_RX2A) {
        if (previousSmaPort != "" && previousSmaPort != "rx2a_mode") {
            app.setProperty(filter_proxy_inst, previousSmaPort.c_str(), "off");
        }
        if (previousSmaPort != "rx2a_mode") {
            app.setProperty(filter_proxy_inst, "rx2a_mode", "rx");
            previousSmaPort = "rx2a_mode";
        }
      } else if (m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_RX2B) {
        if (previousSmaPort != "" && previousSmaPort != "rx2b_mode") {
            app.setProperty(filter_proxy_inst, previousSmaPort.c_str(), "off");
        }
        if (previousSmaPort != "rx2b_mode") {
            app.setProperty(filter_proxy_inst, "rx2b_mode", "rx");
            previousSmaPort = "rx2b_mode";
        }
      } else {
        if (previousSmaPort != "" && previousSmaPort != "trxb_mode") {
            app.setProperty(filter_proxy_inst, previousSmaPort.c_str(), "off");
        }
        if (previousSmaPort != "trxb_mode") {
            app.setProperty(filter_proxy_inst, "trxb_mode", "rx");
            previousSmaPort = "trxb_mode";
        }
      }

      const char* config_inst = m_properties.app_inst_name_ad9361_config_proxy;
      OCPI::API::Property rx_rf_port_output(app, config_inst, "rx_rf_port_input");
      std::string rx_balun;
      if (val_MHz < 1200) {
        app.getProperty(config_inst, "C_BALANCED", rx_balun);
        rx_rf_port_output.setULongValue(strtol(rx_balun.c_str(), NULL, 0));
      } else if (val_MHz < 2600) {
        app.getProperty(config_inst, "B_BALANCED", rx_balun);
        rx_rf_port_output.setULongValue(strtol(rx_balun.c_str(), NULL, 0));
      } else {
        app.getProperty(config_inst, "A_BALANCED", rx_balun);
        rx_rf_port_output.setULongValue(strtol(rx_balun.c_str(), NULL, 0));
      }
    }
    else
    {
      return setError("invalid property: %s", prop.c_str());
    }
    return RCC_OK;
  }

  RCCResult set_in_situ_val(const std::string& prop,
      ocpi_ulong_t adjusted_val_to_be_applied_to_hw) {
    OCPI::API::Application& app = getApplication();
    ocpi_ulong_t& val_adjusted = adjusted_val_to_be_applied_to_hw;
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    if(prop.compare(PROP_STR_SAMPLE_RATE_MHZ) == 0)
    {
      const char* err = set_AD9361_CLKRF_FREQ_Hz(app, inst, val_adjusted);
      if(err != 0) { return setError(err); }
    }
    else if(prop.compare(PROP_STR_BB_CUTOFF_FREQUENCY_MHZ) == 0)
    {
      const char* err = set_AD9361_rx_rf_bandwidth_Hz(app, inst, val_adjusted);
      if(err != 0) { return setError(err); }
    }
    else
    {
      return setError("invalid property: %s", prop.c_str());
    }
    return RCC_OK;
  }

  RCCResult set_in_situ_val(const std::string& prop,
      ocpi_long_t adjusted_val_to_be_applied_to_hw) {
    OCPI::API::Application& app = getApplication();
    ocpi_long_t& val_adjusted = adjusted_val_to_be_applied_to_hw;
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    if(prop.compare(PROP_STR_RF_GAIN_DB) == 0)
    {
      std::string RF_GAIN_MGC_str = get_RF_GAIN_MGC_str();
      app.setProperty(inst, "rx_gain_control_mode", RF_GAIN_MGC_str.c_str());

      // note that No-OS's API call for setting rx gain affects gain
      // settings at both the baseband and the RF stages, and No-OS does not
      // allow (easily?) for discerning between the two. Perhaps with
      // raw register writes this would be possible (I think you just
      // need to read the gain tables and manually separate the values) but
      // for now we just lazily treat the overall AD9361 rx gain
      // as the gain that corresponds to this worker's rf_gain_dB property
      // (worth also noting that the No-OS API also includes *digital*
      // gain as well as analog gain...)
      AD9361_RX_port_t AD9361_RX_port;
      get_AD9361_RX_port_controlled_by_this_worker(app, AD9361_RX_port);
      if(AD9361_RX_port == AD9361_RX_port_t::RX1A || AD9361_RX_port == AD9361_RX_port_t::RX1B || AD9361_RX_port == AD9361_RX_port_t::RX1C)
      {
        const char* err = set_AD9361_gain_RX1_dB(app, inst, val_adjusted);
        if(err != 0) { return setError(err); }
      }
      else if(AD9361_RX_port == AD9361_RX_port_t::RX2A || AD9361_RX_port == AD9361_RX_port_t::RX2B || AD9361_RX_port == AD9361_RX_port_t::RX2C)
      {
        const char* err = set_AD9361_gain_RX2_dB(app, inst, val_adjusted);
        if(err != 0) { return setError(err); }
      }
      else
      {
        return setError("AD9361 configured for invalid RX port for E3XX");
      }
    }
    else
    {
      return setError("invalid property: %s", prop.c_str());
    }
    return RCC_OK;
  }

  const char* get_in_situ_val(const std::string& prop,
      std::pair<double, std::string>& in_situ_after_hw_write) {
    OCPI::API::Application& app = getApplication();
    double val;
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    if(prop.compare(PROP_STR_RF_GAIN_DB) == 0)
    {
      const char* err = get_AD9361_rx_gain_dB(app, inst, val);
      in_situ_after_hw_write = std::make_pair(val, "dB");
      return err;
    }
    else if(prop.compare(PROP_STR_FREQUENCY_MHZ) == 0)
    {
      const char* err = get_AD9361_Rx_RFPLL_LO_freq_Hz(app, inst, val);
      in_situ_after_hw_write = std::make_pair(val, "Hz");
      return err;
    }
    else if(prop.compare(PROP_STR_SAMPLE_RATE_MHZ) == 0)
    {
      const char* err = get_AD9361_CLKRF_FREQ_Hz(app, inst, val);
      in_situ_after_hw_write = std::make_pair(val, "Hz");
      return err;
    }
    else if(prop.compare(PROP_STR_BB_CUTOFF_FREQUENCY_MHZ) == 0)
    {
      //! @todo TODO/FIXME - calc actual in-situ nominal 3dB freq instead of assuming No-OS/ad9361_config_proxy's 'rx_rf_bandwidth' readback value corresponds to the 3dB freq
      const char* err = get_AD9361_rx_rf_bandwidth_Hz(app, inst, val);

      /*double BBBW_Hz;
      const char* err = get_AD9361_Rx_BBBW_Hz(app, inst, BBBW_Hz);

      // AD9361_Reference_Manual_UG-570.pdf Rev. A pg. 9 - 
      // "Note that the BBBW is half the complex bandwidth"
      val = BBBW_Hz*2;*/

      in_situ_after_hw_write = std::make_pair(val, "Hz");
      return err;
    }
    else
    {
      return "invalid property read";
    }
    return 0;
  }

  void convert_value(std::pair<double, std::string>& value, std::string unit_desired)
  {
    std::string& unit = value.second;
    double factor = 1.;
    if    ((unit         == "dB") &&
           (unit_desired == "mdB"))
    {
      factor = 1000.;
    }
    else if((unit         == "mdB") &&
            (unit_desired == "dB"))
    {
      factor = 0.001;
    }
    else if((unit         == "MHz") &&
            (unit_desired == "Hz"))
    {
      factor = 1e6;
    }
    else if((unit         == "Hz") &&
            (unit_desired == "MHz"))
    {
      factor = 0.000001;
    }
    else if(not (((unit         == "Hz" ) &&
                  (unit_desired == "Hz" )) or
                 ((unit         == "MHz") &&
                  (unit_desired == "MHz")) or
                 ((unit         == "dB" ) &&
                  (unit_desired == "dB" )) or
                 ((unit         == "mdB") &&
                  (unit_desired == "mdB"))))
    {
      std::string err;
      err = "calculation for unit combo not supported: ";
      err += unit         + ", " + unit_desired;
      throw err.c_str();
    }
    value.first *= factor;
    unit         = unit_desired;
  }

  void dequeue_prop_writes_dependant_upon_app_inst_strings() {
    bool all_entries_processed = false;
    while(not all_entries_processed)
    {
      all_entries_processed = true;
      auto it = m_pending_prop_write_queue.begin();
      while(it != m_pending_prop_write_queue.end())
      {
        if(*it == "config")
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          config_written();
          break;
        }
        it++;
      }
    }
  }

  // this is necessary because the first write to this worker's init
  // property causes a write to the ad9361_config_proxy.rcc worker's
  // ad9361_init property, which re-initializes the AD9361 which overrides
  // all the desired initial values written to rf_gain_dB, etc. The reason
  // this happens is apparently the OpenCPI framework performs all OCS
  // initializations before the OWD ones (init is an OWD property, and
  // rf_gain_dB, etc are OCS properties). To avoid this undesirable
  // behavior , we skip all the initial writes to rf_gain_dB, etc, and
  // finally perform them here now that init property write is complete.
  void dequeue_prop_writes_dependant_upon_config() {
    bool all_entries_processed = false;
    while(not all_entries_processed)
    {
      all_entries_processed = true;
      auto it = m_pending_prop_write_queue.begin();
      while(it != m_pending_prop_write_queue.end())
      {
        if(     *it == PROP_STR_RF_GAIN_DB)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          rf_gain_dB_written();
          break;
        }
        else if(*it == PROP_STR_BB_GAIN_DB)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          bb_gain_dB_written();
          break;
        }
        else if(*it == PROP_STR_FREQUENCY_MHZ)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          frequency_MHz_written();
          break;
        }
        else if(*it == PROP_STR_SAMPLE_RATE_MHZ)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          sample_rate_MHz_written();
          break;
        }
        else if(*it == PROP_STR_RF_CUTOFF_FREQUENCY_MHZ)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          rf_cutoff_frequency_MHz_written();
          break;
        }
        else if(*it == PROP_STR_BB_CUTOFF_FREQUENCY_MHZ)
        {
          all_entries_processed = false;
          m_pending_prop_write_queue.erase(it);
          bb_cutoff_frequency_MHz_written();
          break;
        }
        it++;
      }
    }
  }

  template<typename T>
  RCCResult do_written(const std::string& prop, bool& previously_skipped) {
    LOG_WRITTEN_START();
    
    // 1. We must allow setting of the property during the "initialized" state
    //    in order to allow the default values to be applied.
    // 2. Otherwise, we only want to allow any operations when in the
    //    "operating" state.
    /*if(not (isOperating() or isInitialized()))
    {
      printf("not operating and not initialized\n");
      LOG_WRITTEN_END();
      return RCC_OK;
    }*/ //! @todo TODO/FIXME - bug is causing isOperating() to erroneously return false
    if(not m_config_initialized)
    {
      previously_skipped = true;

      log_debug("skipping %s property initialization because 'config' property has not yet been initialized", prop.c_str());
      m_pending_prop_write_queue.push_back(prop);

      LOG_WRITTEN_END();
      return RCC_OK;
    }
    if(previously_skipped)
    {
      log_debug("executing previously skipped %s property initialization because 'init' and property has been initialized", prop.c_str());
      previously_skipped = false;
    }

    const std::string unit_desired = strip_unit(prop);

    // first is value, second is value's unit description
    std::pair<double, std::string> desired;
    std::pair<T,      std::string> adjusted_to_be_applied_to_hw;
    std::pair<double, std::string> in_situ_after_hw_write;

    double& val_desired                      = desired.first;
    T&      val_adjusted_to_be_applied_to_hw = adjusted_to_be_applied_to_hw.first;

    desired = std::make_pair(*(m_map_val.at(prop)), unit_desired);

    // ensure attempted write of desired value is valid, exit otherwise
    RCCResult res = check_bounds(prop, val_desired);
    if(res != RCC_OK) { LOG_WRITTEN_END(); return res; }

    // apply step size value adjustment necessary for No-OS API call which
    // ad9361_config_proxy.rcc performs
    get_adjusted_val_to_be_applied_to_hw(prop, val_desired, adjusted_to_be_applied_to_hw);

    // apply property value to hardware
    res = set_in_situ_val(prop, val_adjusted_to_be_applied_to_hw);
    if(res != RCC_OK) { LOG_WRITTEN_END(); return res; }

    if(prop == PROP_STR_SAMPLE_RATE_MHZ) {
      try {
        update_time_ifc_props_for_single_adc(m_properties.config.SMA_channel);
      }
      catch(std::string& err) {
        return setError(err.c_str());
      }
    }

    // for final logging
    if(m_properties.enable_log_info)
    {
      std::string unit;
      const char* err = get_in_situ_val(prop, in_situ_after_hw_write);
      if(err != 0) { return setError(err); }
      do_log_info_desired_vs_in_situ(prop, desired, in_situ_after_hw_write);
    }

    LOG_WRITTEN_END();
    return RCC_OK;
  }

  RCCResult do_written_no_hw(const std::string& prop, bool& previously_skipped) {
    LOG_WRITTEN_START();
    
    // 1. We must allow setting of this property during the "initialized" state
    //    in order to allow the default values to be applied.
    // 2. Otherwise, we only want to allow any operations when in the
    //    "operating" state.
    /*if(not (isOperating() or isInitialized()))
    {
      LOG_WRITTEN_END();
      return RCC_OK;
    }*/ //! @todo TODO/FIXME - bug is causing isOperating() to erroneously return false
    if(not m_config_initialized)
    {
      previously_skipped = true;
      log_debug("skipping %s property initialization because 'config' property has not yet been initialized", prop.c_str());
      LOG_WRITTEN_END();
      return RCC_OK;
    }
    if(previously_skipped)
    {
      log_debug("executing previously skipped %s property initialization because 'config' property has been initialized", prop.c_str());
      previously_skipped = false;
    }

    double val_desired = *(m_map_val.at(prop));

    // ensure attempted write of desired value is valid, exit otherwise
    RCCResult res = check_bounds(prop, val_desired);
    if(res != RCC_OK) { LOG_WRITTEN_END(); return res; }

    LOG_WRITTEN_END();
    return RCC_OK;
  }

  RCCResult do_read(const std::string& prop) {
    LOG_READ_START();
    const std::string unit_desired = strip_unit(prop);
    
    std::pair<double, std::string> in_situ;
    double& val_in_situ = in_situ.first;

    const char* err = get_in_situ_val(prop, in_situ);
    if(err != 0) { return setError(err); }

    convert_value(in_situ, unit_desired);
    *(m_map_val.at(prop)) = val_in_situ;

    log_info("current in-situ nominal %s = %.15f", prop.c_str(), val_in_situ);

    LOG_READ_END();
    return RCC_OK;
  }

  const char* calc_worst_case_Rx_RFPLL_freq_step_for_current_F_REF_and_ref_divider(
      OCPI::API::Application& app, const char* app_inst_name_proxy,
      double& val)
  {
    double d_Rx_RFPLL_input_F_REF;
    double d_Rx_RFPLL_ref_divider;

    { // restrict scope so we don't accidentally use non-double values
      // for later calculation
      bool         Rx_RFPLL_external_div_2_enable;
      ocpi_ulong_t Rx_RFPLL_input_F_REF;
      ocpi_float_t Rx_RFPLL_ref_divider;

      char* err;
      const char*& inst = app_inst_name_proxy;

      err = (char*) get_AD9361_Rx_RFPLL_external_div_2_enable(app, inst, Rx_RFPLL_external_div_2_enable);
      if(err != 0) { return err; }

      if(Rx_RFPLL_external_div_2_enable)
      {
        // in this case, the Rx PLL is fixed, so we set the step size to infinity
        val = std::numeric_limits<double>::infinity();
        return 0;
      }

      err = (char*) get_AD9361_Rx_RFPLL_input_F_REF(app, inst, Rx_RFPLL_input_F_REF );
      if(err != 0) { return err; }
      err = (char*) get_AD9361_Rx_RFPLL_ref_divider(app, inst, Rx_RFPLL_ref_divider);
      if(err != 0) { return err; }

      d_Rx_RFPLL_input_F_REF  = (double) Rx_RFPLL_input_F_REF;
      d_Rx_RFPLL_ref_divider  = (double) Rx_RFPLL_ref_divider;
    }

    double x = d_Rx_RFPLL_input_F_REF;
    x *= d_Rx_RFPLL_ref_divider; // why * and not / ???
    x /= RFPLL_MODULUS;

    // min VCO divider gives us the worst case step
    double d_Rx_RFPLL_VCO_Divider_min = 2.;
    x /= d_Rx_RFPLL_VCO_Divider_min;
    
    val = x;

    return 0;
  }

  void get_FPGA_bitstream_DATA_CLK_Delay(ocpi_ushort_t& DATA_CLK_Delay) {
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_data_sub;

    std::string DATA_CLK_Delay_str;
    app.getProperty(inst, "DATA_CLK_Delay", DATA_CLK_Delay_str);
    DATA_CLK_Delay = strtol(DATA_CLK_Delay_str.c_str(), NULL, 0);
  }

  void get_FPGA_bitstream_RX_Data_Delay(ocpi_ushort_t& RX_Data_Delay) {
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_data_sub;

    std::string RX_Data_Delay_str;
    app.getProperty(inst, "RX_Data_Delay", RX_Data_Delay_str);
    RX_Data_Delay = strtol(RX_Data_Delay_str.c_str(), NULL, 0);
  }

  std::string get_INT_LO_str() {
    std::string INT_LO_str;
    OCPI::API::Application &app = getApplication();
    app.getProperty(m_properties.app_inst_name_ad9361_config_proxy, "INT_LO", INT_LO_str);
    return INT_LO_str;
  }

  std::string get_EXT_LO_str() {
    std::string EXT_LO_str;
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    app.getProperty(inst, "EXT_LO", EXT_LO_str);

    return EXT_LO_str;
  }

  std::string get_RX_1_str() {
    std::string RX_1_str;
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    app.getProperty(inst, "RX_1", RX_1_str);

    return RX_1_str;
  }

  std::string get_RX_2_str() {
    std::string RX_2_str;
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    app.getProperty(inst, "RX_2", RX_2_str);

    return RX_2_str;
  }

  std::string get_RF_GAIN_MGC_str() {
    std::string str;
    OCPI::API::Application &app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    app.getProperty(inst, "RF_GAIN_MGC", str);

    return str;
  }

  uint8_t get_rx_ch_for_sma_channel(SMA_channel sma_channel) const {

    uint8_t ch;

    switch(sma_channel) {
      case CONFIG_SMA_CHANNEL_TRXA:
        ch = RX2;
        break;
      case CONFIG_SMA_CHANNEL_RX2A:
        ch = RX2;
        break;
      case CONFIG_SMA_CHANNEL_RX2B:
        ch = RX1;
        break;
      default: // CONFIG_SMA_CHANNEL_TRXB
        ch = RX1;
        break;
    }
    return ch;
  }

  /*! @return For a specified data_stream_ID, returns value from third column of
   *         Table 2 in https://opencpi.github.io/assets/AD9361_ADC_Sub.pdf.
   *  @todo / FIXME - replace this method width similar one in
   *                  RadioCtrlrNoOSTuneResamp class.
   ****************************************************************************/
  rx_timing_diagram_channel_t get_ad9361_adc_sub_rx_timing_diagram_channel(
      SMA_channel sma_channel) {

    uint8_t rx_ch = get_rx_ch_for_sma_channel(sma_channel);

    // If you look at Table 2 in
    // https://opencpi.github.io/assets/AD9361_ADC_Sub.pdf, you notice that
    // the combination of the AD9361 RX RF Port and AD9361 Register 0x010 Bit D4
    // uniquely identifies the AD9361 timing diagram channel
    const char* pname = "parallel_port_conf_1";
    OA::Application& app = getApplication();
    auto& inst = m_properties.app_inst_name_ad9361_config;
    uint8_t reg_0x010 = app.getPropertyValue<uint8_t>(inst, pname);

    rx_timing_diagram_channel_t timing_diagram_ch;
    if((reg_0x010 & 0x10) == 0x10) {
      if(rx_ch == RX1) {
        timing_diagram_ch = rx_timing_diagram_channel_t::R2;
      }
      else { // rx_ch == RX2
        timing_diagram_ch = rx_timing_diagram_channel_t::R1;
      }
    }
    else {
      if(rx_ch == RX1) {
        timing_diagram_ch = rx_timing_diagram_channel_t::R1;
      }
      else { // rx_ch == RX2
        timing_diagram_ch = rx_timing_diagram_channel_t::R2;
      }
    }
    return timing_diagram_ch;
  }

  /*  @todo / FIXME - replace this method width similar one in
   *                  RadioCtrlrNoOSTuneResamp class, or better yet,
   *                  Ad9361ConfigProxy class.
   *  @todo / FIXME - check this method's accuracy for CMOS
   ******************************************************************************/
  double get_data_clk_p_rate_Hz() {
    const char* pname = "parallel_port_conf_1";
    OA::Application& app = getApplication();
    auto& inst0 = m_properties.app_inst_name_ad9361_config;
    uint8_t reg = app.getPropertyValue<uint8_t>(inst0, pname);
    double ret;
    auto& inst = m_properties.app_inst_name_ad9361_config_proxy;
    const char* err = get_AD9361_CLKRF_FREQ_Hz(app, inst, ret);
    if(err != 0) {
      throw std::string(err);
    }
    bool two_t_two_r_timing_enable = (reg & 0x04) == 0x04;
    if(two_t_two_r_timing_enable) {
      ret *= 2.;
    }
    if(app.getPropertyValue<bool>(inst, "LVDS")) {
      ret *= 2.;
    }
    log(OCPI_LOG_DEBUG, "calculated AD9361 DATA_CLK rate of %0.15f Hz", ret);
    return ret;
  }

  /*! @return std::pair of std::string containing description and double value
   *          representing delay in seconds.
   *  @todo / FIXME - replace this method width similar one in
   *                  RadioCtrlrNoOSTuneResamp class.
   ******************************************************************************/
  std::pair<std::string, double> get_ad9361_adc_sub_pipeline_delay(
      SMA_channel sma_channel) {

    rx_timing_diagram_channel_t timing_ch;
    timing_ch = get_ad9361_adc_sub_rx_timing_diagram_channel(sma_channel);

    // see description in "Latency" section of
    // https://opencpi.github.io/assets/AD9361_ADC_Sub.pdf:
    double num_clk_cycles;
    if(timing_ch == rx_timing_diagram_channel_t::R1) {
      num_clk_cycles = 4.;
    }
    else { // timing_ch == rx_timing_diagram_channel_t::R2
      num_clk_cycles = 6.;
    }

    double delay_sec = num_clk_cycles / get_data_clk_p_rate_Hz();

    return std::make_pair("ad9361_adc_sub.hdl pipeline delay", delay_sec);
  }

  /*! @brief Update value for the time_ifc_sec_off and time_ifc_fra_off
   *         properties of the ad9361_adc.hdl worker.
   *  @todo / FIXME - replace this method width similar one in
   *                  RadioCtrlrNoOSTuneResamp class.
   ****************************************************************************/
  void update_time_ifc_props_for_single_adc(SMA_channel sma_channel) {

    std::string prefix("adc tim_ifc_sec_off/tim_ifc_fra_off property calc: ");

    std::vector<std::pair<std::string, double>> offsets_for_calc;
    std::pair<std::string, double> tmp_offset;
    double offset_sec;

    uint8_t rx_ch = get_rx_ch_for_sma_channel(m_properties.config.SMA_channel);
    offset_sec = get_ad9361_digital_rx_block_delay_sec_by_rx_ch(rx_ch);
    tmp_offset = std::make_pair("AD9361 digital RX block delay", offset_sec);
    offsets_for_calc.push_back(tmp_offset);

    tmp_offset = get_ad9361_adc_sub_pipeline_delay(sma_channel);
    offsets_for_calc.push_back(tmp_offset);

    offset_sec = m_properties.time_correction_sec;
    tmp_offset = std::make_pair("correction delay", offset_sec);
    offsets_for_calc.push_back(tmp_offset);

    const char* cc = prefix.c_str();

    double total_del = 0.;
    auto it = offsets_for_calc.begin();
    for(; it != offsets_for_calc.end(); ++it) {
      const std::string& des = it->first; // description
      auto vn = it->second*1e9; // vns: val in nanoseconds
      log(OCPI_LOG_DEBUG, "%sincluding %s of %0.15f nsec", cc, des.c_str(), vn);
      total_del += it->second;
    }

    OA::Long tim_ifc_sec_off = (OA::Long) round(total_del);

    double fra_fixed_point = (total_del-tim_ifc_sec_off)*std::pow(2.,32.);
    //OA::Long tim_ifc_fra_off = (OA::Long) round(fra_fixed_point);

    slaves.adc.set_time_correction((OA::Long)round(fra_fixed_point));
    log(OCPI_LOG_DEBUG, "%stotal offset is %0.15f nsec", cc, total_del*1e9);
  }

  double get_ad9361_digital_rx_block_delay_sec_by_rx_ch(uint8_t rx_ch) {

    log(OCPI_LOG_INFO, "rf_port");

    OA::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    const char* pr = "digital_rx_block_delay_sec";

    bool seq_idx_matches_rx_port = false;

    // below commented out due to AV-4885
    /*for(unsigned ii = 0; ii<2; ii++) {
      int rf_port = app.getPropertyValue<int>(inst, pr, {ii, "rf_port"});
      log(OCPI_LOG_INFO, "rf_port=%i", rf_port);
      seq_idx_matches_rx_port = rx_ch == rf_port;
      if(seq_idx_matches_rx_port) {
        std::string value;
        app.getProperty(inst, pr, value);
        log(OCPI_LOG_INFO, "vv=%s", value.c_str());
        ret = app.getPropertyValue<double>(inst, pr, {ii, "delay_sec"});
        log(OCPI_LOG_INFO, "v=%0.15f", ret);
        break;
      }
    }
    if(not seq_idx_matches_rx_port) {
      std::ostringstream oss;
      oss << inst << " had no value for " << pr;
      throw oss.str();
    }
    return ret;*/

    std::string value;
    app.getProperty(inst, pr, value);
    // e.g. {rf_port RX1,delay_sec 1.93966e-07},{rf_port RX2,delay_sec 1.93966e-07}
    std::string rf_port_str, val_str;
    int state = 0;
    for(auto it = value.begin(); it != value.end(); ++it) {
      switch(state) {
        case 0:
          // {rf_port RX1,delay_sec 1.93966e-07},{rf_port ...}
          // ^
          if(*it != '{') {
            std::ostringstream oss;
            oss << "error parsing " << inst << "worker's " << pr;
            oss << "property value of \" "<< value << "\n";
            throw oss.str();
          }
          state++;
          break;
        case 1:
          rf_port_str.clear();
          if(*it == ' ') {
            // {rf_port RX1,delay_sec 1.93966e-07},{rf_port ...}
            //         ^
            state++;
          }
          break;
        case 2:
          if(*it == ',') {
            // {rf_port RX1,delay_sec 1.93966e-07},{rf_port ...}
            //             ^
            if(rx_ch == RX1) {
              seq_idx_matches_rx_port = rf_port_str == "RX1";
            }
            else if(rx_ch == RX2) {
              seq_idx_matches_rx_port = rf_port_str == "RX2";
            }
            state++;
          }
          else {
            rf_port_str += *it;
          }
          break;
        case 3:
          if(*it == ' ') {
            // {rf_port RX1,delay_sec 1.93966e-07},{rf_port ...}
            //                       ^
            state++;
          }
          break;
        case 4:
          if(*it == '}') {
            // {rf_port RX1,delay_sec 1.93966e-07},{rf_port ...}
            //                                   ^
            if(seq_idx_matches_rx_port) {
              return strtod(val_str.c_str(), NULL);
            }
            else {
              val_str.clear();
            }
            state++;
          }
          else {
            val_str += *it;
          }
          break;
        case 5:
          if(*it == ',') {
            state = 0;
          }
          else {
            state++;
          }
          break;
        default:
          std::ostringstream oss;
          oss << "error parsing " << inst << "worker's " << pr;
          oss << "property value of \" "<< value << "\n";
          throw oss.str();
          break;
      }
    }

    if(not seq_idx_matches_rx_port) {
      std::ostringstream oss;
      oss << "could not get ad9361 digital rx block delay for dig_radio_ctrlr ";
      oss << "rx_ch" << (rx_ch == RX1 ? "RX1" : "RX2");
      oss << " because rx_ch could not be found";
      throw oss.str();
    }

    return 0;
  }

  void dequeue_prop_writes_if_ready_to_do_so() {

    if(m_app_inst_name_ad9361_config_proxy_initialized and
       m_app_inst_name_ad9361_data_sub_initialized and
       m_app_inst_name_ad9361_adc_sub_initialized and
       m_app_inst_name_e3xx_mimo_xcvr_filter_proxy_initialized and
       m_app_inst_name_ad9361_config_initialized)
    {
      dequeue_prop_writes_dependant_upon_app_inst_strings();
    }
  }

  // notification that rf_gain_dB property has been written
  RCCResult rf_gain_dB_written() {
    const std::string prop = PROP_STR_RF_GAIN_DB;
    static bool previously_skipped = false;
    RCCResult res = do_written<ocpi_long_t>(prop, previously_skipped);
    return res;
  }
  // notification that rf_gain_dB property will be read
  RCCResult rf_gain_dB_read() {
    const std::string prop = PROP_STR_RF_GAIN_DB;
    RCCResult res = do_read(prop);
    return res;
  }
  // notification that rf_gain_max_dB property has been written
  RCCResult rf_gain_max_dB_written() {
    // we would call error_if_prop_val_is_not_prop_fixedval() if there was a
    // default value for this property, but there isn't so writes to this
    // property should NEVER be supported
    return setError("writes to rf_gain_max_dB are invalid");
  }
  // notification that rf_gain_max_dB property will be read
  RCCResult rf_gain_max_dB_read() {
    const std::string prop(PROP_STR_RF_GAIN_DB);
    double val;
    const char* err = get_max_allowed_val(prop, val);
    if(err != 0) { return setError(err); }

    m_properties.rf_gain_max_dB = val;

    return RCC_OK;
  }
  // notification that rf_gain_min_dB property has been written
  RCCResult rf_gain_min_dB_written() {
    // we would call error_if_prop_val_is_not_prop_fixedval() if there was a
    // default value for this property, but there isn't so writes to this
    // property should NEVER be supported
    return setError("writes to rf_gain_min_dB are invalid");
  }
  // notification that rf_gain_min_dB property will be read
  RCCResult rf_gain_min_dB_read() {
    const std::string prop(PROP_STR_RF_GAIN_DB);
    double val;
    const char* err = get_min_allowed_val(prop, val);
    if(err != 0) { return setError(err); }

    m_properties.rf_gain_min_dB = val;

    return RCC_OK;
  }
  // notification that rf_gain_step_dB property has been written
  RCCResult rf_gain_step_dB_written() {
    const std::string prop("rf_gain_step_dB");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_step_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_gain_dB property has been written
  RCCResult bb_gain_dB_written() {
    const std::string prop(PROP_STR_BB_GAIN_DB);
    static bool previously_skipped = false;
    RCCResult res = do_written_no_hw(prop, previously_skipped);
    return res;
  }
  // notification that bb_gain_max_dB property has been written
  RCCResult bb_gain_max_dB_written() {
    const std::string prop("bb_gain_max_dB");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_max_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_gain_min_dB property has been written
  RCCResult bb_gain_min_dB_written() {
    const std::string prop("bb_gain_min_dB");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_min_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_gain_step_dB property has been written
  RCCResult bb_gain_step_dB_written() {
    const std::string prop("bb_gain_step_dB");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_step_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that frequency_MHz property has been written
  RCCResult frequency_MHz_written() {
    const std::string prop = PROP_STR_FREQUENCY_MHZ;
    static bool previously_skipped = false;
    RCCResult res = do_written<ocpi_ulonglong_t>(prop, previously_skipped);
    return res;
  }
  // notification that frequency_MHz property will be read
  RCCResult frequency_MHz_read() {
    const std::string prop = PROP_STR_FREQUENCY_MHZ;
    RCCResult res = do_read(prop);
    return res;
  }
  // notification that frequency_max_MHz property has been written
  RCCResult frequency_max_MHz_written() {
    const std::string prop("frequency_max_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_max_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that frequency_min_MHz property has been written
  RCCResult frequency_min_MHz_written() {
    const std::string prop("frequency_min_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_min_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that frequency_step_MHz property has been written
  RCCResult frequency_step_MHz_written() {
    // we would call error_if_prop_val_is_not_prop_fixedval() if there was a
    // default value for this property, but there isn't so writes to this
    // property should NEVER be supported
    return setError("writes to frequency_step_MHz are invalid");
  }
  // notification that frequency_step_MHz property will be read
  RCCResult frequency_step_MHz_read() {
    log_trace("start of frequency_step_MHz_read()");
    double step_Hz;

    // Note that for the sample rate calculation, it was decided to treat
    // CLKRF_FREQ as the effective AD9361 sample rate, and not the RX_SAMPL_FREQ
    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    //const char* r = get_in_situ_Rx_RFPLL_freq_step(app, step_Hz);
    //if(r != 0) { return setError(r); }
    //log_info("current in-situ nominal RX RF LO step size = %.15f Hz", step_Hz);

    // we use this function instead of get_in_situ_Rx_RFPLL_freq_step()
    // because, after observation of how No-OS choses divider values, No-OS's
    // precision at lower RX RF LO frequencies is nowhere near the theoretical
    // precision of the AD9361 dividers :( - it was observed that every step
    // size was at or below the theoretical worst case value
    const char* r = calc_worst_case_Rx_RFPLL_freq_step_for_current_F_REF_and_ref_divider(app, inst, step_Hz);

    if(r != 0) { return setError(r); }
    log_info("using worst-case in-situ nominal RX RF LO step size = %.15f Hz", step_Hz);

    m_properties.frequency_step_MHz = step_Hz/1e6;

    log_trace("end   of frequency_step_MHz_read()");
    return RCC_OK;
  }
  // notification that sample_rate_MHz property has been written
  RCCResult sample_rate_MHz_written() {
    const std::string prop(PROP_STR_SAMPLE_RATE_MHZ);
    static bool previously_skipped = false;
    RCCResult res = do_written<ocpi_ulong_t>(prop, previously_skipped);
    return res;
  }
  // notification that sample_rate_MHz property will be read
  RCCResult sample_rate_MHz_read() {
    const std::string prop(PROP_STR_SAMPLE_RATE_MHZ);
    RCCResult res = do_read(prop);
    return res;
  }
  // notification that sample_rate_max_MHz property has been written
  RCCResult sample_rate_max_MHz_written() {
    const std::string prop("sample_rate_max_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to ensure end users never write
    // an invalid value
    double diff;
    diff = m_properties.sample_rate_max_MHz - 15.36;

    // ideally the only acceptable difference would be 0. but we have to give
    // some wiggle room due to double precision rounding 0.00000000000001 was
    // arbitrarily chosen to be "very little wiggle room"
    if(std::abs(diff) > 0.00000000000001) {

      diff = m_properties.sample_rate_max_MHz - 30.72;

      if(std::abs(diff) > 0.00000000000001) {

        return setError("attempted to overwrite value of sample_rate_max_MHz with an invalid value of %.19f", m_properties.sample_rate_max_MHz);
      }
    }
    return RCC_OK;
  }
  // notification that sample_rate_max_MHz property will be read
  RCCResult sample_rate_max_MHz_read() {
    /// @todo / FIXME - read from ad9361_config_proxy's ad9361_phy.rx2tx2 property instead of m_two_t_two_r_timing_enable
    if(m_two_t_two_r_timing_enable == 0) {
      m_properties.sample_rate_max_MHz = 30.72;
    }
    else { // m_two_t_two_r_timing_enable should be 1
      m_properties.sample_rate_max_MHz = 15.36;
    }
    return RCC_OK;
  }
  // notification that sample_rate_min_MHz property has been written
  RCCResult sample_rate_min_MHz_written() {
    log_trace("start of sample_rate_min_MHz_written()");
    const std::string prop("sample_rate_min_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_min_prop_val_is_not_prop_fixedval(prop);
    log_trace("end   of sample_rate_min_MHz_written()");
    return res;
  }
  // notification that sample_rate_step_MHz property has been written
  RCCResult sample_rate_step_MHz_written() {
    // we would call error_if_prop_val_is_not_prop_fixedval() if there was a
    // default value for this property, but there isn't so writes to this
    // property should NEVER be supported
    return setError("writes to sample_rate_step_MHz are invalid");
  }
  // notification that sample_rate_step_MHz property will be read
  RCCResult sample_rate_step_MHz_read() {
    log_trace("start of sample_rate_step_MHz_read()");
    double step_Hz;

    // Note that for the sample rate calculation, it was decided to treat
    // CLKRF_FREQ as the "effective" AD9361 sample rate, and not the
    // RX_SAMPL_FREQ
    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;
    const char* r = get_AD9361_CLKRF_FREQ_step_Hz(app, inst, step_Hz);
    if(r != 0) { return setError(r); }

    if(step_Hz < m_No_OS_API_precision_rx_sampling_freq) 
    {
      log_debug("current in-situ nominal RX sample rate step size = %.15f Hz (not achievable due to No-OS API)", step_Hz);
      step_Hz = m_No_OS_API_precision_rx_sampling_freq;
      log_info("current in-situ nominal RX sample rate step size = %.15f Hz (limited by precision of No-OS API, which is less precise than the precision of the AD9361 register set limitations)", step_Hz);
    }
    else
    {
      log_info("current in-situ nominal RX sample rate step size = %.15f Hz (limited by precision of AD9361 register set)", step_Hz);
    }
    m_properties.sample_rate_step_MHz = step_Hz/1e6;

    log_trace("end   of sample_rate_step_MHz_read()");
    return RCC_OK;
  }
  // notification that rf_cutoff_frequency_MHz property has been written
  RCCResult rf_cutoff_frequency_MHz_written() {
    const std::string prop(PROP_STR_RF_CUTOFF_FREQUENCY_MHZ);
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_fixed_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that rf_cutoff_frequency_max_MHz property has been written
  RCCResult rf_cutoff_frequency_max_MHz_written() {
    const std::string prop("rf_cutoff_frequency_max_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_max_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that rf_cutoff_frequency_min_MHz property has been written
  RCCResult rf_cutoff_frequency_min_MHz_written() {
    const std::string prop("rf_cutoff_frequency_min_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_min_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that rf_cutoff_frequency_step_MHz property has been written
  RCCResult rf_cutoff_frequency_step_MHz_written() {
    const std::string prop("rf_cutoff_frequency_step_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_step_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_cutoff_frequency_MHz property has been written
  RCCResult bb_cutoff_frequency_MHz_written() {
    const std::string prop(PROP_STR_BB_CUTOFF_FREQUENCY_MHZ);
    static bool previously_skipped = false;
    RCCResult res = do_written<ocpi_ulong_t>(prop, previously_skipped);
    return res;
  }
  // notification that bb_cutoff_frequency_MHz property will be read
  RCCResult bb_cutoff_frequency_MHz_read() {
    const std::string prop(PROP_STR_BB_CUTOFF_FREQUENCY_MHZ);
    RCCResult res = do_read(prop);
    return res;
  }
  // notification that bb_cutoff_frequency_max_MHz property has been written
  RCCResult bb_cutoff_frequency_max_MHz_written() {
    const std::string prop("bb_cutoff_frequency_max_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_max_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_cutoff_frequency_min_MHz property has been written
  RCCResult bb_cutoff_frequency_min_MHz_written() {
    const std::string prop("bb_cutoff_frequency_min_MHz");
    // because this property has a default value, we need to support the initial
    // write of the default value that occurs during initialization, however,
    // because it is writable we need to call
    // error_if_prop_val_is_not_prop_fixedval() to ensure end users never write
    // an invalid value
    RCCResult res = error_if_min_prop_val_is_not_prop_fixedval(prop);
    return res;
  }
  // notification that bb_cutoff_frequency_step_MHz property has been written
  RCCResult bb_cutoff_frequency_step_MHz_written() {
    // we would call error_if_prop_val_is_not_prop_fixedval() if there was a
    // default value for this property, but there isn't so writes to this
    // property should NEVER be supported
    return setError("writes to bb_cutoff_frequency_step_MHz are invalid");
  }
  // notification that bb_cutoff_frequency_step_MHz property will be read
  RCCResult bb_cutoff_frequency_step_MHz_read() {
    //! @todo TODO/FIXME - implement this calculation
    // ADI does not provide an API call or enough information (PDF and source
    // code documentation are too poor) to calculate this, so we just set
    // to -1 for now
    m_properties.bb_cutoff_frequency_step_MHz = E3XX_RX_TS_NOT_SUPPORTED_P;

    return RCC_OK;
  }
  // notification that app_inst_name_ad9361_config_proxy property has been written
  RCCResult app_inst_name_ad9361_config_proxy_written() {
    m_app_inst_name_ad9361_config_proxy_initialized = true;

    dequeue_prop_writes_if_ready_to_do_so();

    return RCC_OK;
  }
  // notification that app_inst_name_ad9361_data_sub property has been written
  RCCResult app_inst_name_ad9361_data_sub_written() {
    m_app_inst_name_ad9361_data_sub_initialized = true;

    dequeue_prop_writes_if_ready_to_do_so();

    return RCC_OK;
  }//
  // notification that app_inst_name_ad9361_adc_sub property has been written
  RCCResult app_inst_name_ad9361_adc_sub_written() {
    m_app_inst_name_ad9361_adc_sub_initialized = true;

    dequeue_prop_writes_if_ready_to_do_so();

    return RCC_OK;
  }

  // notification that app_inst_name_e3xx_mimo_xcvr_filter_proxy property has been written
  RCCResult app_inst_name_e3xx_mimo_xcvr_filter_proxy_written() {
    m_app_inst_name_e3xx_mimo_xcvr_filter_proxy_initialized = true;

    dequeue_prop_writes_if_ready_to_do_so();

    return RCC_OK;
  }
  // notification that app_inst_name_ad9361_config property has been written
  RCCResult app_inst_name_ad9361_config_written() {
    m_app_inst_name_ad9361_config_initialized = true;

    dequeue_prop_writes_if_ready_to_do_so();

    return RCC_OK;
  }
  // notification that config property has been written
  RCCResult config_written() {
    log_trace("start of config_written()");
    const std::string prop("config");

    static bool previously_skipped = false;

    if(not (m_app_inst_name_ad9361_config_proxy_initialized and
            m_app_inst_name_ad9361_data_sub_initialized and
            m_app_inst_name_ad9361_adc_sub_initialized and
            m_app_inst_name_e3xx_mimo_xcvr_filter_proxy_initialized))
    {
      previously_skipped = true;

      log_debug("skipping %s property initialization because app_inst_name* properties have not yet been initialized", prop.c_str());
      m_pending_prop_write_queue.push_back(prop);

      log_trace("end   of config_written()");
      return RCC_OK;
    }
    if(previously_skipped)
    {
      log_debug("executing previously skipped %s property initialization because app_inst_name* properties have been initialized", prop.c_str());
      previously_skipped = false;
    }

    // write desired values to ad9361_config_proxy's ad9361_init function,
    // but only change the values of the struct members we care about so as to
    // avoid potentially stepping on anyone else's toes
    
    std::string pstr;
    try
    {
      OCPI::API::Application &app = getApplication();
      app.getProperty(m_properties.app_inst_name_ad9361_config_proxy, "ad9361_init", pstr);
    }
    catch(const std::exception& e)
    {
      std::string err;
      err = "Exception when reading ad9361_config_proxy property: ";
      err += e.what();
      return setError(err.c_str());
    }

    ad9361_config_proxy_ad9361_init ad9361_init;
    const char* retc = parse(pstr.c_str(), ad9361_init);
    if(retc != 0) { return setError(retc); }

    ad9361_init.reference_clk_rate = 40e6;

    uint8_t FDD_enable = (m_properties.config.duplex_mode == CONFIG_DUPLEX_MODE_TDD)
                         ? 0 : 1;
    ad9361_init.frequency_division_duplex_mode_enable = FDD_enable; // will be later applied

    // the E3xx's AD9361's XTALP pin is disconnected/floating, meaning the
    // AD9361 uses an external reference clock
    ad9361_init.xo_disable_use_ext_refclk_enable = 1;

    // important - this is the only value supported for now
    ad9361_init.two_t_two_r_timing_enable = m_two_t_two_r_timing_enable;

    // no I/Q swap for RX channel(s)
    // (from AD9361_Register_Map_Reference_Manual_UG-671.pdf pg. 9:
    // "Clearing this bit swaps I and Q (performs spectral inversion).")
    ad9361_init.pp_rx_swap_enable = 1;

    // See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet
    OCPI::API::Application &app = getApplication();
    const char* adc_sub_inst = m_properties.app_inst_name_ad9361_adc_sub;
    OCPI::API::Property p(app, adc_sub_inst, "channels_are_swapped");
    p.setBoolValue(false);

    //if (true) {
    if(m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_TRXB ||
       m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_RX2B) {
      // (this is what performs no channel swap)
      ad9361_init.rx_channel_swap_enable = 0; // will be later applied
      // (this is what ensures desired channel RX_1 is enabled)
      std::string RX_1_str = get_RX_1_str();
      long int RX_1_i = strtol(RX_1_str.c_str(), NULL, 0);
      ad9361_init.one_rx_one_tx_mode_use_rx_num = (uint8_t) (RX_1_i & 0xff); // will be later applied
    } else /*if(m_properties.config.SMA_channel == CONFIG_SMA_CHANNEL_TRXB)*/ {
      // (this is what performs a channel swap)
      ad9361_init.rx_channel_swap_enable = 1; // will be later applied
      // (this is what ensures desired channel RX_2 is enabled)
      std::string RX_2_str = get_RX_2_str();
      long int RX_2_i = strtol(RX_2_str.c_str(), NULL, 0);
      ad9361_init.one_rx_one_tx_mode_use_rx_num = (uint8_t) (RX_2_i & 0xff); // will be later applied
    }

    //! @TODO / FIXME - this really needs to be read from a device worker
    ad9361_init.delay_rx_data = 0; // will be later applied

    // Because our design approach was to build the FPGA bitstream based on
    // assumed values for the on-AD9361 delays, those delays are built into
    // the bitstream metadata (as parameter properties for the
    // ad9361_data_sub.hdl worker). Here is where those assumptions are forced
    // to be true by reading the assumed values from the bitstream's metadata
    // (and values are later applied via the ad9361_config_proxy.rcc worker's
    // ad9361_init property).

    ocpi_ushort_t                     DATA_CLK_Delay;
    get_FPGA_bitstream_DATA_CLK_Delay(DATA_CLK_Delay);
    ad9361_init.rx_data_clock_delay = DATA_CLK_Delay; // will be later applied

    ocpi_ushort_t                     RX_Data_Delay;
    get_FPGA_bitstream_RX_Data_Delay( RX_Data_Delay );
    ad9361_init.rx_data_delay       = RX_Data_Delay;  // will be later applied


    // values are finally applied
    try
    {
      set_ad9361_init(ad9361_init);

      // set gain control mode to manual for channel which this worker controls
      
      OCPI::API::Application& app = getApplication();
      const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

      std::string RF_GAIN_MGC_str = get_RF_GAIN_MGC_str();
      ocpi_uchar_t RF_GAIN_MGC = (ocpi_uchar_t) strtol(RF_GAIN_MGC_str.c_str(), NULL, 0) & 0xff;

      std::string initial_val_str;
      app.getProperty(inst, "rx_gain_control_mode", initial_val_str);

      ad9361_config_proxy_rx_gain_control_mode_t rx_gain_control_mode;
      const char* err = parse(initial_val_str.c_str(), rx_gain_control_mode);
      if(err != 0) { return setError(err); }

#define NO_OS_ONLY_VALID_VALUE_FOR_CH_FOR_1X1_MODE 0 //see description of No-OS rx_gain_control_mode ch parameter
      rx_gain_control_mode[NO_OS_ONLY_VALID_VALUE_FOR_CH_FOR_1X1_MODE] = RF_GAIN_MGC;

      set_AD9361_rx_gain_control_mode(app, inst, rx_gain_control_mode);
    }
    catch(const std::exception& e)
    {
      std::string err;
      err = "Exception when setting ad9361_config_proxy property: ";
      err += e.what();

      log_trace("end   of config_written()");
      return setError(err.c_str());
    }
    m_config_initialized = true;

    //! @todo TODO/FIXME - remove disabling of rx FIR and add FIR support paradigm
    disable_RX_FIR_filter();

    dequeue_prop_writes_dependant_upon_config();

    log_trace("end   of config_written()");
    return RCC_OK;
  }
  // notification that config property will be read
  RCCResult config_read() {

    OCPI::API::Application& app = getApplication();
    const char* inst = m_properties.app_inst_name_ad9361_config_proxy;

    // m_properties.config.reference_clk_rate_Hz is assumed to be what we set it
    // to, we normally would read the ad9361_config_proxy.rcc workers'
    // pdata.clk_refin.rate property but we refrain since the value initially
    // set to this worker's init.reference_clk_rate_Hz property has more
    // precision

    AD9361_duplex_mode_t mode;
    const char* err1 = get_AD9361_duplex_mode(app, inst, mode);
    if(err1 != 0) { return setError(err1); }
    m_properties.config.duplex_mode = (mode == AD9361_duplex_mode_t::FDD) ? CONFIG_DUPLEX_MODE_FDD : CONFIG_DUPLEX_MODE_FDD;

    bool adc_sub_chan_swap;
    const char* adc_sub_inst = m_properties.app_inst_name_ad9361_adc_sub;
    OCPI::API::Property p(app, adc_sub_inst, "channels_are_swapped");
    adc_sub_chan_swap = p.getBoolValue();

    bool rx_channel_swap_enable;
    const char* err3 = get_AD9361_Rx_Channel_Swap(app, inst, rx_channel_swap_enable);
    if(err3 != 0) { return setError(err3); }

    // See Table 1: Channel Connectivity in AD9361 ADC Sub Component Data Sheet
    if(adc_sub_chan_swap == false)
    {
      if(rx_channel_swap_enable == false)
      {
        m_properties.config.SMA_channel = CONFIG_SMA_CHANNEL_RX2A;
      }
      else // rx_channel_swap_enable == true
      {
        m_properties.config.SMA_channel = CONFIG_SMA_CHANNEL_RX2B;
      }
    }
    else // adc_sub_chan_swap == true
    {
      if(rx_channel_swap_enable == false)
      {
        m_properties.config.SMA_channel = CONFIG_SMA_CHANNEL_RX2B;
      }
      else // rx_channel_swap_enable == true
      {
        m_properties.config.SMA_channel = CONFIG_SMA_CHANNEL_RX2A;
      }
    }

    return RCC_OK;
  }
  // notification that ad9361_digital_rx_block_delay_sec property will be read
  RCCResult ad9361_digital_rx_block_delay_sec_read() {

    double val;
    try {
      uint8_t rx_ch;
      rx_ch = get_rx_ch_for_sma_channel(m_properties.config.SMA_channel);
      val = get_ad9361_digital_rx_block_delay_sec_by_rx_ch(rx_ch);
      log(OCPI_LOG_INFO, "v=%0.15f", val);
    }
    catch(std::string& err) {
      return setError(err.c_str());
    }
    m_properties.ad9361_digital_rx_block_delay_sec = val;

    return RCC_OK;
  }
  RCCResult run(bool /*timedout*/) {
    return RCC_DONE; // change this as needed for this worker to do something useful
    // return RCC_ADVANCE; when all inputs/outputs should be advanced each time "run" is called.
    // return RCC_ADVANCE_DONE; when all inputs/outputs should be advanced, and there is nothing more to do.
    // return RCC_DONE; when there is nothing more to do, and inputs/outputs do not need to be advanced.
  }
};

E3XX_RX_TS_START_INFO
// Insert any static info assignments here (memSize, memSizes, portInfo)
// e.g.: info.memSize = sizeof(MyMemoryStruct);
E3XX_RX_TS_END_INFO
