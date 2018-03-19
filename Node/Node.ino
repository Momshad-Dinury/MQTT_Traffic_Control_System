#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define lane_1 15
#define lane_2 13
#define lane_3 14
#define lane_4 3

//Network details:(Editable per Network request)
const char* ssid = "STELLAR";
const char* password = "stellarBD";
const char* mqtt_server = "www.stellarbd.com";

WiFiClient LC_espClient;
PubSubClient client(LC_espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println("Initialized..");
  pinMode(lane_1, OUTPUT);
  pinMode(lane_2, OUTPUT);
  pinMode(lane_3, OUTPUT);
  pinMode(lane_4, OUTPUT);
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
    digitalWrite(lane_1, HIGH);
    digitalWrite(lane_2, LOW);
    digitalWrite(lane_3, LOW);
    digitalWrite(lane_4, LOW);

  } else if ((char)payload[0] == '2') {
    digitalWrite(lane_1, LOW);
    digitalWrite(lane_2, HIGH);
    digitalWrite(lane_3, LOW);
    digitalWrite(lane_4, LOW);
  } else if ((char)payload[0] == '3') {
    digitalWrite(lane_1, LOW);
    digitalWrite(lane_2, LOW);
    digitalWrite(lane_3, HIGH);
    digitalWrite(lane_4, LOW);
  } else if ((char)payload[0] == '4') {
    digitalWrite(lane_1, LOW);
    digitalWrite(lane_2, LOW);
    digitalWrite(lane_3, LOW);
    digitalWrite(lane_4, HIGH);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("LC_espClient")) {
      Serial.println("Connected");
      // Once connected, publish an announcement...
      client.publish("dashboard", "Lane Controller Connection Established!");
      // ... and resubscribe
      client.subscribe("lanes");
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
