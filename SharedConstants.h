#include "Arduino.h"

#ifndef SHARED_CONSTANTS
#define SHARED_CONSTANTS

#define NUM_LEDS 16

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