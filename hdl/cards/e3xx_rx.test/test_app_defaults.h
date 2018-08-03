#ifndef _TEST_APP_DEFAULTS_H
#define _TEST_APP_DEFAULTS_H

#include <string>     // std::string
#include "OcpiApi.hh" // OCPI::API namespace
#include "ocpi_component_prop_type_helpers.h" // ocpi_..._t types
#include "test_app_common.h"// APP_DEFAULT_... macros, did_pass_test_expected_value_<prop>() functions

bool did_pass_test_ocpi_app_default_value_rf_gain_dB()
{
  printf("TEST: default value for rf_gain_dB\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_rf_gain_dB(app, 1., (ocpi_long_t) 1);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_value_bb_gain_dB()
{
  printf("TEST: default value for bb_gain_dB\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_bb_gain_dB(app, -1.);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_value_frequency_MHz()
{
  printf("TEST: default value for frequency_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_frequency_MHz(app, 2400., (ocpi_ulonglong_t) 2400000000);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_value_sample_rate_MHz()
{
  printf("TEST: default value for sample_rate_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_sample_rate_MHz(app, 15.36, (ocpi_ulong_t) 15360000);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_value_rf_cutoff_frequency_MHz()
{
  printf("TEST: default value for rf_cutoff_frequency_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_rf_cutoff_frequency_MHz(app, -1.);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_value_bb_cutoff_frequency_MHz()
{
  printf("TEST: default value for bb_cutoff_frequency_MHz\n");
  bool did_pass;
  try
  {
    {
      OCPI::API::Application app(APP_DEFAULT_E3XX_XML, NULL);
      app.initialize();
      app.start();
      app.stop();
      did_pass = did_pass_test_expected_value_bb_cutoff_frequency_MHz(app, 18., (ocpi_ulong_t) 18000000);
      if(!did_pass) { return false; }
    }
  }
  catch (std::string &e)
  {
    fprintf(stderr, "Exception thrown: %s\n", e.c_str());
    return false;
  }
  return true;
}

bool did_pass_test_ocpi_app_default_values()
{
  if(!did_pass_test_ocpi_app_default_value_rf_gain_dB())              { return false; }
  if(!did_pass_test_ocpi_app_default_value_bb_gain_dB())              { return false; }
  if(!did_pass_test_ocpi_app_default_value_frequency_MHz())           { return false; }
  if(!did_pass_test_ocpi_app_default_value_sample_rate_MHz())         { return false; }
  if(!did_pass_test_ocpi_app_default_value_rf_cutoff_frequency_MHz()) { return false; }
  if(!did_pass_test_ocpi_app_default_value_bb_cutoff_frequency_MHz()) { return false; }

  return true;
}

#endif // _TEST_APP_DEFAULTS_H
