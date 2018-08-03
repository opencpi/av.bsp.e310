#ifndef _TEST_APP_MAXES_H
#define _TEST_APP_MAXES_H

#include "OcpiApi.hh" // OCPI::API namespace
#include "ocpi_component_prop_type_helpers.h" // ocpi_..._t types
#include "test_app_common.h"// APP_DEFAULT_... macros, did_pass_test_expected_value_<prop>() functions

bool did_pass_test_ocpi_app_max_value_rf_gain_dB()
{
  printf("TEST: max     value for rf_gain_dB\n");
  bool did_pass;
  try
  {
    OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
    app.initialize();
    app.setProperty(APP_DEFAULT_XML_INST_NAME_RX, "enable_log_info",  "true");
    app.setProperty(APP_DEFAULT_XML_INST_NAME_RX, "enable_log_trace", "true");
    app.setProperty(APP_DEFAULT_XML_INST_NAME_RX, "enable_log_debug", "true");
    app.start();

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "frequency_MHz");
      p.setDoubleValue(1299.999999);
    }
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "rf_gain_dB");
      p.setDoubleValue(77.);
    }
    did_pass = did_pass_test_expected_value_frequency_MHz(app, 1299.999999, (ocpi_ulonglong_t) 1299999999);
    if(!did_pass) { return false; }
    did_pass = did_pass_test_expected_value_rf_gain_dB(app, 77., (ocpi_long_t) 77);
    if(!did_pass) { return false; }

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "frequency_MHz");
      p.setDoubleValue(2400.);
    }
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "rf_gain_dB");
      p.setDoubleValue(71.);
    }
    did_pass = did_pass_test_expected_value_frequency_MHz(app, 2400., (ocpi_ulonglong_t) 2400000000);
    if(!did_pass) { return false; }
    did_pass = did_pass_test_expected_value_rf_gain_dB(app, 71., (ocpi_long_t) 71);
    if(!did_pass) { return false; }

    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "frequency_MHz");
      p.setDoubleValue(4000.000005); // rounded up to nearest Hz from 4GHz +4.768 Hz
    }
    {
      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "rf_gain_dB");
      p.setDoubleValue(62.);
    }
    did_pass = did_pass_test_expected_value_frequency_MHz(app, 4000.000001, (ocpi_ulonglong_t) 4000000001);
    if(!did_pass) { return false; }
    did_pass = did_pass_test_expected_value_rf_gain_dB(app, 62., (ocpi_long_t) 62);
    if(!did_pass) { return false; }
 
    app.stop();
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_max_value_bb_gain_dB()
{
  printf("TEST: max     value for bb_gain_dB\n");
  try
  {
  OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
  app.initialize();
  app.start();
  OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "bb_gain_max_dB");
  double max = pp.getValue<double>();

  std::ostringstream oss;
  oss << std::setprecision(17) << max;
  app.setProperty("rx", "bb_gain_dB", oss.str().c_str()); // test for exception
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }

  return did_pass_test_ocpi_app_default_value_bb_gain_dB();
}

bool did_pass_test_ocpi_app_max_value_frequency_MHz()
{
  printf("TEST: max     value for frequency_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();

      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "frequency_MHz");
      p.setDoubleValue(6000.);

      did_pass = did_pass_test_expected_value_frequency_MHz(app, 6000., (ocpi_ulonglong_t) 6000000000);
      if(!did_pass) { return false; }

      {
      OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "frequency_max_MHz");
      double max = pp.getValue<double>();

      std::ostringstream oss;
      oss << std::setprecision(17) << max;
      app.setProperty("rx", "frequency_MHz", oss.str().c_str()); // test for exception
      }
   
      app.stop();
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_max_value_sample_rate_MHz()
{
  printf("TEST: max     value for sample_rate_MHz in one-r-one-t timing\n");
  bool did_pass;
  try
  {
    { // 1 r 1 t timing test
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();

      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "sample_rate_MHz");
      p.setDoubleValue(30.72);

      did_pass = did_pass_test_expected_value_sample_rate_MHz(app, 30.72, (ocpi_ulong_t) 30720000);
      if(!did_pass) { return false; }

      {
      OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "sample_rate_max_MHz");
      double max = pp.getValue<double>();

      std::ostringstream oss;
      oss << std::setprecision(17) << max;
      app.setProperty("rx", "sample_rate_MHz", oss.str().c_str()); // test for exception
      }
   
      app.stop();
    }

    printf("TEST: max     value for sample_rate_MHz in two-r-two-t timing\n");
    {
      OCPI::API::Application app(APP_2R2T_E3XX_XML, NULL);
      app.initialize();
      app.start();

      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "sample_rate_MHz");
      p.setDoubleValue(15.36);

      did_pass = did_pass_test_expected_value_sample_rate_MHz(app, 15.36, (ocpi_ulong_t) 15360000);
      if(!did_pass) { return false; }

      {
      OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "sample_rate_max_MHz");
      double max = pp.getValue<double>()/2; //todo: This division should happen in the rx proxy

      std::ostringstream oss;
      oss << std::setprecision(17) << max;
      app.setProperty("rx", "sample_rate_MHz", oss.str().c_str()); // test for exception
      }

      app.stop();
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }

  return true;
}

bool did_pass_test_ocpi_app_max_value_rf_cutoff_frequency_MHz()
{
  printf("TEST: max     value for rf_cutoff_frequency_MHz\n");
  try
  {
  OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
  app.initialize();
  app.start();
  OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "rf_cutoff_frequency_max_MHz");
  double max = pp.getValue<double>();

  std::ostringstream oss;
  oss << std::setprecision(17) << max;
  app.setProperty("rx", "rf_cutoff_frequency_MHz", oss.str().c_str()); // test for exception
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }

  return did_pass_test_ocpi_app_default_value_rf_cutoff_frequency_MHz();
}

bool did_pass_test_ocpi_app_max_value_bb_cutoff_frequency_MHz()
{
  printf("TEST: max     value for bb_cutoff_frequency_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();

      OCPI::API::Property p(app, APP_DEFAULT_XML_INST_NAME_RX, "bb_cutoff_frequency_MHz");
      p.setDoubleValue(39.2);

      did_pass = did_pass_test_expected_value_bb_cutoff_frequency_MHz(app, 39.2, (ocpi_ulong_t) 39200000);
      if(!did_pass) { return false; }

      {
      OCPI::API::Property pp(app, APP_DEFAULT_XML_INST_NAME_RX, "bb_cutoff_frequency_max_MHz");
      double max = pp.getValue<double>();

      std::ostringstream oss;
      oss << std::setprecision(17) << max;
      app.setProperty("rx", "bb_cutoff_frequency_MHz", oss.str().c_str()); // test for exception
      }
   
      app.stop();
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }

  return true;
}

bool did_pass_test_ocpi_app_max_values()
{
  if(!did_pass_test_ocpi_app_max_value_rf_gain_dB())              { return false; }
  if(!did_pass_test_ocpi_app_max_value_bb_gain_dB())              { return false; }
  if(!did_pass_test_ocpi_app_max_value_frequency_MHz())           { return false; }
  if(!did_pass_test_ocpi_app_max_value_sample_rate_MHz())         { return false; }
  if(!did_pass_test_ocpi_app_max_value_rf_cutoff_frequency_MHz()) { return false; }
  if(!did_pass_test_ocpi_app_max_value_bb_cutoff_frequency_MHz()) { return false; }

  return true;
}

#endif // _TEST_APP_MAXES_H
