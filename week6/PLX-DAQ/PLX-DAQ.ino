int sensorPin = A1;
 
void setup() { 
  Serial.begin(9600); 
  Serial.println("CLEARDATA"); 
  Serial.println("LABEL,Time, Started Time, Register value"); 
  Serial.println("RESETTIMER"); 
} 
 
void loop() { 
  int sensorValue = analogRead(sensorPin);
  Serial.print("DATA, TIME, TIMER, "); 
  Serial.println(sensorValue); 
  delay(1500); 
}