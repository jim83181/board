#include "SharedConstants.h"
#include "LedControl.h"
#include "CellData.h"
#include "RotaryEncoderWithButton.h"


#define SELECTED_BLINK_DELAY 2500
#define SELECTED_BLINK_DURATION 500

uint64_t _currentTime = 0;

uint8_t _selectedCell = 0;
uint64_t _selectedBlinkStart = 0;
uint8_t _selectedBlinkInfluence = 0;

uint8_t _copiedCell = 0;

CellData _cellData;
LedControl _ledControl;

RotaryEncoderWithButton encoderSelection = RotaryEncoderWithButton(PIN_ROTARY_SELECTION_A, PIN_ROTARY_SELECTION_B, PIN_ROTARY_SELECTION_BUTTON, 0, NUM_LEDS - 1);
RotaryEncoderWithButton encoderColor = RotaryEncoderWithButton(PIN_ROTARY_COLOR_A, PIN_ROTARY_COLOR_B, PIN_ROTARY_COLOR_BUTTON, 0, NUM_OF_COLORS - 1);

void setup()
{
	Serial.begin(19200);

	_ledControl.setCellData(&_cellData);
}

uint8_t skipFast = 0;

void loop() {
	_currentTime = millis();

	encoderSelection.tick();
	encoderColor.tick();

	if(encoderSelection.hasRotaryPositionChanged())
	{
		changeSelected(encoderSelection.getRotaryPosition());
		uint8_t colorOfSelected = _cellData.getCellValue(_selectedCell, COLOR_BIT_MASK);
		encoderColor.setRotaryPosition(colorOfSelected);
		encoderSelection.serialPrintState();
	}
	else if (encoderColor.hasRotaryPositionChanged())
	{
		int newColor = encoderColor.getRotaryPosition();
		_cellData.setCellColor(_selectedCell, newColor);
		deferSelectionBlink();
		encoderColor.serialPrintState();
	}

	if(encoderSelection.hasButtonStateChanged()) {
		if(encoderSelection.getButtonState()) {
			_copiedCell = _selectedCell;
		}
		encoderSelection.serialPrintState();
	}

	if(encoderColor.getButtonState()) {
		uint8_t colorOfCopied = _cellData.getCellValue(_copiedCell, COLOR_BIT_MASK);
		_cellData.setCellColor(_selectedCell, colorOfCopied);
	}

	updateSelectedBlinkInfluence();

	_ledControl.setAllLeds(_selectedCell, _selectedBlinkInfluence);

	skipFast++;

	skipFast = skipFast % 10;

	if(skipFast == 0) {
		FastLED.show();
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

void deferSelectionBlink() {
	_selectedBlinkStart = _currentTime - SELECTED_BLINK_DURATION;
}