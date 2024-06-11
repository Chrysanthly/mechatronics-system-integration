#include <Wire.h>
const int buttonPin = 10;
int buttonState = 0;      // variable for reading the pushbutton status
const int slaveAddress = 8;  // I2C address of the slave Arduino

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize I2C communication as master:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  Serial.print("Button State: ");
  Serial.println(buttonState);

  // send the state of the pushbutton to the slave:
  Wire.beginTransmission(slaveAddress);
  Wire.write(buttonState);  // send the button state to the slave
  Wire.endTransmission();

  delay(100);  // a short delay to avoid overwhelming the I2C bus
}
