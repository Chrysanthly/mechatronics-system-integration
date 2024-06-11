#include <Wire.h>
#include <Servo.h>

const int ledPin = 10;
int buttonState = 0;    // variable for storing the received pushbutton status
int servoAngle = 0;     // variable for storing the received servo angle
Servo myServo;

void setup() {
  pinMode(ledPin, OUTPUT);
  myServo.attach(13);
  // initialize I2C communication as slave:
  Wire.begin(8);  // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event handler for receiving data
  Serial.begin(9600);
}

void loop() {
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);  // turn LED on
    Serial.println("LED State: ON");
  } else {
    digitalWrite(ledPin, LOW);   // turn LED off
    Serial.println("LED State: OFF");
  }

  myServo.write(servoAngle);
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle);

//  if (Serial.available() > 0) {
//int angle = Serial.parseInt(); // Read angle data from serial port
//servoMotor.write(servoAngle); // Move servo to the specified angle
//}

  delay(50);  // a short delay to avoid rapid LED and servo state changes
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (Wire.available()) {
    buttonState = Wire.read();  // read the incoming byte
    if (Wire.available()) {
     servoAngle = Wire.read();  // read the next incoming byte
    }
    // print the received button state and servo angle to the Serial Monitor
    Serial.print("Received Button State: ");
    Serial.print(buttonState);
    Serial.print(", Received Servo Angle: ");
    Serial.println(servoAngle);
  }
}