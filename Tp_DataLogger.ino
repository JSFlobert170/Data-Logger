#include "Seeed_BMP280.h"
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>
#include <Digital_Light_TSL2561.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 17


RTC_DS3231 rtc;
BMP280 bmp280;
unsigned long time;

int broche_CS = 10;
char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup()
{
  Wire.begin();
  sensors.begin();
  Serial.begin(9600);
  delay(500);
  Serial.println("TENTATIVE INITIALISATION SD : ");
  while(!SD.begin(broche_CS)) {
  Serial.println("SD ABSENTE,HS,OU MALE BRANCHEE...");
  delay(2000); 
}
  
  Serial.println("SD INITIALISEE");
  Serial.println(" ");
  bmp280.init();
  Serial.println("DATE;TEMPERATURE;PRESSION;LUMINOSITE;SONDE");
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    TSL2561.init();

    digitalWrite(15,HIGH);
}

void loop()
{
  DateTime now = rtc.now();
  uint32_t timestamp   = now.unixtime();
float    temperature = bmp280.getTemperature();
uint16_t pression    = bmp280.getPressure();
  time=millis();
//--------------------------------------------------------------------
  Serial.print( timestamp );
  Serial.print(" s");
  Serial.print(" ; ");
  Serial.print(temperature);
  Serial.print(" °C ; ");
  Serial.print(pression);
  Serial.print(" Pa ; ");
  Serial.print(TSL2561.readVisibleLux());
  Serial.print(" Lux ; ");
  sensors.requestTemperatures();
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" °C ");
//----------------------------------------------------------------------
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(' ');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.month(), DEC);
  Serial.print(' ');
  Serial.print(now.year(), DEC);
  Serial.print(" à ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);

String dataString = "";
  File dataFile = SD.open("new.txt", FILE_WRITE);
if (dataFile) {
    if (dataFile) {
    dataString +=timestamp;
    dataString += ';';
    dataString += temperature;
    dataString += ';';
    dataString += pression;
    dataString += ';';
    dataString += TSL2561.readVisibleLux();
    dataString += ';';
    dataString += sensors.getTempCByIndex(0);
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("ENREGISTREMENT SUR SD DE : ");
    Serial.println(dataString);
  }
  else {
    Serial.println("IMPOSSIBLE DE CREER LE FICHIER");
  }
}
  Serial.println("--------------------------------------------");
  delay(1000);

}
