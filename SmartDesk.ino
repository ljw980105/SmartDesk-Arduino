#include <FastLED.h>
#include <SoftwareSerial.h>
#include "OutgoingCommand.h"
#include "IncomingCommands.h"

#define WB_NUM_LEDS 32
#define WB_LIGHT_PIN 5
CRGB wb_leds[WB_NUM_LEDS];
uint8_t wb_colorTempPosition = 4;

#define DK_NUM_LEDS 60
#define DK_LIGHT_PIN 6
CRGB dk_leds[DK_NUM_LEDS];
uint8_t dk_colorTempPosition = 4;

#define txPin 11
#define rxPin 10

SoftwareSerial ble(rxPin, txPin);
int redValue;
int greenValue;
int blueValue;

int isWhiteboardLightOn = 0;
uint8_t whiteboardLightBrightness = 50;
int isDeskLightOn = 0;
uint8_t deskLightBrightness = 50;

CRGB colorTemps[9] = {CRGB(255,147,41), CRGB(255,197,143), CRGB(255,214,170), CRGB(255,241,224), CRGB(255,250,244),
                       CRGB(255,255,251), CRGB(255,255,255), CRGB(201,226,255), CRGB(64,156,255) };

# pragma mark - function prototypes

void showColors(int numPins, CRGB type[], CRGB color);
void toggleWhiteboardLight(char cmd);
void controlWhiteboardLight(char cmd);
void toggleDeskLight(char cmd);
void controlDeskLight(char cmd);

# pragma mark - main arduino methods

