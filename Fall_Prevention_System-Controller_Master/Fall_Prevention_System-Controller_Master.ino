#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#ifndef STASSID
#define STASSID "collabo"
#define STAPSK  "aguni2718"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

int controlFlag = 0;

ESP8266WebServer server(80);
StaticJsonDocument<200> doc;
SoftwareSerial controller(D7, D8);

void handleControll() {
  String request;
  for (uint8_t i = 0; i < server.args(); i++) {
    request += server.arg(i);
  }

  String response = "{\"res\":\"true\"}";
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "application/json", response);
  
  if(request != "") {
    Serial.print("Receive from Client: ");
    Serial.println(request);
    deserializeJson(doc, request);

    if(doc["code"] == 1) {
      if(controlFlag == 0) {
        Serial.println("Send to Slave: START");
        controller.write(1);
        controlFlag = 1;
      } else {
        Serial.println("Already started");
      }
    } else if(doc["code"] == 2) {
      if(controlFlag == 1) {
        Serial.println("Send to Slave: STOP");
        controller.write(2);
        controlFlag = 0;
      } else {
        Serial.println("Already stopped");
      }
    }
  }
}

void handleNotFound() {
  String response = "{\"error\":\"404 Not Found\"}";
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(404, "application/json", response);
}

void setup() {
  Serial.begin(9600);
  controller.begin(9600);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/control", handleControll);
  server.onNotFound(handleNotFound);
  
  server.enableCORS(true);
  server.begin();
  Serial.println("Server listening");
}
 
void loop() {
  server.handleClient();
}