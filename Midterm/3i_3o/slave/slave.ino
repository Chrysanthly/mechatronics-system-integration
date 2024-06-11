#include <Wire.h>
#include <Servo.h>

// Define the pins for the 7-segment display (assuming common anode)
const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8};
const int digitPin = 9; // Common pin for the 7-segment display
const int ledPin = 13;  // the number of the LED pin
const int servoPin = 10; // the number of the servo pin

int buttonState = 0;    // variable for storing the received pushbutton status
int lastButtonState = 0; // variable to store the last button state
int servoAngle = 0;     // variable for storing the received servo angle
int counter = 0;        // counter for pushbutton presses
bool rfidScanned = false; // flag to indicate if RFID has been scanned

Servo myServo;          // create a servo object

const byte digitCodes[10] = {
  0b11000000, // 0
  0b11111001, // 1
  0b10100100, // 2
  0b10110000, // 3
  0b10011001, // 4
  0b10010010, // 5
  0b10000010, // 6
  0b11111000, // 7
  0b10000000, // 8
  0b10010000  // 9
};

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the servo:
  myServo.attach(servoPin);
  // initialize I2C communication as slave:
  Wire.begin(8);  // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event handler for receiving data
  // initialize Serial communication:
  Serial.begin(9600);

  // set all segment pins as outputs
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(digitPin, OUTPUT);
  digitalWrite(digitPin, HIGH); // turn off the digit initially (common anode)
  
  // Display initial counter value
  displayDigit(counter);
}

void loop() {
  // Control the 7-segment display and servo motor only if RFID has been scanned
  if (rfidScanned) {
    // use the received buttonState to control the counter:
    if (buttonState == HIGH && lastButtonState == LOW) {
      counter++;
      if (counter > 9) counter = 0; // Reset counter if it exceeds 9
      displayDigit(counter); // Update the 7-segment display
      Serial.print("Counter incremented to: ");
      Serial.println(counter);
    }
    lastButtonState = buttonState; // Update last button state

    // use the received servoAngle to control the servo:
    myServo.write(servoAngle);
    Serial.print("Servo Angle: ");
    Serial.println(servoAngle);
  }

  delay(50);  // a short delay to avoid rapid state changes
}

void displayDigit(int digit) {
  // Turn off all segments
  turnOff();

  // condition for displaying segment a
  if (digit != 1 && digit != 4) digitalWrite(segmentPins[0], (digitCodes[digit] & B10000000) ? HIGH : LOW);
  // condition for displaying segment b
  if (digit != 5 && digit != 6) digitalWrite(segmentPins[1], (digitCodes[digit] & B01000000) ? HIGH : LOW);
  // condition for displaying segment c
  if (digit != 2) digitalWrite(segmentPins[2], (digitCodes[digit] & B00100000) ? HIGH : LOW);
  // condition for displaying segment d
  if (digit != 1 && digit != 4 && digit != 7) digitalWrite(segmentPins[3], (digitCodes[digit] & B00010000) ? HIGH : LOW);
  // condition for displaying segment e
  if (digit == 2 || digit == 6 || digit == 8 || digit == 0) digitalWrite(segmentPins[4], (digitCodes[digit] & B00001000) ? HIGH : LOW);
  // condition for displaying segment f
  if (digit != 1 && digit != 2 && digit != 3 && digit != 7) digitalWrite(segmentPins[5], (digitCodes[digit] & B00000100) ? HIGH : LOW);
  // condition for displaying segment g
  if (digit != 0 && digit != 1 && digit != 7) digitalWrite(segmentPins[6], (digitCodes[digit] & B00000010) ? HIGH : LOW);

  // Turn on the common pin for the digit
  digitalWrite(digitPin, LOW);
}

void turnOff() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], LOW);
  }
  digitalWrite(digitPin, HIGH); // Turn off the common pin for the digit
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  if (howMany >= 3) {
    buttonState = Wire.read(); // read the button state
    servoAngle = Wire.read();  // read the servo angle
    rfidScanned = Wire.read() == 1; // read the RFID scanned flag

    // Control the LED based on the RFID scanned flag
    digitalWrite(ledPin, rfidScanned ? HIGH : LOW);
    Serial.print("RFID Scanned: ");
    Serial.println(rfidScanned);
    Serial.print("Received Button State: ");
    Serial.print(buttonState);
    Serial.print(", Received Servo Angle: ");
    Serial.println(servoAngle);
  }
}