void setup() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    Serial.begin(9600);
    ble.begin(9600);

    FastLED.addLeds<WS2812B, WB_LIGHT_PIN, GRB>(wb_leds, WB_NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(whiteboardLightBrightness);
    FastLED.addLeds<WS2812B, DK_LIGHT_PIN, GRB>(dk_leds, DK_NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(deskLightBrightness);

    int i;
    for (i = 0; i< WB_NUM_LEDS; i++ ){
        wb_leds[i] = CRGB(255,255,255);
        FastLED.show();
        delay(5);
    }
}

void loop() {
    if (ble.available()) {
        char data = ble.read();
        Serial.print(data);
        toggleWhiteboardLight(data);
        controlWhiteboardLight(data);
        toggleDeskLight(data);
        controlDeskLight(data);
    }
}

# pragma mark - helper methods

void handleLighting() {
    int i;
  int brightness = 0;
  for (i = 0; i< WB_NUM_LEDS; i++ ){
    redValue = random(0,255);
    greenValue = random(0,255);
    blueValue = random(0,255);
    wb_leds[i] = CRGB(redValue, greenValue, blueValue);
    brightness ++;
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(25);
  }

  for (i = WB_NUM_LEDS - 1; i >= 0; i-- ){
    redValue = random(0,255);
    greenValue = random(0,255);
    blueValue = random(0,255);
    wb_leds[i+1] = CRGB(0, 0, 0);
    wb_leds[i] = CRGB(redValue, greenValue, blueValue);
    brightness --;
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(25);
  }
}

void showColors(int numPins, CRGB type[],CRGB color) {
    int i;
    for (i = 0; i< WB_NUM_LEDS; i++ ){
        type[i] = color;
        FastLED.show();
        delay(5);
    }
}

# pragma mark - whiteboard light methods

void toggleWhiteboardLight(char cmd) {
    if (cmd == incoming_whiteboardLightToggle) {
        isWhiteboardLightOn = !isWhiteboardLightOn;
        if (isWhiteboardLightOn == 0) {
            ble.print(outgoing_whiteboardLigihtOff);
            FastLED.clear();
            FastLED.show();
        } else if (isWhiteboardLightOn == 1) {
            ble.print(outgoing_whiteboardLigihtOn);
            showColors(WB_NUM_LEDS, wb_leds, CRGB::White);
        }
    }
}

void controlWhiteboardLight(char cmd) {
    if (!isWhiteboardLightOn) return;
    if (cmd == incoming_whiteboardLightColor) {//read color
        char obtainedColor[3];
        ble.readBytes(obtainedColor, 3);
        CRGB newColor = CRGB(byte(obtainedColor[0]),byte(obtainedColor[1]),byte(obtainedColor[2]));
        showColors(WB_NUM_LEDS, wb_leds, newColor);
    } if (cmd == incoming_whiteboardLightUpBrightness) { // up brightness
        whiteboardLightBrightness += 10;
        if (whiteboardLightBrightness > 100) whiteboardLightBrightness = 100;
        FastLED.setBrightness(whiteboardLightBrightness);
        FastLED.show();
        Serial.print("Upping brightness");
    } if (cmd == incoming_whiteboardLightReduceBrightness) { // down brightness
        whiteboardLightBrightness -= 10;
        if (whiteboardLightBrightness < 0) whiteboardLightBrightness = 0;
        FastLED.setBrightness(whiteboardLightBrightness);
        FastLED.show();
        Serial.print("Reducing brightness");
    } if (cmd == incoming_whiteboardLightUpColorTemp) { // up color temp
        wb_colorTempPosition += 1;
        if (wb_colorTempPosition > 8) wb_colorTempPosition = 8;
        FastLED.setTemperature(colorTemps[wb_colorTempPosition]);
        FastLED.show();
    } if (cmd == incoming_whiteboardLightReduceColorTemp) { // down color temp
        wb_colorTempPosition -= 1;
        if (wb_colorTempPosition < 0) wb_colorTempPosition = 0;
        FastLED.setTemperature(colorTemps[wb_colorTempPosition]);
        FastLED.show();
    } 
}

# pragma mark - desk light methods

void toggleDeskLight(char cmd) {
    if (cmd == incoming_deskLightToggle) {
        isDeskLightOn = !isDeskLightOn;
        if (isDeskLightOn == 0) {
            ble.print(outgoing_deskLightOff);
            FastLED.clear();
            FastLED.show();
        } else if (isDeskLightOn == 1) {
            ble.print(outgoing_deskLightOn);
            showColors(DK_NUM_LEDS, dk_leds, CRGB::White);
        }
    }
}

void controlDeskLight(char cmd) {
    if (!isDeskLightOn) return;
    if (cmd == incoming_deskLightColor) {//read color
        char obtainedColor[3];
        ble.readBytes(obtainedColor, 3);
        CRGB newColor = CRGB(byte(obtainedColor[0]),byte(obtainedColor[1]),byte(obtainedColor[2]));
        showColors(DK_NUM_LEDS, dk_leds, newColor);
    } if (cmd == incoming_deskLightUpBrightness) { // up brightness
        deskLightBrightness += 10;
        if (deskLightBrightness > 100) deskLightBrightness = 100;
        FastLED.setBrightness(deskLightBrightness);
        FastLED.show();
        Serial.print("Upping brightness");
    } if (cmd == incoming_deskLightReduceBrightness) { // down brightness
        deskLightBrightness -= 10;
        if (deskLightBrightness < 0) deskLightBrightness = 0;
        FastLED.setBrightness(deskLightBrightness);
        FastLED.show();
        Serial.print("Reducing brightness");
    } if (cmd == incoming_deskLightUpColorTemp) { // up color temp
        dk_colorTempPosition += 1;
        if (dk_colorTempPosition > 8) dk_colorTempPosition = 8;
        FastLED.setTemperature(colorTemps[dk_colorTempPosition]);
        FastLED.show();
    } if (cmd == incoming_deskLightReduceColorTemp) { // down color temp
        dk_colorTempPosition -= 1;
        if (dk_colorTempPosition < 0) dk_colorTempPosition = 0;
        FastLED.setTemperature(colorTemps[dk_colorTempPosition]);
        FastLED.show();
    }
}
