#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define PIN 15

//Network details:(Editable per Network request)
const char* ssid = "STELLAR";
const char* password = "stellarBD";
const char* mqtt_server = "www.stellarbd.com";

WiFiClient N_espClient;
PubSubClient client(N_espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println("Initialized..");
  pinMode(PIN,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}


void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1')  {
    digitalWrite(PIN, HIGH);
  } else if ((char)payload[0] == '0') {
    digitalWrite(PIN, LOW);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("N_espClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("dashboard", "Node Connection Established!");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
}