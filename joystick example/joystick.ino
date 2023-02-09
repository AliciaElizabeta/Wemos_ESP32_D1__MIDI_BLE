

// Arduino pin numbers
const int SW_pin = 25; // IO17
const int X_pin = 36;  // IO36
// const int Y_pin = 34;  // IO34

void setup() {
  pinMode(SW_pin, INPUT);
  // digitalWrite(SW_pin, HIGH);
  Serial.begin(250000);


}

void loop() {
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));

  Serial.print(" ");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));

  Serial.print(" ");
    // Serial.print("Y-axis: ");
  // Serial.print(analogRead(Y_pin));
  delay(100);
  Serial.print("\n");
}

