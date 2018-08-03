# This is the Makefile for worker e3xx_tx.rcc
RccIncludeDirs+=$(OCPI_PREREQUISITES_DIR)/ad9361/include/ # for ad9361.h
RccIncludeDirs+=./include
RccIncludeDirs+=./include/ad9361/
RccIncludeDirs+=./../e3xx_rx.rcc/include
RccIncludeDirs+=./../e3xx_rx.rcc/include/ad9361
RccOnlyPlatforms+=xilinx13_4
include $(OCPI_CDK_DIR)/include/worker.mk
