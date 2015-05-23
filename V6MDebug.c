#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "DrvUSB.h"
#include "DrvGPIO.h"
#include "DrvSys.h"
#include "LCD_Driver.h"
#include "HIDSysIO.h"
#include "V6MDebug.h"
#include "DrvPWM.h"
#include "Motor_Ctrl.h"
extern void USB_SendBackData(uint8_t bError, const uint8_t *pu8Buffer, uint32_t u32Size);
static void VCMD_AckCommand(uint32_t u32Errno, const uint8_t *pu8Buffer, uint32_t u32Len)
{
    USB_SendBackData((u32Errno == 0 ? FALSE : TRUE), pu8Buffer, u32Len);
	
}

static uint32_t VCMD_WillResetCommandSerial(const uint8_t *pu8Buffer, uint32_t u32Len)
{
    return 0;	//Reset in V6M_ProcessCommand
}

static uint32_t V6M_ProcessOneCommand(const uint8_t *pu8Buffer, uint32_t u32Len)
{
    uint32_t u32Cmd =  pu8Buffer[0];
    //memcpy(&u32Cmd, pu8Buffer, sizeof(uint8_t));
    switch (u32Cmd)
    {
    case V6M_CMD_RESET_CMD_SRIAL:
        return VCMD_WillResetCommandSerial(pu8Buffer, u32Len);
				break;
		case CMD_STEP:
		{
				uint16_t stepX = Convert_u8_u16(pu8Buffer[1],pu8Buffer[2]);
				uint8_t dX = pu8Buffer[3];
				uint16_t stepY = Convert_u8_u16(pu8Buffer[4],pu8Buffer[5]);
				uint8_t dY = pu8Buffer[6];
				return uMotor_Move(stepX,dX,stepY,dY);
				break;
		}
		case CMD_PAUSE:
				return uMotor_Pause(pu8Buffer[1]);
				break;
		case CMD_RUN:
				return uMotor_Run(pu8Buffer[1]);
				break;
		case CMD_DIR:
				return uMotor_ChDir(pu8Buffer[1], pu8Buffer[2]);
				break;
		case CMD_SPEED:
				{
					uint8_t mcontroller_name = pu8Buffer[1];
					uint16_t speed = Convert_u8_u16(pu8Buffer[2],pu8Buffer[3]);
					return uMotor_ChSpeed(mcontroller_name,speed);
					break;
				}
		case CMD_SETHOME:
				return uMotor_SetHome();
				break;
		case CMD_MOVEHOME:
				return uMotor_MoveHome();
				break;
		case CMD_SETSTATUS:
				return uMotor_SetWorkingStatus(pu8Buffer[1]);
		case CMD_GET_STATUS:
				return uMotor_Status();
				break;
    default:
        //DrvSIO_printf("Unknown cmd: %02x\n", u32Cmd);

        return 1;
    }
}


void V6M_ProcessCommand(const uint8_t *pu8Buffer, uint32_t u32Len)
{
    static uint8_t au8CmdBuffer[V6M_MAX_COMMAND_LENGTH];
    static uint32_t u32BufferLen = 0;
    memcpy(au8CmdBuffer, pu8Buffer, 62);
    V6M_ProcessOneCommand(au8CmdBuffer, u32BufferLen);
}


uint32_t uMotor_Move(uint16_t x, uint8_t dirX,uint16_t y, uint8_t dirY){
	//uMotor_SetWorkingStatus(NOT_FINISHED);
	mcX.c_motor->direction = dirX;
	mcY.c_motor->direction = dirY;
	MoveMotor(&mcX,mcX.c_motor->direction,x);
	MoveMotor(&mcY,mcY.c_motor->direction,y);
}

uint32_t uMotor_Pause(uint8_t mc_name) {
	//uMotor_SetWorkingStatus(NOT_FINISHED);
	if(mc_name == MC_X) PauseMotor(&mcX);
	else if(mc_name == MC_Y) PauseMotor(&mcY);
	//else (mc_name == SERVO_Z) PauseMotor(&mcZ);
}

uint32_t uMotor_Run(uint8_t mc_name) {
	//uMotor_SetWorkingStatus(NOT_FINISHED);
	if(mc_name == MC_X)
		RunMotor(&mcX);
	else RunMotor(&mcY);
	
}

uint32_t uMotor_ChDir(uint8_t mc_name,uint8_t dir) {
	//uMotor_SetWorkingStatus(NOT_FINISHED);
	if(mc_name == MC_X)
		ChangeDir(&mcX, dir);
	else ChangeDir(&mcY,dir);
}

uint32_t uMotor_ChSpeed(uint8_t mc_name,uint16_t speed) {
	//uMotor_SetWorkingStatus(NOT_FINISHED);
		if(mc_name == MC_X)
			ChangeSpeed(&mcX,speed);
		else ChangeSpeed(&mcY,speed);
}


uint32_t uMotor_Status() {
		int i=0;
			uint8_t au32Data[62] = {0};
  uint32_t u32Errno = 0;
	//if(mcX.enable == 0 && mcY.enable == 0 && SystemStatus!=WAIT) SystemStatus = FINISHED;


	au32Data[0] = SystemStatus;
	au32Data[1] = (mcX.step_remain)>>8;
	au32Data[2] = (uint8_t)mcX.step_remain;
	au32Data[3] = mcX.c_motor->currentStep >>8;
	au32Data[4] =(uint8_t) mcX.c_motor->currentStep;
	au32Data[5] = mcX.c_motor->direction;
	au32Data[6] = mcX.enable;
	au32Data[7] = mcX.c_motor->speed >>8;
	au32Data[8] = (uint8_t)mcX.c_motor->speed;
	
	au32Data[9] = (mcY.step_remain)>>8;
	au32Data[10] = (uint8_t)mcY.step_remain;
	au32Data[11] = mcY.c_motor->currentStep >>8;
	au32Data[12] =(uint8_t) mcY.c_motor->currentStep;
	au32Data[13] = mcY.c_motor->direction;
	au32Data[14] = mcY.enable;
	au32Data[15] = mcY.c_motor->speed >>8;
	au32Data[16] = (uint8_t)mcY.c_motor->speed;
	
	VCMD_AckCommand(u32Errno, (const uint8_t *)&au32Data, 62);
}

uint32_t uMotor_Clear() {
	uint8_t au32Data[62] = {0};
  uint32_t u32Errno = 0;
	VCMD_AckCommand(u32Errno, (const uint8_t *)&au32Data, 62);
}

uint32_t uMotor_SetWorkingStatus(uint8_t status){
	SystemStatus = status;
};

uint32_t uMotor_SetHome() {
	//uMotor_SetWorkingStatus(NOT_FINISHED);
	SetHome(&mcX,&mcY);
}

uint32_t uMotor_MoveHome() {
	MoveHome(&mcX,&mcY);
}