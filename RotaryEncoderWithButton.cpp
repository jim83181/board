#include "RotaryEncoderWithButton.h"
#include <RotaryEncoder.h>
#include <Arduino.h>

RotaryEncoderWithButton::RotaryEncoderWithButton(int rotaryPinA, int rotaryPinB, int buttonPin, long rotaryMin, long rotaryMax)
{
	_buttonPin = buttonPin;

	_encoder = new RotaryEncoder(rotaryPinA, rotaryPinB, RotaryEncoder::LatchMode::FOUR3);
	pinMode(_buttonPin, INPUT_PULLUP);

	_rotaryPosition = _encoder->getPosition();
	_lastRotaryPosition = _rotaryPosition;

	_buttonState = !digitalRead(_buttonPin);
	_lastButtonState = _buttonState;

	_rotaryMax = rotaryMax;
	_rotaryMin = rotaryMin;
}

bool RotaryEncoderWithButton::tick() {
	_encoder->tick();

	setRotaryPosition(_encoder->getPosition());

	_lastButtonState = _buttonState;
	_buttonState = !digitalRead(_buttonPin);

	return hasStateChanged();
}

bool RotaryEncoderWithButton::hasStateChanged(){
	return hasButtonStateChanged() || hasRotaryPositionChanged();
}

bool RotaryEncoderWithButton::hasButtonStateChanged()
{
	return (_lastButtonState != _buttonState);
}

bool RotaryEncoderWithButton::hasRotaryPositionChanged()
{
	return (_lastRotaryPosition != _rotaryPosition);
}

bool RotaryEncoderWithButton::getButtonState()
{
	return _buttonState;
}

int RotaryEncoderWithButton::getRotaryPosition()
{
	return _rotaryPosition;
}

void RotaryEncoderWithButton::setRotaryPosition(int newPosition)
{
	_lastRotaryPosition = _rotaryPosition;

	_rotaryPosition = newPosition;
	if (_rotaryPosition < _rotaryMin)
	{
		_rotaryPosition = _rotaryMax;
	}
	else if (_rotaryPosition > _rotaryMax)
	{
		_rotaryPosition = _rotaryMin;
	}

	if (_encoder->getPosition() != _rotaryPosition)
	{
		_encoder->setPosition(_rotaryPosition);
	}
}

void RotaryEncoderWithButton::serialPrintState()
{
	Serial.print(" Button: ");
	Serial.print(_buttonState ? "ON" : "OFF");
	Serial.print(",");
	Serial.print(" Position: ");
	Serial.println(_rotaryPosition);
}