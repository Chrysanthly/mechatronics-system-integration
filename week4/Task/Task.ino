#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;
const int threshold = 1000; // Adjust this threshold as needed
int previousGesture = -1;
void setup() {
Serial.begin(9600);
Wire.begin();
mpu.initialize();
}
void loop() {
int gesture = detectGesture();
Serial.print("Detected Gesture: ");
if (gesture == 1) {
Serial.println("Gesture 1");
// Perform an action for Gesture 1
} else if (gesture == 2) {
  Serial.println("Gesture 2");
// Perform an action for Gesture 2
}
// Add more gesture cases as needed
previousGesture = gesture;
// Read sensor data and send it to Python
int ax, ay, az, gx, gy, gz;
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
Serial.print(ax);
Serial.print(",");
Serial.println(ay);
delay(100); // Adjust the delay as needed to control the data transmission rate
}
int detectGesture() {
int ax, ay, az, gx, gy, gz;
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
// Perform gesture recognition here based on sensor data
// Define conditions to recognize specific gestures
if (ax > threshold && ay < threshold) {
return 1; // Gesture 1
} else if (ax < -threshold && ay > threshold) {
return 2; // Gesture 2
}
// Add more gesture conditions as needed
return 0; // No gesture detected
}