-- THIS FILE WAS ORIGINALLY GENERATED ON Thu Feb  8 22:11:15 2018 EST
-- BASED ON THE FILE: ad5662_em.xml
-- YOU *ARE* EXPECTED TO EDIT IT
-- This file initially contains the architecture skeleton for worker: ad5662_em

library IEEE; use IEEE.std_logic_1164.all; use ieee.numeric_std.all;
library ocpi; use ocpi.types.all; -- remove this to avoid all ocpi name collisions
architecture rtl of ad5662_em_worker is
begin
  ctl_out.finished <= btrue; -- remove or change this line for worker to be finished when appropriate
                             -- workers that are never "finished" need not drive this signal
 -- comment to make it stop autogenerating
  VCTCXO_TO_MB <= '0';
end rtl;
