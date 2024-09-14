#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

Servo my_servo;

const char* ssid = "CatFeeder";
const char* password = "12345678";

const char* PARAM_1 = "input1";
const char* BUTTON_1 = "button1";

int position = 0;
int servo_pin = 18;

AsyncWebServer server(80);

const char WEB_PAGE[] PROGMEM = R"====( 
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>Feed Cat</title>
    </head>
    <body>
        Hello, Welcome to the Cat Feeder
        <form action = "/get">
          <input type = "text" name = "input1" placeholder = "How much would you like to feed your cat?>
          <input type = "submit">
        </form> <br>
        <button onclick = "send_button_xml('/button/on')" name = "button1">Click me to Spin</button>
        <button onclick = "send_button_xml('/button/off')" name = "">Click me to Stop Spinning</button>
    </body>
    
    <script>
        function send_button_xml(url){
            var xhr = new XMLHttpRequest();
            xhr.open("GET", url, true);
            xhr.onreadystatechange = function (){
                if (xhr.readyState == 4 && xhr.status == 200){
                    console.log(xhr.responseText)
                }
            };
            xhr.send();
        }
    </script>

</html>
)====";

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.println("Start Network");
  Serial.println(WiFi.softAPIP());

  //send webpage to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", WEB_PAGE);
  });

  //send input from user to esp32
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    String input_message;
    String input_param;

    if (request->hasParam(PARAM_1)){
      input_message = request->getParam(PARAM_1)->value();
      input_param = PARAM_1;
    }

    Serial.println(input_message);

    request->send(200, "text/plain", input_message + " amount of cat food send");
  });

  //send spin request
  server.on("/button/on", HTTP_GET, [](AsyncWebServerRequest *request){
    my_servo.attach(servo_pin);
    my_servo.write(180);
    request->send(200, "text/plain", "Servo spinning");
  });

  server.on("/button/off", HTTP_GET, [](AsyncWebServerRequest *request){
    my_servo.detach();
    request->send(200, "text/plain", "Servo spinning");
  });

  server.begin();
}

void loop() { 
  // put your main code here, to run repeatedly:
}

