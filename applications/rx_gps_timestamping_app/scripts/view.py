#!/usr/bin/env python2
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of OpenCPI <http://www.opencpi.org>
#
# OpenCPI is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# OpenCPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.

import numpy as np
import math
import sys
from enum import Enum
import matplotlib.pyplot as plt
import time

pl_off_32w = 2 # file_write's payload offset in number of 32 words

class ComplexShortWithMetaDataProtOpcode(Enum):
    SAMPLES=0
    TIME=1
    INTERVAL=2
    FLUSH=3
    SYNC=4
    USER=5

class PortMessage:

    def __init__(self, opcode, payload=None):
        self.opcode = opcode
        self.payload = payload

    def __str__(self):
        ret = "opcode=" + str(self.opcode)
        if self.payload is not None:
            ret += ",payload="
            ret += "["
            ret += str(self.payload[0])
            for elem in self.payload[1:]:
                ret += ", "
                is_first = False
                ret += str(elem)
            ret += "]"
            #ret += str(self.payload)
        return ret

class ComplexShortWithMetaDataProtMessage(PortMessage):

    def __init__(self, opcode, payload=None):
        PortMessage.__init__(self, opcode, payload)

    def get_file_rw_payload_length_32w(self):
        """
        Get payload length in number of 32-bit words
        """
        ret = None
        if self.opcode == ComplexShortWithMetaDataProtOpcode.SAMPLES:
            ret = len(self.payload)
        elif self.opcode == ComplexShortWithMetaDataProtOpcode.TIME:
            ret = len(self.payload)*2
        #elif self.opcode == ComplexShortWithMetaDataProtOpcode.INTERVAL:
        #    ret = len(self.payload)*2
        return ret

class Q32p32:
    """
    Q32.32 fixed point number.
    """

    def __init__(self, int_32_val, fract_32_val):
        self.int_32_val = int_32_val
        self.fract_32_val = fract_32_val
        self.float_val = float(int_32_val)
        self.float_val += float(fract_32_val) / pow(2.,32.)

    def get_int_str(self):
        return str(self.int_32_val)

    def get_fract_str(self):

        ret = ""
        xx = self.fract_32_val
        yy = 0
        test_val = (1 << 31)

        #  0.50000000000000000000000000000000
        zz = 50000000000000000000000000000000 # 32 digits

        for ii in range(32):
            if(xx >= test_val):
                xx -= test_val
                yy += zz
            test_val >>= 1
            zz /= 2
        
        yy_str = str(yy)

        for ii in range(32-len(yy_str)):
            ret += "0"

        ret += yy_str

        return ret

    def __str__(self):
        """
        This functionality provided by this method is the main thing that makes
        this class necessary. This prints out every digit of the Q32.32
        value.
        """
        return self.get_int_str() + "." + self.get_fract_str()

    def __float__(self):
        return self.float_val

    def __add__(self, other):
        return self.float_val + other

    def __sub__(self, other):
        return self.float_val - other

def get_complex_short_with_metadata_port_message(file_32_buffer,
        en_print = False):

    ret = None

    pl_len_bytes = file_32_buffer[0]
    opcode = ComplexShortWithMetaDataProtOpcode(file_32_buffer[1])

    pl_len_32w = pl_len_bytes / 4 # payload length
    if pl_len_bytes > 0:

        payload = []
        if opcode == ComplexShortWithMetaDataProtOpcode.SAMPLES:
            for ii in range(int(math.floor(pl_len_32w))):

                # note that endianness has been verified
                re = np.int16(file_32_buffer[pl_off_32w + ii] & 0x0000ffff)
                im = np.int16((file_32_buffer[pl_off_32w + ii] & 0xffff0000) >> 16)

                payload.append(complex(re, im))
        elif opcode == ComplexShortWithMetaDataProtOpcode.TIME:
            is_lsw = True
            for ii in range(int(math.floor(pl_len_32w))):
                if(is_lsw):
                    tfract = file_32_buffer[pl_off_32w + ii]
                else:
                    tint = file_32_buffer[pl_off_32w + ii]
                    payload.append(Q32p32(tint, tfract))
                is_lsw = not is_lsw
        #elif opcode == ComplexShortWithMetaDataProtOpcode.INTERVAL:
        #    is_lsw = True
        #    delta_time = 0
        #    for ii in range(int(math.floor(pl_len_32w))):
        #        if(is_lsw):
        #            delta_time = file_32_buffer[pl_off_32w + ii]
        #        else:
        #            delta_time += (file_32_buffer[pl_off_32w + ii] << 32)
        #            payload.append(delta_time)
        #            time = 0
        #        is_lsw = not is_lsw
        else:
            payload = file_32_buffer[pl_off_32w:pl_len_32w+pl_off_32w]

        ret = ComplexShortWithMetaDataProtMessage(opcode, payload)

    else:
        ret = ComplexShortWithMetaDataProtMessage(opcode)

    if en_print:
        print(ret)

    return ret

