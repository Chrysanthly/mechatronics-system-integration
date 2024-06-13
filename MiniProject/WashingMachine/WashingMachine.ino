#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
MFRC522 mfrc522(10, 9);   // Create MFRC522 instance
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int button = 5;
#define RED_PIN 13
#define GREEN_PIN 12
#define BLUE_PIN 11
#define CLK 19
#define DT 18
#define SW 6
const int tapPin = A0;
const int buzzerPin = 22;
const int trigPin = 3;
const int echoPin = 2;
byte rowPins[4] = {37, 36, 35, 34};
byte colPins[4] = {33, 32, 31, 30};

//volatile int encoderValue = 0;
//int lastEncoderValue = 0;
//int lastButtonState = HIGH; // Button not pressed
//int buttonState;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

const int noteC = 261;
const int noteD = 294;
const int noteE = 329;
const int noteF = 349;
const int noteG = 392;
const int noteA = 440;
const int noteB = 493;
const int noteC_High = 523;

// Define the melody for the song (e.g., "Twinkle Twinkle Little Star")
int melody[] = {
  noteC, noteC, noteG, noteG, noteA, noteA, noteG,
  noteF, noteF, noteE, noteE, noteD, noteD, noteC,
  noteG, noteG, noteF, noteF, noteE, noteE, noteD,
  noteG, noteG, noteF, noteF, noteE, noteE, noteD,
  noteC, noteC, noteG, noteG, noteA, noteA, noteG,
  noteF, noteF, noteE, noteE, noteD, noteD, noteC
};

// Define the note durations: 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2
};

void setup() {
  Serial.begin(9600);
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522
  // Initialize the LCD with the I2C address
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
  pinMode(tapPin, INPUT);
  pinMode(button,INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  myServo.attach(8);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("System initialized. Waiting for RFID card...");
}

void loop() {

  char key = keypad.getKey();
    if (key) {
    if (key == 'D') {
      playSong();
    } else {
      playNote(key);
    }
    Serial.println(key);
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID card detected.");
    delay(50);
    setRGBColor(0, 225, 0); //red
    lcd.clear();
    lcd.print("Load clothes");
    delay(3000);
    lcd.clear();
    lcd.print("Close the door");
     while (!digitalRead(tapPin)) {
      delay(100); // Wait until tap module senses door closed
    }
    Serial.println("Tap detected");
    delay(100);
    lcd.clear();
    lcd.print("What service?");
    lcd.setCursor(0, 1);
    lcd.print("1:Wash 2:Dry");
//    int potButtonState = digitalRead(SW);
//    int buttonState = digitalRead(button);

//   if (potButtonState == LOW) {
//    Serial.println("pot pressed");
//    lcd.clear();
//    lcd.print("Drying...");
//    delay(50);
//    startDrying();
//    } else if (buttonState == LOW) {
//    Serial.println("bottun pressed");
//    lcd.clear();
//    lcd.print("Washing...");
//    delay(50);
//   startWashing();
//    }
  }

  if (digitalRead(SW) == LOW) {
     Serial.println("pot pressed");
     delay(50);
     startDrying();
     signalCompletion();
  } else if (digitalRead(button) == LOW) {
     Serial.println("bottun pressed");
     delay(50);
     startWashing();
     signalCompletion();
  }
}

void startWashing() {
  lcd.clear();
  lcd.print("Washing...");
  setRGBColor(0, 0, 255);  // Set RGB to blue
  Serial.println("Washing started.");
  int cycles = determineCycles();
  for (int i = 0; i < cycles; i++) {
    myServo.write(0);
    delay(200);
    myServo.write(180);
    delay(200);
    myServo.write(0);
    delay(1000);
    for (int j = 0; j < 5; j++) {
      myServo.write(0);
      delay(200);
      myServo.write(180);
      delay(200);
    }
  delay(1000);
  startDrying();
  }
}

void startDrying() {
  lcd.clear();
  lcd.print("Drying...");
  setRGBColor(0, 200, 225); //purple
  int cycles = determineCycles();
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < 3; j++) {
      myServo.write(0);
      delay(200);
      myServo.write(180);
      delay(200);
    }
  }
}

  int determineCycles() {
  long duration;
  int distance;

  // Send a 10us pulse to the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin and calculate the duration
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 5) {
    return 1; // If distance is less than or equal to 5 cm
  } else if (distance <= 20) {
    return 2; // If distance is between 6 cm and 20 cm
  } else {
    return 3; // If distance is more than 21 cm
  }
}

void signalCompletion() {
  lcd.clear();
  lcd.print("Take out clothes");
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    delay(1000);
  }
  delay(2000);
  setRGBColor(255, 0, 0);  // Set RGB to green
  lcd.clear();
  lcd.print("Thank you");
  delay(2000);
  lcd.clear();
}

void setRGBColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

//void updateEncoder() {
//  int clkState = digitalRead(CLK);
//  int dtState = digitalRead(DT);

 // if (clkState != dtState) {
 //   encoderValue++;
 // } else {
 //   encoderValue--;
//  }
//}

void playNote(char key) {
  switch (key) {
    case '1': tone(buzzerPin, noteC, 200); break;
    case '2': tone(buzzerPin, noteD, 200); break;
    case '3': tone(buzzerPin, noteE, 200); break;
    case 'A': tone(buzzerPin, noteF, 200); break;
    case '4': tone(buzzerPin, noteG, 200); break;
    case '5': tone(buzzerPin, noteA, 200); break;
    case '6': tone(buzzerPin, noteB, 200); break;
    case 'B': tone(buzzerPin, noteC_High, 200); break;
    case '7': tone(buzzerPin, noteC / 2, 200); break; // Lower octave C
    case '8': tone(buzzerPin, noteD / 2, 200); break; // Lower octave D
    case '9': tone(buzzerPin, noteE / 2, 200); break; // Lower octave E
    case 'C': tone(buzzerPin, noteF / 2, 200); break; // Lower octave F
    case '*': tone(buzzerPin, noteG / 2, 200); break; // Lower octave G
    case '0': tone(buzzerPin, noteA / 2, 200); break; // Lower octave A
    case '#': tone(buzzerPin, noteB / 2, 200); break; // Lower octave B
    default: break;
  }
}

void playSong() {
  int melodyLength = sizeof(melody) / sizeof(melody[0]);
  for (int i = 0; i < melodyLength; i++) {
    int noteDuration = 1000 / noteDurations[i];
    tone(buzzerPin, melody[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}