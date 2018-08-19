#include <Adafruit_Sensor.h>
//Libraries DHT22
#include <DHT_U.h>
//Libraries LCD Screen
#include <LiquidCrystal.h>
//Libraries liquid temperature
#include <OneWire.h>
#include <DallasTemperature.h>


// Pin which is connected to the DHT sensor
#define DHT22PIN 13
// Select the sensor DHT11 DHT21 DHT22
#define DHTTYPE DHT22

//Time lapse DHT22 sensor
const int DHT22_lapse = 10000;
//Time lapse DS18B20 sensor
const int DS18B20_lapse = 10001;

//Water level Sensor variables
const int val1 = 60;
const int maxTemp = 40;
const int dryValue = 300;
const int fullValue = 990;
const int percentageDryValue = 100;
const int percentageWetValue = 0;
const int lapse = 3000;

// Pin which is connected to the DS18B20 sensor
const int dataPinDS18B20 = 11;

//Objects LCD Screen
LiquidCrystal lcd(7,6,5,4,3,2);
//Objects DHT22 sensor
DHT_Unified dht(DHT22PIN, DHTTYPE);
//Objects DS18B20 sensor
OneWire objectDS18B20(dataPinDS18B20);
DallasTemperature sensorDS18B20(&objectDS18B20);

uint32_t delayMS;

void sensorDHT22_ID(){
  // Print temperature sensor details.
  Serial.println("DHT22 Unified Sensor");
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 10000;
}

void environmentSensor(){
  // Set delay between sensor readings based on sensor details.
  // Get temperature event and print its value.
  // Delay between measurements.

  delay(DHT22_lapse);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.println();
    Serial.println("*** SENSOR DHT22 - AMBIENTE ***");
    Serial.print("Temperatura ambiente: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humedad relativaSensor Nivel: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }

}

void waterLevel() {

  int rawValue = 0;
  float level = 0;

  rawValue = analogRead(A1);

  //map(value, fromLow, fromHigh, toLow, toHigh)
  level = map(rawValue, dryValue, fullValue, percentageDryValue, percentageWetValue);
  //LCD prints soil humidity data

  Serial.println("*** SENSOR - NIVEL H2O TANQUE ***");
  Serial.print("Valor directo: ");
  Serial.print(rawValue);
  Serial.println();
  Serial.print("Nivel interpretado : ");
  Serial.print(level);
  Serial.println();
  lcd.clear();


  if( rawValue > 900 ) {

    lcd.print("Nivel bajo");
    lcd.setCursor(0, 1);
    lcd.blink();
    delay(300);
    lcd.print("Revisar tanque 1");

    //Programar encendido bomba

  }
  else  {
    lcd.print("Nivel del tanque:");
    lcd.setCursor(0,1);
    lcd.print(level);
    lcd.print("%");
  }

}

void waterTemperature(){

  Serial.println("*** SENSOR DS18B20 - TEMPERATURA H20 ***");
  sensorDS18B20.requestTemperatures();
  // Leemos y mostramos los datos de los sensores DS18B20
  Serial.print("Temperatura H20 sensor 1: ");
  //Reads the first sensor
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  Serial.println(" C");
  Serial.print("Temperatura H20 sensor 2: ");
  //Reads the second sensor
  Serial.print(sensorDS18B20.getTempCByIndex(1));
  Serial.println(" C");
  Serial.println();
  delay(DS18B20_lapse);

}

void setup() {

  Serial.begin(9600);
  // Initialize DHT22 Sensor device.
  dht.begin();
  //Runs Sensor ID Funtion once
  sensorDHT22_ID();
  // Initialize DS18B20 Sensor device.
  sensorDS18B20.begin();

}

void loop() {
  //Runs DHT22 data lecture function
  environmentSensor();
  waterLevel();
  waterTemperature();

}
