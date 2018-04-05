/********************************************************************/
// First we include the libraries
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
// #include <nRF24L01.h>
#include <RF24.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4
RF24 radio(7, 8); // CE, CSN
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/********************************************************************/
const byte addresses[][6] = {"00001", "00002"};
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MAX);
  sensors.begin();
}
int x = 0, AckfR, AckfT = 55, i;
float temp;
void loop()
{
  if (x == 0)
  {
    radio.stopListening();
    radio.write(&x, sizeof(x));
    x = 1;
  }

  radio.startListening();
  do {
    while (!radio.available());
    radio.read(&AckfR, sizeof(AckfR));
    Serial.println(AckfR);
  } while (AckfR != 55);
  AckfR = 0;
  radio.stopListening();
  /********************************************************************/
  //Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  /********************************************************************/
  //Serial.print("Temperature is: ");
  //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire
  temp = sensors.getTempCByIndex(0);
  /********************************************************************/
  radio.write(&temp, sizeof(temp));
  Serial.println(temp);
}

