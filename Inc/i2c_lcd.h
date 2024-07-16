/*
 * i2c_lcd.h
 *
 *  Created on: Jun 16, 2024
 *      Author: Malefetsabe Lenka
 */

#ifndef INCLUDE_I2C_LCD_H_
#define INCLUDE_I2C_LCD_H_
#include "main.h"
/* Basic controls */
#define BYTE                                   (unsigned char)
#define LCD_CMD_DIS_CLEAR                      BYTE(0x01)
#define LCD_CMD_DIS_RETURN_HOME                BYTE(0x02)

/* Entry mode
 * DEC -> decrement
 * INC -> INCEREMENT
 * */
#define LCD_CMD_DIS_DEC_NO_SHIFT                 BYTE(0x04)
#define LCD_CMD_DIS_DEC_SHIFT                    BYTE(0x05)
#define LCD_CMD_DIS_INC_NO_SHIFT                 BYTE(0x06)
#define LCD_CMD_DIS_INC_SHIFT                    BYTE(0x07)

/* Display on and off ctrl */
#define LCD_CMD_DIS_OFF                          BYTE(0x08)
/* Display on, and cursor control */
#define LCD_CMD_CUR_OFF_BLI_OFF                  BYTE(0x0C)
#define LCD_CMD_CUR_OFF_BLI_ON                   BYTE(0x0D)
#define LCD_CMD_CUR_ON_BLI_OFF                   BYTE(0x0E)
#define LCD_CMD_CUR_ON_BLI_ON                    BYTE(0x0F)

/* Display and cursor shift */
#define LCD_CMD_CUR_SHIFT_L                          BYTE(0x10)
#define LCD_CMD_CUR_SHIFT_R                          BYTE(0x14)
#define LCD_CMD_DIS_SHIFT_L                          BYTE(0x18)
#define LCD_CMD_DIS_SHIFT_R                          BYTE(0x1C)

/* Function set */

#define LCD_CMD_4DL_5X8F_1L                          BYTE(0x20)
#define LCD_CMD_4DL_5X8F_2L                          BYTE(0x28)
#define LCD_CMD_8DL_5X8F_1L                          BYTE(0x30)
#define LCD_CMD_8DL_5X8F_2L                          BYTE(0x38)

#define LCD_CMD_4DL_5X10F_1L                         BYTE(0x24)
#define LCD_CMD_8DL_5X10F_1L                         BYTE(0x34)

#define LCD_EN_MASK                 BYTE(0x04)


#define ADDR             BYTE(0x4E)
#define LCD_DATA_Msk     BYTE(0x09)
#define LCD_CMD_Msk      BYTE(0x08)
typedef struct{
	/*@Function set */
	uint8_t FnSet;
	/*@Display and cursor shift*/
	uint8_t DisCurShift;
	/*@ Display on and off ctrl */
	uint8_t DisOnCtrl;
	/* @ Entry mode */
	uint8_t EntryMode;
	/*@ I2C Peripheral */
	I2C_HandleTypeDef* I2Cx;
}LCDHandle_t;
HAL_StatusTypeDef xLCDInit(LCDHandle_t* pxHandle);
HAL_StatusTypeDef xLCDWriteCMD(I2C_HandleTypeDef* I2Cx, uint8_t cmd);
HAL_StatusTypeDef xLCDPrintChar(I2C_HandleTypeDef* I2Cx, uint8_t ch);
HAL_StatusTypeDef xLCDPrintString(I2C_HandleTypeDef* I2Cx, char* message);
HAL_StatusTypeDef xLCDDisplayClear(LCDHandle_t* pxHandle);
HAL_StatusTypeDef xLCDReturnHome(LCDHandle_t* pxHandle);
HAL_StatusTypeDef xLCDSetCursorPos(LCDHandle_t* pxHandle, uint8_t col, uint8_t row);
#endif /* INCLUDE_I2C_LCD_H_ */
