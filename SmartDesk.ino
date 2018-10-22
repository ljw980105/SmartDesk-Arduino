#include <FastLED.h>

#define NUM_LEDS 60
#define LED_PIN 5

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(0);
}

int redValue;
int greenValue;
int blueValue;

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  int i;
  int brightness = 0;
  for (i = 0; i< NUM_LEDS; i++ ){
    redValue = random(0,255);
    greenValue = random(0,255);
    blueValue = random(0,255);
    leds[i] = CRGB(redValue, greenValue, blueValue);
    brightness ++;
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(25);
  }

  for (i = NUM_LEDS - 1; i >= 0; i-- ){
    redValue = random(0,255);
    greenValue = random(0,255);
    blueValue = random(0,255);
    leds[i] = CRGB(redValue, greenValue, blueValue);
    brightness --;
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(25);
  }
}


