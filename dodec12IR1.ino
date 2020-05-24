//playa1.ino
//https://github.com/foustja/Arduino/edit/master/IR24loop1alt.ino
//08/14/2018
//Milton Foust
//foustja@gmail.com
//
//This code has been tested successfully on the Arduino Leonardo, Adafruit Metro Mini,
//and Pololu A-Star 32U4 Prime LV. It allows IR remote control of WS2822S LEDs embedded 
//in BlinkyTile PCBs (https://blinkinlabs.com/ using DMXSimple and IRLib2/IRLibAll.
//Displays light patterns in a 12 sided (dodecahedral) BlinkyTile polygon.
// 
//Compiles and uploads with Arduino 1.8.5 running under Ubuntu 14.04.
//
//Credit needs to be given to Daniel Garcia and Mark Kriegsman of the FastLED Project,
//(http://fastled.io/) and Chris Young, the creator of IRLib2 (https://github.com/cyborg5/IRLib2).
//I borrowed and modified some of the code they provided, which saved a great deal of time.
//
//Chris Young also has a great tutorial on IR communication at the Adafruit web site
//(https://learn.adafruit.com/using-an-infrared-library/overview).
// 
//Note that in order to upload this compiled sketch, you will need to be added to the dailout group:
//
//sudo usermod -a -G dialout <username>
//
//log out and then back in again
//

#include <IRLibAll.h>   //library for transmitting and receiving IR remote control codes
#include <DmxSimple.h>  //library for controlling WS2822S LEDs
#include <FastLED.h>    //library for LEDs
#define NUM_LEDS    12  //LEDs on a 12-sided BlinkyTile polygon
#define LED_TYPE    DMXSIMPLE
#define DATA_PIN    00  //Pin 0 controls BlinkyTile LEDs
#define RECV_PIN    02  //Pin 2 receives IR codes
#define BLU_PIN     03  //Pin 3 controls an array of LEDs on a separate board

int dot;
int brite;
int arraySize;
int i;
long int control;
long int lastcontrol;
long int buttoncode;    //code from IR receiver
long int controlArray[] = {0xffa25d, 0xffe21d, 0xff22dd, 0xffc23d, 0xffe01f, 
                           0xff906f, 0xff6897, 0xffb04f, 0xff30cf, 0xff7a85};
unsigned long previousMillis; 
unsigned long currentMillis;


IRrecvPCI myReceiver(RECV_PIN); //Create a receiver object to listen on pin 2 (RECV_PIN)
IRdecodeNEC myDecoder;         //Create a decoder object for NEC remote
CRGB leds[NUM_LEDS];           //Create a CRGB object                

void irSignal();
void offleds();
void whiteleds();
void bluleds();
void grnleds();
void violeds();
void pulse(long int, float, float, float);
void blinkleds();

void setup()
{
  FastLED.addLeds<DMXSIMPLE, DATA_PIN>(leds, NUM_LEDS);
  myDecoder.value = 0xff6897; //begin with code for button 5 on NEC remote
  control =  myDecoder.value;
  lastcontrol = control;
  arraySize = 10;
  pinMode(BLU_PIN, OUTPUT); 
  previousMillis = 0; 
  currentMillis = 0;
}

void loop()
{ 
  irSignal();  //capture IR signal
  //currentMillis = millis();  //check time

  switch (control)
  {
    case 0xffe21d://Off
      brite = 0;
      analogWrite(BLU_PIN, brite);  // turn off blue LEDs by making the voltage LOW
      offleds();                    // turn off BlinkyTile (WS2822S) LEDs
      break;

    case 0xffa25d://On
      analogWrite(BLU_PIN, 10);     // turn on blue LEDs by making voltage HIGH
      whiteleds();
      break;

    case 0xff22dd://button 1
      analogWrite(BLU_PIN, 10);
      bluleds();
      break;

    case 0xffc23d://button 2
      analogWrite(BLU_PIN, 10);
      grnleds();
      break;

    case 0xffe01f://button 3
      violeds();
      break;

    case 0xff906f://button 4
      blinkleds();
      break;

    case 0xff6897://button 5
      pulse(0xff6897, 0.8, 0.8, 0.8);
      break;

    case 0xffb04f://button 6
      pulse(0xffb04f, 0.8, 0.8, 1.3);
      break;

    case 0xff30cf://button 7calc
      pulse(0xff30cf, 1.6, 1.0, 0.1);
      break;
      
    case 0xff7a85://button 8
      pulse(0xff7a85, 1.0, 1.6, 0.1);
      break;
  }
}

void irSignal()
{

  myReceiver.enableIRIn();
  myReceiver.getResults();
  myDecoder.decode();

  for (i = 0; i < arraySize; i++)
  {
    if (controlArray[i] == myDecoder.value)
    {
      control = myDecoder.value;
      lastcontrol = control;
    }

    else
    {
      control = lastcontrol;
    }
  }
}

void offleds()
{ 
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(0, 0, 0);
    FastLED.show();
  }
}

void whiteleds()
{ 
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(10, 10, 10);
    FastLED.show();
  }
}

void bluleds()
{
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(60, 15, 5);
    FastLED.show();
  }
}

void grnleds()
{ 
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(15, 60, 5);
    FastLED.show();
  }
}

void violeds()
{ 
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(15, 15, 30);
    FastLED.show();
  }
}

void blinkleds()
{     
  currentMillis = millis();  
  if (currentMillis - previousMillis >= 1000) 
  {
    previousMillis = currentMillis;

    if (brite != 30)
    {
      brite = 30;
      analogWrite(BLU_PIN, brite);
    }
  
    else
    {
      brite = 10;
      analogWrite(BLU_PIN, brite);
    }
  }   
}

void pulse(long int buttoncode, float blu, float grn, float red)
{     
  brite == 0;
  
  while (brite < 30)
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 90) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      { 
        analogWrite(BLU_PIN, brite);
        leds[dot].setRGB(blu * brite, grn * brite, red * brite);
        FastLED.show();
      }
      brite++;
      previousMillis = currentMillis;    
    } 
    irSignal();
    if (control != buttoncode) break;   
  }

  while (brite == 30)
  {
   currentMillis = millis();  
    if (currentMillis - previousMillis >= 3000) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      { 
        analogWrite(BLU_PIN, brite);
        leds[dot].setRGB(blu * brite, grn * brite, red * brite);
        FastLED.show();
      }
      brite--;
      previousMillis = currentMillis;    
    } 
    irSignal();
    if (control != buttoncode) break;    
  }

  while (brite > 0 && brite < 30) 
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 90) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      {
        analogWrite(BLU_PIN, brite);
        leds[dot].setRGB(blu * brite, grn * brite, red * brite);
        FastLED.show();
      }
      brite--;
      previousMillis = currentMillis;
    }
    irSignal();
    if (control != buttoncode) break;   
  }   
}

