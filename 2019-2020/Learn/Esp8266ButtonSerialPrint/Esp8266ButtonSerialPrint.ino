int buttonPin = 4; // Momentary switch input pin, GPIO4 = D2 on the wemos d1 mini
int buttonState = 0; // Store the state of the button
void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting Input Tracking");
  delay(2000);
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read and affect the state of the GPIO linked to buttonPin
  Serial.println(buttonState); // Print the button state
  delay(10); // let a little delay to not overload the monitor
}
