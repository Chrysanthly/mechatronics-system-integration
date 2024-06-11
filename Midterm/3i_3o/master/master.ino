#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

const int buttonPin = 2;  // the number of the pushbutton pin
const int potPin = A0;    // the number of the potentiometer pin
const int RST_PIN = 9;    // RST pin for MFRC522
const int SS_PIN = 10;    // SDA pin for MFRC522

int buttonState = 0;      // variable for reading the pushbutton status
int lastButtonState = 0;  // variable to store the last button state
int potValue = 0;         // variable for reading the potentiometer value
bool rfidScanned = false; // flag to indicate if RFID has been scanned
const int slaveAddress = 8;  // I2C address of the slave Arduino

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  // initialize I2C communication as master:
  Wire.begin();
  // initialize Serial communication:
  Serial.begin(9600);
  // initialize SPI bus and MFRC522 sensor:
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // read the potentiometer value:
  potValue = analogRead(potPin);
  // map the potentiometer value to the servo range (0-180):
  int servoAngle = map(potValue, 0, 1023, 0, 180);

  // Check for new RFID card
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    rfidScanned = true;
    Serial.println("RFID Scanned");
  }

  // Send data to slave only if RFID has been scanned
  if (rfidScanned) {
    // Send button state, servo angle, and a flag for RFID scan to slave
    Wire.beginTransmission(slaveAddress);
    Wire.write(buttonState);
    Wire.write(servoAngle);
    Wire.write(rfidScanned ? 1 : 0); // send 1 if RFID is scanned, 0 otherwise
    Wire.endTransmission();
  }

  // Delay a bit to avoid bouncing and overwhelming the I2C bus
  delay(100);
}