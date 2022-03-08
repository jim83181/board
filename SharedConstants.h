#include <Arduino.h>

#ifndef SHARED_CONSTANTS
#define SHARED_CONSTANTS

#define LED_MATRIX_DATA_PIN 4

#define PIN_ROTARY_SELECTION_A 11
#define PIN_ROTARY_SELECTION_B 12
#define PIN_ROTARY_SELECTION_BUTTON 13

#define PIN_ROTARY_COLOR_A 9
#define PIN_ROTARY_COLOR_B 10
#define PIN_ROTARY_COLOR_BUTTON 8

#define VRX_PIN A0
#define VRY_PIN A1
#define SW_PIN 52
#define NUM_OF_COLS 8
#define NUM_OF_ROWS 8

const float JOYSTICK_RANGE = 10;
const uint16_t NUM_LEDS = NUM_OF_COLS * NUM_OF_ROWS;

#define BRIGHTNESS_MAX 4
#define NUM_OF_COLORS 12
const int colorPalette[NUM_OF_COLORS][3] = {
		{255, 0, 255},	 // white
		{0, 255, 219},	 // red
		{8, 255, 232},	 // red-orange
		{15, 255, 255},	 // orange
		{29, 255, 255},	 // orange-yellow
		{41, 255, 255},	 // yellow
		{66, 255, 187},	 // yellow-green
		{90, 255, 99},	 // green
		{129, 255, 155}, // green-blue
		{168, 255, 212}, // blue
		{186, 255, 175}, // purple
		{229, 255, 255}, // pink
};

#endif