#ifndef _WRITERS_AD9361_RX_ADC_H
#define _WRITERS_AD9361_RX_ADC_H

/*! @brief Set the nominal in-situ value with No-OS precision
 *         of the
 *         AD9361 CLKRF frequency in Hz
 *         to an operating AD9361 IC controlled by the specified OpenCPI
 *         application instance of the ad9361_config_proxy.rcc worker.
 *
 *  @param[in]  app                 OpenCPI application reference
 *  @param[in]  app_inst_name_proxy OpenCPI application instance name of the
 *                                  OpenCPI ad9361_config_proxy.rcc worker
 *  @param[in]  val                 Value to assign. Note that the precision
 *                                  is the same as the corresponding OpenCPI
 *                                  property which is the same as the
 *                                  underlying No-OS API call.
 *  @return 0 if there are no errors, non-zero char array pointer if there
 *          are errors (char array content will describe the error).
 ******************************************************************************/
const char* set_AD9361_CLKRF_FREQ_Hz(
    OCPI::API::Application& app, const char* app_inst_name_proxy,
    const ocpi_ulong_t& val)
{
  OCPI::API::Property p(app, app_inst_name_proxy, "rx_sampling_freq");
  p.setULongValue(val);

  return 0;
}

#endif // _WRITERS_AD9361_RX_ADC_H
