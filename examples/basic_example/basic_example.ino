// This library needs IRLib2 installed (https://github.com/cyborg5/IRLib2)

// Connect the LED () to the pin 3 in the Arduino Uno

#include <PanavoxAC.h>

PanavoxAC ac;

void setup() {

  // Check available modes on ACMode.h
  ac.setMode(ACMode::Cool);
  ac.setTemperature(24);
  ac.setFanSpeed(3);

  // Send the configured settings to the AC
  ac.sendState();

  // Enable Super mode
  ac.sendSuperMode();
}

void loop() {
  // Nothing
}