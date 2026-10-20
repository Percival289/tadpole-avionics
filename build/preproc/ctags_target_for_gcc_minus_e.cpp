# 1 "C:\\Users\\crisl\\Documents\\GitHub\\tadpole-avionics\\RDL_Leapfrog_Tadpole_V0\\RDL_Leapfrog_Tadpole_V0.ino"
// GLOBAL VARIABLES

const float codeVersion = 0.01;

void setup() {

  // Start serial communications
  Serial0.begin(38400);

  delay(500);

  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("\n\n")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("========================================")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("|  ___ ____ ___  ___  ____ _    ____   |")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("|   |  |__| |  \\ |__] |  | |    |___   |")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("|   |  |  | |__/ |    |__| |___ |___   |")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("|            Version 0.0.1             |")))));
  Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("========================================")))));


  // Check for in-flight recovery (are we already flying and restart)


  // Initialize SD card storage


  // Load settings from SD into EEPROM if available


  // Read pin assignments from EEPROM
  Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("\nReading EEPROM...        ")))));

  // read read read

  Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("[complete]")))));

  // check for uninitialized EEPROM
  // check a value of the EEPROM and see if it's 255 or sum


  // Configure pinModes
  pinMode(3, 0x03);
  pinMode(4, 0x03);

  // chip-select pins high
  digitalWrite(3, 0x1);

  // firing pins LOW for safety
  digitalWrite(4, 0x0);
  // Set all pyros and systems to initial state (pyros low, valves sealed)


  // Init GPS eventually

  // Check if test mode button is being held on startup
  bool testMode = true;

  if(testMode){ // TODO: not default test mode later
    Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("\n\n------------------------------------------")))));
    //Serial.println(F("        "));
    Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("        Bench-Test Mode Confirmed")))));
    Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("        - [options]")))));
    //Serial.println(F("        "));
    Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("------------------------------------------")))));
  }


  // Initialize sensors
  Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("\nStarting sensors...      ")))));
  // sensors.accel = beginAccel(); (bool)
  bool sensorsOK = false;


  // Check sensor status
  // check that all sensors.etc bools returned true
  if(!sensorsOK){Serial0.print("[SENSOR FAILURE] - will not continue with flight");}

  while(!sensorsOK && false){ // BUG: remove the false here
    for(uint8_t i = 0; i < 20; i++){
      digitalWrite(LED_BUILTIN /* allow testing #ifdef LED_BUILTIN*/, 0x1);
      delay(100);
      digitalWrite(LED_BUILTIN /* allow testing #ifdef LED_BUILTIN*/, 0x0);
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
    Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("\nSignaling test mode...   ")))));

    // Beep for test mode

    Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("[OK]")))));
  }


  // Calibration if needed


  // Read last orientation values from EEPROM
  // testMode: display orientation values


  // testMode: override radio power and other values


  // Start tracking and recording
  // open new file on SD card


  // Start sensor sampling
  if(testMode){Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("\nSampling sensors...      ")))));}
  // sample for x amount of seconds and determine offset and initial values


  // if sensors are ok
  if(sensorsOK){Serial0.print("[OK]\n");} else {Serial0.print("[FAILED]");} // TODO: alert


  // Read battery voltage + signal 


  // TANK PRESSURE???


  if(testMode){
    Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("\n\n[SETUP COMPLETE]\n")))));
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
