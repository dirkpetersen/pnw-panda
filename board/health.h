#pragma once

// NOTE: python/__init__.py::_parse_c_struct() parses this struct line-by-line and rejects ANY
// line inside it that is not exactly `<type> <name>;` -- so no comments between the braces.
//
// madsheartbeat2pnw added `controls_allowed_lateral_pkt` at the END: is LATERAL tx currently
// permitted by safety, i.e. exactly the (controls_allowed || controls_allowed_lateral) expression
// every lateral tx gate in opendbc/safety/lateral.h evaluates. It lets openpilot detect a
// panda-side revoke it did not ask for (selfdrived's lateral mismatch counter). On a stock or
// non-MADS build controls_allowed_lateral is permanently false, so the field simply equals
// controls_allowed -- correct, not misleading.
//
// ADDING A FIELD HERE CHANGES A VERSIONED WIRE STRUCT. HEALTH_PACKET_VERSION is a sha256 over
// this whole file (python/constants.py::compute_version_hash, used by both python/__init__.py and
// board/SConscript), so the version bumps itself and a mismatched pair fails LOUDLY rather than
// mis-parsing: pypanda's @ensure_health_packet_version raises "health packet version mismatch ...
// Reflash panda", and pandad's Panda::get_state() flags a short read. Append at the END only.
//
// `mads_disengage_reason_pkt` is the DisengageReason (opendbc/safety/pnw/mads_declarations.h) that
// last took lateral authority down -- BRAKE / LAG / ACC_MAIN_OFF / OP_DISENGAGE /
// HEARTBEAT_ENGAGED_MISMATCH / STEERING_DISENGAGE. Without it, "the panda revoked lateral" is
// visible but its CAUSE is not, which is the difference between a diagnosable log and a shrug.
// Added in the same version bump as controls_allowed_lateral_pkt so it does not cost a second
// flash. Diagnostic only: nothing reads it for control.
struct __attribute__((packed)) health_t {
  uint32_t uptime_pkt;
  uint32_t voltage_pkt;
  uint32_t current_pkt;
  uint32_t safety_tx_blocked_pkt;
  uint32_t safety_rx_invalid_pkt;
  uint32_t tx_buffer_overflow_pkt;
  uint32_t rx_buffer_overflow_pkt;
  uint32_t faults_pkt;
  uint8_t ignition_line_pkt;
  uint8_t ignition_can_pkt;
  uint8_t controls_allowed_pkt;
  uint8_t car_harness_status_pkt;
  uint8_t safety_mode_pkt;
  uint16_t safety_param_pkt;
  uint8_t fault_status_pkt;
  uint8_t power_save_enabled_pkt;
  uint8_t heartbeat_lost_pkt;
  uint16_t alternative_experience_pkt;
  float interrupt_load_pkt;
  uint8_t fan_power;
  uint8_t safety_rx_checks_invalid_pkt;
  uint16_t spi_error_count_pkt;
  uint16_t sbu1_voltage_mV;
  uint16_t sbu2_voltage_mV;
  uint8_t som_reset_triggered;
  uint16_t sound_output_level_pkt;
  uint8_t controls_allowed_lateral_pkt;
  uint8_t mads_disengage_reason_pkt;
};

typedef struct __attribute__((packed)) {
  uint8_t bus_off;
  uint32_t bus_off_cnt;
  uint8_t error_warning;
  uint8_t error_passive;
  uint8_t last_error; // real time LEC value
  uint8_t last_stored_error; // last LEC positive error code stored
  uint8_t last_data_error; // DLEC (for CANFD only)
  uint8_t last_data_stored_error; // last DLEC positive error code stored (for CANFD only)
  uint8_t receive_error_cnt; // Actual state of the receive error counter, values between 0 and 127. FDCAN_ECR.REC
  uint8_t transmit_error_cnt; // Actual state of the transmit error counter, values between 0 and 255. FDCAN_ECR.TEC
  uint32_t total_error_cnt; // How many times any error interrupt was invoked
  uint32_t total_tx_lost_cnt; // Tx event FIFO element lost
  uint32_t total_rx_lost_cnt; // Rx FIFO 0 message lost due to FIFO full condition
  uint32_t total_tx_cnt;
  uint32_t total_rx_cnt;
  uint32_t total_fwd_cnt; // Messages forwarded from one bus to another
  uint32_t total_tx_checksum_error_cnt;
  uint16_t can_speed;
  uint16_t can_data_speed;
  uint8_t canfd_enabled;
  uint8_t brs_enabled;
  uint8_t canfd_non_iso;
  uint32_t irq0_call_rate;
  uint32_t irq1_call_rate;
  uint32_t irq2_call_rate;
  uint32_t can_core_reset_cnt;
} can_health_t;
