#pragma once
#include "types.h"

inline constexpr const char* LEVEL_NAMES[] = {
    "DEBUG", "INFO ", "PASS ", "WARN ", "FAIL ", "ERROR", "FATAL", "STATE"
};

inline constexpr const char* ANSI_COLORS[] = {
    "\033[90m",    // DEBUG - grey
    "\033[0m",     // INFO  - white/default
    "\033[32m",    // PASS  - green
    "\033[33m",    // WARN  - yellow
    "\033[31m",    // FAIL  - red
    "\033[1;31m",  // ERROR - red, bold
    "\033[97;41m", // FATAL - white on red
    "\033[36m"     // STATE - cyan
};

inline constexpr const char* ANSI_RESET = "\033[0m";

void logMessage(alertLevel level, const char* src, const char* msg);