#include <Arduino.h>
#include "SharedConstants.h"

#ifndef CELL_DATA_H
#define CELL_DATA_H

#define COLOR_BIT_MASK 0b11110000
#define BRIGHTNESS_BIT_MASK 0b00001100
#define ZERO_MASK 0b00000000

class CellData {
	public:
		CellData();

		uint8_t getCellValue(uint8_t cellIndex, byte bitMask);

		void setCellValue(uint8_t cellIndex, byte bitMask, uint8_t value);

		void setCellData(uint8_t cellIndex, uint8_t color, uint8_t brightness);

		void setCellColor(uint8_t cellIndex, uint8_t value);

		void setCellBrightness(uint8_t cellIndex, uint8_t value);

	private:
		byte _cellData[NUM_LEDS];
};



#endif