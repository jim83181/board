
#ifndef ROTARY_ENCODER_WITH_BUTTON
#define ROTARY_ENCODER_WITH_BUTTON

#include "Arduino.h"
#include <RotaryEncoder.h>

class RotaryEncoderWithButton
{
	public:
		RotaryEncoderWithButton(
				int rotaryPinA,
				int rotaryPinB,
				int buttonPin,
				long rotaryMin,
				long rotaryMax);

		bool tick();

		bool hasStateChanged();

		bool hasButtonStateChanged();

		bool hasRotaryPositionChanged();

		bool getButtonState();

		int getRotaryPosition();

		void setRotaryPosition(int newPosition);

		void serialPrintState();

	private:
		int _buttonPin;
		RotaryEncoder* _encoder;

		bool _buttonState;
		bool _lastButtonState;

		long _rotaryPosition;
		long _lastRotaryPosition;

		long _rotaryMin;
		long _rotaryMax;
};

#endif