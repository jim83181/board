#include <Arduino.h>
#include <FastLED.h>
#include "SharedConstants.h"

#include "LedControl.h"

LedControl::LedControl()
{
	FastLED.addLeds<NEOPIXEL, LED_MATRIX_DATA_PIN>(_leds, NUM_LEDS);
}

void LedControl::setCellData(CellData* cellData) {
	_cellData = cellData;
}

		// Push all cellData to LEDs
		void LedControl::setAllLeds(uint16_t selectedCellIndex, uint8_t selectedBlinkInfluence)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		setLed(i, selectedCellIndex, selectedBlinkInfluence);
	}
}

void LedControl::setLed(uint16_t cellIndex, uint16_t selectedCellIndex, uint8_t selectedBlinkInfluence)
{
	uint8_t colorIndex = _cellData->getCellValue(cellIndex, COLOR_BIT_MASK);
	
	uint8_t hueValue = colorPalette[colorIndex][0];
	uint8_t saturationValue = colorPalette[colorIndex][1];
	uint8_t brightnessValue = colorPalette[colorIndex][2];

	uint8_t brightnessAdjust = _cellData->getCellValue(cellIndex, BRIGHTNESS_BIT_MASK);
	brightnessValue = brightnessAdjust * (brightnessValue / 3);

	if (selectedCellIndex == cellIndex)
	{
		int saturationInfluence = 0 - selectedBlinkInfluence;
		int brightnessInfluence = selectedBlinkInfluence;
		saturationValue = constrain(saturationValue + saturationInfluence, 0, 255);
		brightnessValue = constrain(brightnessValue + brightnessInfluence, 0, 150);
	}

	_leds[cellIndex].setHSV(hueValue, saturationValue, brightnessValue);
}