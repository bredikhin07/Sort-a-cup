#include "LEDController.h"

LEDController::LEDController() : isEnabled(true) {
}

void LEDController::init() {
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, LED_NUM_LEDS);
    setBrightness(80);
	isEnabled = true;
	setColorRGB(255, 244, 229);           // изначальный цвет – тёплый белый
}

void LEDController::setBrightness(byte brightness) {
	_brightness = brightness;
	//FastLED.setBrightness(_brightness);
	update();
}

void LEDController::setColorCRGB(CRGB color){
	_color = color;
	for (int i = 0; i < LED_NUM_LEDS; i++) {
        leds[i] = _color;
    }
	update();
}

void LEDController::setColorRGB(uint8_t r, uint8_t g, uint8_t b) {
    CRGB color = CRGB(r, g, b);
	setColorCRGB(color);
}


void LEDController::setColorHex(uint32_t colorcode) {
	CRGB color = CRGB(colorcode);
	setColorCRGB(color);
}

void LEDController::update() {
    if (isEnabled) {
		FastLED.setBrightness(_brightness);
        FastLED.show();
    }
}

void LEDController::enable() {
    isEnabled = true;
	setColorCRGB(_color);
	FastLED.setBrightness(_brightness);
    update();
}

void LEDController::disable() {
    isEnabled = false;
    for (int i = 0; i < LED_NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
