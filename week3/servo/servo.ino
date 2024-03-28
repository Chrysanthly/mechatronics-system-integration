#include <Servo.h>

Servo servoMotor;

void setup() {
    Serial.begin(9600);
    servoMotor.attach(9);
}

void loop() {
    if (Serial.available() > 0) {
        int angle = Serial.parseInt(); // Read angle data from serial port
        servoMotor.write(angle); // Move servo to the specified angle
    }
}