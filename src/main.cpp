#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "CatFeeder";
const char* password = "11111";

WebServer server(80);

const char WEB_PAGE[] PROGMEM = R"====( 
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>Feed Cat</title>
    </head>
    <body>
        Hello
        <button>Feed Cat</button>
        <input placeholder="How many grams would you like to feed the cat">
        <button onclick="window.location = 'http://' + location.hostname + '/feedcat'" type="submit">Submit</button>
    </body>
</html>
)====";

void send_website(){
  Serial.println("Sending Website");
  server.send(200, "text/html", WEB_PAGE);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.println("Start Network");
  Serial.println(WiFi.softAPIP());

  server.on("/", send_website);

  server.begin();
}

void loop() { 
   server.handleClient(); 
  // put your main code here, to run repeatedly:
}

