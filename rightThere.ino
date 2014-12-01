#include <NewPing.h>

#define RedLEDPin 4
#define YellowLEDPin 6
#define GreenLEDPin 8
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 500

int huns, tens, ones, distance, returnbyte, sonar_data, i; 
unsigned long time_start;
void wake_up(){}

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200); 
  pinMode(GreenLEDPin, OUTPUT);
  pinMode(RedLEDPin, OUTPUT);
  pinMode(YellowLEDPin, OUTPUT);
//  turnOffLEDs();
}

void turnOffLEDs() {
  digitalWrite(GreenLEDPin, LOW);
  digitalWrite(RedLEDPin, LOW);
  digitalWrite(YellowLEDPin, LOW);
}

void go_to_sleep(){
   delay(10000);
  for(i=0; i<10; i++){
    cycleGreenToRed(50);
  }
  time_start= millis();
}

void cycleGreenToRed(int delaySeconds) {
  digitalWrite(GreenLEDPin, HIGH);
  delay(delaySeconds);
  digitalWrite(GreenLEDPin, LOW);
  digitalWrite(YellowLEDPin, HIGH);
  delay(delaySeconds);
  digitalWrite(YellowLEDPin, LOW);
  digitalWrite(RedLEDPin, HIGH);
  delay(delaySeconds);
  digitalWrite(RedLEDPin, LOW);
  delay(delaySeconds);  
}

void loop() {
  delay(50);                    
  unsigned int distance = sonar.ping()/100; 
  Serial.print("Ping: ");
  Serial.print(distance); 
  Serial.println(" in");

if(distance>36){
      digitalWrite(GreenLEDPin, HIGH);
      digitalWrite(RedLEDPin, LOW);
      digitalWrite(YellowLEDPin, LOW);
  }
  else if(distance>12  && distance<=36){
      digitalWrite(GreenLEDPin, LOW);
      digitalWrite(RedLEDPin, LOW);
      digitalWrite(YellowLEDPin, HIGH);
  }
  else if(distance<=12){      
      digitalWrite(RedLEDPin, HIGH);    
      digitalWrite(YellowLEDPin, LOW);
      digitalWrite(GreenLEDPin, LOW);
      delay(1000);
      cycleGreenToRed(50);
  }

    if (millis()-time_start<0) // means we had a roll over since time_start is greater than millis
    time_start=0;
    
    if(millis()-time_start>60000){
        turnOffLEDs();  
        go_to_sleep();
    }
}

