This is the source distribution of the OpenCPI E310 BSP project.

Getting Started:
---
See pdf for Ettus_E3XX_Getting_Started_Guide

Notes:
---
  hdl/assemblies
    Contains copies of assemblies from the OpenCPI assets project, with the
    addition of E310-specific container XML files. A desirable framework
    improvement would be to mitigate the need to copy assemblies from external
    projects in order to use containers made possible internally.

  applications/FSK,
  applications/rx_app,
  hdl/devices/ad9361_adc.test,
  hdl/devices/ad9361_dac.test
    Contains copies of ACI applications from the OpenCPI assets project, with
    ACI modifications and new application XML files for E310 support. A
    desirable improvement to the aforementioned ACI applications would be to
    make their ACI implementations completely platform/BSP-agonstic, which would
    mitigate the need to copy ACI among platform/BSP-specifc projects. Ongoing
    work is being done to make this possible.
