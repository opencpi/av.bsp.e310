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
 * THIS FILE WAS ORIGINALLY GENERATED ON Wed Feb 27 16:41:41 2019 EST
 * BASED ON THE FILE: e3xx_mimo_xcvr_gps_time_proxy.xml
 * YOU *ARE* EXPECTED TO EDIT IT
 *
 * This file contains the implementation skeleton for the e3xx_mimo_xcvr_gps_time_proxy worker in C++
 */

#include <cinttypes> // PRIu32
#include "e3xx_mimo_xcvr_gps_time_proxy-worker.hh"
#include <fstream> // std::ifstream
#include <string> // std::string
#include <cstdlib> // system()
#include <cmath> // std::pow()
#include <vector> // std::vector
#include <sstream> // std::stringstream
#include <cstdio> // FILE
#include <time.h>
#include <unistd.h>
#include "UBloxPacket.h"

using namespace OCPI::RCC; // for easy access to RCC data types and constants
using namespace E3xx_mimo_xcvr_gps_time_proxyWorkerTypes;
namespace OA = OCPI::API;

#define LOG_DEBUG(...) log(OCPI_LOG_DEBUG, __VA_ARGS__)
#define SYNC_TIMEOUT_SEC_P E3XX_MIMO_XCVR_GPS_TIME_PROXY_SYNC_TIMEOUT_SEC_P
#define FILENAME_P         E3XX_MIMO_XCVR_GPS_TIME_PROXY_FILENAME_P

class E3xx_mimo_xcvr_gps_time_proxyWorker : public E3xx_mimo_xcvr_gps_time_proxyWorkerBase {

  struct GPRMC {
    std::string utc;
    std::string day;
    std::string month;
    std::string year;
    bool        gps_fix;
  };

  class E3xxCfgAntSetUBloxPacket : public UBloxPacket {

  public:

    E3xxCfgAntSetUBloxPacket() : UBloxPacket() {

      // CFG-ANT
      this->set_ublox_class(0x06);
      this->set_id(0x13);

      // initialize length, payload, checksum (compare against values in:
      // https://github.com/EttusResearch/meta-ettus/blob/40cf300296b889df5b8445bf7db457c160220919/e300-bsp/recipes-navigation/gps-config/files/ettus-e300/gps_prep)
      this->set_length(4);
      this->set_payload_byte(0, 0x1b);
      this->set_payload_byte(1, 0x00);
      this->set_payload_byte(2, 0x51);
      this->set_payload_byte(3, 0x82);
    }

    /// @brief Set Enable Antenna Supply Voltage Control Signal.
    void set_svcs(bool val) {

      if(val) {
        uint8_t val = get_payload_byte(0);
        val |= 0x01;
        set_payload_byte(0, val);
      }
      else {
        uint8_t val = get_payload_byte(0);
        val &= ~(0x01);
        set_payload_byte(0, val);
      }
    }

  }; // class E3xxCfgAntSetUBloxPacket

  class E3xxMonVerUBloxPacket : public UBloxPacket {

  public:

    E3xxMonVerUBloxPacket() : UBloxPacket() {

      // MON-VER
      this->set_ublox_class(0x0a);
      this->set_id(0x04);

      this->set_length(0);
    }

  }; // class E3xxMonVerUBloxPacket

  FILE*         m_fp;
  char          m_uart_buffer[512];
  RunCondition  m_run_condition;

public:

  E3xx_mimo_xcvr_gps_time_proxyWorker() : m_run_condition(RCC_NO_PORTS) {

    // run function should never be called
    setRunCondition(&m_run_condition);
  }

private:

  void open_file() {

    m_fp = fopen(FILENAME_P, "a+b");
    if(m_fp) {
      //setvbuf(m_fp, (char*)NULL, _IONBF, 0);
      //setvbuf(m_fp, (char*)NULL, _IOLBF, 0);
    }
    else {
      const char* err = "error opening file";
      perror(err);
      throw std::string(err);
    }

    log(OCPI_LOG_DEBUG, "file opened");
  }

  void close_file() {

    log(OCPI_LOG_DEBUG, "closing file...");
    fclose(m_fp); /// @todo / FIXME - sometimes takes >= 10 seconds
    log(OCPI_LOG_DEBUG, "... file closed");
  }

  void set_gps_antenna_is_powered_via_file(bool is_powered) {

    E3xxCfgAntSetUBloxPacket ublox_packet;

    ublox_packet.set_svcs(is_powered);

    for(uint16_t ii = 0; ii < ublox_packet.get_length()+8; ii++) {
      log(OCPI_LOG_DEBUG, "buffer[%u]=0x%" PRIx8, ii, ublox_packet.get_buffer_byte(ii));
    }

    fwrite(ublox_packet.get_pbuffer(), 1, ublox_packet.get_length()+8, m_fp);

    if(ferror(m_fp)) {
      const char* err = "error setting GPS antenna power";
      perror(err);
      throw std::string(err);
    }

    const char* cc = is_powered ? "ON" : "OFF";
    log(OCPI_LOG_INFO, "GPS antenna was powered %s", cc);
  }

  void set_gps_antenna_is_powered_via_gpsctl(bool is_powered) {
    
    std::string cmd_str("gpsctl ");
    cmd_str += FILENAME_P;
    cmd_str += " -t 'u-blox' -x '";

    if(is_powered) {
      cmd_str += "\\x06\\x13\\x1b\\x00\\x51\\x82";
    }
    else {
	    cmd_str += "\\x06\\x13\\x1a\\x00\\x51\\x82";
    }

    cmd_str += "'";
    
    std::cout << "executing command: <" << cmd_str << "<\n";
    int res = system(cmd_str.c_str());
    if(res != 0) {
      throw std::string("system call returned non-zero value");
    }
  }

  void set_gps_antenna_is_powered(bool is_powered) {

    set_gps_antenna_is_powered_via_file(is_powered);
  }

  std::string get_fract_time_str(double time, bool do_question_mark=true) {

    std::string ret;

    std::ostringstream oss;
    oss << time;
    std::string str = oss.str();

    ret = str.substr(2, str.size()-1);
    ret = do_question_mark ? ("?." + ret) : ret;

    return ret;
  }

  void log_nmea_gprmc_msg(const GPRMC& gprmc_msg, double sst) {

    std::string msg("UTC=");
    msg += gprmc_msg.utc;
    msg += ", day=";
    msg += gprmc_msg.day;
    msg += ", month=";
    msg += gprmc_msg.month;
    msg += ", year=";
    msg += gprmc_msg.year;
    msg += ", fix valid=";
    msg += (gprmc_msg.gps_fix ? "true" : "false");

    LOG_DEBUG("@GPS unix epoch time=%s   \t\tsec, %s received  NMEA (GPRMC) message with %s", get_fract_time_str(sst).c_str(), (gprmc_msg.gps_fix ? "[event=1/2]" : "           "), msg.c_str());
  }

  /*! @brief Return a list of the words of the string s separating at the given
   *         delimiter, char delim.
   ****************************************************************************/
  std::vector<std::string> split(std::string s, char delim) {

    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while(getline(ss, item, delim)) {
      result.push_back(item);
    }

    return result;
  }

  GPRMC get_nmea_gprmc_msg(std::string nmea_str) {

    GPRMC ret;

    bool saw_utc = false;
    bool saw_fix = false;
    bool saw_dmy = false;

    unsigned idx = nmea_str.find_last_of('*');
    std::vector<std::string> fields = split(nmea_str.substr(0,idx-1), ',');
    unsigned ii = 0;
    for(auto it = fields.begin(); it != fields.end(); ++it) {
      if(ii > 0 && *it != "") {
        if (ii == 1) {
          ret.utc = *it;
          saw_utc = true;
        }
        else if (ii == 2) {
          ret.gps_fix = *it == "A";
          saw_fix = true;
        }
        else if (ii == 9) {
          ret.day   = it->substr(0,2);
          ret.month = it->substr(2,2);
          ret.year  = "20" + it->substr(4,2);
          saw_dmy = true;
        }
      }
      ii++;
    }

    if(not(saw_utc and saw_fix and saw_dmy)) {
      throw std::string("NMEA GPRMC message invalid");
    }

    return ret;
  }

