#include <WiFi.h>
#include <HTTPClient.h>

const int pirPin = 27;
const int vibrationPin = 26;
const int ledPin = 14;

// ------ API LINKS -------
String whatsapp_api = "https://api.callmebot.com/whatsapp.php?phone=250788966395&text=ALERT!+Motion+or+Vibration+Detected!&apikey=7183818";
String telegram_api = "http://api.callmebot.com/text.php?source=web&user=@omarwacu&text=ALERT!+Motion+or+Vibration+Detected!";

// ------ WiFi ------
const char* ssid = "EdNet";
const char* password = "Huawei@123";

// ------ Cooldown ------
unsigned long lastSentTime = 0;
unsigned long cooldown = 2000;   // 2 secondS

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  int motionDetected = digitalRead(pirPin);
  int vibrationDetected = digitalRead(vibrationPin);

  if (motionDetected == HIGH || vibrationDetected == HIGH) {
    Serial.println("ALERT! Motion or Vibration Detected!");
    digitalWrite(ledPin, HIGH);

    // Send alerts every 2 seconds max
    if (millis() - lastSentTime > cooldown) {
      sendWhatsApp();
      sendTelegram();
      lastSentTime = millis();

    }

  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(50);
}

// -------------- ALERT FUNCTIONS --------------

void sendWhatsApp() {
  sendRequest(whatsapp_api, "WhatsApp");
}

void sendTelegram() {
  sendRequest(telegram_api, "Telegram");
}

// Generic request sender
void sendRequest(String url, String serviceName) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);

    Serial.print("Sending ");
    Serial.print(serviceName);
    Serial.println(" Alert...");

    int code = http.GET();

    Serial.print(serviceName);
    Serial.print(" Response: ");
    Serial.println(code);

    http.end();
  }
}
