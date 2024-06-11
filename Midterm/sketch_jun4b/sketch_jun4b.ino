//mega slave
#include <Wire.h>
#include <Servo.h>

const int ledPin = 10;
Servo myServo;        // Create a servo object
int buttonState = 0;  // Variable for storing the received pushbutton status
int potValue = 0;     // Variable for storing the received potentiometer value

void setup() {
  pinMode(ledPin, OUTPUT);
  
  // Initialize I2C communication as slave
  Wire.begin(8);  // Join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // Register event handler for receiving data
  
  myServo.attach(9);
  Serial.begin(9600);
}

void loop() {
    if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Control the servo motor based on potentiometer value
  int servoPos = map(potValue, 0, 1023, 0, 180);
  myServo.write(servoPos);

  // Serial output
  Serial.print("Pot Value: ");
  Serial.print(potValue);
  Serial.print(" Servo Position: ");
  Serial.println(servoPos);

  delay(50);  // A short delay to avoid rapid LED state changes
}

// Function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (howMany >= 3) {
    buttonState = Wire.read(); // Read the button state
    int highByte = Wire.read(); // Read the high byte of pot value
    int lowByte = Wire.read();  // Read the low byte of pot value
    potValue = (highByte << 8) | lowByte; // Combine high and low bytes

    // Print the received button state and potentiometer value to the Serial Monitor
    Serial.print("Received Button State: ");
    Serial.print(buttonState);
    Serial.print(" Pot Value: ");
    Serial.println(potValue);
  }
}
