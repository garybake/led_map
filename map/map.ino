#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#include "config.h"

//IPAddress ip(192,168,0,30); // fixed IP address
//IPAddress gateway(192,168,0,255); // network gateway address
//IPAddress subnet(255,255,255,0); // network subnet mask

ESP8266WebServer server(8222);

const int PINCOUNT = 8;
int leds[] = {D0, D1, D2, D3, D4, D5, D7, D8};

void handleRoot() {
//    digitalWrite(LED01, 1);
    server.send(200, "text/plain", "hello from esp8266!");
//    digitalWrite(led, 0);
}

void handleNotFound(){
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
}

void handleImat(){

    String message = "";
    long locId = -1;
    int led;

    if (server.arg("location")== ""){     //Parameter not found
    
        message = "location Argument not found";
    
    } else {
        locId = server.arg("location").toInt();
        led = locId;
        switch (locId) {
            case 0:
                message = "At location0\n";
                break;
            case 1:
                message = "At location1\n";
                break;
            case 2:
                message = "At location2\n";
                break;
            case 3:
                message = "At location3\n";
                break;
            case 4:
                message = "At location4\n";
                break;
            case 5:
                message = "At location5\n";
                break;
            case 6:
                message = "At location6\n";
                break;
            case 7:
                message = "At location7\n";
                break;
            case 8:
                message = "At location8\n";
                break;
            case 9:
                message = "At location9\n";
                break;
            default:
                led = 0;
                message = "location Argument = ";
                message += server.arg("location");
        }

    }
    light_led(led);
    server.send(200, "text/plain", message);     //Returns the HTTP response

}

void light_led(int led){

    for (int i = 0; i < PINCOUNT; i++ ) {
        digitalWrite(leds[i], LOW);
    }

    digitalWrite(leds[led], HIGH);
}

void flashAll(){
    for (int i = 0; i < PINCOUNT; i++ ) {
        digitalWrite(leds[i], HIGH);
        delay(200);
        digitalWrite(leds[i], LOW);
    }
}

void printMac(){
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[5],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.println(mac[0],HEX);
}

void doConnect(){
    //WiFi.config(ip, gateway, subnet);
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
    printMac();
}

void setup(void){
    Serial.begin(115200);
//    delay(1000);

    for (int i = 0; i < PINCOUNT; i++ ) {
        Serial.println(i);
        pinMode(leds[i], OUTPUT);
    }

    doConnect();

    flashAll(); flashAll();

//    if (MDNS.begin("esp8266")) {
//        Serial.println("MDNS responder started");
//    }

    server.on("/", handleRoot);
    server.on("/imat", handleImat);

    server.on("/inline", [](){
        server.send(200, "text/plain", "this works as well\n");
    });

    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
}

void loop(void){
    server.handleClient();
}
