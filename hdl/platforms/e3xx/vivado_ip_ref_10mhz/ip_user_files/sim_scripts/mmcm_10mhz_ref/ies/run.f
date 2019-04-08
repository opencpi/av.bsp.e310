-makelib ies/xil_defaultlib -sv \
  "/opt/Xilinx/Vivado/2017.1/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
-endlib
-makelib ies/xpm \
  "/opt/Xilinx/Vivado/2017.1/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../../mmcm_10mhz_ref/mmcm_10mhz_ref_clk_wiz.v" \
  "../../../../mmcm_10mhz_ref/mmcm_10mhz_ref.v" \
-endlib
-makelib ies/xil_defaultlib \
  glbl.v
-endlib

