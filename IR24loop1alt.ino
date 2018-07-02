#include <IRLibAll.h>
#include <DmxSimple.h>
#include <FastLED.h>

#define DATA_PIN     0
#define NUM_LEDS    24
#define LED_TYPE    DMXSIMPLE

int RECV_PIN = 2;
int dot;
int brite;
int arraySize;
int i;
long int control;
long int lastcontrol;
long int controlArray[] = {0xffa25d, 0xffe21d, 0xff22dd, 0xffc23d, 0xffe01f, 0xff906f, 0xff6897, 0xffb04f, 0xff30cf, 0xff7a85};

IRrecvPCI myReceiver(2);//Create a receiver object to listen on pin 2
IRdecodeNEC myDecoder;//Create a decoder object    
CRGB leds[NUM_LEDS];

void irSignal();
void offleds();
void whiteleds();
void bluleds();
void grnleds();
void redleds();
void mixleds(int, int, int);
void ebbflow(int, int, int);

void setup() 
{
    FastLED.addLeds<DMXSIMPLE, DATA_PIN>(leds, NUM_LEDS);
    myDecoder.value = 0xffe21d;
    control =  myDecoder.value;
    lastcontrol = control;
    //brite = 15;
    arraySize = 10;
}

void loop() 
{    
    irSignal();
    switch(control)
    {
        case 0xffe21d://Off
        offleds();
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
        for (dot = 0; dot < NUM_LEDS; dot++) 
        redleds();
        break;

        case 0xff906f://button 4
        ebbflowleds(2, 2, 2);
        break;

        case 0xff6897://button 5
        ebbflowleds(3, 1, 1);
        break;
        
        case 0xffb04f://button 6
        for (dot = 0; dot < NUM_LEDS; dot++)
        ebbflowleds(3, 2, 1);
        break;

        case 0xff30cf://button 7
        ebbflowleds(3, 3, 0);
        break;

        case 0xff7a85://button 8
        ebbflowleds(2, 1, 2);
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
        leds[dot].setRGB(45, 45, 45);
        FastLED.show();  
    }
}

void bluleds()

{
    for (dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot].setRGB(45, 15, 15);
        FastLED.show();  
    } 
}

void grnleds()

{
    for (dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot].setRGB(15, 45, 15);
        FastLED.show();  
    } 
}

void redleds()

{
    for (dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot].setRGB(15, 15, 45);
        FastLED.show();  
    } 
}

void mixleds(long int blu, long int grn, long int red)

{
    for (dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot].setRGB(blu, grn, red);
        FastLED.show();  
    } 
}

void ebbflowleds(int blulevel, int grnlevel, int redlevel)

{
    for (dot = 0; dot < NUM_LEDS; dot++)
    {
        leds[dot].setRGB(0, 0, 0);
        FastLED.show();  
    } 
    for (dot = 0; dot < NUM_LEDS; dot++) 
    {
        for (brite = 0; brite <10; brite++)  
        {
            leds[dot].setRGB(brite * blulevel, brite * grnlevel, brite * redlevel);
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
            leds[dot].setRGB(brite * blulevel, brite * grnlevel, brite * redlevel);
            FastLED.show();                  
            delay(5);
        }
        irSignal();
        if (control == 0xffe21d) break;  
    }
}
