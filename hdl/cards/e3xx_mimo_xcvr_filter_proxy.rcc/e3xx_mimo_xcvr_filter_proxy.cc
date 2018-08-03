/*
 * THIS FILE WAS ORIGINALLY GENERATED ON Mon Oct 16 18:20:18 2017 EDT
 * BASED ON THE FILE: e3xx_mimo_xcvr_filter_proxy.xml
 * YOU *ARE* EXPECTED TO EDIT IT
 *
 * This file contains the implementation skeleton for the e3xx_mimo_xcvr_filter_proxy worker in C++
 */

#include "e3xx_mimo_xcvr_filter_proxy-worker.hh"

using namespace OCPI::RCC; // for easy access to RCC data types and constants
using namespace E3xx_mimo_xcvr_filter_proxyWorkerTypes;

class E3xx_mimo_xcvr_filter_proxyWorker : public E3xx_mimo_xcvr_filter_proxyWorkerBase {

    void update_leds()
    {
        slave.set_LED_TXRX2_TX(m_properties.trxa_mode == TRXA_MODE_TX);
        slave.set_LED_TXRX2_RX(m_properties.trxa_mode == TRXA_MODE_RX);
        slave.set_LED_RX2_RX(m_properties.rx2a_mode == RX2A_MODE_RX);

        slave.set_LED_TXRX1_TX(m_properties.trxb_mode == TRXB_MODE_TX);
        slave.set_LED_TXRX1_RX(m_properties.trxb_mode == TRXB_MODE_RX);
        slave.set_LED_RX1_RX(m_properties.rx2b_mode == RX2B_MODE_RX);
    }

    // Function to update the switches/LEDs for frontend A depending on the
    // current frequency and mode selection.
    void frontend_a_update()
    {
        const bool tx_low_band = m_properties.tx_frequency_MHz < 2940.0;
        const bool rx_low_band = m_properties.rx_frequency_MHz < 2600.0;

        if (m_properties.trxa_mode == TRXA_MODE_RX) {
            slave.set_VCTXRX2_V1(0);
            slave.set_VCTXRX2_V2(1);
 
            slave.set_TX_ENABLE2A(0);
            slave.set_TX_ENABLE2B(0);
 
            slave.set_VCRX2_V1(rx_low_band ? 1 : 0);
            slave.set_VCRX2_V2(rx_low_band ? 0 : 1);
        } else {
            slave.set_VCTXRX2_V1(1);
            slave.set_VCTXRX2_V2(tx_low_band ? 0 : 1);

            if (m_properties.trxa_mode == TRXA_MODE_TX) {
                slave.set_TX_ENABLE2A(tx_low_band ? 0 : 1);
                slave.set_TX_ENABLE2B(tx_low_band ? 1 : 0);
            } else {
                slave.set_TX_ENABLE2A(0);
                slave.set_TX_ENABLE2B(0);
            }

            slave.set_VCRX2_V1(rx_low_band ? 0 : 1);
            slave.set_VCRX2_V2(rx_low_band ? 1 : 0);
        }

        update_leds();
    }

    // Function to update the switches/LEDs for frontend B depending on the
    // current frequency and mode selection.
    void frontend_b_update()
    {
        const bool tx_low_band = m_properties.tx_frequency_MHz < 2940.0;
        const bool rx_low_band = m_properties.rx_frequency_MHz < 2600.0;

        if (m_properties.trxb_mode == TRXB_MODE_RX) {
            slave.set_VCTXRX1_V1(1);
            slave.set_VCTXRX1_V2(0);
 
            slave.set_TX_ENABLE1A(0);
            slave.set_TX_ENABLE1B(0);

            slave.set_VCRX1_V1(rx_low_band ? 1 : 0);
            slave.set_VCRX1_V2(rx_low_band ? 0 : 1);
        } else {
            slave.set_VCTXRX1_V1(tx_low_band ? 0 : 1);
            slave.set_VCTXRX1_V2(1);

            if (m_properties.trxb_mode == TRXB_MODE_TX) {
                slave.set_TX_ENABLE1A(tx_low_band ? 0 : 1);
                slave.set_TX_ENABLE1B(tx_low_band ? 1 : 0);
            } else {
                slave.set_TX_ENABLE1A(0);
                slave.set_TX_ENABLE1B(0);
            }

            slave.set_VCRX1_V1(rx_low_band ? 0 : 1);
            slave.set_VCRX1_V2(rx_low_band ? 1 : 0);
        }

        update_leds();
    }

