#include <SoftwareSerial.h>
#include <dht.h>
#define DHT11_PIN 7

SoftwareSerial BTSerial(10, 11); // RX | TX

char temperatureConstant = 't';
char humidityConstant = 'h';
char lightSensorConstant = 'l';
char soilHumiditySensorConstant = 's';
char LDRSensorConstant = 'd';
char command = 'z';

int lightSensorPIN = 8;
int lightSensorStatus = 0;

int outputPIN13 = 13;
int outputPWMPIN9 = 9;

int soilHumiditySensorPINA0 = A0;
int soilHumiditySensorValue = 0;

int LDRSensorPINA1 = A1;
int LDRSensorValue = 0;

int generalDelayTime = 600;
int generalCounter = 0;
int generalCounterLimit = 30;

dht DHT;
int dhtCounter = 0;
int dhtMaxLimitArrayData = 255;
int humidityData[256];
int temperatureData[256];
int temperature = -999;
int humidity = -999;

void updateDHTValues(){
  DHT.read11(DHT11_PIN);
  humidity = DHT.humidity;
  temperature = DHT.temperature;
  while( humidity==-999 or temperature==-999) {
    DHT.read11(DHT11_PIN);
    humidity = DHT.humidity;
    temperature = DHT.temperature;
  }
  humidityData[dhtCounter] = humidity;
  temperatureData[dhtCounter] = temperature;
}

void updateLightDetector(){
  lightSensorStatus = !digitalRead(lightSensorPIN);
}

void updateSoilHumiditySensor(){
  soilHumiditySensorValue = analogRead(soilHumiditySensorPINA0);
}

void updateLDRSensor(){
  LDRSensorValue = 1023 - analogRead(LDRSensorPINA1);
  int outputValue = map(LDRSensorValue, 0, 1023, 0, 255);
  analogWrite(outputPWMPIN9, outputValue);
}

void setup(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Bluetooth
  pinMode(outputPWMPIN9, OUTPUT);
  digitalWrite(outputPWMPIN9, HIGH);
  BTSerial.begin(38400);  // HC-05 default speed in AT command more

  // Light detector ON / OFF
  pinMode(lightSensorPIN,INPUT);
  pinMode(outputPIN13,OUTPUT);

  // DHT initial values
  updateDHTValues();

  // Light detector
  updateLightDetector();

  // Soil Humidity Sensor
  updateSoilHumiditySensor();

  // LDR Sensor Value
  updateLDRSensor();

  // Program ready
  Serial.println("{\"status\":\"Program ready\"}");
}

void selector(){
  if(command == temperatureConstant) {
    BTSerial.println(temperature);
  }
  if(command == humidityConstant) {
    BTSerial.println(humidity);
  }
  if(command == lightSensorConstant) {
    BTSerial.println(lightSensorStatus);
  }
  if(command == soilHumiditySensorConstant) {
    BTSerial.println(soilHumiditySensorValue);
  }
  if(command == LDRSensorConstant) {
    BTSerial.println(LDRSensorValue);
  }
}

void loop(){    
  if (dhtCounter >= dhtMaxLimitArrayData) {
    // Reset arrays
    Serial.println("Reset DHT arrays");
    for (int i = 0; i < dhtMaxLimitArrayData; i = i + 1) {
      temperatureData[i] = 0;
      humidityData[i] = 0;
    }
  }

  if (generalCounter >= generalCounterLimit) {
    generalCounter = 0;
 
    // Update DHT values
    updateDHTValues();
    dhtCounter += 1;
  }
  
  // Update Light detector state
  updateLightDetector();
  
  // Soil Humidity Sensor
  updateSoilHumiditySensor();

  // LDR Sensor Value
  updateLDRSensor();

  if (Serial.available()) {
    command = Serial.read();
    selector();
  }
  
  if (BTSerial.available()) {
    command = BTSerial.read();
    selector();
  }

  generalCounter += 1;
  delay(generalDelayTime);
}

/*
 * References
 * DHT 11 -> https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/
 * 
 * LDR -> https://www.circuitbasics.com/arduino-basics-controlling-led-part-2/
 * 
 * Light detector LM393 -> https://create.arduino.cc/projecthub/kiranpaul/light-magic-using-lm393-and-arduino-uno-14eadc
 * 
 */
