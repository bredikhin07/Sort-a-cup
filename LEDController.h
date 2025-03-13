#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <FastLED.h>
#include "Config.h"

class LEDController {
private:
    CRGB leds[LED_NUM_LEDS];
    bool isEnabled;
	CRGB _color;
	byte _brightness;
public:
    LEDController();
    void init();
	void setBrightness(byte brightness);
	void setColorCRGB(CRGB color);
    void setColorRGB(uint8_t r, uint8_t g, uint8_t b);
    void setColorHex(uint32_t colorcode);
    void update();
    // Методы включения и выключения ленты
    void enable();
    void disable();
};

#endif // LEDCONTROLLER_H
