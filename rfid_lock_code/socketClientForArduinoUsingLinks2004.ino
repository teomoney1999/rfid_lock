#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <SoftwareSerial.h>
#include <SerialCommand.h>

#include <SocketIOclient.h>

#include <Hash.h>
#include <string>

SocketIOclient client;

ESP8266WiFiMulti WiFiMulti;
SerialCommand sCmd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  const char* ssid = "Hoi Lam Gi";
  const char* password = "tutimhieu";

  const char* host = "192.168.1.199";
  int port = 8266;

  Serial.print(F("Connecting to Wifi name: "));
  Serial.println(ssid);

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP(ssid, password);
  WiFi.begin(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println(F("\nConnect successfully to Wifi!"));
  Serial.print(F("IP address of ESP8266: "));
  Serial.println(WiFi.localIP().toString());

  client.begin(host, port, "/socket.io/?EIO=4");

  client.setReconnectInterval(100);

  client.onEvent(socketIOEvent);

  delay(10);

  //  sCmd.addDefaultHandler(defaultCommand);
  //  sCmd.addCommand("led-status", defaultCommand);
  sCmd.addCommand("rfid-code", rfidHandle);

}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();

  sCmd.readSerial();
}

void handlePayload(uint8_t* payload) {
  if (!payload) return;
  
  String sPayload = (char*) payload;

  // need to get tName and tExe
  String tName = sPayload.substring(sPayload.indexOf("[\"") + 2, sPayload.indexOf("\","));
  String tExe = sPayload.substring(sPayload.indexOf("\",") + 2, sPayload.length() - 1);

  // Sent to Arduino
  Serial.print(tName);
  Serial.print("\r");
  Serial.print(tExe);
  Serial.print("\r");
  //   Serial.flush();
}

void defaultCommand() {
  char* json = sCmd.next();
  DynamicJsonDocument jsonDoc(200);
  JsonArray docArray = jsonDoc.to<JsonArray>();
  docArray.add("led-status");

  JsonObject docParam = docArray.createNestedObject();
  docParam["status"] = (String) json;
  String docOutput;
  serializeJson(jsonDoc, docOutput);
  client.sendEVENT(docOutput);
}

void rfidHandle() {
  char* input = sCmd.next();
  
  if (!input) {
    return; 
  }
  
  StaticJsonDocument<150> doc;

  JsonArray docArray = doc.to<JsonArray>();
  docArray.add("rfid-code");
  docArray.add(input);

  char output[80];
  serializeJson(doc, output);
  client.sendEVENT(output);
  
}

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[IOc] Connected to url: %s\n", payload);
      // join default namespace (no auto join in Socket.IO V3)
      client.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      handlePayload(payload);
      break;
    case sIOtype_ACK:
      Serial.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      Serial.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      Serial.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}
