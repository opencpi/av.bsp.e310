#ifndef _AD9361_COMMON_H
#define _AD9361_COMMON_H

enum class AD9361_duplex_mode_t {FDD, TDD};
enum class AD9361_RX_port_t {RX1A, RX2A, RX1B, RX2B, RX1C, RX2C};
enum class AD9361_rx_rf_port_input_t {A_BALANCED, B_BALANCED, C_BALANCED, A_N, A_P, B_N, B_P, C_N, C_P};
enum class AD9361_RX_timing_diagram_channel_t {R1, R2};
enum class AD9361_TX_timing_diagram_channel_t {T1, T2};
typedef AD9361_RX_timing_diagram_channel_t AD9361_one_rx_one_tx_mode_use_rx_num_t;

#endif // _AD9361_COMMON_H
