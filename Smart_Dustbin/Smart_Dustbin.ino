#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// Define servo and ultrasonic pins
const int servoPin = 13;
const int triggerPin1 = 26;
const int echoPin1 = 25;
const int triggerPin2 = 33;
const int echoPin2 = 32;

Servo servo1;

// WiFi credentials
const char* ssid = "xyz";
const char* password = "12345678";

// WhatsApp API details
// Replace with your recipient's phone number and API key
const char* whatsappAPI = "https://api.callmebot.com/whatsapp.php?phone=+91xxxxxxxx&apikey=xxxxxxx&text=";

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int distance1 = getDistance(triggerPin1, echoPin1);
  Serial.println(distance1);


  if (distance1 < 7) {
    servo1.write(180);
    delay(5000); 
  } else {
    servo1.write(90);
  }
  delay(100); 

}

int getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert duration to centimeters

  return distance;
}

void sendWhatsAppMessage(String message) {
  HTTPClient http;

  String url = whatsappAPI + message;
  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("WhatsApp message sent. Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending WhatsApp message. HTTP error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
