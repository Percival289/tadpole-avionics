#include <Arduino.h>
#line 1 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
// GLOBAL VARIABLES

const float codeVersion = 0.01;

void setup() {

  // Start serial communications
  Serial.begin(38400);

  delay(500);

  Serial.println(F("--------------------------"));
  Serial.println(F("GOOD MORNING!! Tadpole V0"));
  Serial.println(F("--------------------------"));


  // Check for in-flight recovery (are we already flying and restart)


  // Initialize SD card storage


  // Load settings from SD into EEPROM if available


  // Read pin assignments from EEPROM
  Serial.println(F("Reading EEPROM..."));

  // read read read

  Serial.println(F("complete!"));

  // check for uninitialized EEPROM
  // check a value of the EEPROM and see if it's 255 or sum


  // Configure pinModes
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  // chip-select pins high
  digitalWrite(2, HIGH);

  // firing pins LOW for safety
  digitalWrite(3, LOW);
  // Set all pyros and systems to initial state (pyros low, valves sealed)


  // Init GPS eventually

  // Check if test mode button is being held on startup
  if(true){ // TODO: not default test mode later
    Serial.println(F("------------------------------------------------------------"));
    Serial.println(F("    Bench-Test Mode Confirmed"));
    Serial.println(F("    "));
    Serial.println(F("    "));
    Serial.println(F("    "));
    Serial.println(F("------------------------------------------------------------"));
  }


  // Initialize sensors
  Serial.println(F("Starting Sensors..."));
  // sensors.accel = beginAccel(); (bool)
  bool sensorsOK = false;

  #define LED_PIN 2
  // Check sensor status
  // check that all sensors.etc bools returned true
  if(!sensorsOK){Serial.println("[SENSOR FAILURE]");}
  while(!sensorsOK){
    for(uint8_t i = 0; i < 20; i++){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(80);
    }
    delay(1000);
  }


  // Init radio and telemetry

  // Save settings to EEPROM

  // Configure pyros

  // Config radio frequency

  // Initialize TVC servos

  // Run TVC servo test

  // Calibraition if needed

  // Start sensor sampling

  // Start tracking and recording

  // Start countdown (or do we want to put this in loop so it's another condition?)

}

void loop() {
  
  // Update clock

  // Read sensor data

  // Process sensor data

  // Calculate orientation and position state

  // Look for shutdown or other commands

  // Detect craft states (liftoff etc)

  // Trigger events

  // Valve adjustments

  // Update controls + TVC

  // Check system health

  // Send watchdog signal to secondary controller

  // Update telemetry

  // Write data to onboard SD card

  // Process radio or serial input

  // Check for landing or end mission conditions
}

