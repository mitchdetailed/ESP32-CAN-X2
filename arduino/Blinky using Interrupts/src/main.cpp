#include <Arduino.h>
#define LED1 2            // Define LED1 as 2
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
bool x10hz_trigger = false;

void digitalToggle(uint8_t pin) {
  // Read the current state of the pin
  int currentState = digitalRead(pin);
  // Write the opposite state to the pin
  digitalWrite(pin, !currentState);
}

// ISR Routine for Timer event...
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  x10hz_trigger = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  // Initialize the digital pin IO2 as an output.
  pinMode(LED1, OUTPUT);

  // Configure the timer
  timer = timerBegin(0, 80, true); // Timer 0, prescaler 80, count up
  timerAttachInterrupt(timer, &onTimer, true); // Attach the interrupt to the ISR
  timerAlarmWrite(timer, 100000, true); // Set the timer to trigger every {n} microseconds(100,000 microseconds in this example)
  timerAlarmEnable(timer); // Enable the timer
}

void loop() {
  if (x10hz_trigger == true){
    digitalToggle(2); // Toggle the LED
    x10hz_trigger = false;
  }
}