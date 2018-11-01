#include <FastLED.h>
#include <SoftwareSerial.h>

#define NUM_LEDS 60
#define LED_PIN 5

#define txPin 11
#define rxPin 10

CRGB leds[NUM_LEDS];
SoftwareSerial ble(rxPin, txPin);
uint8_t brightness = 50;
int redValue;
int greenValue;
int blueValue;
int isDeskLightOn = 0;

CRGB colorTemps[9] = {CRGB(255,147,41), CRGB(255,197,143), CRGB(255,214,170), CRGB(255,241,224), CRGB(255,250,244),
                       CRGB(255,255,251), CRGB(255,255,255), CRGB(201,226,255), CRGB(64,156,255) };
uint8_t colorTempPosition = 4;

# pragma mark - function prototypes

void showColors(CRGB color);
void processData2();
void toggleDeskLight(char cmd);

# pragma mark - main arduino methods

void setup() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    Serial.begin(9600);
    ble.begin(9600);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(brightness);

    int i;
    for (i = 0; i< NUM_LEDS; i++ ){
        leds[i] = CRGB(255,255,255);
        FastLED.show();
        delay(5);
    }
}

void loop() {
     processData2();
}

# pragma mark - helper methods

void handleLighting() {
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
    leds[i+1] = CRGB(0, 0, 0);
    leds[i] = CRGB(redValue, greenValue, blueValue);
    brightness --;
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(25);
  }
}

void toggleDeskLight(char cmd) {
    if (cmd == 'A') {
        isDeskLightOn = !isDeskLightOn;
        if (isDeskLightOn == 0) {
            ble.print("D-OF");
            FastLED.clear();
            FastLED.show();
        } else if (isDeskLightOn == 1) {
            ble.print("D-ON");
            showColors(CRGB::White);
        }
    }
}

void controlDeskLights(char cmd) {
    if (!isDeskLightOn) return;
    if (cmd == 'F') {//read color
        char obtainedColor[3];
        ble.readBytes(obtainedColor, 3);
        showColors(CRGB(byte(obtainedColor[0]),byte(obtainedColor[1]),byte(obtainedColor[2])));
    } if (cmd == 'B') { // up brightness
        brightness += 10;
        if (brightness > 100) brightness = 100;
        FastLED.setBrightness(brightness);
        FastLED.show();
        Serial.print("Upping brightness");
    } if (cmd == 'C') { // down brightness
        brightness -= 10;
        if (brightness < 0) brightness = 0;
        FastLED.setBrightness(brightness);
        FastLED.show();
        Serial.print("Reducing brightness");
    } if (cmd == 'D') { // up color temp
        colorTempPosition += 1;
        if (colorTempPosition > 8) colorTempPosition = 8;
        FastLED.setTemperature(colorTemps[colorTempPosition]);
        FastLED.show();
    } if (cmd == 'E') { // down color temp
        colorTempPosition -= 1;
        if (colorTempPosition < 0) colorTempPosition = 0;
        FastLED.setTemperature(colorTemps[colorTempPosition]);
        FastLED.show();
    } 
}

void showColors(CRGB color) {
    int i;
    for (i = 0; i< NUM_LEDS; i++ ){
        leds[i] = color;
        FastLED.show();
        delay(5);
    }
}

void processData2() {
      if (ble.available()) {
        char data = ble.read();
        controlDeskLights(data);
        toggleDeskLight(data);
      }
}


