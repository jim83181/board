#include "Arduino.h"
#include "SharedConstants.h"
#include "CellData.h"
#include <FastLED.h>

#ifndef LED_CONTROL
#define LED_CONTROL

#define LED_MATRIX_DATA_PIN 4
#define COLOR_BIT_MASK 0b11110000
#define BRIGHTNESS_BIT_MASK 0b00001100
#define ZERO_MASK 0b00000000

class LedControl
{
public:
	LedControl();
	void setAllLeds(uint8_t selectedCellIndex, uint8_t selectedBlinkInfluence);

	void setLed(uint8_t cellIndex, uint8_t selectedCellIndex, uint8_t selectedBlinkInfluence);

	void setCellData(CellData cellData);

private:
	CRGB _leds[NUM_LEDS];
	CellData _cellData;
};

#endif