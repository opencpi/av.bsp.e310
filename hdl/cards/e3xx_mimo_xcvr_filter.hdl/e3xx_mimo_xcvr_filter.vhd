-- THIS FILE WAS ORIGINALLY GENERATED ON Mon Oct 16 19:41:19 2017 EDT
-- BASED ON THE FILE: e3xx_mimo_xcvr_filter.xml
-- YOU *ARE* EXPECTED TO EDIT IT
-- This file initially contains the architecture skeleton for worker: e3xx_mimo_xcvr_filter

library IEEE; use IEEE.std_logic_1164.all; use ieee.numeric_std.all;
library ocpi; use ocpi.types.all; -- remove this to avoid all ocpi name collisions
architecture rtl of e3xx_mimo_xcvr_filter_worker is
begin

  TX_ENABLE1A <= props_in.TX_ENABLE1A;
  TX_ENABLE2A <= props_in.TX_ENABLE2A;
  TX_ENABLE1B <= props_in.TX_ENABLE1B;
  TX_ENABLE2B <= props_in.TX_ENABLE2B;

  VCTXRX1_V1 <= props_in.VCTXRX1_V1;
  VCTXRX1_V2 <= props_in.VCTXRX1_V2;
  VCTXRX2_V1 <= props_in.VCTXRX2_V1;
  VCTXRX2_V2 <= props_in.VCTXRX2_V2;

  VCRX1_V1 <= props_in.VCRX1_V1;
  VCRX1_V2 <= props_in.VCRX1_V2;
  VCRX2_V1 <= props_in.VCRX2_V1;
  VCRX2_V2 <= props_in.VCRX2_V2;

  TX_BANDSEL(2 downto 0) <= from_uchar(props_in.TX_BANDSEL)(2 downto 0);

  RX1_BANDSEL(2 downto 0)  <= from_uchar(props_in.RX1_BANDSEL)(2 downto 0);
  RX1B_BANDSEL(1 downto 0) <= from_uchar(props_in.RX1B_BANDSEL)(1 downto 0);
  RX1C_BANDSEL(1 downto 0) <= from_uchar(props_in.RX1C_BANDSEL)(1 downto 0);

  RX2_BANDSEL(2 downto 0)  <= from_uchar(props_in.RX2_BANDSEL)(2 downto 0);
  RX2B_BANDSEL(1 downto 0) <= from_uchar(props_in.RX2B_BANDSEL)(1 downto 0);
  RX2C_BANDSEL(1 downto 0) <= from_uchar(props_in.RX2C_BANDSEL)(1 downto 0);

  LED_TXRX1_TX <= props_in.LED_TXRX1_TX;
  LED_TXRX1_RX <= props_in.LED_TXRX1_RX;
  LED_RX1_RX   <= props_in.LED_RX1_RX;

  LED_TXRX2_TX <= props_in.LED_TXRX2_TX;
  LED_TXRX2_RX <= props_in.LED_TXRX2_RX;
  LED_RX2_RX   <= props_in.LED_RX2_RX;

end rtl;