  uint32_t get_time_server_time_fract() {

    return slaves.time_server.get_timeNow() & 0x00000000ffffffff;
  }

  /// @brief get GPS fix from NMEA GPRMC message by polling
  GPRMC get_gps_fix_nmea_gprmc_msg(uint16_t timeout_sec = SYNC_TIMEOUT_SEC_P,
      bool will_log_debug=false) {

    GPRMC ret;

    //wait_for_pps_okay(li);

    double sst; // sub-second time in seconds, e.g. 0.1235 (based on
                // timeNow's fractional value)
    uint32_t gps_fix_nmea_gprmc_msg_time_fract;

    time_t start;
    time(&start);

    while(1) {
      if(fgetc(m_fp) == '$') {
        gps_fix_nmea_gprmc_msg_time_fract = get_time_server_time_fract();
        if(will_log_debug) {
          sst = ((double)gps_fix_nmea_gprmc_msg_time_fract) / std::pow(2.,32.);
          std::string fts = get_fract_time_str(sst);
          const char* c1 = fts.c_str();
          const char* c2 = "start of  NMEA message";
          LOG_DEBUG("@GPS unix epoch time=%s   \t\tsec, %s", c1, c2);
        }

        fgets(m_uart_buffer, sizeof(m_uart_buffer), m_fp);

        if(get_pps_okay() && gps_fix_nmea_gprmc_msg_time_fract > 0x00000ccc) {
          std::string line(m_uart_buffer);

          if(will_log_debug) {
            sst = ((double)get_time_server_time_fract()) / std::pow(2.,32.);
            std::string fts = get_fract_time_str(sst);
            const char* c1 = fts.c_str();
            const char* c2 = "end   of  NMEA message with content: $";
            LOG_DEBUG("@GPS unix epoch time=%s   \t\tsec, %s%s", c1, c2, line.c_str());
          }
          if(line.find("GPRMC") != std::string::npos) {
            log(OCPI_LOG_INFO, "GPRMC found");
            ret = get_nmea_gprmc_msg(line);
            if(will_log_debug) {
              log_nmea_gprmc_msg(ret, sst);
            }
            if(ret.gps_fix) {
              log(OCPI_LOG_INFO, "NMEA GPRMC fix found");
              break;
            }
          }
        }
      }

      time_t end;
      time(&end);

      if(difftime(end, start) >= (double)timeout_sec) {
        throw std::string("sync timeout occurred");
      }
    }

    return ret;
  }

  void log_platform_pps_src(OA::UChar pps_src) {

#define PLATFORM_PPS_SRC_GENERIC  0
#define PLATFORM_PPS_SRC_GPS_PPS  1
#define PLATFORM_PPS_SRC_EXTERNAL 2

    std::string msg;
    if(pps_src == PLATFORM_PPS_SRC_GPS_PPS) {
      msg.assign("platform timebase PPS source detected: GPS PPS");
    }
    else if(pps_src == PLATFORM_PPS_SRC_GENERIC) {
      msg.assign("platform timebase PPS source detected: generic/unknown");
    }
    else if(pps_src == PLATFORM_PPS_SRC_EXTERNAL) {
      msg.assign("platform timebase PPS source detected: SYNC");
    }
    else {
      msg.assign("platform timebase PPS source detected: reserved");
    }
    log(OCPI_LOG_INFO, msg.c_str());
  }

  bool get_pps_okay() {

    /// @todo / FIXME - comment back in once bug fixed (reads back wrong value)
    /*static bool first_run = true;

    if(first_run) {
      uint8_t src;
      src = getApplication().getPropertyValue<uint8_t>("platform", "pps_src");
      log_platform_pps_src(src);
      if(src != PLATFORM_PPS_SRC_GPS_PPS) {
        std::cout << "WARN: platform timebase PPS source is not GPS PPS";
      }
      first_run = false;
    }*/

    const uint32_t PPS_OK_BITMASK = 0x08000000;
    return (slaves.time_server.get_status()& PPS_OK_BITMASK) == PPS_OK_BITMASK;
  }

