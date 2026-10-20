#include <Arduino.h>
#line 1 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
// GLOBAL VARIABLES

const float codeVersion = 0.01;

#line 5 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
void setup();
#line 147 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
void loop();
#line 5 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
void setup() {

  // Start serial communications
  Serial.begin(38400);

  delay(500);

  Serial.println(F("\n\n"));
  Serial.println(F("========================================"));
  Serial.println(F("|  ___ ____ ___  ___  ____ _    ____   |"));
  Serial.println(F("|   |  |__| |  \\ |__] |  | |    |___   |"));
  Serial.println(F("|   |  |  | |__/ |    |__| |___ |___   |"));
  Serial.println(F("|            Version 0.0.1             |"));
  Serial.println(F("========================================"));


  // Check for in-flight recovery (are we already flying and restart)


  // Initialize SD card storage


  // Load settings from SD into EEPROM if available


  // Read pin assignments from EEPROM
  Serial.print(F("\nReading EEPROM...        "));

  // read read read

  Serial.print(F("[complete]"));

  // check for uninitialized EEPROM
  // check a value of the EEPROM and see if it's 255 or sum


  // Configure pinModes
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  // chip-select pins high
  digitalWrite(3, HIGH);

  // firing pins LOW for safety
  digitalWrite(4, LOW);
  // Set all pyros and systems to initial state (pyros low, valves sealed)


  // Init GPS eventually

  // Check if test mode button is being held on startup
  bool testMode = true;

  if(testMode){ // TODO: not default test mode later
    Serial.println(F("\n\n------------------------------------------"));
    //Serial.println(F("        "));
    Serial.println(F("        Bench-Test Mode Confirmed"));
    Serial.println(F("        - [options]"));
    //Serial.println(F("        "));
    Serial.println(F("------------------------------------------"));
  }


  // Initialize sensors
  Serial.print(F("\nStarting sensors...      "));
  // sensors.accel = beginAccel(); (bool)
  bool sensorsOK = false;

  #define LED_PIN 2
  // Check sensor status
  // check that all sensors.etc bools returned true
  if(!sensorsOK){Serial.print("[SENSOR FAILURE] - will not continue with flight");}
  
  while(!sensorsOK && false){ // BUG: remove the false here
    for(uint8_t i = 0; i < 20; i++){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(80);
    }
    delay(1000);
  }


  // Init radio and telemetry

  // Save new settings to EEPROM

  // Configure pyros

  // Config radio frequency

  // Initialize TVC servos

  // Run TVC servo test

  if (testMode){
    Serial.print(F("\nSignaling test mode...   "));

    // Beep for test mode

    Serial.print(F("[OK]"));
  }


  // Calibration if needed


  // Read last orientation values from EEPROM
  // testMode: display orientation values


  // testMode: override radio power and other values


  // Start tracking and recording
  // open new file on SD card


  // Start sensor sampling
  if(testMode){Serial.print(F("\nSampling sensors...      "));}
  // sample for x amount of seconds and determine offset and initial values


  // if sensors are ok
  if(sensorsOK){Serial.print("[OK]\n");} else {Serial.print("[FAILED]");} // TODO: alert
  

  // Read battery voltage + signal 


  // TANK PRESSURE???


  if(testMode){
    Serial.println(F("\n\n[SETUP COMPLETE]\n"));
  }

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

