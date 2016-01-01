#include <NewPing.h>

#define RedLEDPin 4
#define YellowLEDPin 6
#define GreenLEDPin 8
#define TRIGGER_PIN  13
#define ECHO_PIN     11

#define MAX_DISTANCE 300

//inches for minimum threshold
int RED_MIN_DISTANCE;
int YELLOW_MIN_DISTANCE;
int GREEN_MIN_DISTANCE;


boolean seenRed = false;
int huns, tens, ones, distance, sonar_data, i;
unsigned long time_start;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void showGreenLED();

void showYellowLED();

void showRedLED();

void sleepModeHandler();

void cycleGreenToRed();

void setDistances(int minimum);

void setup() {
    Serial.begin(9600);
    pinMode(GreenLEDPin, OUTPUT);
    pinMode(RedLEDPin, OUTPUT);
    pinMode(YellowLEDPin, OUTPUT);
}

void loop() {
    int sensorValue = analogRead(A0);

    unsigned int distance = sonar.ping() / 100;
    turnOffLEDs();
//    Serial.print("Ping: ");
//    Serial.print(distance);
//    Serial.println(" in");
    int potentiometerDistance = analogRead(A0) / 10;
    setDistances(potentiometerDistance);

    unsigned int GREEN_TIME_MULTIPLIER = 300 / (MAX_DISTANCE - GREEN_MIN_DISTANCE);
    unsigned int YELLOW_TIME_MULTIPLIER = 300 / (GREEN_MIN_DISTANCE - YELLOW_MIN_DISTANCE);
    unsigned int RED_TIME_MULTIPLIER = 500 / (YELLOW_MIN_DISTANCE - RED_MIN_DISTANCE);


    Serial.print("distance: ");
    Serial.print(distance);
    Serial.println(" inches");
    Serial.print("Red min: ");
    Serial.println(RED_MIN_DISTANCE);
    Serial.print("Yellow min: ");
    Serial.println(YELLOW_MIN_DISTANCE);
    Serial.print("Green min: ");
    Serial.println(GREEN_MIN_DISTANCE);
    Serial.println("");
    
    if (distance > GREEN_MIN_DISTANCE) {
        seenRed = false;
        delay((distance - GREEN_MIN_DISTANCE) * GREEN_TIME_MULTIPLIER);
        showGreenLED();
    }
    else if (distance > YELLOW_MIN_DISTANCE && distance <= GREEN_MIN_DISTANCE) {
        seenRed = false;
        delay((distance - YELLOW_MIN_DISTANCE) * YELLOW_TIME_MULTIPLIER);
        showYellowLED();
    }
    else if (distance <= YELLOW_MIN_DISTANCE && distance >= RED_MIN_DISTANCE) {
        delay((distance - RED_MIN_DISTANCE) * RED_TIME_MULTIPLIER);
        showRedLED();
    }
    else if (distance < RED_MIN_DISTANCE && distance > 0) {
        cycleGreenToRed(50);
    }
    sleepModeHandler();
    delay(100);

}

void turnOffLEDs() {
    digitalWrite(GreenLEDPin, LOW);
    digitalWrite(RedLEDPin, LOW);
    digitalWrite(YellowLEDPin, LOW);
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

void wake_up() {
    cycleGreenToRed(50);
}

void go_to_sleep() {
    delay(10000);
    for (i = 0; i < 1; i++) {
        cycleGreenToRed(50);
    }
    time_start = millis();
}

void sleepModeHandler() {
    if (millis() - time_start < 0) // means we had a roll over since time_start is greater than millis
        time_start = 0;

    if (millis() - time_start > 60000 || time_start == 0) {
        turnOffLEDs();
        go_to_sleep();
    }
}

void setDistances(int minimum) {
    RED_MIN_DISTANCE = 2 + minimum;
    YELLOW_MIN_DISTANCE = RED_MIN_DISTANCE + 30;
    GREEN_MIN_DISTANCE = YELLOW_MIN_DISTANCE + 30;

}

void showRedLED() {
    digitalWrite(RedLEDPin, HIGH);
    digitalWrite(YellowLEDPin, LOW);
    digitalWrite(GreenLEDPin, LOW);
}

void showYellowLED() {
    digitalWrite(GreenLEDPin, LOW);
    digitalWrite(RedLEDPin, LOW);
    digitalWrite(YellowLEDPin, HIGH);
}

void showGreenLED() {
    digitalWrite(GreenLEDPin, HIGH);
    digitalWrite(RedLEDPin, LOW);
    digitalWrite(YellowLEDPin, LOW);
}
