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

-- THIS FILE WAS ORIGINALLY GENERATED ON Sun Jan  7 14:15:38 2018 EST
-- BASED ON THE FILE: e3xx_mimo_xcvr_ad5662.xml
-- YOU *ARE* EXPECTED TO EDIT IT
-- This file initially contains the architecture skeleton for worker: e3xx_mimo_xcvr_ad5662

-- TODO/FIXME - This is an implementation that REQUIRES the only item on the
-- SPI bus being the AD5662; that aspect needs to be refactored for generic
-- non-E3XX usage.
library IEEE; use IEEE.std_logic_1164.all; use ieee.numeric_std.all;
library ocpi; use ocpi.types.all; -- remove this to avoid all ocpi name collisions
library util; use util.util.all; use util.types.all;

architecture rtl of e3xx_mimo_xcvr_ad5662_worker is

  -- Internal signals
  constant data_width_c  : natural := 18;
  signal   wdata         : std_logic_vector(data_width_c-1 downto 0) := (others => '0');
  signal   rdata         : std_logic_vector(data_width_c-1 downto 0) := (others => '0');
  signal   reset         : std_logic := '0';
  constant addr_width_c  : natural := 5;
  signal   addr          : unsigned(addr_width_c-1 downto 0) := (others => '0');
  signal   myReset_r     : std_logic := '0'; -- stable output based on all clients
  signal   done          : bool_t := bfalse;
  signal   renable       : bool_t := bfalse;

  -- signals interface
  signal TUNE_DAC_SDIN_s  : std_logic := '0';
  signal TUNE_DAC_SCLK_s  : std_logic := '0';
  signal TUNE_DAC_SYNCn_s : std_logic := '0';

begin

  wdata <= props_in.raw.data(data_width_c-1 downto 0);
  reset <= ctl_in.reset;
  props_out.raw.error <= '0';
  props_out.raw.data <= (others => '0');
  props_out.raw.done <= done;

  -- Drive SPI from raw props
  -- Write a word of the following format to control the VCO
  --    [xxxx] | [mode] | [data ]
  --    -------+--------+---------
  --    [5..0] | [1..0] | [15..0]
  -- Where:
  --    xxxx = don't care
  --    mode = [00] normal operation
  --           [01] 1k resistor to ground   \
  --           [10] 100k resistor to ground  > Power Down Modes
  --           [11] three state             /
  --    data = D in the calculation [Vout = Vref*(D/65536)]
  -- -The don't care bits are mapped to the addr field of the
  --  spi component
  -- -wdata is calculated from the raw property voltageScale from
  --  the raw properties
  spi : util.util.spi
    generic map(
      data_width    => data_width_c,
      addr_width    => addr_width_c,
      clock_divisor => 8,
      capture_fall  => true)
    port map(
      clk     => ctl_in.clk,
      reset   => reset,
      renable => renable,
      wenable => props_in.raw.is_write,
      addr    => addr,
      wdata   => wdata,
      rdata   => rdata,
      done    => done,
      sdo     => '0',
      sclk    => TUNE_DAC_SCLK_s,
      sen     => TUNE_DAC_SYNCn_s,
      sdio    => TUNE_DAC_SDIN_s);

  signals : entity work.signals
    port map(
      w_TUNE_DAC_SDIN  => TUNE_DAC_SDIN_s,
      w_TUNE_DAC_SCLK  => TUNE_DAC_SCLK_s,
      w_TUNE_DAC_SYNCn => TUNE_DAC_SYNCn_s,
      w_VCTCXO_TO_MB   => open,
      TUNE_DAC_SDIN    => TUNE_DAC_SDIN,
      TUNE_DAC_SCLK    => TUNE_DAC_SCLK,
      TUNE_DAC_SYNCn   => TUNE_DAC_SYNCn,
      VCTCXO_TO_MB     => VCTCXO_TO_MB);

end rtl;
