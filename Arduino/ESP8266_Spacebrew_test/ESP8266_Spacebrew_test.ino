#include <SPI.h>
#include <Spacebrew.h>
#include <Ethernet.h>
#include <WebSocketsClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>


// docs here: https://github.com/mattfelsen/spacebrew-esp8266

Spacebrew sb;
// http://192.168.1.96:9000/?server=192.168.1.96
String sb_host = "192.168.1.96";

const char* ssid     = "PILGRIMAGE_25";
const char* password = "";
 


void setup() {
    Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.println("starting");


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
//5c:CF:7F:3a:db:cf

  uint8_t mac[] = {0x5c, 0xcf, 0x74, 0x3a, 0xdb, 0xcf};
  Ethernet.begin(mac);
  
  //Ethernet.start(mac);//as per the Ethernet library example, where mac is a byte[6]
  sb.connect("192.168.1.96", "ESP8266_1", "Inputs and Outputs?");
  sb.onStringMessage(receivedString);
  sb.addPublish("EntryDetected", "string");
  sb.addSubscribe("SubStringName", "string");

  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
}


void loop() {
  // put your main code here, to run repeatedly:
  sb.monitor();
    sb.send("EntryDetected", "entry");

  delay(100);

}

void receivedString(char* name, char* value){
    Serial.print("[onStringMessage]");
    Serial.print(name);
    Serial.print(": ");
    Serial.println(value);
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  sb.send("EntryDetected", "entry");

  
}

