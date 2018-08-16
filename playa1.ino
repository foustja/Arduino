//playa1.ino
//https://github.com/foustja/Arduino/edit/master/IR24loop1alt.ino
//08/14/2018
//Milton Foust
//foustja@gmail.com
//
//This code has been tested successfully on the Arduino Leonardo, Adafruit Metro Mini,
//and Pololu A-Star 32U4 Prime LV. It allows IR remote control of WS2822S LEDs embedded 
//in BlinkyTile PCBs (https://blinkinlabs.com/). Compiles and uploads with Arduino 1.8.5 
//running under Ubuntu 14.04.
//
//Credit needs to be given to Daniel Garcia and Mark Kriegsman of the FastLED Project,
//(http://fastled.io/) and Chris Young, the creator of IRLib2 (https://github.com/cyborg5/IRLib2).
//I borrowed and modified some of the code they provided, which saved a great deal of time.
//
//Chris Young also has a great tutorial on IR communication at the Adafruit web site
//(https://learn.adafruit.com/using-an-infrared-library/overview).
//

#include <IRLibAll.h>
#include <DmxSimple.h>
#include <FastLED.h>
#define NUM_LEDS    12
#define LED_TYPE    DMXSIMPLE
#define TIME_INT    30
#define DATA_PIN    00
#define RECV_PIN    02
#define BLU_PIN     03

int dot;
int brite;
int arraySize;
int i;
long int control;
long int lastcontrol;
long int buttoncode;
long int controlArray[] = {0xffa25d, 0xffe21d, 0xff22dd, 0xffc23d, 0xffe01f, 0xff906f, 0xff6897, 0xffb04f, 0xff30cf, 0xff7a85};
unsigned long previousMillis; 
unsigned long currentMillis;


IRrecvPCI myReceiver(RECV_PIN);//Create a receiver object to listen on pin 2 (RECV_PIN)
IRdecodeNEC myDecoder;//Create a decoder object
CRGB leds[NUM_LEDS];

void irSignal();
void offleds();
void whiteleds();
void bluleds();
void grnleds();
void pulse(long int, float, float, float);
void blinkleds();

void setup()
{
  FastLED.addLeds<DMXSIMPLE, DATA_PIN>(leds, NUM_LEDS);
  myDecoder.value = 0xff6897;
  control =  myDecoder.value;
  lastcontrol = control;
  arraySize = 10;
  pinMode(BLU_PIN, OUTPUT); 
  previousMillis = 0; 
  currentMillis = 0;
}

void loop()
{ 
  irSignal();
  currentMillis = millis();

  switch (control)
  {
    case 0xffe21d://Off
      brite = 0;
      analogWrite(BLU_PIN, brite);    // turn off blue LEDs by making the voltage LOW
      offleds();                         // turn off BlinkyTile (WS2822S) LEDs
      break;

    case 0xffa25d://On
      whiteleds();
      break;

    case 0xff22dd://button 1
      bluleds();
      break;

    case 0xffc23d://button 2
      grnleds();
      break;

    case 0xffe01f://button 3
      blinkleds();
      break;

    case 0xff906f://button 4
      blinkleds();
      break;

    case 0xff6897://button 5
      pulse(0xff6897, 1.0, 1.0, 1.0);
      break;

    case 0xffb04f://button 6
      pulse(0xffb04f, 1.3, 1.3, 0.3);
      break;

    case 0xff30cf://button 7
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

void blinkleds()
{     
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
  while (brite <= 30)
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 30) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      { 
        analogWrite(BLU_PIN, brite);
        leds[dot].setRGB(blu * brite, grn * brite, red * brite);
        FastLED.show();
      }
      brite = brite + 1;
      previousMillis = currentMillis;previousMillis = currentMillis;    
    } 
    irSignal();
    if (control != buttoncode) break;   
  }

  while (brite > 0) 
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 30) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      {
        analogWrite(BLU_PIN, brite);
        leds[dot].setRGB(blu * brite, grn * brite, red * brite);
        FastLED.show();
      }
      brite = brite - 1;
      previousMillis = currentMillis;
    }
    irSignal();
    if (control != buttoncode) break;   
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
    leds[dot].setRGB(6, 6, 3);
    FastLED.show();
  }
}

void bluleds()
{
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(9, 3, 3);
    FastLED.show();
  }
}

void grnleds()
{
  for (dot = 0; dot < NUM_LEDS; dot++)
  {
    leds[dot].setRGB(3, 9, 3);
    FastLED.show();
  }
}

