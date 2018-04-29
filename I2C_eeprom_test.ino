#include "Arduino.h"
#include <string.h>
#include <stdio.h>
#include "eepromi2c.h"

#define I2C_CLOCK 100000

int n = 0;

//for Serial input
short eeBS;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

#pragma pack(push, 1)//force unaligned data (16 bit / 32 bit data sharing compatibility); __attribute__((packed, aligned(1))) ignored for some reason
struct eespool
{
  int16_t ID;
  float wireDiam;//mm
  float spoolLength;//m
  float used;//used length (m)
  int16_t savecnt;
} eespool;
#pragma pack(pop)

void setup() {
  SerialUSB.begin(115200);  // start serial for output
  while(!SerialUSB){
  ; // wait for serial port to connect.
  }
  SerialUSB.println("Master connected");
  I2C.attachReadCallback(read_cb);
  I2C.begin(100000,REG_ADDR_16BIT);//start i2c communication
   
  inputString.reserve(2);

  //load last used spool
  eeBS = eeRead(0,eespool);//returns block size
  SerialUSB.print("eeBS");
  SerialUSB.println(eeBS);
  SerialUSB.print("eespool size");
  SerialUSB.println(sizeof(eespool));    

  serialOut(0);
}

void loop() {
  if(stringComplete){
    if(inputString=="r\r"){
      NVIC_SystemReset();      // processor software reset
    }
    if(inputString=="d\r"){
      n++;
      serialOut(n);
    }
    inputString = "";
    stringComplete = false;
  }
}

void serialOut(int n){
  eeRead(n*eeBS,eespool);
}

void read_cb(){
  SerialUSB.print("n");
  SerialUSB.println(n);  
  SerialUSB.print("ID:         ");
  SerialUSB.println(eespool.ID);
  SerialUSB.print("wireDiam:   ");
  SerialUSB.println(eespool.wireDiam);
  SerialUSB.print("spoolLength:");
  SerialUSB.println(eespool.spoolLength);
  SerialUSB.print("used:       ");
  SerialUSB.println(eespool.used);
  SerialUSB.print("savecnt:    ");
  SerialUSB.println(eespool.savecnt);  
}

//get serial commands
void serialEvent(){
  int se_cnt = 0;
  while(SerialUSB.available()){
    // get the new byte:
    char inChar =(char)SerialUSB.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if(inChar == '\r'){
      stringComplete = true;
    }
    se_cnt++;
    if(se_cnt>50){
      se_cnt=0;
      break;
    }
  }
}

//serial event for SAMD
void serialEventRun(void){
  if(SerialUSB.available()) serialEvent();
}
