#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "W04_44C3465CEA14";      //Wi-Fi Name
const char* password = "q19q6yhe2yna8b3";   // Wi-Fi Password
//the current condition of car (assigne 's' for STOP condition)
char actionStatus = 's';

ESP8266WebServer server(80);
Servo servo;

void setup() {
  //Set Serial Number
  Serial.begin(115200);
  
 
  servo.attach(2); //D4
  servo.write(0);
  delay(2000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to ");
  Serial.println(ssid);
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());

  //Default Page
  server.on("/", handleRoot); 
  //Feed Page
  server.on("/f", handle_feed);
  //Stop Page
  server.on("/s", handle_stop);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
String handleAction(char cs) {
  String message = "<!DOCTYPE html> <html>\n";
  message +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  message +="<style>html {text-align: center;}\n";
  message +=".button {display: block;width: 60px;margin: 0px auto 35px;}\n";
  message +="body{margin-top: 50px;} ";
  message +=".controlBtn {width:100%;}\n";
  message +="</style>\n";

  message += "<html> <head> <title>PI CAR</title><head>";
  message += "<body><h3>Ivory's Pet Feeder</h1>";

  if(cs == 'f')
  {
  message +="<h3>FEEDING</h3>\n";
  }

  message += "<table class=\"controlBtn\">\n";
  
  message += "<tr>";
  message += "<td> </td>";
  message += "<td><p><a href=\"/f\"><button style=\"width:10;height:10;font-size:30px;\">F E E D</button></a></p></td>";
  message += "<td> </td>";
  message += "<tr>";

  message += "<tr>";
  message += "<td> </td>";
  message += "<td><p><a href=\"/s\"><button style=\"width:10;height:10;font-size:30px;\">S T O P</button></a></p></td>";
  message += "<td> </td>";
  message += "<tr>";

  message += "</table> ";
  message += "</body></html>";
  return message;
}
void loop() {
  // put your main code here, to run repeatedly:


  server.handleClient();
  if(actionStatus == 'f' ){

  }
  if(actionStatus == 's' ){
    petFeedStop();
  }

}
void petFeed(){
  //Turn servo arm to 90 Degree
  servo.write(50);
  delay(300);
  //Turn back servo arm to 0 Degree
  servo.write(0);
  delay(1000);
}
void petFeedStop(){
  //Turn back servo arm to 0 Degree
  servo.write(0);
  delay(1000);
}
void handle_feed(){
  actionStatus = 'f';
  Serial.println("Feed");
  //Serial.println(Carstatus);
  petFeed();
  server.send(200, "text/html", handleAction(actionStatus));
}
void handle_stop(){
  actionStatus = 's';
  Serial.println("Stop");
  //Serial.println(Carstatus);
  server.send(200, "text/html", handleAction(actionStatus));
}
void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266 Pet Feeder!");

  Serial.println("STOP");
  //server.send(200, "text/html", handleCar(Carstatus));
}
void handleNotFound() {
  String message = "File Not Found ¥n¥n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
