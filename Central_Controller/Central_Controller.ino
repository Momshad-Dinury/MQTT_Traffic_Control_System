#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Flags:
boolean lane_1_flag = true;
boolean lane_2_flag = true;
boolean lane_3_flag = true;
boolean lane_4_flag = true;
//Pinmap:
#define lane_1 14
#define lane_2 12
#define lane_3 13
#define lane_4 15

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
	pinMode(lane_1, INPUT);
	pinMode(lane_1, OUTPUT);
	pinMode(lane_3, INPUT);
  pinMode(lane_3, OUTPUT);
	pinMode(lane_2, INPUT);
  pinMode(lane_2, OUTPUT);
	pinMode(lane_4, INPUT);
  pinMode(lane_4, OUTPUT);

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
  
	if( digitalRead(lane_1) == HIGH && lane_1_flag == true) {
		client.publish("lanes", "1");

		#ifdef DEBUG
		Serial.println("Publishing to lane topic, value 1");
		#endif

    lane_1_flag = false;
    lane_2_flag = true;
    lane_3_flag = true;
    lane_4_flag = true;
	}else if ( digitalRead(lane_2) == HIGH && lane_2_flag == true) {
    client.publish("lanes", "2");

    #ifdef DEBUG
    Serial.println("Publishing to lane topic, value 2");
    #endif

    lane_1_flag = true;
    lane_2_flag = false;
    lane_3_flag = true;
    lane_4_flag = true;
  }else if ( digitalRead(lane_3) == HIGH && lane_3_flag == true) {
    client.publish("lanes", "3");

    #ifdef DEBUG
    Serial.println("Publishing to lane topic, value 3");
    #endif

    lane_1_flag = true;
    lane_2_flag = true;
    lane_3_flag = false;
    lane_4_flag = true;
  }else if ( digitalRead(lane_4) == HIGH && lane_4_flag == true) {
    client.publish("lanes", "4");

    #ifdef DEBUG
    Serial.println("Publishing to lane topic, value 4");
    #endif

    lane_1_flag = true;
    lane_2_flag = true;
    lane_3_flag = true;
    lane_4_flag = false;
  }
	
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
