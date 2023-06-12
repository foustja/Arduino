//dodec34irSignal
//https://github.com/foustja/Arduino/edit/master/docec34irSignal.ino
//06.11.2023
//Milton Foust
//foustja@gmail.com
//
//This code has been tested successfully on the Arduino Leonardo, Adafruit Metro Mini,
//and Pololu A-Star 32U4 Prime LV. It allows IR remote control of WS2822S LEDs embedded 
//in BlinkyTile PCBs (https://blinkinlabs.com/ using DMXSimple and IRLib2/IRLibAll.
//Displays light patterns in a 34 sided (multi-dodecahedral) BlinkyTile polygon.
// 
//Compiles and uploads with Arduino 1.8.19 running under Ubuntu 22.04.
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


#include <IRLibAll.h>
#include <DmxSimple.h>
#include <FastLED.h>

#define DATA_PIN     0
#define NUM_LEDS    34
#define LED_TYPE    DMXSIMPLE

int RECV_PIN = 2;
int dot;
int brite;
int arraySize;
int i;
long int control;
long int lastcontrol;
long int controlArray[] = {0xffa25d, 0xffe21d, 0xff22dd, 0xffc23d, 0xffe01f, 0xff906f, 0xff6897, 0xffb04f};


IRrecvPCI myReceiver(2);//Create a receiver object to listen on pin 2
IRdecodeNEC myDecoder;//Create a decoder object    
CRGB leds[NUM_LEDS];

void setup() 
{
    FastLED.addLeds<DMXSIMPLE, DATA_PIN>(leds, NUM_LEDS);
    myDecoder.value = 0xffe21d;
    control =  myDecoder.value;
    lastcontrol = control;
    brite = 15;
    arraySize = 8;
}

void loop() 
{    
    irSignal();
    switch(control)
    {
        case 0xffe21d://Off
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(0, 0, 0);
            FastLED.show();  
        } 
        break;
            
        case 0xffa25d://On
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(45, 45, 45);
            FastLED.show();  
        }
        break;

        case 0xff22dd://button 1
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(45, 15, 15);
            FastLED.show();  
        }
        break;

        case 0xffc23d://button 2
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(45, 5, 45);
            FastLED.show();  
        }
        break;

        case 0xffe01f://button 3 
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(45, 45, 15);
            FastLED.show();  
        }
        break;

        case 0xff906f://button 4
        for (dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].setRGB(0, 0, 0);
            FastLED.show();  
        } 
        for (dot = 0; dot < NUM_LEDS; dot++) 
        {
            for (brite = 0; brite <10; brite++)  
            {
                leds[dot].setRGB(brite * 3, brite, brite);
                FastLED.show(); 
                delay(5);
            }
            irSignal();
            if (control == 0xffe21d) break;               
        }   
        for (dot = NUM_LEDS - 1; dot >= 0; dot--) 
        {  
            for (brite = 10; brite >= 0; brite--) 
            {
                leds[dot].setRGB(brite * 3, brite, brite);
                FastLED.show();                  
                delay(5);
            }
            irSignal();
            if (control == 0xffe21d) break;  
        }
        break;

        case 0xff6897://button 5

        for (brite = 0; brite <10; brite++) 
            {
                for (dot = 0; dot < NUM_LEDS; dot++) 
                    {
                        leds[dot].setRGB(brite * 3, brite, brite);

                        irSignal();
                        if (control == 0xffe21d) break;                 
                    } 
                    
                FastLED.show(); 
                delay(30);
            }
            
        delay (3000);
            
        for (brite = 10; brite > 0; brite--) 
            {
                for (dot = 0; dot < NUM_LEDS; dot++) 
                    {
                        leds[dot].setRGB(brite * 3, brite, brite);
                        
                        irSignal();
                        if (control == 0xffe21d) break;
                    }
                    
                FastLED.show(); 
                delay(30);
            }

        delay (1000);    
            
        break;

        case 0xffb04f://button 6
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
