#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#include "config.h"

ESP8266WebServer server(80);

IPAddress ip(192,168,0,21); // fixed IP address
IPAddress gateway(192,168,0,255); // network gateway address
IPAddress subnet(255,255,255,0); // network subnet mask

const int led = 13;

const int LED01 = D5;
const int LED02 = D6;
const int LED03 = D7;

void handleRoot() {
    digitalWrite(LED01, 1);
    server.send(200, "text/plain", "hello from esp8266!");
    digitalWrite(led, 0);
}

void handleNotFound(){
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led, 0);
}

void handleImat(){
//    digitalWrite(LED01, LOW);
    String message = "";
    long locId = -1;
    int led = LED01;

    if (server.arg("location")== ""){     //Parameter not found
    
        message = "location Argument not found";
    
    } else {
        locId = server.arg("location").toInt();
        switch (locId) {
            case 1:
                message = "At location1";
                led = LED01;
                break;
            case 2:
                message = "At location2";
                led = LED02;
                break;
            case 3:
                message = "At location3";
                led = LED03;
                break;
            default:
                message = "location Argument = ";
                message += server.arg("location");
        }

    }
    digitalWrite(led, LOW);
    server.send(200, "text/plain", message);     //Returns the HTTP response
    
    delay(1000);
    digitalWrite(LED01, HIGH);
}

void setup(void){
    pinMode(LED01, OUTPUT);
    digitalWrite(LED01, HIGH);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

//    if (MDNS.begin("esp8266")) {
//        Serial.println("MDNS responder started");
//    }

    server.on("/", handleRoot);
    server.on("/imat", handleImat);

    server.on("/inline", [](){
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
}

void loop(void){
    server.handleClient();
}
