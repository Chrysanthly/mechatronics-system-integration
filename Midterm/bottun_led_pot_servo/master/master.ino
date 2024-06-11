#include <Wire.h>

const int buttonPin = 10;
const int potPin = A0;
int buttonState = 0;      // variable for reading the pushbutton status
int potValue = 0;         // variable for reading the potentiometer value
const int slaveAddress = 8;  // I2C address of the slave Arduino

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize I2C communication as master:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  potValue = analogRead(potPin);
  int servoAngle = map(potValue, 0, 1023, 0, 180);

  // print the button state and potentiometer value to the Serial Monitor
  Serial.print("Button State: ");
  Serial.print(buttonState);
  Serial.print(", Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(", Servo Angle: ");
  Serial.println(servoAngle);

  // send the button state and servo angle to the slave:
  Wire.beginTransmission(slaveAddress);
  Wire.write(buttonState);  // send the button state
  Wire.write(servoAngle);   // send the servo angle
  Wire.endTransmission();

  delay(100);  // a short delay to avoid overwhelming the I2C bus
}