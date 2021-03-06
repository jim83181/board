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

		uint8_t getCellValue(uint16_t cellIndex, byte bitMask);

		void setCellValue(uint16_t cellIndex, byte bitMask, uint8_t value);

		void setCellData(uint16_t cellIndex, uint8_t color, uint8_t brightness);

		void setCellColor(uint16_t cellIndex, uint8_t value);

		void setCellBrightness(uint16_t cellIndex, uint8_t value);

		void copyCellData(uint16_t cellSourceIndex, uint16_t cellTargetIndex);

		bool doesCellDataMatch(uint16_t cellA, uint16_t cellB);

	private:
		byte _cellData[NUM_LEDS];
};



#endif