// GLOBAL VARIABLES
const float codeVersion = 0.1; // BUG: FreeRTOS is NOT happy about something...

/*
Current state of this code:

Built the start of a log system and some basic sensor logic and confirmation. Start of an events system
Nothing in here is flight-worthy or reviewed but it's all not awful, lots of fixes to be done.


*/


// FIXME is this needed?
#include "declarations.h"


void setup() {

  // Start serial communications
  Serial.begin(115200);

  timestamps.bootTime = millis();


  // ====================
  //   BENCH TEST MODE
  // ====================


  // ???
  // Keep from floating active low
  pinMode(pins.testRead, INPUT_PULLUP);
  delay(10);

  // Check if test mode button is being held on startup (or if testMode is enabled in settings)
  if(digitalRead(pins.testRead) == LOW || settings.testMode){

    while (!Serial && millis() < 1000) {}  // give the monitor time to connect

    // We are in test mode
    settings.testMode = true;

    Serial.println(F("\n\n"));
    Serial.println(F("========================================"));
    Serial.println(F("|  ___ ____ ___  ___  ____ _    ____   |"));
    Serial.println(F("|   |  |__| |  \\ |__] |  | |    |___   |"));
    Serial.println(F("|   |  |  | |__/ |    |__| |___ |___   |"));
    Serial.println(F("|             Version 0.0              |"));
    Serial.println(F("========================================"));

    // Define debug color
    const char* color = settings.useSerialColors ? ansiColors[L_DEBUG] : "";
    const char* reset = settings.useSerialColors ? ansiReset : "";

    Serial.printf("\n\n%s------------------------------------------\n", color);
    Serial.println("        Bench-Test Mode");
    Serial.println("        All flight functions disabled");
    Serial.printf("------------------------------------------%s\n", reset);

    logMessage(L_INFO, "STARTUP", "Signaling test mode...");
    // beep beep

  }



  // Check for in-flight recovery (are we already flying and restart)


  // TODO: Initialize SD card storage


  // TODO: Load settings from SD into EEPROM if available


  // Read pin assignments from EEPROM
  logMessage(L_INFO, "BOOT", "Reading EEPROM...");
  // TODO: read read read

  // check for uninitialized EEPROM
  // check a value of the EEPROM and see if it's 255 or sumthin
  logMessage(L_WARN, "BOOT", "EEPROM not available");



  // Configure pinModes
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  
  // chip-select pins high
  // firing pins LOW for safety
  // Set all pyros and systems to initial state (pyros low, valves sealed)

  // Init GPS eventually





  // ====================
  //    SENSORS INIT
  // ====================

  // Reset status
  sensors.allOK = true;

  // Initialize sensors
  logMessage(L_INFO, "SENSORS", "Initializing sensors...");

  
  // Start IMU1 and check status
  sensors.statusAccel = checkSensor("IMU1", beginMPU6050());
  sensors.allOK &= sensors.statusAccel;

  // Start other sensors



  // TODO: line too long

  // Summary of sensors on new line
  logMessage(sensors.allOK ? L_INFO : L_ERROR, "SENSORS", sensors.allOK ? "Sensors initialized" : "One or more sensors FAILED, see above");


  // ABORT if sensors not OK
  if (!sensors.allOK) events.abort = true;



  // Init radio and telemetry

  // Save new settings to EEPROM

  // Configure pyros

  // Config radio frequency

  // Initialize TVC servos

  // Run TVC servo test

  // Calibration if needed


  // Read last orientation values from EEPROM
  // settings.testMode: display orientation values


  // settings.testMode: override radio power and other values to low power


  // Start tracking and recording
  // open new file on SD card






  // ====================
  //   SENSORS SAMPLING
  // ====================

  // Start sensor sampling
  logMessage(L_INFO, "SENSORS", "Sampling sensors...");

  // sample for x amount of seconds and determine offset and initial values
  // TODO: per-sensor alerts if failed and &= into sensors.allOK


  // Verify status of all sensors again
  // REVIEW why did I do this twice
  logMessage(sensors.allOK ? L_PASS : L_FAIL, "SENSORS", sensors.allOK ? "OK" : "FAILED");


  

  // Read battery voltage + signal 


  // TANK PRESSURE???

  

  // ========================
  //   END OF BOOT SEQUENCE
  // ========================

  

  if(events.abort){
    logMessage(L_FATAL, "BOOT", "FLIGHT ABORTED"); 
  } else {
    logMessage(L_STATE, "BOOT", "SETUP COMPLETE");
  };

  timestamps.setupCompleteTime = millis();
  // show how long boot took



  // Start countdown (or do we want to put this in loop so it's another condition?)
}

void loop() {

  
  // Update clock


  // Read sensor data


  // Process sensor data
  // calc vehicle state


  // Look for shutdown or other commands


  // Detect craft states (liftoff etc)
  // !events.liftoff && accel.z > gTrigger && !events.touchdown && !events.timeOut


  // Trigger events


  // Valve adjustments


  // Update controls + TVC


  // Check system health + battery


  // Send watchdog signal to secondary controller


  // Check for timeouts


  // Update telemetry


  // Write data to onboard SD card


  // Process radio or serial input


  // Check for landing or end mission conditions
}


// Display log messages for each sensor.
// REVIEW: not sure if I like the status value being passed through yet another thing.
// tbf this will be logging to a file too. not just serial
bool checkSensor(const char* name, bool status) {

  // Log the sensor status
  logMessage(L_INFO, "SENSORS", name);
  logMessage(status ? L_PASS : L_FAIL, "SENSORS", status ? "OK" : "FAILED");
  return status;

}


// Display messages on serial monitor, eventually log to file
void logMessage(alertLevel lvl, const char* src, const char* msg) {

  // REVIEW: Make this function not slow things down when not in test mode
  // Right now it still makes a function call which is not a big issue but unecessary
  
  if (!settings.testMode) return;

  // minLevel allows the selection of errors or warnings only etc.
  if (lvl < settings.minLevel) return;

  if (settings.useSerialColors) Serial.print(ansiColors[lvl]); // Set color


  // REVIEW need to look at this bs again
  // Don't print pass/fail on new line
  switch (lvl) {
  case L_PASS:
  case L_FAIL:
    Serial.printf("[%s]", msg);
    if (settings.useSerialColors) Serial.print(ansiReset); // Reset colors
    return;

  default:
    break;
  }
  
  // Print log message on newline
  Serial.printf("\n[%6.3f][%s] %-25s", millis() / 1000.0, alertLevelNames[lvl], msg); // Show message
  if (settings.useSerialColors) Serial.print(ansiReset); // Reset colors

  // TODO: log to file
}