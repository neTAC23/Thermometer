/********************************************************************/
// First we include the libraries
#include <Arduino.h>
#include <TM1637Display.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define CLK 2
#define DIO 3
#define ONE_WIRE_BUS 4 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
TM1637Display display(CLK, DIO);
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
void setup(void) 
{ 
   int k;
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x63 };
  display.setBrightness(0x02);

  // All segments on
  display.setSegments(data);
  
  uint8_t segto=0x80; 
  display.setSegments(&segto, 1, 1);
 // start serial port 
 Serial.begin(9600); 
 //Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 sensors.begin(); 
} 
void loop(void) 
{ 
  float temp;
  int i, msb, lsb, mfraction, lfraction, count=0;
  if (count==0)
  {
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 //Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 //Serial.println("DONE"); 
/********************************************************************/
 //Serial.print("Temperature is: "); 
 //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   temp=sensors.getTempCByIndex(0); 

/*********convert temp to three digit and print**********/
   temp=temp*100;
   i=(int) temp;
  lfraction=i%10;
  mfraction=(i/10)%10;
  i=i/100;
  lsb=i%10;                      
  msb=i/10; 
  uint8_t data[] = {};
  data[0]= display.encodeDigit(msb); 
  data[1]= display.encodeDigit(lsb);
  data[2]= display.encodeDigit(mfraction);
  data[3]= display.encodeDigit(lfraction);
  display.setSegments(data);  
  count++; 
  } 
   if (count==1)
  {
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 //Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 //Serial.println("DONE"); 
/********************************************************************/
 //Serial.print("Temperature is: "); 
 //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   temp=sensors.getTempCByIndex(0); 

/*********convert temp to three digit and print**********/
   temp=temp*100;
   i=(int) temp;
  lfraction=i%10;
  mfraction=(i/10)%10;
  i=i/100;
  lsb=i%10;                      
  msb=i/10; 
  uint8_t data[] = {};
  data[0]= display.encodeDigit(msb); 
  data[1]= display.encodeDigit(lsb);
  data[2]= display.encodeDigit(mfraction);
  display.setSegments(data);
  data[0] = 0x63;
  display.setSegments(data, 1, 3);  
  count++; 
  }  
    if (count==2)
  {
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 //Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 //Serial.println("DONE"); 
/********************************************************************/
 //Serial.print("Temperature is: "); 
 //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   temp=sensors.getTempCByIndex(0); 

/*********convert temp to three digit and print**********/
   temp=temp*100;
   i=(int) temp;
  lfraction=i%10;
  mfraction=(i/10)%10;
  i=i/100;
  lsb=i%10;                      
  msb=i/10; 
  uint8_t data[] = {};
  data[0]= display.encodeDigit(msb); 
  data[1]= display.encodeDigit(lsb);
  data[2]= display.encodeDigit(mfraction);
  display.setSegments(data);
  data[0] = 0x58;
  display.setSegments(data, 1, 3);  
  count=0; 
  }              
}

