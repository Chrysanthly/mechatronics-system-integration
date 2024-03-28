const int led = 7;

void setup() { 
 Serial.begin(9600); 
 pinMode(led,OUTPUT);
} 
void loop() { 
 int potValue = analogRead(A0); 
 Serial.println(potValue); 
 delay(1000); // add a delay to avoid sending data too fast 
 if (potValue >= 300)
 {
   digitalWrite(led,HIGH); //LED turns on when readings are higher than 300
 }
 else
 digitalWrite(led,LOW);
}