  uint8_t get_rolling_pps_count() {

    return slaves.time_server.get_status() & 0x000000ff;
  }

  /*! @brief Get epoch time from an GPS NMEA message containing an affirmative
   *         fix. Note that not all NMEA messages contain an affirmative fix.
   ****************************************************************************/
  uint32_t get_uepoch_time_int_from_nmea_gprmc_fix(
      uint16_t timeout_sec = SYNC_TIMEOUT_SEC_P, bool will_log_debug=false) {

    GPRMC gprmc = get_gps_fix_nmea_gprmc_msg(timeout_sec, will_log_debug);

    // Calculate unix timestamp
    // Used this to calculate unix timestamp:
    // http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_16
    // And the idea of a array of month days from stackoverflow:
    // https://stackoverflow.com/questions/283166/easy-way-to-convert-a-struct-tm-expressed-in-utc-to-time-t-type

    int month_days[12] = {0, 31, 59, 90, 120, 151,
                          181, 212, 243, 273, 304, 334};
    int hours = std::stoi(gprmc.utc.substr(0,2));
    int minutes = std::stoi(gprmc.utc.substr(2,2));
    int seconds = std::stoi(gprmc.utc.substr(4, gprmc.utc.back()));
    int tm_year = std::stoi(gprmc.year)-1900; // calendar year minus 1900
    int tm_month = std::stoi(gprmc.month);
    int tm_day = std::stoi(gprmc.day);

    // days since January 1 of the year
    int tm_yday = month_days[tm_month-1] + tm_day -1;

    return (uint32_t)(seconds + minutes*60 + hours*3600 +
                     tm_yday*86400 +(tm_year-70)*31536000 +
                     ((tm_year-69)/4)*86400 - ((tm_year-1)/100)*86400 +
                     ((tm_year+299)/400)*86400);
  }

  uint32_t get_uepoch_time_int_from_gps_fix(
      uint16_t timeout_sec = SYNC_TIMEOUT_SEC_P, bool ld=false) {

    return get_uepoch_time_int_from_nmea_gprmc_fix(timeout_sec, ld);
  }

