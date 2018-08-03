-- THIS FILE WAS ORIGINALLY GENERATED ON Sun Jan  7 14:15:38 2018 EST
-- BASED ON THE FILE: ad5662.xml
-- YOU *ARE* EXPECTED TO EDIT IT
-- This file initially contains the architecture skeleton for worker: ad5662

library IEEE; use IEEE.std_logic_1164.all; use ieee.numeric_std.all;
library ocpi; use ocpi.types.all; -- remove this to avoid all ocpi name collisions
library util; use util.util.all; use util.types.all;

architecture rtl of ad5662_worker is

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
  signal SPI_DI_s     : std_logic := '0';
  signal SPI_CLK_s    : std_logic := '0';
  signal SPI_ENB_s    : std_logic := '0';

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
      sclk    => SPI_CLK_s,
      sen     => SPI_ENB_s,
      sdio    => SPI_DI_s);

  signals : entity work.signals
    port map(
      w_SPI_DI     => SPI_DI_s,
      w_SPI_CLK    => SPI_CLK_s,
      w_SPI_ENB    => SPI_ENB_s,
      SPI_DI       => TUNE_DAC_SDIN,
      SPI_CLK      => TUNE_DAC_SCLK,
      SPI_ENB      => TUNE_DAC_SYNCn);

end rtl;
