// Circuit Playground Express

#include <Adafruit_NeoPixel.h>

#define PIN 8
#define NUM_LEDS    10

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

uint32_t color  = 0xCC99FF; // Start violet
uint32_t brite;
uint32_t dot;

unsigned long previousMillis; 
unsigned long currentMillis;

void setup() 
{
  brite = 0; //start off
  color  = 0xCC99FF; // start violet
  pixels.begin();
  pixels.setBrightness(brite); // brightness
}

void loop() 
{
  pulse();
}

void pulse()
{     
  brite == 0;
  
  while (brite < 30)
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 90) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      { 
        pixels.setBrightness(brite); // brightness
        pixels.setPixelColor(dot, color);
        pixels.show();
      }
      brite++;
      previousMillis = currentMillis;    
    }  
  }

  while (brite == 30)
  {
   currentMillis = millis();  
    if (currentMillis - previousMillis >= 9000) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      { 
        pixels.setBrightness(brite); // brightness
        pixels.setPixelColor(dot, color);
        pixels.show();
      }
      brite--;
      previousMillis = currentMillis;    
    } 
  }

  while (brite > 0 && brite < 30) 
  { 
    currentMillis = millis();  
    if (currentMillis - previousMillis >= 90) 
    {
      for (dot = 0; dot < NUM_LEDS; dot++) 
      {
        pixels.setBrightness(brite); // brightness
        pixels.setPixelColor(dot, color);
        pixels.show();
      }
      brite--;
      previousMillis = currentMillis;
    }
  }
  color >>= 8;                 // Next color R->G->B
  if(!color) color = 0xCC99FF; // Reset to violet
}


