/* This sketch simulate the working principle of the Esp8266 ScoreScreen. 
 * The idea is to display the score "85" when the autonomus robot come and push a button. (PullUp,GPIO4, D2 on the Wemos D1 mini)
 * If that occure, the sketch should display and serial print the score, "85".
 * If not, the sketch should let the wifi udp process unintented.
 */

int buttonPin = 4; // Momentary switch input pin, GPIO4 = D2 on the wemos d1 mini
int buttonState; // Store the actually trusted state of the button
int lastButtonState = LOW; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 250;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting Input Tracking");
  delay(2000);
}

void loop() {
  int reading = digitalRead(buttonPin); // Read and affect the none trusted state of the GPIO linked to buttonPin

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        Serial.println(buttonState); // Print the button state
        Serial.println("85");
      }
    }
  }
  lastButtonState = reading;
}
