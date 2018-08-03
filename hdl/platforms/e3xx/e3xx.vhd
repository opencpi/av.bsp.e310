-- THIS FILE WAS ORIGINALLY GENERATED ON Fri Sep 29 20:11:03 2017 EDT
-- BASED ON THE FILE: e3xx.xml
-- YOU *ARE* EXPECTED TO EDIT IT
-- This file initially contains the architecture skeleton for worker: e3xx

library IEEE; use IEEE.std_logic_1164.all; use ieee.numeric_std.all; use ieee.std_logic_misc.all;
library ocpi; use ocpi.types.all; -- remove this to avoid all ocpi name collisions
library platform; use platform.platform_pkg.all;
library zynq; use zynq.zynq_pkg.all;
library axi; use axi.axi_pkg.all;
library unisim; use unisim.vcomponents.all;
library bsv;
library sdp; use sdp.sdp.all, sdp.sdp_axi.all;
architecture rtl of e3xx_worker is
  constant whichGP : natural := to_integer(unsigned(from_bool(useGP1)));
  signal ps_m_axi_gp_in  : m_axi_gp_in_array_t(0 to C_M_AXI_GP_COUNT-1);  -- s2m
  signal ps_m_axi_gp_out : m_axi_gp_out_array_t(0 to C_M_AXI_GP_COUNT-1); -- m2s
  signal ps_s_axi_gp_in  : s_axi_gp_in_array_t(0 to C_S_AXI_GP_COUNT-1);  -- m2s
  signal ps_s_axi_gp_out : s_axi_gp_out_array_t(0 to C_S_AXI_GP_COUNT-1); -- s2m
  signal ps_axi_hp_in  : s_axi_hp_in_array_t(0 to C_S_AXI_HP_COUNT-1);    -- m2s
  signal ps_axi_hp_out : s_axi_hp_out_array_t(0 to C_S_AXI_HP_COUNT-1);   -- s2m
  signal fclk             : std_logic_vector(3 downto 0);
  signal clk              : std_logic;
  signal raw_rst_n        : std_logic; -- FCLKRESET_Ns need synchronization
  signal rst_n            : std_logic; -- the synchronized negative reset
  signal reset            : std_logic; -- our positive reset
  signal count            : unsigned(25 downto 0);
  signal my_zynq_out      : zynq_out_array_t;
  signal my_zynq_out_data : zynq_out_data_array_t;
  signal dbg_state        : ulonglong_array_t(0 to 3);
  signal dbg_state1       : ulonglong_array_t(0 to 3);
  signal dbg_state2       : ulonglong_array_t(0 to 3);
  signal dbg_state_r      : ulonglong_array_t(0 to 3);
  signal dbg_state1_r     : ulonglong_array_t(0 to 3);
  signal dbg_state2_r     : ulonglong_array_t(0 to 3);

begin
  props_out.onswitch_db_p <= ONSWITCH_DB;
  
  -- Connect the time server to clock, reset, and a PPS in
  -- for time-keeping
  timebase_out.clk   <= clk;
  timebase_out.reset <= reset;
  timebase_out.ppsIn <= PPS_EXT_IN;

  clkbuf   : BUFG   port map(I => fclk(0),
                             O => clk);
  -- The FCLKRESET signals from the PS are documented as asynchronous with the
  -- associated FCLK for whatever reason.  Here we make a synchronized reset from it.
  sr : bsv.bsv.SyncResetA
    generic map(RSTDELAY => 17)

    port map(IN_RST  => raw_rst_n,
             CLK     => clk,
             OUT_RST => rst_n);
  reset <= not rst_n;

  -- Instantiate the processor system (i.e. the interface to it).
  -- This gives us pins for AXI for both the control and data planes
  -- It also provides us with a clock (fclk) which we use to drive
  -- the control and data plane clocks.
  ps : zynq_ps
    port map(
      -- Signals from the PS used in the PL
      ps_in.debug              => (31 => useGP1,
                                   others => '0'),
      ps_out.FCLK                  => fclk,
      ps_out.FCLKRESET_N           => raw_rst_n,
      m_axi_gp_in                  => ps_m_axi_gp_in,
      m_axi_gp_out                 => ps_m_axi_gp_out,
      s_axi_gp_in                  => ps_s_axi_gp_in,
      s_axi_gp_out                 => ps_s_axi_gp_out,
      s_axi_hp_in                  => ps_axi_hp_in,
      s_axi_hp_out                 => ps_axi_hp_out
      );
  -- Adapt the axi master from the PS to be a CP Master
  cp : axi2cp
    port map(
      clk     => clk,
      reset   => reset,
      axi_in  => ps_m_axi_gp_out(whichGP),
      axi_out => ps_m_axi_gp_in(whichGP),
      cp_in   => cp_in,
      cp_out  => cp_out
      );
  zynq_out               <= my_zynq_out;
  zynq_out_data          <= my_zynq_out_data;
  props_out.sdpDropCount <= zynq_in(0).dropCount;
  props_out.debug_state  <= dbg_state_r;
  props_out.debug_state1 <= dbg_state1_r;
  props_out.debug_state2 <= dbg_state2_r;
  -- Adapt the data plane to AXI and connect to AXI HP
  -- Do this for 4 AXI HP ports
  g : for i in 0 to 3 generate
    dp : sdp2axi
      generic map(ocpi_debug => true,
                  sdp_width  => to_integer(sdp_width),
                  axi_width  => ps_axi_hp_in(0).W.DATA'length/dword_size)
      port map(   clk          => clk,
                  reset        => reset,
                  sdp_in       => zynq_in(i),
                  sdp_in_data  => zynq_in_data(i),
                  sdp_out      => my_zynq_out(i),
                  sdp_out_data => my_zynq_out_data(i),
                  axi_in       => ps_axi_hp_out(i),
                  axi_out      => ps_axi_hp_in(i),
                  axi_error    => props_out.axi_error(i),
                  dbg_state    => dbg_state(i),
                  dbg_state1   => dbg_state1(i),
                  dbg_state2   => dbg_state2(i));
  end generate;
-- Output/readable properties
  props_out.dna             <= (others => '0');
  props_out.nSwitches       <= (others => '0');
  props_out.switches        <= (others => '0');
  props_out.memories_length <= to_ulong(1);
  props_out.memories        <= (others => to_ulong(0));
  props_out.nLEDs           <= to_ulong(0); --led'length);
  props_out.UUID            <= metadata_in.UUID;
  props_out.romData         <= metadata_in.romData;

  -- e3xx_mimo_xcvr card is always present
  props_out.slotCardIsPresent <= (0 => To_bool('0'), -- active low, this coincides with index 0 of slotName property
                                  others => '0');

  -- Drive metadata interface
  metadata_out.clk          <= clk;
  metadata_out.romAddr      <= props_in.romAddr;
  metadata_out.romEn        <= props_in.romData_read;

  -- Keep debug states up to date
  work : process(clk)
  begin
    if rising_edge(clk) then
      if reset = '0' then
        dbg_state_r <= dbg_state;
        dbg_state1_r <= dbg_state1;
        dbg_state2_r <= dbg_state2;
      end if;
    end if;
  end process;
end rtl;