def get_file_rw_complex_short_with_metadata_port_messages(file_32_buffer,
        max_num_messages=None):
    """
    Read file_read/file_write messages_in_file=true formatted file as uint32
    """
    ret = [] 
    idx = 0
    while idx < len(file_32_buffer):
        msg = get_complex_short_with_metadata_port_message(file_32_buffer[idx:])
        if msg.payload is not None:
            idx = idx + msg.get_file_rw_payload_length_32w() + pl_off_32w
        else:
            idx = idx + pl_off_32w
        ret.append(msg)
        if len(ret) == max_num_messages:
            break
    return ret

def get_file_32_buffer():
    ofile = open(sys.argv[1], 'rb')
    return np.fromfile(ofile, dtype=np.uint32, count=-1)

def plot_time_sample_message_pairs(messages, bw):
    """
    Perform time-domain plot of the first occurrence of time message which is
    immediately followed by a samples message. The x axis starts and is
    labelled at the timestamp from the TIME message.
    """

    time_message = None
    samples_message = None

    last_was_time = False
    pair_found = False

    for message in messages:
        if message.opcode == ComplexShortWithMetaDataProtOpcode.TIME:
            time_message = message
            last_was_time = True
        elif message.opcode == ComplexShortWithMetaDataProtOpcode.SAMPLES:
            samples_message = message
            pair_found = last_was_time
            last_was_time = False
        else:
            last_was_time = False

        if pair_found:
            pair_found = False

            fig=plt.figure(1)
            ax1=fig.add_subplot(2,1,1)
            ax1.set_title('Time Domain Plot')
            ax1.set_ylabel('Amplitude (LSBs)')

            ax1.plot(np.real(samples_message.payload), label='I')
            ax1.plot(np.imag(samples_message.payload), label='Q')
            ax1.set_xticks([0])
          
            ax1.set_xlabel('Time')
            unix_epoch_time = time_message.payload[0]
            epoch_str = str(unix_epoch_time)
            epoch_str += " (UNIX epoch)"

            lt = time.localtime(unix_epoch_time)
            utc_str = time.strftime('%Y-%m-%d %H:%M:%S.', lt)
            utc_str += unix_epoch_time.get_fract_str()
            utc_str += " (UTC)"

            print "plotting SAMPLES message whose first sample was"
            print "  received at time: ", utc_str

            time_str = utc_str + "\n" + epoch_str
            ax1.set_xticklabels([time_str])
            ax1.set_xlim(-20, len(samples_message.payload))
            ax1.grid()

            leg1=ax1.legend()

            # sample spacing
            T=1./bw

            #Generate FFT bins
            xf=np.fft.fftfreq(len(samples_message.payload),T)
            xf=np.fft.fftshift(xf)

            #Perform FFT
            yf=np.fft.fft(samples_message.payload)
            yplot=np.fft.fftshift(yf)
            yf_plot=1.0/len(samples_message.payload)*np.abs(yplot)

            #Create FFT plot
            ax2=fig.add_subplot(2,1,2)
            epsilon= pow(10,-10) #Error factor to avoid divide by zero in log10
            ax2.plot(xf,20*np.log10(yf_plot+epsilon))

            #Beautify plot
            ax2.set_title(str(len(samples_message.payload))+'-Point Complex FFT')
            ax2.set_xlabel('Frequency (Hz)')
            ax2.set_ylabel('Amplitude (dB)')
            ax2.set_ylim([-50,150])
            ax2.grid()

            #Show plot
            plt.show()

def usage():
    print('usage is: view.py <filew-file> <m or p > <max-num-msgs>')
    print('                  <filew-file>   file saved from file_write')
    print('                  <m or p>       m : print messages,')
    print('                                 p : plot')
    print('                  <bw>           data bandwidth to file in sps')
    print('                  <max-num-msgs> (optional) max number of messages to read from file')
    print('example:  view.py odata/rx_gps_timestamping_e3xx_app.out p 4')

def main():
    if len(sys.argv) == 4:
        max_num_messages = 2
    elif len(sys.argv) == 5:
        max_num_messages = int(sys.argv[4])
    else:
        print('Invalid arguments:  usage is: view.py <filerw-file> <m or p> <bw> <(optional) num-pairs>')
        sys.exit(1)
    bw = float(sys.argv[3])

    buf = get_file_32_buffer()
    messages = get_file_rw_complex_short_with_metadata_port_messages(buf,
        max_num_messages)

    if sys.argv[2] == 'm':
        for message in messages:
          print(message)
    elif sys.argv[2] == 'p':
        plot_time_sample_message_pairs(messages, bw)
    else:
        usage()

if __name__ == "__main__":
    main()
