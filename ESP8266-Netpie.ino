#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = "pondthaitay";
const char* password = "55021744";

#define APPID   "20scoopsSmartHome"
#define KEY     "9m6JZvFwYd42YWR"
#define SECRET  "tRH075AnfqAi6Z2XY62TyDyjZ"
#define ALIAS   "claw_machine"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> topic : " + (String) topic + " ");
  msg[msglen] = '\0';
  controlPins((char)msg[0] == '0', (char)msg[0]);
  Serial.println((char)msg[0]);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() {
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);

  /* Add Event listeners */
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);
  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, onFoundgear);
  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, onLostgear);
  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);
  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}

void loop() {
  if (microgear.connected()) {
    microgear.loop();
  }
  else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}

void controlPins(bool flag, char pin) {
  if (!flag) {
    if (pin == '1') {
      digitalWrite(0, HIGH);
    }
    else if (pin == '2') {
      digitalWrite(2, HIGH);
    }
    else if (pin == '3') {
      digitalWrite(15, HIGH);
    }
    else if (pin == '4') {
      digitalWrite(4, HIGH);
    }
    else if (pin == '5') {
      digitalWrite(5, HIGH);
    }
    else if (pin == '6') {
      digitalWrite(5, LOW);
      digitalWrite(13, HIGH);
    }
  } else {
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(15, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
  }
}
