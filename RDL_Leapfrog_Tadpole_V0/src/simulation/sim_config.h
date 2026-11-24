#pragma once

#include <stdint.h>

// This checkout is configured for HIL testing by default. Set this to 0 for a
// normal flight/hardware build; it replaces physical sensor input with USB
// frames when enabled.
#ifndef TADPOLE_SIM_TEST
#define TADPOLE_SIM_TEST 1
#endif

constexpr float TADPOLE_SIM_VEHICLE_MASS_KG = 204.1166f; // 450 lb SYS-4 limit
constexpr float TADPOLE_SIM_MAX_THRUST_N = 2224.11f;     // 500 lbf engine target
constexpr float TADPOLE_SIM_MAX_GIMBAL_DEGREES = 10.0f;  // ENG-5 / TVC limit

// Increment when the HIL controller behavior or packet contract changes.
constexpr uint32_t TADPOLE_SIM_FIRMWARE_VERSION = 3u;
constexpr uint32_t TADPOLE_SIM_CAPABILITIES = 0x3u; // HIL + position-to-tilt controller
