//dodec12.ino
//https://github.com/foustja/Arduino/edit/master/dodec12.ino
//07/03/2018
//Milton Foust
//foustja@gmail.com
//
//Code has been tested successfully on the Arduino Leonardo and Adafruit Metro Mini, and
//allows control of LEDs embedded in BlinkyTile PCBs (https://blinkinlabs.com/). 
//Compiles and uploads with Arduino 1.8.5 running under Ubuntu 14.04.
//
//Credit needs to be given to to Daniel Garcia and Mark Kriegsman of the FastLED Project,
//(http://fastled.io/).
//
//I borrowed and re-worked a lot of code they provided in the FastLED documentation, which 
//was very instructive and saved me a great deal of time. 
 
#include <DmxSimple.h>
#include <FastLED.h>

#define DATA_PIN     0
#define NUM_LEDS    12
#define LED_TYPE    DMXSIMPLE

int dot;
int brite;

CRGB leds[NUM_LEDS];

void setup() 
    { 
    FastLED.addLeds<DMXSIMPLE, DATA_PIN>(leds, NUM_LEDS);
    }

void loop() 
    {
    for (dot = 0; dot < NUM_LEDS; dot++) 
        {
        for (brite = 0; brite <10; brite++)  
            {
            leds[dot].setRGB(brite * 3, brite * 2, brite * 1);
            FastLED.show(); 
            delay(5);
            }               
        }   
     for (dot = 0; dot < NUM_LEDS; dot++) 
        {  
        for (brite = 10; brite >= 0; brite--) 
            {
            leds[dot].setRGB(brite * 3, brite * 2, brite* 1);
            FastLED.show(); 
            delay(5);
            }
        }
    }

