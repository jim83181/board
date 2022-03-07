#include <Arduino.h>
#include "SharedConstants.h"
#include "CellData.h"


CellData::CellData()
{

	// initialize all cellData to bright white
	for (int i = 0; i < NUM_LEDS; i++)
	{
		_cellData[i] = 0b00000100;
	}
}

uint8_t CellData::getCellValue(uint16_t cellIndex, byte bitMask)
{
	byte b = (byte)_cellData[cellIndex];

	while ((bitMask & 0b00000001) == ZERO_MASK)
	{
		bitMask = bitMask >> 1;
		b = b >> 1;
	}

	b = b & bitMask;

	return (uint8_t)b;
}

void CellData::setCellValue(uint16_t cellIndex, byte bitMask, uint8_t value)
{
	byte b = (byte)value;
	byte originalBitmask = bitMask;

	while ((bitMask & 0b00000001) == ZERO_MASK)
	{
		bitMask = bitMask >> 1;
		b = b << 1;
	}

	byte inverseBitMask = originalBitmask ^ 0b11111111;
	byte currentCellData = _cellData[cellIndex];
	byte newCellData = currentCellData & inverseBitMask;
	newCellData = newCellData | b;

	_cellData[cellIndex] = newCellData;
}

void CellData::setCellData(uint16_t cellIndex, uint8_t color, uint8_t brightness)
{
	setCellColor(cellIndex, color);
	setCellBrightness(cellIndex, brightness);
}

void CellData::setCellColor(uint16_t cellIndex, uint8_t value)
{
	setCellValue(cellIndex, COLOR_BIT_MASK, value);
}

void CellData::setCellBrightness(uint16_t cellIndex, uint8_t value)
{
	setCellValue(cellIndex, BRIGHTNESS_BIT_MASK, value);
}
