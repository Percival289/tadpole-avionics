void setup() {

  // Start serial communications
  Serial.begin(38400);
  while (! Serial);

  delay(500);
  Serial.println("\n\n");
  Serial.println(F("----------------------"));
  Serial.println(F("Tadpole V0 INITIALIZE"));
  Serial.println(F("----------------------"));

  // Check for in-flight recovery (are we already flying and restart)

  // Initialize SD card storage

  // Load settings into EEPROM

  // Read pin assignments

  // Read sensor configuration

  // Configure pins

  // Set all pyros and systems to initial state (pyros low, valves sealed)

  // Init GPS

  // Check for test mode

  // Initialize sensors

  // Check sensor status

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
