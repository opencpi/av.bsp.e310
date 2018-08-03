#ifndef _TEST_APP_COMMON_H
#define _TEST_APP_COMMON_H

#include "OcpiApi.hh" // OCPI::API namespace
#include "ocpi_component_prop_type_helpers.h" // ocpi_..._t types
#include "worker_prop_parsers_ad9361_config_proxy.h" // ad9361_config_proxy_rx_rf_gain_t
#include "test_helpers.h" // TEST_EXPECTED_VAL(), TEST_EXPECTED_VAL_DIFF()

#define APP_DEFAULT_E3XX_XML               "app_default_e3xx.xml"
#define APP_2R2T_E3XX_XML                  "app_2r2t_e3xx.xml"
#define APP_DEFAULT_XML_INST_NAME_RX       "rx"
#define APP_DEFAULT_XML_INST_NAME_PROXY    "ad9361_config_proxy"
#define APP_DEFAULT_XML_INST_NAME_DATA_SUB "ad9361_data_sub"
#define APP_DEFAULT_XML_INST_NAME_ADC_SUB  "ad9361_adc_sub"

bool did_pass_test_expected_value_rf_gain_dB(
    OCPI::API::Application& app,
    double expected_rx_val,
    ocpi_long_t expected_ad9361_config_proxy_val)
{
  std::string prop("rf_gain_dB");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.5);
    }

    {
      std::string str;
      app.getProperty(APP_DEFAULT_XML_INST_NAME_PROXY, "rx_rf_gain", str);
      ad9361_config_proxy_rx_rf_gain_t rx_rf_gain;
      const char* err = parse(str.c_str(), rx_rf_gain);
      if(err != 0) { printf("%s",err); return false; }
      TEST_EXPECTED_VAL(rx_rf_gain[0], expected_ad9361_config_proxy_val);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_expected_value_bb_gain_dB(
    OCPI::API::Application& app,
    double expected_rx_val)
{
  std::string prop("bb_gain_dB");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      // some wiggle room due to double precision rounding 0.00000000000001 was
      // arbitrarily chosen to be "very little wiggle room"
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.00000000000001);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_expected_value_frequency_MHz(
    OCPI::API::Application& app,
    double expected_rx_val,
    ocpi_ulonglong_t expected_ad9361_config_proxy_val)
{
  std::string prop("frequency_MHz");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.000004769);
    }

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_PROXY, "rx_lo_freq");
      ocpi_ulonglong_t actual_ad9361_config_proxy_val = p.getULongLongValue();
      //TEST_EXPECTED_VAL(actual_ad9361_config_proxy_val, expected_ad9361_config_proxy_val);
      TEST_EXPECTED_VAL_DIFF((double)actual_ad9361_config_proxy_val, (double)expected_ad9361_config_proxy_val, 4.769);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_expected_value_sample_rate_MHz(
    OCPI::API::Application& app,
    double expected_rx_val,
    ocpi_ulong_t expected_ad9361_config_proxy_val)
{
  std::string prop("sample_rate_MHz");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.0000005);
    }

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_PROXY, "rx_sampling_freq");
      ocpi_ulong_t actual_ad9361_config_proxy_val = p.getULongValue();
      TEST_EXPECTED_VAL(actual_ad9361_config_proxy_val, expected_ad9361_config_proxy_val);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_expected_value_rf_cutoff_frequency_MHz(
    OCPI::API::Application& app,
    double expected_rx_val)
{
  std::string prop("rf_cutoff_frequency_MHz");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      // some wiggle room due to double precision rounding 0.00000000000001 was
      // arbitrarily chosen to be "very little wiggle room"
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.00000000000001);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_expected_value_bb_cutoff_frequency_MHz(
    OCPI::API::Application& app,
    double expected_rx_val,
    ocpi_ulong_t expected_ad9361_config_proxy_val)
{
  std::string prop("bb_cutoff_frequency_MHz");
  try
  {
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, prop.c_str());
      double actual_rx_val = p.getDoubleValue();
      TEST_EXPECTED_VAL_DIFF(actual_rx_val, expected_rx_val, 0.01); // just trying 10 kHz step for now
    }

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_PROXY, "rx_rf_bandwidth");
      ocpi_ulong_t actual_ad9361_config_proxy_val = p.getULongValue();
      TEST_EXPECTED_VAL(actual_ad9361_config_proxy_val, expected_ad9361_config_proxy_val);
    }
  }
  catch (std::string &e) {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

#endif // _TEST_APP_COMMON_H
