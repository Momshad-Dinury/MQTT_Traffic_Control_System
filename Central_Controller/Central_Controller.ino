#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Flags:
boolean location1flag = true;
// boolean location2flag = true;
// boolean location3flag = true;
// boolean location4flag = true;
//Pinmap:
#define location1 14

//WiFi
const char* ssid = "STELLAR";
const char* password = "stellarBD";
WiFiClient CC_espClient;
//MQTT
const char* mqtt_server = "www.stellarbd.com";
PubSubClient client(CC_espClient);
//DEBUG
#define DEBUG true

void setup() {
	Serial.begin(115200);
	#ifdef DEBUG
	Serial.println("System Initialized");
	#endif
	//Pinmode:
	pinMode(location1, INPUT);
	pinMode(location1, OUTPUT);
	// pinMode(location2, INPUT);
	// pinMode(location3, INPUT);
	// pinMode(location4, INPUT);

	#ifdef DEBUG
	Serial.println("Pinmode defined");
	#endif
	//WiFi setup:
	setup_wifi();
	#ifdef DEBUG
	Serial.println("WiFi Configured Successfully");
	#endif
	//Mqtt server:
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);

	#ifdef DEBUG
	Serial.println("System Configuration finished");
	#endif
}

void loop() {
	if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
  control();
}


void control() {
  
	if( digitalRead(location1) == HIGH && location1flag == true) {
		client.publish("inTopic", "1");

		#ifdef DEBUG
		Serial.println("Publishing to topic, value 1");
		#endif
		//digitalWrite(location1, LOW);

    	location1flag = false;
	}
	//location1flag = true;
	
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
 }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
  	#ifdef DEBUG
    Serial.print("Attempting MQTT connection...");
    #endif
    // Attempt to connect
    if (client.connect("CC_espClient")) {
    	#ifdef DEBUG
      	Serial.println("Connected");
      	#endif
      	//Once connected, publish an announcement...
      	client.publish("dashboard", " Central Control Connection Established!");
     	//... and resubscribe
      	// client.subscribe("inTopic");
    } else {
    	#ifdef DEBUG
      	Serial.print("failed, rc=");
      	Serial.print(client.state());
      	Serial.println(" try again in 5 seconds");
      	#endif
      // Wait 5 seconds before retrying
      	delay(5000);
    }
  }
}
