ifndef OCPI_CDK_DIR
OCPI_CDK_DIR=$(realpath ../../exports)
endif

ifeq ($(filter clean%,$(MAKECMDGOALS)),)
  include $(OCPI_CDK_DIR)/include/ocpisetup.mk
endif
DIR=target-$(OCPI_TARGET_DIR)
PROG=$(DIR)/$(APP)
OUT= > /dev/null

INCS = -I$(OCPI_INC_DIR) -I$(OCPI_CDK_DIR)/platforms/$(OCPI_TARGET_PLATFORM)/include/ -I../e3xx_rx.rcc/include/

$(DIR):
	mkdir -p $(DIR)

ifdef APP
$(PROG): $(APP).cxx | $(DIR)
	$(AT)echo Building $@...
	$(AT)$(CXX) $(OCPI_TARGET_CXXFLAGS) -g -Wall $(OCPI_EXPORT_DYNAMIC) -o $@ $(INCS) $^ $(OCPI_LD_FLAGS)
endif

clean::
	$(AT)rm -r -f lib target-* *.*~

