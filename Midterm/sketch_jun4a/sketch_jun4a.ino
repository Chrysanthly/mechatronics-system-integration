//uno master
#include <Wire.h>

const int buttonPin = 10;
const int potPin = A0;  // Potentiometer connected to analog pin A0
const int slaveAddress = 8;  // I2C address of the slave Arduino

int buttonState = 0;  // Variable for reading the pushbutton status
int potValue = 0;     // Variable for reading the potentiometer value

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Initialize I2C communication as master
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  potValue = analogRead(potPin);  // Read the potentiometer value


  // Send the state of the pushbutton and potentiometer value to the slave
  Wire.beginTransmission(slaveAddress);
  Wire.write(buttonState);        // Send the button state to the slave
  Wire.write(potValue >> 8);      // Send the high byte of pot value
  Wire.write(potValue & 0xFF);    // Send the low byte of pot value
  Wire.endTransmission();

  delay(100);  // A short delay to avoid overwhelming the I2C bus
}
