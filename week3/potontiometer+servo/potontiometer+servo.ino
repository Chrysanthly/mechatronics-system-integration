#include <Servo.h>

Servo servoMotor;
int potPin = A0;
int potValue = 0;
int angle = 0;

void setup() {
    Serial.begin(9600);
    servoMotor.attach(9);
}

void loop() {
    // Read potentiometer value
    potValue = analogRead(potPin);
    // Map potentiometer value (0-1023) to servo angle (0-180)
    angle = map(potValue, 0, 1023, 0, 180);
    // Move servo to the mapped angle
    servoMotor.write(angle);

    // Check if any data is available from serial port
    if (Serial.available() > 0) {
        char receivedChar = Serial.read();  // Read the incoming byte
        if (receivedChar == 's') {
            // Stop servo motion if 's' is received
            servoMotor.write(90);  // Stop servo at the center position
        }
    }
    delay(50);
}