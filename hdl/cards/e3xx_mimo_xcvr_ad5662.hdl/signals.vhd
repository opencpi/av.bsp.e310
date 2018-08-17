-- This file is protected by Copyright. Please refer to the COPYRIGHT file
-- distributed with this source distribution.
--
-- This file is part of OpenCPI <http://www.opencpi.org>
--
-- OpenCPI is free software: you can redistribute it and/or modify it under the
-- terms of the GNU Lesser General Public License as published by the Free
-- Software Foundation, either version 3 of the License, or (at your option) any
-- later version.
--
-- OpenCPI is distributed in the hope that it will be useful, but WITHOUT ANY
-- WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
-- A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
-- details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with this program. If not, see <http://www.gnu.org/licenses/>.

library IEEE; use IEEE.std_logic_1164.all, ieee.numeric_std.all;
library util; use util.util.all;
entity signals is
  port (w_TUNE_DAC_SDIN  : in  std_logic;
        w_TUNE_DAC_SCLK  : in  std_logic;
        w_TUNE_DAC_SYNCn : in  std_logic;
        w_VCTCXO_TO_MB   : out std_logic;
        TUNE_DAC_SDIN    : out std_logic;
        TUNE_DAC_SCLK    : out std_logic;
        TUNE_DAC_SYNCn   : out std_logic;
        VCTCXO_TO_MB     : in  std_logic);
end entity signals;
architecture rtl of signals is
begin

  -- device worker signal TUNE_DAC_SDIN (output)
  TUNE_DAC_SDIN_buffer : BUFFER_OUT_1
    generic map(DIFFERENTIAL => false)
    port map(I => w_TUNE_DAC_SDIN,
             O => TUNE_DAC_SDIN);

  -- device worker signal TUNE_DAC_SCLK (output)
  TUNE_DAC_SCLK_buffer : BUFFER_OUT_1
    generic map(DIFFERENTIAL => false)
    port map(I => w_TUNE_DAC_SCLK,
             O => TUNE_DAC_SCLK);

  -- device worker signal TUNE_DAC_SYNCn (output)
  TUNE_DAC_SYNCn_buffer : BUFFER_OUT_1
    generic map(DIFFERENTIAL => false)
    port map(I => w_TUNE_DAC_SYNCn,
             O => TUNE_DAC_SYNCn);

  -- device worker signal VCTCXO_TO_MB (input)
  VCTCXO_TO_MB_buffer : BUFFER_IN_1
    generic map(DIFFERENTIAL => false)
    port map(I => VCTCXO_TO_MB,
             O => w_VCTCXO_TO_MB);

end rtl;

