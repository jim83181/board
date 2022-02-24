#include <SimpleRotary.h>

#include "LedControl.h"
#include "CellData.h"
#include "SharedConstants.h"

#define SELECTED_BLINK_DELAY 4500
#define SELECTED_BLINK_DURATION 1500

uint64_t _currentTime = 0;

uint8_t _selectedCell = 0;
uint64_t _selectedBlinkStart = 0;
uint8_t _selectedBlinkInfluence = 0;

uint64_t _changeTime = 0;
uint64_t _randomDelay = random(500, 2000);


CellData _cellData;
LedControl _ledControl;

void setup()
{
	Serial.begin(19200);

	for (uint8_t i = 0; i < NUM_LEDS; i++)
	{
		_cellData.setCellColor(i, i % NUM_OF_COLORS);
	}

	_ledControl.setCellData(_cellData);
}

void loop() {
	_currentTime = millis();

	updateSelectedBlinkInfluence();

	_ledControl.setAllLeds(_selectedCell, _selectedBlinkInfluence);

	FastLED.show();

	delay(10);

	if(_currentTime - _changeTime > _randomDelay) {
		_randomDelay = random(2000, 6000);
		_changeTime = _currentTime;
		changeSelected(_selectedCell + 1);
	}
}

void updateSelectedBlinkInfluence()
{
	uint16_t blinkDelta = _currentTime - _selectedBlinkStart;

	if(blinkDelta > SELECTED_BLINK_DELAY) {
		blinkDelta -= SELECTED_BLINK_DELAY;
		_selectedBlinkStart = _currentTime - blinkDelta;
	}

	long dConstrained = constrain(blinkDelta, 0, SELECTED_BLINK_DURATION);
	long lMapped = map(dConstrained, 0, SELECTED_BLINK_DURATION, 0, 31415);

	float dMapped = ((float)lMapped) / 10000.0f;

	double dMapSin = sin(dMapped);

	_selectedBlinkInfluence = dMapSin * 255;
}

void changeSelected(uint8_t newSelectedCellIndex) {
	_selectedCell = newSelectedCellIndex;
	_selectedCell = _selectedCell % NUM_LEDS;
	_selectedBlinkStart = _currentTime - (SELECTED_BLINK_DURATION * .25);
}