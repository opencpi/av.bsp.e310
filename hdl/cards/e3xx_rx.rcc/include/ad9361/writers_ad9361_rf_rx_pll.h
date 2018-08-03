#ifndef _WRITERS_AD9361_RF_RX_PLL_H
#define _WRITERS_AD9361_RF_RX_PLL_H

/*! @file readers_ad9361_rx_rfpll.h
 *  @brief Provides functions for writing in-situ Receiver (RX) Radio
 *         Frequency (RF) Phase-Locked Loop (RFPLL) values to an operating
 *         AD9361 IC using an OpenCPI application.
 ******************************************************************************/

/*! @brief Set the nominal in-situ value with No-OS precision
 *         of the
 *         AD9361 RX RF LO frequency in Hz
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
const char* set_AD9361_Rx_RFPLL_LO_freq_Hz(
    OCPI::API::Application& app, const char* app_inst_name_proxy,
    const ocpi_ulonglong_t& val)
{
  OCPI::API::Property p(app, app_inst_name_proxy, "rx_lo_freq");
  p.setULongLongValue(val);

  return 0;
}

#endif // _WRITERS_AD9361_RF_RX_PLL_H
