// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>
//灯带测试程序
///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 160

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3
#define CLOCK_PIN 13

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the ledsand tells the controller about them
void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(1000);
    FastLED.setBrightness( 100 );//设置亮度
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
   
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop() {
   for(int i=0;i<NUM_LEDS;i++)
   {
        leds[i] = CRGB::Red;
   }
   FastLED.show();
   delay(1000);

    for(int i=0;i<NUM_LEDS;i++)
   {
        leds[i] = CRGB::Green;
   }
   FastLED.show();
   delay(1000);


    for(int i=0;i<NUM_LEDS;i++)
   {
        leds[i] = CRGB::Blue;
   }
   FastLED.show();
   delay(1000);

   for(int i=0;i<NUM_LEDS;i++)
   {
        leds[i] = CRGB::White;
   }
   FastLED.show();
   delay(1000);

   for(int i=0;i<NUM_LEDS;i++)
   {
        leds[i] = CRGB::Black;
   }
   FastLED.show();
   delay(1000);

   
}
