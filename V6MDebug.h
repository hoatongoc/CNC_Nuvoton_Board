#include "DrvPWM.h"
#ifndef INC__V6MDEBUG_H__
#define INC__V6MDEBUG_H__
#ifdef __cplusplus
extern "C"
{
#endif

    /*---------------------------------------------------------------------------------------------------------*/
    /*                                                                                                         */
    /* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
    /*                                                                                                         */
    /*---------------------------------------------------------------------------------------------------------*/

#define V6M_CMD_RESET_CMD_SRIAL	0xFFFFFFFFUL

	
	//cac command ID xu ly motor
#define CMD_GET_STATUS 	0xA0UL	
#define CMD_RUN					0xA1UL
#define CMD_PAUSE 			0xA2UL
#define CMD_WAIT				0xA3UL
#define CMD_SETHOME			0xA4UL
#define CMD_MOVEHOME 		0xA5UL
#define CMD_SETSTATUS 	0xA6UL
#define CMD_MOVE 				0xAAUL
#define CMD_CLR 				0xABUL
#define CMD_DIR 				0xACUL
#define CMD_STEP 				0xADUL
#define CMD_SPEED				0xAEUL
#define CMD_RESET				0xAFUL


#define V6M_MAX_COMMAND_LENGTH	(HID_MAX_PACKET_SIZE_EP0-2)
void V6M_ProcessCommand(const uint8_t *pu8Buffer, uint32_t u32Len);
//uint32_t uMotor_Move(uint8_t motor,uint16_t step);
uint32_t uMotor_Move(uint16_t x, uint8_t dirX,uint16_t y, uint8_t dirY);
uint32_t uMotor_Pause(uint8_t mc_name);
uint32_t uMotor_Run(uint8_t mc_name);
uint32_t uMotor_ChDir(uint8_t mc_name,uint8_t dir);
uint32_t uMotor_ChSpeed(uint8_t mc_name,uint16_t speed);
uint32_t uMotor_Status();
uint32_t uMotor_Clear();
uint32_t uMotor_SetWorkingStatus(uint8_t status);
uint32_t uMotor_SetHome();
uint32_t uMotor_MoveHome();
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
