This is the source distribution of the OpenCPI E3xx (E310/E312/E313) Board
Support Package (BSP) project.

Getting Started:
---
See https://opencpi.github.io/bsp_e310/Ettus_E3xx_Getting_Started_Guide.pdf

Notes:
---
  - hdl/assemblies
      Contains copies of assemblies from the OpenCPI assets project, with the
      addition of E3xx-specific container XML files. A desirable framework
      improvement would be to mitigate the need to copy assemblies from external
      projects in order to use containers made possible internally.

  - applications/FSK,
    applications/rx_app,
    applications/ad9361_adc_test,
    applications/ad9361_dac_test,
    applications/tx_event_test
      Contains copies of ACI and XML applications from the OpenCPI assets
      project, with ACI modifications and new application XML files for E3xx
      support. A desirable improvement to the aforementioned ACI applications
      would be to make their ACI implementations completely
      platform/BSP-agonstic, which would mitigate the need to copy ACI among
      platform/BSP-specifc projects. Ongoing work is being done to make this
      possible.
  - applications/e3xx_mimo_xcvr_ad5662_test,
    applications/e3xx_mimo_xcvr_filter_proxy_test,
    applications/e3xx_rx_test,
    applications/e3xx_tx_test,
      Applications which perform tests for E3xx-specific workers.
