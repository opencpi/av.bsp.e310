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

#include <iostream>
#include <vector>
#include "OcpiApi.h"

namespace OA = OCPI::API;

void print_help()
{
    std::cout << "Usage: ./e3xx_mimo_xcvr_filter_proxy [-h,--help] [RX_FREQ TX_FREQ]\n";
    std::cout << "Where:\n";
    std::cout << "  RX_FREQ       Receive LO tuning frequency, in MHz.\n";
    std::cout << "  TX_FREQ       Transmit LO tuning frequency, in MHz.\n";
    std::cout << "  -h,--help     Print this help menu, then exit.\n";
    std::cout << "Description:\n";
    std::cout << "  A simple testbench for the e3xx_mimo_xcvr_filter device and proxy\n";
    std::cout << "  worker. When run with no parameters, it will cycle through all 4\n";
    std::cout << "  configurations for every filter bank selection. If RX_FREQ and\n";
    std::cout << "  TX_FREQ are provided, it will only use those frequencies. There\n";
    std::cout << "  is no option to run with only RX_FREQ or only TX_FREQ specified,\n";
    std::cout << "  either both must be specified, or neither of them.\n";
    std::cout << "Examples:\n";
    std::cout << "  To run cycling through all of the configurations\n";
    std::cout << "       ./e3xx_mimo_xcvr_filter_proxy\n";
    std::cout << "  To run with specific frequencies\n";
    std::cout << "       ./e3xx_mimo_xcvr_filter_proxy 1500 1300\n";
}

std::vector<std::string> get_rx_freqs(int argc, char *argv[])
{
    if (argc == 1) {
        return {
            "440",
            // 450 MHz filter bank crossover threshold
            "600",
            // 700 MHz filter bank crossover threshold
            "1000",
            // 1200 MHz filter bank crossover threshold
            "1500",
            // 1800 MHz filter bank crossover threshold
            "2000",
            // 2350 MHz filter bank crossover threshold
            "2550",
            // 2600 MHz  filter bank crossover threshold
            "2700"
        };
    } else {
        return { argv[1] };
    }
}

std::vector<std::string> get_tx_freqs(int argc, char *argv[])
{
    if (argc == 1) {
        return {
            "115",
            // 117.7 MHz filter bank crossover threshold
            "168",
            // 178.2 MHz filter bank crossover threshold
            "200",
            // 284.3 MHz filter bank crossover threshold
            "400",
            // 453.7 MHz filter bank crossover threshold
            "600",
            // 723.8 MHz filter bank crossover threshold
            "900",
            // 1154.9 MHz  filter bank crossover threshold
            "1300",
            // 1842.6 MHz  filter bank crossover threshold
            "2550",
            // 2940.0 MHz  filter bank crossover threshold
            "3000"
        };
    } else {
        return { argv[2] };
    }
}

void set_configuration(OA::Application &app, size_t i, std::string trxa_mode, std::string rx2a_mode,
                       std::string rx2b_mode, std::string trxb_mode, std::string expected_mode_state)
{
    std::cout << "\nConfiguration " << i << ":\n";
    std::cout << "    Frontend | TRXA | RX2A | RX2B | TRXB \n";
    std::cout << "   ----------+------+------+------+-------\n";
    std::cout << "        Mode |" <<   expected_mode_state  << "\n";
    app.setProperty("e3xx_mimo_xcvr_filter_proxy", "trxa_mode", trxa_mode.c_str());
    app.setProperty("e3xx_mimo_xcvr_filter_proxy", "rx2a_mode", rx2a_mode.c_str());
    app.setProperty("e3xx_mimo_xcvr_filter_proxy", "rx2b_mode", rx2b_mode.c_str());
    app.setProperty("e3xx_mimo_xcvr_filter_proxy", "trxb_mode", trxb_mode.c_str());
    std::cout << "Press 'Enter' to continue...";
    std::cin.ignore();
}

int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 3) {
        print_help();
        return 1;
    }

    for (int i=0; i < argc; i++) {
        std::string tmpargv = std::string(argv[i]);
        if (tmpargv == "--help" || tmpargv == "-h" || tmpargv == "-help") {
            print_help();
            return 0;
        }
    }

    std::vector<std::string> rx_freqs = get_rx_freqs(argc, argv);
    std::vector<std::string> tx_freqs = get_tx_freqs(argc, argv);

    try {
        std::string appxml = "e3xx_mimo_xcvr_filter_proxy-test.xml";
        OA::Application app(appxml.c_str(), NULL);
        app.initialize();
        app.start();
        std::cout << "Start of Testbench\n";

        for (size_t i=0; i < tx_freqs.size(); i++) {
            std::cout << "===================Frequency Set " << i+1 << " of " << tx_freqs.size() << "===================\n";
            std::cout << "    RX Frequency: " << rx_freqs[i%rx_freqs.size()] << "\n";
            std::cout << "    TX Frequency: " << tx_freqs[i%tx_freqs.size()] << "\n";
            app.setProperty("e3xx_mimo_xcvr_filter_proxy", "rx_frequency_MHz", rx_freqs[i%7].c_str());
            app.setProperty("e3xx_mimo_xcvr_filter_proxy", "tx_frequency_MHz", tx_freqs[i].c_str());

            //                        trxa   rx2a    rx2b    trxb
            set_configuration(app, 1, "tx",  "rx",  "rx",  "tx",  "  TX  |  RX  |  RX  |  TX  ");
            set_configuration(app, 2, "rx",  "off", "rx",  "tx",  "  RX  |  XX  |  RX  |  TX  ");
            set_configuration(app, 3, "rx",  "off", "off", "rx",  "  RX  |  XX  |  XX  |  RX  ");
            set_configuration(app, 4, "tx",  "rx",  "off", "rx",  "  TX  |  RX  |  XX  |  RX  ");
            set_configuration(app, 5, "tx",  "rx",  "off", "off", "  TX  |  RX  |  XX  |  XX  ");
            set_configuration(app, 6, "off", "off", "rx",  "tx",  "  XX  |  XX  |  RX  |  TX  ");
            set_configuration(app, 7, "off", "off", "off", "off", "  XX  |  XX  |  XX  |  XX  ");

            std::cout << "===================Frequency Set End===================\n";
        }
        std::cout << "\n\nFinished.\n";
        app.stop();
    } catch (std::string &e) {
        std::cerr << "Exception thrown: " << e << "\n";
        return 1;
    }
    return 0;
}
