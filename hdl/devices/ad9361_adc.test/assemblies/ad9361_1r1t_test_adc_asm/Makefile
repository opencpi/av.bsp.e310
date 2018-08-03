Containers= \
        cnt_1rx_0tx_thruasm_mode_2_cmos_e3xx
#        cnt_1rx_0tx_thruasm_mode_5_cmos_e3xx

#        cnt_2rx_2tx_thruasm_mode_2_cmos_e3xx

#Containers+= \
        cnt_1rx_1tx_thruasm_mode_5_cmos_e3xx \
        cnt_1rx_2tx_thruasm_mode_5_cmos_e3xx \
        cnt_2rx_0tx_thruasm_mode_5_cmos_e3xx \
        cnt_2rx_1tx_thruasm_mode_5_cmos_e3xx \
        cnt_2rx_2tx_thruasm_mode_5_cmos_e3xx

export VivadoExtraOptions_route:= -directive NoTimingRelaxation
export VivadoIncrementalCompilation:=true

DefaultContainers=
ExcludePlatforms=isim modelsim xsim
include $(OCPI_CDK_DIR)/include/hdl/hdl-assembly.mk
