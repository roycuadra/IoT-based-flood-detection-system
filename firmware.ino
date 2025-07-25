// Blynk template configuration
#define BLYNK_TEMPLATE_ID   "TMPL6W6xPxCgO"     // Unique template ID assigned by Blynk
#define BLYNK_TEMPLATE_NAME "Sample"            // Name of your Blynk project template
#define BLYNK_AUTH_TOKEN    "0ZbcIx1CBDr1lLtSqXU8ur39IwkGwVlH"  // Auth token for your device

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Ultrasonic sensor pins
const int trigPin      = 12;  // Trigger pin of the ultrasonic sensor
const int echoPin      = 14;  // Echo pin of the ultrasonic sensor

// Output devices
const int buzzerPin    = 13;  // Buzzer for alerts
const int greenLedPin  = 15;  // Green LED (e.g., safe status)
const int blueLedPin   = 2;   // Blue LED (e.g., standby or info)
const int redLedPin    = 3;   // Red LED (e.g., warning or danger)


#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
int distanceInch;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, "Polaris", "cuadra123");  // Replace with your WiFi credentials

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(500);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Set up Blynk Timer to send distance to Blynk app
  timer.setInterval(1000L, sendDistanceToBlynk);
}

void loop() {
  Blynk.run();  // Run Blynk background tasks
  timer.run();  // Run Blynk Timer

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  display.clearDisplay();
  display.setCursor(0, 25);
  display.print(distanceCm);
  display.print(" cm");

  display.setCursor(0, 50);
  display.print(distanceInch);
  display.print(" in");

  display.display();

  // Check distance conditions and control LEDs
  if (distanceInch <= 5) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
  } else if (distanceInch <= 15) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
  } else if (distanceInch <= 30) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(blueLedPin, LOW);
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
  }

  if (distanceInch <= 2) {
    digitalWrite(buzzerPin, HIGH);
     Blynk.logEvent("high_water_levels");
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(500);
}

void sendDistanceToBlynk() {
  Blynk.virtualWrite(V1, distanceCm);  // Virtual pin V1 for distance in centimeters
  Blynk.virtualWrite(V2, distanceInch);  // Virtual pin V2 for distance in inches
}
