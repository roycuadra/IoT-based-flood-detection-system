/***************************************************
 * Project: Flood Detection with Blynk
 * Author: Roy Cuadra
 * Date: 08/24/2025
 * NO OLED VERSION
 ***************************************************/

// --- Blynk Template Configuration ---
#define BLYNK_TEMPLATE_ID   "Your_Blynk_Template_ID"
#define BLYNK_TEMPLATE_NAME "Flood Detection"
#define BLYNK_AUTH_TOKEN    "Your_Blynk_Auth_Token"  // <--- Replace with your token from Blynk app

// --- WiFi Credentials ---
char ssid[] = "Your_WiFi_Name";       // <--- Replace with your WiFi SSID
char pass[] = "Your_WiFi_Password";   // <--- Replace with your WiFi Password

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// --- Ultrasonic Sensor Pins ---
const int trigPin      = 12;  // D6  -> Ultrasonic Trigger
const int echoPin      = 14;  // D5  -> Ultrasonic Echo

// --- Output Devices ---
const int buzzerPin    = 13;  // D7  -> Buzzer
const int greenLedPin  = 15;  // D8  -> Green LED
const int blueLedPin   = 2;   // D4  -> Blue LED (built-in LED on most ESP8266 boards)
const int redLedPin    = 3;   // RX  -> Red LED (be careful, shared with Serial RX)
const int yellowLedPin = 16;  // D0  -> Yellow LED (Blynk connection status)

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
int distanceInch;

BlynkTimer timer;

// --- Yellow LED variables ---
unsigned long previousYellowMillis = 0;
const long yellowBlinkInterval = 500; // Blink every 500ms
bool yellowLedState = false;

// --- Buzzer variables ---
unsigned long previousBuzzerMillis = 0;
int buzzerOnTime = 200;   // Default ON duration
int buzzerOffTime = 300;  // Default OFF duration
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

  // Start WiFi manually
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN); // Non-blocking setup

  // Timer tasks
  timer.setInterval(1000L, sendDistanceToBlynk);
  timer.setInterval(200L, checkBlynkConnection);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    Blynk.run();
  }
  timer.run();

  // --- Ultrasonic measurement ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms
  if (duration > 0) {
    distanceCm = duration * SOUND_SPEED / 2;
    distanceInch = distanceCm * CM_TO_INCH;
  } else {
    distanceCm = 999;  
    distanceInch = 999;
  }

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  // --- LED logic based on water level ---
  if (distanceInch <= 2.5) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    buzzerOnTime = 100;  // Faster beep
    buzzerOffTime = 100;
  } else if (distanceInch <= 4.5) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
    buzzerOnTime = 400;  // Slower beep
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

  // --- Buzzer pulsing ---
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

  delay(50);  // small delay to smooth timing
}

// --- Send distance to Blynk ---
void sendDistanceToBlynk() {
  if (Blynk.connected()) {
    Blynk.virtualWrite(V1, distanceCm);
    Blynk.virtualWrite(V2, distanceInch);
  }
}

// --- Yellow LED for Blynk status ---
void checkBlynkConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!Blynk.connected()) {
      Blynk.connect(1000); // Try reconnect
      // Blink yellow LED slowly
      unsigned long currentMillis = millis();
      if (currentMillis - previousYellowMillis >= yellowBlinkInterval) {
        previousYellowMillis = currentMillis;
        yellowLedState = !yellowLedState;
        digitalWrite(yellowLedPin, yellowLedState);
      }
    } else {
      digitalWrite(yellowLedPin, HIGH); // Solid if connected
    }
  } else {
    digitalWrite(yellowLedPin, LOW); // No WiFi
  }
}


// --- Blynk Template Configuration ---
#define BLYNK_TEMPLATE_ID   "Your_Blynk_Template_ID"
#define BLYNK_TEMPLATE_NAME "Flood Detection"
#define BLYNK_AUTH_TOKEN    "Your_Blynk_Auth_Token"  // <--- Replace with your token from Blynk app


#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

// --- WiFi Credentials ---
char ssid[] = "Your_WiFi_Name";       // <--- Replace with your WiFi SSID
char pass[] = "Your_WiFi_Password";   // <--- Replace with your WiFi Password

// Ultrasonic sensor pins
const int trigPin      = 12;  
const int echoPin      = 14;  

// Output devices
const int buzzerPin    = 13;  
const int greenLedPin  = 15;  
const int blueLedPin   = 2;   
const int redLedPin    = 3;   
const int yellowLedPin = 16;  // LED for Blynk connection status

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
int distanceInch;

BlynkTimer timer;

// --- Yellow LED variables ---
unsigned long previousYellowMillis = 0;
const long yellowBlinkInterval = 500; // Blink 500ms
bool yellowLedState = false;

// --- Buzzer variables ---
unsigned long previousBuzzerMillis = 0;
int buzzerOnTime = 200;   // Default ON duration
int buzzerOffTime = 300;  // Default OFF duration
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

  // Start WiFi manually
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN); // Non-blocking setup

  // Timer tasks
  timer.setInterval(1000L, sendDistanceToBlynk);
  timer.setInterval(200L, checkBlynkConnection);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    Blynk.run();
  }
  timer.run();

  // --- Ultrasonic measurement ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms to avoid lock
  if (duration > 0) {
    distanceCm = duration * SOUND_SPEED / 2;
    distanceInch = distanceCm * CM_TO_INCH;
  } else {
    distanceCm = 999;  
    distanceInch = 999;
  }

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  // --- LED logic based on water level ---
  if (distanceInch <= 2.5) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    buzzerOnTime = 100;  // Faster beep
    buzzerOffTime = 100;
  } else if (distanceInch <= 4.5) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
    buzzerOnTime = 400;  // Slower beep
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

  // --- Buzzer pulsing ---
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

  delay(50);  // small delay to smooth timing
}

// --- Send distance to Blynk if connected ---
void sendDistanceToBlynk() {
  if (Blynk.connected()) {
    Blynk.virtualWrite(V1, distanceCm);
    Blynk.virtualWrite(V2, distanceInch);
  }
}

// --- Yellow LED for Blynk status ---
void checkBlynkConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!Blynk.connected()) {
      Blynk.connect(1000); // Try reconnect
      // Blink yellow LED slowly
      unsigned long currentMillis = millis();
      if (currentMillis - previousYellowMillis >= yellowBlinkInterval) {
        previousYellowMillis = currentMillis;
        yellowLedState = !yellowLedState;
        digitalWrite(yellowLedPin, yellowLedState);
      }
    } else {
      digitalWrite(yellowLedPin, HIGH); // Solid if connected
    }
  } else {
    digitalWrite(yellowLedPin, LOW); // No WiFi
  }
}
