#include <Wire.h>
const int ledPin = 10;
int buttonState = 0;    // variable for storing the received pushbutton status

void setup() {
  pinMode(ledPin, OUTPUT);
  // initialize I2C communication as slave:
  Wire.begin(8);  // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event handler for receiving data
  Serial.begin(9600);
}

void loop() {
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED State: ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED State: OFF");
  }

  delay(50);  // a short delay to avoid rapid LED state changes
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (Wire.available()) {
    buttonState = Wire.read();  // read the incoming byte
    // print the received button state to the Serial Monitor
    Serial.print("Received Button State: ");
    Serial.println(buttonState);
  }
}