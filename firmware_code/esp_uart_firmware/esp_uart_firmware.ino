/* This file is meant to be used with the FLOSS-Arduino toolbox*/

#include <WiFi.h>                    
const char* ssid     = "honor MediaPad T3";     //SSID of the WiFi network to which ESP32 has to be connected
const char* password = "nithin12";
WiFiServer server(80);
#define RXp2 16
#define TXp2 17
char c;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    digitalWrite(LED_BUILTIN,HIGH);//to turn on LED
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    while(1){
         WiFiClient client = server.available();   // listen for incoming clients
      
        if (client) {                             // if you get a client,
          Serial.println("New Client.");           // print a message out the serial port
          String currentLine = "";                // make a String to hold incoming data from the client
          while (client.connected()) {            // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
              char c = client.read();             // read a byte, then
              Serial.write(c);                    // print it out the serial monitor
              if (c == '\n') {                    // if the byte is a newline character
      
                // if the current line is blank, you got two newline characters in a row.
                // that's the end of the client HTTP request, so send a response:
                if (currentLine.length() == 0) {
                  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                  // and a content-type so the client knows what's coming, then a blank line:
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-type:text/html");
                  client.println();
      
                  // the content of the HTTP response follows the header:
                  while(Serial2.available()){
                    client.println(Serial2.readStringUntil(' '));
                    Serial2.read();
                    client.print("<br>");
                  }
                  // The HTTP response ends with another blank line:
                  client.println();
                  // break out of the while loop:
                  break;
                } else {    // if you got a newline, then clear currentLine:
                  currentLine = "";
                }
              } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;      // add it to the end of the currentLine
              }
            }
          }
          // close the connection:
          client.stop();
          Serial.println("Client Disconnected.");
        }
    }
}
