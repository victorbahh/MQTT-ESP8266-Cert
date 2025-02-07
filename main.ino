#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

static const char cacert[] = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----

)EOF";

static const char client_cert[] = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";

static const char privkey[] = R"KEY(
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
)KEY";

// WiFi credentials
const char WIFI_SSID[] = "********";
const char WIFI_PASSWORD[] = "********";

// Device ID
const char deviceID[] = "********";

// MQTT broker host address
const char MQTT_HOST[] = "********";

// MQTT topics
const char PUBLISH_TOPIC[] = "esp8266/pub";
const char SUBSCRIBE_TOPIC[] = "esp8266/sub";

// Publishing interval
const long interval = 5000;

// Timezone offset from UTC (-3 for BRAZIL)
const int8_t TIME_ZONE = -3;

// Last time message was sent
unsigned long lastMillis = 0;

// WiFiClientSecure object for secure communication
WiFiClientSecure net;

// X.509 certificate for the CA
BearSSL::X509List cert(cacert);

// X.509 certificate for the client
BearSSL::X509List client_crt(client_cert);

// RSA private key
BearSSL::PrivateKey key(privkey);

// MQTT client instance
PubSubClient client(net);

// Function to connect to NTP server and set time
void NTPConnect() {
  // Set time using SNTP
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1510592825) { // January 13, 2018
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
}

// Callback function for message reception
void messageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to connect to MQTT broker
void connectMQTT() {
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // Connect to NTP server to set time
  NTPConnect();

  // Set CA and client certificate for secure communication
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
  // net.setInsecure();

  // Connect MQTT client to broker
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);

  Serial.println("Connecting to Broker");

  // Attempt to connect to broker
  while (!client.connect(deviceID)) {
    Serial.print(".");
    delay(1000);
  }

  // Check if connection is successful
  if (!client.connected()) {
    Serial.println("Connection Timeout!");
    return;
  }

  // Subscribe to MQTT topic
  client.subscribe(SUBSCRIBE_TOPIC);
  Serial.println("Connected!");
}

// Function to publish message to broker
void publishMessage() {
  // Generate random humidity and temperature values
  float h = random(0, 100); 
  float t = random(0, 50); 

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Create JSON document for message
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["humidity"] = h;
  doc["temperature"] = t;

  // Serialize JSON document to string
  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);

  // Publish message to MQTT topic
  client.publish(PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Connect to MQTT Broker
  connectMQTT();
}

void loop() {
  // Check if it's time to publish a message
  if (millis() - lastMillis > interval) {
    lastMillis = millis();
    if (client.connected()) {
      // Publish message
      publishMessage();
    }
  }
  // Maintain MQTT connection
  client.loop();
}