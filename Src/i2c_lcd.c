/*
 * i2c_lcd.c
 *
 *  Created on: Jun 21, 2024
 *      Author: malef
 */

#include "i2c_lcd.h"
static HAL_StatusTypeDef xLCDWriteNibble(I2C_HandleTypeDef* I2Cx, uint8_t nibble);
static HAL_StatusTypeDef xLCDSentENPulse(I2C_HandleTypeDef* I2Cx, uint8_t busData);
HAL_StatusTypeDef xLCDInit(LCDHandle_t* pxHandle){
	/* LCD Initialization */
	HAL_Delay(40);
	if (xLCDWriteNibble(pxHandle->I2Cx, 0x3) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(5);
	if (xLCDWriteNibble(pxHandle->I2Cx, 0x3) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(1);
	if (xLCDWriteNibble(pxHandle->I2Cx, 0x3) != HAL_OK){
		return HAL_ERROR;
	}
	if (xLCDWriteNibble(pxHandle->I2Cx, 0x2) != HAL_OK){
		return HAL_ERROR;
	}
	/* Function set */
	if (xLCDWriteCMD(pxHandle->I2Cx, pxHandle->FnSet) != HAL_OK){
		return HAL_ERROR;
	}
	if (xLCDWriteCMD(pxHandle->I2Cx, pxHandle->DisOnCtrl) != HAL_OK){
		return HAL_ERROR;
	}
	if (xLCDDisplayClear(pxHandle) != HAL_OK){
		return HAL_ERROR;
	}
	return xLCDWriteCMD(pxHandle->I2Cx, pxHandle->EntryMode);

}

static HAL_StatusTypeDef xLCDWriteNibble(I2C_HandleTypeDef* I2Cx, uint8_t nibble){
	if (HAL_I2C_Master_Transmit(I2Cx, ADDR, &nibble, 1, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	return xLCDSentENPulse(I2Cx, nibble);

}
static HAL_StatusTypeDef xLCDSentENPulse(I2C_HandleTypeDef* I2Cx, uint8_t busData){
	busData |= LCD_EN_MASK;
	if (HAL_I2C_Master_Transmit(I2Cx, ADDR, &busData, 1, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	HAL_Delay(1);
	busData &= ~LCD_EN_MASK;
	if (HAL_I2C_Master_Transmit(I2Cx, ADDR, &busData, 1, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	return HAL_OK;
}
HAL_StatusTypeDef xLCDDisplayClear(LCDHandle_t* pxHandle){
	HAL_StatusTypeDef err =  xLCDWriteCMD(pxHandle->I2Cx, LCD_CMD_DIS_CLEAR);
	HAL_Delay(100);
	return err;
}
HAL_StatusTypeDef xLCDWriteCMD(I2C_HandleTypeDef* I2Cx, uint8_t cmd){
	uint8_t buffer = LCD_CMD_Msk;
	if (HAL_I2C_Master_Transmit(I2Cx, ADDR, &buffer, 1, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	buffer  |= ((cmd)&0xF0);
	if (xLCDWriteNibble(I2Cx, buffer) != HAL_OK){
		return HAL_ERROR;
	}
	buffer = LCD_CMD_Msk;
	buffer |= ((cmd&0x0F)<<4);
	return xLCDWriteNibble(I2Cx, buffer);
}
HAL_StatusTypeDef xLCDPrintChar(I2C_HandleTypeDef* I2Cx, uint8_t ch){
	uint8_t buffer = LCD_DATA_Msk;
	if (HAL_I2C_Master_Transmit(I2Cx, ADDR, &buffer, 1, HAL_MAX_DELAY) != HAL_OK){
		return HAL_ERROR;
	}
	buffer  |= ((ch)&0xF0);
	if (xLCDWriteNibble(I2Cx, buffer) != HAL_OK){
		return HAL_ERROR;
	}
	buffer =  LCD_DATA_Msk;
	buffer |= ((ch&0x0F)<<4);
	return xLCDWriteNibble(I2Cx, buffer);
}
HAL_StatusTypeDef xLCDReturnHome(LCDHandle_t* pxHandle){
	HAL_StatusTypeDef err = xLCDWriteCMD(pxHandle->I2Cx, LCD_CMD_DIS_RETURN_HOME);
	HAL_Delay(50);
	return err;
}
HAL_StatusTypeDef xLCDPrintString(I2C_HandleTypeDef* I2Cx, char* message){
	do {
		if (xLCDPrintChar(I2Cx, (uint8_t)*message++) != HAL_OK){
			return HAL_ERROR;
		}
	}while (*message != '\0');
	return HAL_OK;
}
HAL_StatusTypeDef xLCDSetCursorPos(LCDHandle_t* pxHandle, uint8_t col, uint8_t row){
	col %=21;
	row %=5;
	col--;
	uint8_t rowaddr;
	switch (row){
	case 1:
		rowaddr = 0x80 | 0x00;
		break;
	case 2:
		rowaddr = 0x80 | 0x40;
		break;
	case 3:
		rowaddr = 0x80 | 0x14;
		break;
	case 4:
		rowaddr = 0x80 | 0x54;
		break;
	default:
		break;

	}
	return xLCDWriteCMD(pxHandle->I2Cx, rowaddr+col);
}
