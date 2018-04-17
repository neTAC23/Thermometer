/********************************************************************/
// First we include the libraries
#include <Arduino.h>
#include <TM1637Display.h>
#include <SPI.h>
// #include <nRF24L01.h>
#include <RF24.h>
#include <TimeLib.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define CLK 2
#define DIO 3
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
TM1637Display display(CLK, DIO);
RF24 radio(7, 8); // CE, CSN
/********************************************************************/
const byte addresses[][6] = {"00001", "00002"};
void setup() {
  int k;
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x63 };
  display.setBrightness(0x02);
  // All segments on
  display.setSegments(data);
  uint8_t segto = 0x80;
  display.setSegments(&segto, 1, 1);
  setTime(21,34,30,17,4,2018);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MAX);
}

int x = 0, AckfR = 55, AckfT;
float temp, last_temp;
int i, msb, lsb, mfraction, lfraction, count = 1;
float to;
bool timeout = false;
unsigned long started_waiting_at;
int timeout_counter = 0;
bool err = false;
char buffer [50];
int j, k;

// display the temperature on the display
void display_three_digits(float temp, int counter){
  int i, msb, lsb, mfraction, lfraction;
  temp = temp * 100;
  i = (int) temp;
  lfraction = i % 10;
  mfraction = (i / 10) % 10;
  i = i / 100;
  lsb = i % 10;
  msb = i / 10;
  uint8_t data[] = {};
  data[0] = display.encodeDigit(msb);
  data[1] = 0x80 | display.encodeDigit(lsb);
  data[2] = display.encodeDigit(mfraction);
  if (count == 1){
    data[3] = display.encodeDigit(lfraction);
    display.setSegments(data);
  }
  if (count == 2){
    display.setSegments(data);
    data[0] = 0x63;
    display.setSegments(data, 1, 3);
  }
  if (count == 3){
    display.setSegments(data);
    data[0] = 0x58;
    display.setSegments(data, 1, 3);
    count = 0;
  }
}

void display_err(){
  uint8_t err_data[] = {0x79, 0x77, 0x77, 0x0};
  display.setSegments(err_data);
}

// main loop
void loop()
{
  if (x == 0)
  {
    radio.stopListening();
    radio.write(&AckfR, sizeof(AckfR));
    x = 1;
    timeout = false;
    temp = last_temp;
  }
  radio.startListening();
  started_waiting_at = millis();
  while (!radio.available() && !timeout) {
    if (millis() - started_waiting_at > 1000) {
      timeout = true;
    }
  }
  if (timeout) {
    x = 0;
    Serial.print("got timeout at ");
    k=sprintf (buffer, "%d/%d %d:%d:%d\n",day(), month(), hour(), minute(), second());
    for(int l= 0; l<=k; l++) 
      Serial.print(buffer[l]);
    temp = last_temp;
    timeout_counter++;
  }
  else {
    radio.read(&temp, sizeof(temp));
    last_temp = temp;
    timeout_counter = 0;
    if(err){
      err = false;
      count = 0; 
    }
    j=sprintf (buffer, "%d/%d %d:%d:%d",day(), month(), hour(), minute(), second());
    for(int l= 0; l<=j; l++) 
      Serial.print(buffer[l]);
    Serial.print(", ");
    Serial.println (temp);
  }

  if (timeout_counter > 1){
    display_err();
    err = true; 
  }
  else{
    
    display_three_digits(temp, count);
  }

  count++;
  radio.stopListening();
  radio.write(&AckfR, sizeof(AckfR));
}
