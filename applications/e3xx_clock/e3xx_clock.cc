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
#include <unistd.h>
#include <cstdio>
#include <cassert>
#include <string>
//#include <signal.h>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <cinttypes>
#include "OcpiApi.hh"

namespace OA = OCPI::API;

OA::Application* p_app;

/*void signal_handler(int signal) {

  std::cout << "received signal: " << signal << "\n";
  p_app->stop();
}*/

class UQ32p32 {

public:
  UQ32p32(uint32_t int_32_val, uint32_t fract_32_val) :
      m_int_32_val(int_32_val), m_fract_32_val(fract_32_val) {
  }

  std::string get_int_str() {

    std::ostringstream oss;
    oss << m_int_32_val;

    return oss.str();
  }

  std::string get_fract_str() {

    std::string ret;

    // -----------------------------------------------
    // lower 16 bits
    // -----------------------------------------------

    uint16_t xx = m_fract_32_val & 0x0000ffff;
    uint64_t yy = 0;
    uint16_t test_val = (1 << 15);

    // 0.00000000000000005000000000000000
    uint64_t zz = 5000000000000000; // 16 digits

    for(size_t ii = 0; ii < 16; ii++) {
      if(xx >= test_val) {
        xx -= test_val;
        yy += zz;
      }
      else {
      }
      test_val >>= 1;
      zz /= 2;
    }
    uint64_t yy_low = yy;

    // -----------------------------------------------
    // upper 16 bits
    // -----------------------------------------------

    xx = (m_fract_32_val & 0xffff0000) >> 16;
    yy = 0;
    test_val = (1 << 15);

    // 0.5000000000000000
    zz = 5000000000000000; // 16 digits

    for(size_t ii = 0; ii < 16; ii++) {
      if(xx >= test_val) {
        xx -= test_val;
        yy += zz;
      }
      test_val >>= 1;
      zz /= 2;
    }
    uint64_t yy_upp = yy;

    // -----------------------------------------------
    // get string
    // -----------------------------------------------

    std::ostringstream oss;
    oss << yy_upp << yy_low;
    std::string yy_str = oss.str();

    for(size_t ii = 0; ii < 32 - yy_str.size(); ii++) {
      ret += "0";
    }

    ret += yy_str;

    return ret;
  }

  std::string get_str() {
    return get_int_str() + "." + get_fract_str();
  }

protected:
  uint32_t m_int_32_val, m_fract_32_val;

}; // class Q32p32

int main(int argc, char **argv) {
  // Reference https://opencpi.github.io/OpenCPI_Application_Development.pdf for
  // an explanation of the ACI.

  try {
    OA::Application app("e3xx_clock.xml");
    p_app = &app;
    app.initialize(); // all resources have been allocated
    app.start();      // execution is started

    bool enable_print = false;
    // Do work here.
    /*struct sigaction handler;
    handler.sa_handler = signal_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, NULL);*/
    
    int nsec = (argc > 1) ? atoi(argv[1]) : 0;
    while(1) {
      const char* inst = "time_server";
      //double epoch_time = app.getPropertyValue<double>(inst, "unix_epoch_time");
      uint64_t epoch_time = app.getPropertyValue<uint64_t>(inst, "timeNow");
      uint32_t int_32_val = (epoch_time & 0xffffffff00000000) >> 32;
      uint32_t fract_32_val = epoch_time & 0x00000000ffffffff;
      if(fract_32_val < 0x80000000) {
        if(enable_print) {
          if(nsec >= 0) {
            if(nsec-- == 0) {
              break;
            }
          }
          std::cout << "time=" << UQ32p32(int_32_val, fract_32_val).get_str();
          std::cout << ",  \tsource=" << "GPS-disciplined OpenCPI Hardware Time Service";
          std::cout << ",\tformat=" << "unix epoch";
          std::cout << "\n";
          enable_print = false;
        }
      }
      else {
        enable_print = true;
      }
    }

    p_app->stop();

  } catch (std::string &e) {
    std::cerr << "app failed: " << e << std::endl;
    return 1;
  }
  return 0;
}
