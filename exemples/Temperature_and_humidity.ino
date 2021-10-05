#include <SoftwareSerial.h>
#include <dht.h>
#define DHT11_PIN 7

dht DHT;
int generalDelayTime = 600;
int generalCounter = 0;
int generalCounterLimit = 30;

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

void setup(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // DHT initial values
  updateDHTValues();
  Serial.println("Program started");
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
 
    // DHT
    updateDHTValues();
    dhtCounter += 1;
  }

  if (Serial.available()) {
    char command = Serial.read();
    if(command == 't') {
      Serial.println("========== temperature ==========");
      Serial.print("[");
      for (int i = 0; i < dhtMaxLimitArrayData; i = i + 1) {
        int item = temperatureData[i];
        if (item != 0) {
          Serial.print(item);
          Serial.print(",");
        }
      }
      Serial.print(temperature);
      Serial.println("]");
      Serial.println("==============================");
    }
    if(command == 'h') {
      Serial.println("========== humidity ==========");
      Serial.print("[");
      for (int i = 0; i < dhtMaxLimitArrayData; i = i + 1) {
        int item = humidityData[i];
        if (item != 0) {
          Serial.print(item);
          Serial.print(",");
        }
      }
      Serial.print(humidity);
      Serial.println("]");
      Serial.println("==============================");
    }
  }
  generalCounter += 1;
  delay(generalDelayTime);
}
