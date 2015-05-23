
#include <stdio.h>
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvUSB.h"
#include "LCD_Driver.h"
#include "DrvPWM.h"
#include "NUC1xx.h"
#include "Motor_Ctrl.h"
//#define DEBUG 0
/* khai bao extern ham xu ly HID */
extern int32_t HID_MainProcess(void);


void Delay(uint32_t delayCnt)
{
    while(delayCnt--)
    {
        __NOP();
        __NOP();
    }
}
 
void Init_SYS() {
	InitMotor(&motorX,MAX_STEP_X,MAX_SPEED);
	InitMotor(&motorY,MAX_STEP_Y,MAX_SPEED);
	motorX.speed = motorY.speed = (DEF_SPEED < MAX_SPEED ? DEF_SPEED : MAX_SPEED);
	InitMotorController(&mcX,&motorX,E_FUNC_PWM01,DRVPWM_TIMER0,MC_X,0);
	InitMotorController(&mcY,&motorY,E_FUNC_PWM23,DRVPWM_TIMER2,MC_Y,1);
}

void Init_GPIO() {
DrvGPIO_Open(E_GPC,0,E_IO_OPENDRAIN);
DrvGPIO_SetBit(E_GPC,0);
DrvGPIO_Open(E_GPC,1,E_IO_OPENDRAIN);
DrvGPIO_SetBit(E_GPC,0);
}
void DRVPWM_PwmIRQHandler_X() {
		if(mcX.step_remain>0 && mcX.enable == RUN) {
			
				mcX.step_remain--;
				if(mcX.c_motor->direction == CW) {
					mcX.c_motor->currentStep++;
				}
				else {
					mcX.c_motor->currentStep--;
				}
		}
		
		if(mcX.step_remain <= 0 || mcX.c_motor->currentStep ==0){
			PauseMotor(&mcX);
			mcX.step_remain = 0;
		}
		
 }

void DRVPWM_PwmIRQHandler_Y() {
		if(mcY.step_remain>0 && mcY.enable == RUN) {
			
				mcY.step_remain--;
				if(mcY.c_motor->direction == CW) {
					mcY.c_motor->currentStep++;
				}
				else {
					mcY.c_motor->currentStep--;
				}
		}
		
		if(mcY.step_remain <= 0 || mcY.c_motor->currentStep ==0){
			PauseMotor(&mcY);
			mcY.step_remain = 0;
		}
 }
void UART_init()
{
STR_UART_T sParam;
	/* SYSCLK =>12Mhz*/
	UNLOCKREG();
	/* Enable 12M Crystal */
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1);
	/* Switch to PLL clock */
	DrvSYS_SelectHCLKSource(0);
	LOCKREG();

	/* Set UART0 Pin */
	DrvGPIO_InitFunction(E_FUNC_UART0);
	
	/* UART Setting */
    sParam.u32BaudRate 		= 115200;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Select UART Clock Source From 12Mhz*/
	DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0); 

	/* Set UART0 Configuration */
	DrvUART_Open(UART_PORT0, &sParam);
}



/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
UNLOCKREG();
SYSCLK->PWRCON.XTL12M_EN = 1;
SysTimerDelay(5000); 
DrvSYS_SelectHCLKSource(0);
LOCKREG();
	
/* Begin init PWM*/
	//init motor and motor controller
	Init_SYS();

DrvGPIO_InitFunction(mcX.PWM_func);
DrvGPIO_InitFunction(mcY.PWM_func);
DrvPWM_Open();
	
//select PWM clock source	
DrvPWM_SelectClockSource(mcX.timer, DRVPWM_HCLK);
DrvPWM_SelectClockSource(mcY.timer, DRVPWM_HCLK);

	//config frequency/pulse/mode/inverter function  
DrvPWM_SetTimerClk(mcX.timer, &(mcX.spt));
DrvPWM_SetTimerClk(mcY.timer, &(mcY.spt));
	
	
DrvPWM_SetTimerIO(mcX.timer, 1);
DrvPWM_SetTimerIO(mcY.timer, 1);

DrvPWM_Enable(mcX.timer,0);
DrvPWM_Enable(mcY.timer,0);
DrvPWM_EnableInt(mcX.timer,0,DRVPWM_PwmIRQHandler_X);
DrvPWM_EnableInt(mcY.timer,0,DRVPWM_PwmIRQHandler_Y);
	//END INIT PWM	
	
	/* Khoi tao cong GPIO de dieu khien chieu dong co qua cong GPC 0/1
		Phai de che do open-drain do cong GPIO bi sut ap, phai dung dien tro pull-up
*/


Init_GPIO();

UART_init();   

    SYSCLK->PWRCON.XTL12M_EN = 1;   
    /* Enable PLL */
    DrvSYS_SetPLLMode(0);
	Delay(1000);

	/* Switch to PLL clock */
	DrvSYS_SelectHCLKSource(2);    
    Delay(100);

    /* Update system core clock */
    SystemCoreClockUpdate();
    Initial_pannel();  //call initial pannel function
    clr_all_pannal();  //clear all pannel
    print_lcd(0, "Initial USB   ");		//printf information
	
    /* Execute HID process */
	HID_MainProcess();


return 0;
}




