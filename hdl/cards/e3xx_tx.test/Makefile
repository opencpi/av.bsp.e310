# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of OpenCPI <http://www.opencpi.org>
#
# OpenCPI is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# OpenCPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.

# This file is used to build the Application Control Interface api testbench,
# calls make on the underlying unit test hdl assemblies, and is used to test the
# respective component. This file handles hardware workers and has been tested on
# the zed platform.

$(if $(realpath $(OCPI_CDK_DIR)),,$(error The OCPI_CDK_DIR environment variable is not set correctly.))

APP=e3xx_tx_test_app
RccOnlyPlatforms+=centos6 centos7 xilinx13_3
include setup.mk

all: build

buildswonly: $(PROG)

ad9361_config_proxy:
	make -C ../../devices/ad9361_config_proxy.rcc

e3xx_tx:
	make -C ../e3xx_tx.rcc

e3xx_rx:
	make -C ../e3xx_rx.rcc

# custom make target which runs software-only tests (which run much faster than
# the full software + hardware test), obviously this is a subset of the full
# test which should be run for this worker (full test would be ./$(PROG) )
runswonly: buildswonly
	./$(PROG) swonly

# mimicking targets defined in OpenCPI_Component_Development.pdf Section 13.9

# "The make command is used with the following goals to invoke the phases:"
# this includes e3xx_rx (as well as the ..._1rx_... assembly) because of the rx_corruption tests
build: buildswonly ad9361_config_proxy e3xx_tx e3xx_rx
	make -C assemblies Containers='cnt_1rx_1tx_mode_2_bypassasm_e3xx_mimo_xcvr_CMOS_e3xx.xml'

run: verify

# clean target is defined in setup.mk

# "The following goals control the process at a finer granularity:"

generate:

prepare:

runonprepare: run

runonly: run

verify: build
	OCPI_LIBRARY_PATH=$(OCPI_LIBRARY_PATH):../../devices/lib/:../../cards/lib/:../e3xx_rx.rcc/:../e3xx_tx.rcc/:./assemblies/empty/ ./$(PROG)

view:

cleanrun:

cleansim:

