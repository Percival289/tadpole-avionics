#include "logger.h"
#include "state.h"

// Display messages on serial monitor, eventually log to file
void logMessage(alertLevel lvl, const char* src, const char* msg) {

  // TODO: should not just return, eventually should skip to log to disk and then skip the serial
  if (!settings.testMode) return;

  // minLevel allows the selection of errors or warnings only etc.
  if (lvl < settings.minLevel) return;

  if (settings.useSerialColors) Serial.print(ANSI_COLORS[lvl]); // Set color


  // REVIEW need to look at this bs again
  // Don't print pass/fail on new line
  switch (lvl) {
  case L_PASS:
  case L_FAIL:
    Serial.printf("[%s]", msg);
    if (settings.useSerialColors) Serial.print(ANSI_RESET); // Reset colors
    return;

  default:
    break;
  }
  
  // Print log message on newline
  Serial.printf("\n[%6.3f][%s] %-25s", millis() / 1000.0, LEVEL_NAMES[lvl], msg); // Show message
  if (settings.useSerialColors) Serial.print(ANSI_RESET); // Reset colors

  // TODO: log to file
  
}