  /*! @brief Sync time server time to GPS epoch fix. GPS antenna is assumed to
   *          be powered on before this function is called.
   ****************************************************************************/
  void sync(uint16_t timeout_sec = SYNC_TIMEOUT_SEC_P) {

    // variables for logging
    const bool will_log_i = willLog(OCPI_LOG_INFO);
    const bool will_log_d = willLog(OCPI_LOG_DEBUG);

    // once this function call returns, time_server.hdl ppsOK will be true
    // (required for write to timeNow to ignore fractional bits) and GPS time
    // fix has been received
    bool ld = will_log_i;
    uint32_t uepoch_time_int_from_gps_fix;
    uint16_t tos = timeout_sec;
    uepoch_time_int_from_gps_fix = get_uepoch_time_int_from_gps_fix(tos, ld);

    // write time_server.hdl's timeNow's integer portion
    uint64_t old_ts_time = slaves.time_server.get_timeNow();
    uint64_t new_ts_time = ((uint64_t)uepoch_time_int_from_gps_fix) << 32;
    uint64_t nb = slaves.time_server.get_timeNow(); // new time before write
    slaves.time_server.set_timeNow(new_ts_time);

    uint64_t na = slaves.time_server.get_timeNow(); // new time after write

    // sync event window is intended to ensure integer portion of time is
    // correct
    const uint32_t time_fract_sync_window_size = 0xffff0000;
    uint32_t na_fract = (uint32_t)(na & 0x00000000ffffffff);
    bool cond1 = na_fract > 0xffff0000;
    bool cond2 = (nb - old_ts_time) > time_fract_sync_window_size;
    if(cond1 or cond2) {
      LOG_DEBUG("old_ts_time=0x%" PRIx64 ", na=0x%" PRIx64 ", nb=0x%" PRIx64, old_ts_time, na, nb);
      printf("old_ts_time=0x%" PRIx64 "\n", old_ts_time);
      printf("na=0x%" PRIx64 "\n", na);
      std::string err("sync occurred but is invalid because ");
      err += "integer portion of time may be incorrect";
      throw err;
    }
    if(will_log_d) {
      double sst; // sub-second time (in seconds)
      sst = ((double)(old_ts_time & 0x00000000ffffffff)) / std::pow(2.,32);

      const char* c1 = "@GPS epoch time=";
      uint32_t t = uepoch_time_int_from_gps_fix;
      std::string fts = get_fract_time_str(sst, false);
      const char* c2 = fts.c_str();
      const char* c3 = "\tsec, [event=2/2] wrote time_server.hdl's timeNow ";
      const char* c4 = "integer portion = 0x";
      const char* c5 = " (time sync'd to GPS [via UART NMEA])";
      LOG_DEBUG("%s%" PRIu32 ".%s%s%s%" PRIx32 "%s", c1, t, c2, c3, c4, t, c5);
    }
    if(will_log_i) {
      // old upper
      uint32_t ou = (uint32_t)((old_ts_time & 0xffffffff00000000) >> 32);
      // new upper
      uint32_t nu = (uint32_t)((na & 0xffffffff00000000) >> 32);
      const char* c1 = "time_server.hdl's timeNow upper 32 (prev val=0x";
      const char* c2 = ") written to 0x";
      const char* c3 = " during sync with GPS epoch time";
      log(OCPI_LOG_INFO, "%s%" PRIx32 "%s%" PRIx32 "%s", c1, ou, c2, nu, c3);
      log(OCPI_LOG_INFO, "GPS SYNC SUCCESSFUL");
    }
  }

  RCCResult initialize() {

    RCCResult ret = RCC_OK;

    log(OCPI_LOG_INFO, "using tx/rx UART NMEA or UBlox");
    open_file(); // always using file in order to get fix

    try {
      // first try to sync, which is expected to work in cases where GPS
      // antenna is already powered on outside of this worker...
      sync(0);
    }
    catch(std::string& err) {
      // ... if that fails, try to set GPS antenna power on (which requires
      // opening file) and re-attempt sync
      try {
        if(m_fp == 0) { // if file not opened already
          open_file();
        }
        set_gps_antenna_is_powered(true);
        sync();
      }
      catch(std::string& err1) {
        ret = setError(err1.c_str());
      }
    }

    return ret;
  }
  // notification that sync property has been written
  RCCResult sync_written() {

    RCCResult ret = RCC_OK;

    if(m_properties.sync) {
      try {
        sync();
      }
      catch(std::string& err) {
        ret = setError(err.c_str());
      }
    }

    return ret;
  }
  // notification that unix_epoch_time property will be read
  RCCResult unix_epoch_time_read() {

    RCCResult ret = RCC_OK;

    try {
      //sync();

      uint64_t tmp = slaves.time_server.get_timeNow();
      m_properties.unix_epoch_time = ((double)tmp) / std::pow(2.,32.);
    }
    catch(std::string& err) {
      ret = setError(err.c_str());
    }

    return ret;
  }
  // notification that pps_okay property will be read
  RCCResult pps_okay_read() {

    m_properties.pps_okay = get_pps_okay();

    return RCC_OK;
  }
  // notification that rolling_pps_count property will be read
  RCCResult rolling_pps_count_read() {

    m_properties.rolling_pps_count = get_rolling_pps_count();

    return RCC_OK;
  }
  RCCResult run(bool /*timedout*/) {
    return RCC_DONE;
  }
};

E3XX_MIMO_XCVR_GPS_TIME_PROXY_START_INFO
// Insert any static info assignments here (memSize, memSizes, portInfo)
// e.g.: info.memSize = sizeof(MyMemoryStruct);
E3XX_MIMO_XCVR_GPS_TIME_PROXY_END_INFO
