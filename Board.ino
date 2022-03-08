#include "SharedConstants.h"
#include "LedControl.h"
#include "CellData.h"
#include "RotaryEncoderWithButton.h"

#include <Joystick.h>
#include <AxisJoystick.h>


Joystick *joystick;
float _xPos = 0.0;
float _yPos = 0.0;
int _xCenter;
int _yCenter;
const float _centerDeadZone = 8.0;

#define SELECTED_BLINK_DELAY 2500
#define SELECTED_BLINK_DURATION 500

uint64_t _currentTime = 0;
uint64_t _elapsedTime;

uint16_t _selectedCell = 0;
uint64_t _selectedBlinkStart = 0;
uint8_t _selectedBlinkInfluence = 0;

uint16_t _copiedCell = 0;

CellData _cellData;
LedControl _ledControl;

RotaryEncoderWithButton encoderBrightness = RotaryEncoderWithButton(PIN_ROTARY_SELECTION_A, PIN_ROTARY_SELECTION_B, PIN_ROTARY_SELECTION_BUTTON, 0, BRIGHTNESS_MAX - 1, true);
RotaryEncoderWithButton encoderColor = RotaryEncoderWithButton(PIN_ROTARY_COLOR_A, PIN_ROTARY_COLOR_B, PIN_ROTARY_COLOR_BUTTON, 0, NUM_OF_COLORS - 1, false);

bool _massSelection[NUM_LEDS];
bool _wasMassSelected = false;

void setup()
{
	Serial.begin(19200);
	
	joystick = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);

	_ledControl.setCellData(&_cellData);

	_xCenter = joystick->xAxis();
	_yCenter = joystick->yAxis();
}

uint8_t skipFast = 0;

void loop() {

	unsigned long currentTime = millis();
	_elapsedTime = (currentTime - _currentTime);
	_currentTime = currentTime;

	calculeJoystick();

	encoderBrightness.tick();
	encoderColor.tick();

	if (encoderBrightness.hasRotaryPositionChanged())
	{
		int newBrightness = encoderBrightness.getRotaryPosition();
		_cellData.setCellBrightness(_selectedCell, newBrightness);
		deferSelectionBlink();
	}
	else if (encoderColor.hasRotaryPositionChanged())
	{
		int newColor = encoderColor.getRotaryPosition();
		_cellData.setCellColor(_selectedCell, newColor);
		deferSelectionBlink();
	}

	if (encoderBrightness.hasButtonStateChanged())
	{
		if (encoderBrightness.getButtonState())
		{
			_copiedCell = _selectedCell;
		}
	}

	if(encoderColor.getButtonState())
	{
		_cellData.copyCellData(_copiedCell, _selectedCell);
	}

	if(joystick->isPress())
	{
		if(_wasMassSelected == false) {
			for (uint16_t i = 0; i < NUM_LEDS; i++)
			{
				_massSelection[i] = _cellData.doesCellDataMatch(_selectedCell, i);
			}
		}

		for (uint16_t i = 0; i < NUM_LEDS; i++) {
			if(_massSelection[i]) {
				_cellData.copyCellData(_selectedCell, i);
			}
		}

			_wasMassSelected = true;
	}
	else
	{
		_wasMassSelected = false;
	}

	updateSelectedBlinkInfluence();

	_ledControl.setAllLeds(_selectedCell, _selectedBlinkInfluence);

	skipFast++;

	skipFast = skipFast % 10;

	if(skipFast == 0) {
		_ledControl.pushLedData();
	}
	

}

void calculeJoystick() {

	float elapsedSeconds = _elapsedTime / 1000.0f;

	//get adjusted x axis
	int xAxis = joystick->xAxis();
	xAxis = map(xAxis, _xCenter - 500, _xCenter + 500, -JOYSTICK_RANGE, JOYSTICK_RANGE);
	xAxis = abs(xAxis) < (JOYSTICK_RANGE / _centerDeadZone) ? 0 : xAxis;
	xAxis *= -1;

	//get adjusted y axis
	int yAxis = joystick->yAxis();
	yAxis = map(yAxis, _yCenter - 500, _yCenter + 500, -JOYSTICK_RANGE, JOYSTICK_RANGE);
	yAxis = abs(yAxis) < (JOYSTICK_RANGE / _centerDeadZone) ? 0 : yAxis;

	//center positions if not moving
	_xPos = xAxis == 0 ? (floor(_xPos) + .5f) : _xPos;
	_yPos = yAxis == 0 ? (floor(_yPos) + .5f) : _yPos;

	//adjust positions based on joystick axis value * elapsed time
	_xPos += ((xAxis * elapsedSeconds) * abs(xAxis)) / JOYSTICK_RANGE;
	_yPos += ((yAxis * elapsedSeconds) * abs(yAxis)) / JOYSTICK_RANGE;

	//confine positions within the columns and rows available
	if(_xPos > NUM_OF_COLS)
		_xPos -= NUM_OF_COLS;
	else if(_xPos < 0)
		_xPos += NUM_OF_COLS;

	if (_yPos > NUM_OF_ROWS)
		_yPos -= NUM_OF_ROWS;
	else if (_yPos < 0)
		_yPos += NUM_OF_ROWS;

	//floor position floats to ints
	int xPosInt = (int)_xPos;
	int yPosInt = (int)_yPos;

	/*
				_X0_X1_X2_X3_
		Y0	|00|15|16|31|
		Y1	|01|14|17|30|
		Y2	|02|13|18|29|
		Y3	|03|12|19|28|
		Y4	|04|11|20|27|
		Y5	|05|10|21|26|
		Y6	|06|09|22|25|
		Y7	|07|08|23|24|

	*/

	//Example 1:  X:1, Y:1  => LED 14
	//Example 2:	X:2, Y:4	=> LED 20

	//Example 1: colMultiple1 = 2
	//Exmaple 2: colMultiple1 = 2
	int colMultiple1 = ((xPosInt + 1) / 2) * 2; //2

	//Example 1: yAdjust = 1
	//Example 2: yAdjust = 4
	int yAdjust = yPosInt; //0

	//Example 1: true
	//Example 2: false
	if (xPosInt % 2 == 1) //true
	{
		//Example 1: yAdjust = -2
		yAdjust = -yPosInt - 1; //-1
	}

	//Example 1:
	//	14 = 2 * 8 + -2
	//Example 2:
	//	20 = 2 * 8 + 4
	int idx = (colMultiple1 * NUM_OF_ROWS) + yAdjust;

	changeSelected(idx);
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

void changeSelected(uint16_t newSelectedCellIndex) {
	if(newSelectedCellIndex != _selectedCell) {
		_selectedCell = newSelectedCellIndex;
		_selectedCell = _selectedCell % NUM_LEDS;
		_selectedBlinkStart = _currentTime - (SELECTED_BLINK_DURATION * .25);

		uint8_t colorOfSelected = _cellData.getCellValue(_selectedCell, COLOR_BIT_MASK);
		encoderColor.setRotaryPosition(colorOfSelected);

		uint8_t brightnessOfSelected = _cellData.getCellValue(_selectedCell, BRIGHTNESS_BIT_MASK);
		encoderBrightness.setRotaryPosition(brightnessOfSelected);
	}
	
}

void deferSelectionBlink() {
	_selectedBlinkStart = _currentTime - SELECTED_BLINK_DURATION;
}