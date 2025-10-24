/***************************************************
 * Project: Flood Detection with Blynk
 * Author: Roy Cuadra
 * Date: 08/24/2025
 ***************************************************/

// --- Blynk Template Configuration ---
#define BLYNK_TEMPLATE_ID   "Your_Blynk_Template_ID"
#define BLYNK_TEMPLATE_NAME "Flood Detection"
#define BLYNK_AUTH_TOKEN    "Your_Blynk_Auth_Token"  // <--- Replace with your token from Blynk app

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char ssid[] = "Your_WiFi_Name";       // <-- Replace with your WiFi SSID
char pass[] = "Your_WiFi_Password";   // <-- Replace with your WiFi Password


const int trigPin      = 12;  // D6  -> Ultrasonic Trigger
const int echoPin      = 14;  // D5  -> Ultrasonic Echo
const int buzzerPin    = 13;  // D7  -> Buzzer
const int greenLedPin  = 15;  // D8  -> Green LED
const int blueLedPin   = 2;   // D4  -> Blue LED 
const int redLedPin    = 3;   // RX  -> Red LED 
const int yellowLedPin = 16;  // D0  -> Yellow

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
int distanceInch;

BlynkTimer timer;

unsigned long previousYellowMillis = 0;
const long yellowBlinkInterval = 500;
bool yellowLedState = false;

unsigned long previousBuzzerMillis = 0;
int buzzerOnTime = 200;  
int buzzerOffTime = 300;  
bool buzzerState = false;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);

  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN);
  
  timer.setInterval(1000L, sendDistanceToBlynk);
  timer.setInterval(200L, checkBlynkConnection);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    Blynk.run();
  }
  timer.run();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration > 0) {
    distanceCm = duration * SOUND_SPEED / 2;
    distanceInch = distanceCm * CM_TO_INCH;
  } else {
    distanceCm = 999;  
    distanceInch = 999;
  }

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  if (distanceInch <= 2.5) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    buzzerOnTime = 100; 
    buzzerOffTime = 100;
  } else if (distanceInch <= 4.5) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
    buzzerOnTime = 400;  
    buzzerOffTime = 500;
  } else if (distanceInch <= 5) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(blueLedPin, LOW);
    buzzerOnTime = 0;
    buzzerOffTime = 0;
    buzzerState = false;
    digitalWrite(buzzerPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    buzzerOnTime = 0;
    buzzerOffTime = 0;
    buzzerState = false;
    digitalWrite(buzzerPin, LOW);
  }

  if (buzzerOnTime > 0 && buzzerOffTime > 0) {
    unsigned long currentMillis = millis();
    if (buzzerState && currentMillis - previousBuzzerMillis >= buzzerOnTime) {
      buzzerState = false;
      previousBuzzerMillis = currentMillis;
      digitalWrite(buzzerPin, LOW);
    } else if (!buzzerState && currentMillis - previousBuzzerMillis >= buzzerOffTime) {
      buzzerState = true;
      previousBuzzerMillis = currentMillis;
      digitalWrite(buzzerPin, HIGH);
      if (Blynk.connected() && distanceInch <= 2.5) {
        Blynk.logEvent("high_water_levels");
      }
    }
  }

  delay(50); 
}

// --- Send Data distance to Blynk ---
void sendDistanceToBlynk() {
  if (Blynk.connected()) {
    Blynk.virtualWrite(V1, distanceCm);
    Blynk.virtualWrite(V2, distanceInch);
  }
}

void checkBlynkConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!Blynk.connected()) {
      Blynk.connect(1000);
      unsigned long currentMillis = millis();
      if (currentMillis - previousYellowMillis >= yellowBlinkInterval) {
        previousYellowMillis = currentMillis;
        yellowLedState = !yellowLedState;
        digitalWrite(yellowLedPin, yellowLedState);
      }
    } else {
      digitalWrite(yellowLedPin, HIGH);
    }
  } else {
    digitalWrite(yellowLedPin, LOW);
  }
}
