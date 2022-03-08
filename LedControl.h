#include "SharedConstants.h"
#include <FastLED.h>

#include "CellData.h"

#ifndef LED_CONTROL
#define LED_CONTROL

#define COLOR_BIT_MASK 0b11110000
#define BRIGHTNESS_BIT_MASK 0b00001100
#define ZERO_MASK 0b00000000

class LedControl
{
public:
	LedControl();
	void setAllLeds(uint16_t selectedCellIndex, uint8_t selectedBlinkInfluence);

	void setLed(uint16_t cellIndex, uint16_t selectedCellIndex, uint8_t selectedBlinkInfluence);

	void setCellData(CellData* cellData);

	void pushLedData();

private:
	CRGB _leds[NUM_LEDS];
	CellData* _cellData;
};

#endif