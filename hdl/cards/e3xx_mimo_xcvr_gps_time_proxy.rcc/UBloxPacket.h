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

#ifndef __UBLOX_PACKET_H
#define __UBLOX_PACKET_H

class UBloxPacket {

public:

  struct Checksum {
    const uint8_t m_ck_a;
    const uint8_t m_ck_b;
  };

  struct ChecksumCalculator{

  public:

    ChecksumCalculator() : m_ck_a(0), m_ck_b(0) {
    }

    void init() {
      m_ck_a = m_ck_b = 0;
    }

    void add(uint8_t val) {
      m_ck_a = m_ck_a + val;
      m_ck_b = m_ck_b + m_ck_a;
    }

    Checksum get_result() const {
      return {m_ck_a, m_ck_b};
    }

  protected:

    uint8_t m_ck_a;
    uint8_t m_ck_b;

  }; // struct ChecksumCalculator

  UBloxPacket() : m_payload(&m_buffer[6]), m_ublox_class(m_buffer[2]),
      m_id(m_buffer[3]), m_length(0) {

    size_t ii = 0;
    m_buffer[ii++] = 0xb5; // sync char 1
    m_buffer[ii++] = 0x62; // sync char 2

    for(; ii < sizeof(m_buffer); ii++) {
      m_buffer[ii] = 0;
    }
  }

  uint16_t get_length() const {

    return m_length;
  }

  void set_length(uint16_t length) {

    m_length = length;
    if(m_length > sizeof(m_buffer)-8) {
      assert(false);
    }

    apply_checksum_and_resync_buffer();
  }

  void set_ublox_class(uint8_t ublox_class) {

    m_ublox_class = ublox_class;

    apply_checksum_and_resync_buffer();
  }

  void set_id(uint8_t id) {

    m_id = id;

    apply_checksum_and_resync_buffer();
  }

  uint8_t get_payload_byte(uint16_t idx) const {

    if(idx >= m_length) {
      assert(false);
    }
    return m_payload[idx];
  }

  void set_payload_byte(uint16_t idx, uint8_t val) {

    if(idx >= m_length) {
      assert(false);
    }
    m_payload[idx] = val;

    apply_checksum_and_resync_buffer();
  }

  uint8_t get_buffer_byte(uint16_t idx) const {

    if(idx >= m_length+8) {
      assert(false);
    }
    return m_buffer[idx];
  }

  const uint8_t* get_pbuffer() const {
    return m_buffer;
  }

protected:

  uint8_t  m_buffer[128]; // make larger as needed
  uint8_t* m_payload;

  void apply_checksum_and_resync_buffer() {

    m_buffer[4] = (uint8_t)(m_length & 0x00ff);
    m_buffer[5] = (uint8_t)((m_length & 0xff00) >> 8);

    Checksum checksum = get_checksum_of_buffer();
    m_buffer[6+m_length] = checksum.m_ck_a;
    m_buffer[7+m_length] = checksum.m_ck_b;
  }

  Checksum get_checksum_of_buffer() const {

    static ChecksumCalculator calculator;

    calculator.init(); // because object is static
    calculator.add(m_ublox_class);
    calculator.add(m_id);

    calculator.add(m_buffer[4]);
    calculator.add(m_buffer[5]);

    for(uint16_t ii = 0; ii < m_length; ii++) {
      calculator.add(m_payload[ii]);
    }

    return calculator.get_result();
  }

private:

  uint8_t& m_ublox_class;
  uint8_t& m_id;

  /// @brief payload length
  uint16_t m_length;

}; // class UBloxPacket

#endif // __UBLOX_PACKET_H
