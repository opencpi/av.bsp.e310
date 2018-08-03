#ifndef _WRITERS_AD9361_RX_FILTERS_ANALOG
#define _WRITERS_AD9361_RX_FILTERS_ANALOG

/*! @brief Set the nominal in-situ value with No-OS precision
 *         of the 
 *         rx_rf_bandwidth in Hz
 *         to an operating AD9361 IC controlled by the specified OpenCPI
 *         application instance of the ad9361_config_proxy.rcc worker.
 *
 *  @param[in]  app                 OpenCPI application reference
 *  @param[in]  app_inst_name_proxy OpenCPI application instance name of the
 *                                  OpenCPI ad9361_config_proxy.rcc worker
 *  @param[out] val                 Retrieved value.
 *  @return 0 if there are no errors, non-zero char array pointer if there
 *          are errors (char array content will describe the error).
 ******************************************************************************/
const char* set_AD9361_rx_rf_bandwidth_Hz(
    OCPI::API::Application& app, const char* app_inst_name_proxy,
    ocpi_ulong_t& val)
{
  OCPI::API::Property p(app, app_inst_name_proxy, "rx_rf_bandwidth");
  p.setULongValue(val);

  return 0;
}

#endif // _WRITERS_AD9361_RX_FILTERS_ANALOG
