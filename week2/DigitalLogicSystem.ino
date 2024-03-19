 const int a = 2;
 const int b = 3;
 const int c = 4;
 const int d = 5;
 const int e = 6;
 const int f = 7;
 const int g = 8;
 
 bool bPress = false;
 const int buttonPin = 9;
 
 //Variables will change
 int buttonPushCounter = 0;
 int buttonState = 0;
 int lastButtonState = 0;
 
 void setup()
 {
   pinMode(a,OUTPUT);
   pinMode(b,OUTPUT);
   pinMode(c,OUTPUT);
   pinMode(d,OUTPUT);
   pinMode(e,OUTPUT);
   pinMode(f,OUTPUT);
   pinMode(g,OUTPUT);
   
   pinMode(buttonPin,INPUT_PULLUP);
   Serial.begin(9600);
   displayDigit(buttonPushCounter);
 }
 
 void loop()
 {
   buttonState = digitalRead(buttonPin);
   
   //compare the buttonState to its previous state
   if(buttonState != lastButtonState)
   {
     //if the state has changed,increment the counter
     if(buttonState == LOW)
     {
       //if the current state is HIGH then the button went from off to on
       bPress = true;
       buttonPushCounter++;
       if(buttonPushCounter>9) buttonPushCounter = 0;
       Serial.println("on");
     }
     else{
       //if the current state is LOW then the button went from on to off
       Serial.println("off");
     }
     //Delay a little bit to avoid bouncing
     delay(50);
   }
   //save the current state as the last state,for ext time through the loop
   lastButtonState = buttonState;
   
   if(bPress ){
     turnoff();
     displayDigit(buttonPushCounter);
   }
 }
 
 void displayDigit(int digit)
 {
   //condition for displaying segment a
   if(digit != 1 && digit !=4)
   digitalWrite(a,HIGH);
   
   //condition for displaying segment b
   if(digit != 5  && digit != 6)
   digitalWrite(b,HIGH);
   
   //condition for displaying segment c
   if(digit != 2)
   digitalWrite(c,HIGH);
   
   //condition for displaying segment d
   if(digit != 1 && digit !=4 && digit != 7)
   digitalWrite(d,HIGH);
   
   //condition for displaying segment e
   if(digit == 2 || digit ==6 || digit ==8 || digit == 0)
   digitalWrite(e,HIGH);
   
   //condition for displaying segment f
   if(digit != 1 && digit !=2 && digit != 3 && digit != 7)
   digitalWrite(f,HIGH);
   
   //condition for displaying segment g
   if(digit != 0 && digit !=1 && digit!= 7)
   digitalWrite(g,HIGH);
 }
 
 void turnoff()
 {
   digitalWrite(a,LOW);
   digitalWrite(b,LOW);
   digitalWrite(c,LOW);
   digitalWrite(d,LOW);
   digitalWrite(e,LOW);
   digitalWrite(f,LOW);
   digitalWrite(g,LOW);
 }
