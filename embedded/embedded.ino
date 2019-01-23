#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#include <Wire.h>
#include <Adafruit_BME280.h>

#include <ArduinoJson.h>

#define IP "http://192.168.43.251:3001/api/newreading"

ESP8266WiFiMulti WiFiMulti;

Adafruit_BME280 sensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin();

  if(!sensor.begin(0x76)){
    Serial.println("Error: Sensor not found");
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Pixel_3", "88888888");
}

void loop() {
  // put your main code here, to run repeatedly:
  StaticJsonBuffer<JSON_OBJECT_SIZE(4)> jbuffer;
  String output = "";
  JsonObject& data = jbuffer.createObject();

  data["name"] = "Teemu";
  data["temperature"] = sensor.readTemperature();
  data["pressure"] = sensor.readPressure();
  data["humidity"] = sensor.readHumidity();

  data.prettyPrintTo(Serial);
  Serial.println();
  data.printTo(output);
  
  if(WiFiMulti.run() == WL_CONNECTED){
    Serial.print("Connection established, local IP: ");
    Serial.println(WiFi.localIP());
    
    HTTPClient http;
    
    http.begin(IP);
    http.addHeader("Content-Type", "application/json");

    int httpcode = http.POST(output);

    if(httpcode == HTTP_CODE_OK){
      Serial.println("http post succesful");
    }else{
      Serial.print("http error: ");
      Serial.println(httpcode);
    }
  }else{
    Serial.println("Connecting...");
  }
   /*
  Serial.print("Temp: ");
  Serial.println(sensor.readTemperature());
  Serial.print("Humidity: ");
  Serial.println(sensor.readHumidity());
  Serial.print("Pressure: ");
  Serial.println(sensor.readPressure());
  */

  
  delay(5000);
}