    // The two RX channels of the AD9361 share an LO so when
    // the frequency is written, set the filter banks for
    // both channels
    RCCResult rx_frequency_MHz_written()
    {
        frontend_a_update();
        frontend_b_update();

        if (m_properties.rx_frequency_MHz < 450.0) {
            slave.set_RX1_BANDSEL(0b100);
            slave.set_RX1B_BANDSEL(0b11); //don't care
            slave.set_RX1C_BANDSEL(0b10);

            slave.set_RX2_BANDSEL(0b101);
            slave.set_RX2B_BANDSEL(0b11); //don't care
            slave.set_RX2C_BANDSEL(0b01);
        } else if (m_properties.rx_frequency_MHz < 700.0) {
            slave.set_RX1_BANDSEL(0b010);
            slave.set_RX1B_BANDSEL(0b11); //don't care
            slave.set_RX1C_BANDSEL(0b11);

            slave.set_RX2_BANDSEL(0b011);
            slave.set_RX2B_BANDSEL(0b11); //don't care
            slave.set_RX2C_BANDSEL(0b11);
        } else if (m_properties.rx_frequency_MHz < 1200.0) {
            slave.set_RX1_BANDSEL(0b000);
            slave.set_RX1B_BANDSEL(0b11); //don't care
            slave.set_RX1C_BANDSEL(0b01);

            slave.set_RX2_BANDSEL(0b001);
            slave.set_RX2B_BANDSEL(0b11); //don't care
            slave.set_RX2C_BANDSEL(0b10);
        } else if (m_properties.rx_frequency_MHz < 1800.0) {
            slave.set_RX1_BANDSEL(0b001);
            slave.set_RX1B_BANDSEL(0b10);
            slave.set_RX1C_BANDSEL(0b11); //don't care

            slave.set_RX2_BANDSEL(0b000);
            slave.set_RX2B_BANDSEL(0b01);
            slave.set_RX2C_BANDSEL(0b11); //don't care
        } else if (m_properties.rx_frequency_MHz < 2350.0) {
            slave.set_RX1_BANDSEL(0b011);
            slave.set_RX1B_BANDSEL(0b11);
            slave.set_RX1C_BANDSEL(0b11); //don't care

            slave.set_RX2_BANDSEL(0b010);
            slave.set_RX2B_BANDSEL(0b11);
            slave.set_RX2C_BANDSEL(0b11); //don't care
        } else if (m_properties.rx_frequency_MHz < 2600.0) {
            slave.set_RX1_BANDSEL(0b101);
            slave.set_RX1B_BANDSEL(0b01);
            slave.set_RX1C_BANDSEL(0b11); //don't care

            slave.set_RX2_BANDSEL(0b100);
            slave.set_RX2B_BANDSEL(0b10);
            slave.set_RX2C_BANDSEL(0b11); //don't care
        } else {
            slave.set_RX1_BANDSEL(0b111); //don't care
            slave.set_RX1B_BANDSEL(0b11); //don't care
            slave.set_RX1C_BANDSEL(0b11); //don't care

            slave.set_RX2_BANDSEL(0b111); //don't care
            slave.set_RX2B_BANDSEL(0b11); //don't care
            slave.set_RX2C_BANDSEL(0b11); //don't care
        }

        return RCC_DONE;
    }

    RCCResult tx_frequency_MHz_written()
    {
        frontend_a_update();
        frontend_b_update();

        if (m_properties.tx_frequency_MHz < 117.7) {
            slave.set_TX_BANDSEL(0b111); // 7
        } else if (m_properties.tx_frequency_MHz < 178.2) {
            slave.set_TX_BANDSEL(0b110); // 6
        } else if (m_properties.tx_frequency_MHz < 284.3) {
            slave.set_TX_BANDSEL(0b101); // 5
        } else if (m_properties.tx_frequency_MHz < 453.7) {
            slave.set_TX_BANDSEL(0b100); // 4
        } else if (m_properties.tx_frequency_MHz < 723.8) {
            slave.set_TX_BANDSEL(0b011); // 3
        } else if (m_properties.tx_frequency_MHz < 1154.9) {
            slave.set_TX_BANDSEL(0b010); // 2
        } else if (m_properties.tx_frequency_MHz < 1842.6) {
            slave.set_TX_BANDSEL(0b001); // 1
        } else if (m_properties.tx_frequency_MHz < 2940.0) {
            slave.set_TX_BANDSEL(0b000); // 0
        } else {
            slave.set_TX_BANDSEL(0b111); // 7 (don't care)
        }

        return RCC_DONE;
    }

    RCCResult trxa_mode_written()
    {
        frontend_a_update();
        return RCC_DONE;
    }

    RCCResult rx2a_mode_written()
    {
        frontend_a_update();
        return RCC_DONE;
    }

    RCCResult trxb_mode_written()
    {
        frontend_b_update();
        return RCC_DONE;
    }

    RCCResult rx2b_mode_written()
    {
        frontend_b_update();
        return RCC_DONE;
    }

    RCCResult run(bool /*timedout*/) {
        return RCC_OK; // change this as needed for this worker to do something useful
        // return RCC_ADVANCE; when all inputs/outputs should be advanced each time "run" is called.
        // return RCC_ADVANCE_DONE; when all inputs/outputs should be advanced, and there is nothing more to do.
        // return RCC_DONE; when there is nothing more to do, and inputs/outputs do not need to be advanced.
    }
};

E3XX_MIMO_XCVR_FILTER_PROXY_START_INFO
// Insert any static info assignments here (memSize, memSizes, portInfo)
// e.g.: info.memSize = sizeof(MyMemoryStruct);
E3XX_MIMO_XCVR_FILTER_PROXY_END_INFO
