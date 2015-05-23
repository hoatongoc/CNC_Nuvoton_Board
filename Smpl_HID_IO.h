
//#include "DrvPWM.h"
//#include "DrvGPIO.h"
//#ifndef _HID_IO
//	#define _HID_IO
//	#define MAX_STEP_X 21400
//	#define MAX_STEP_Y 21400
//	#define CW 1;
//	#define CCW 0;
//	#define DEF_FREQ 5000
//	#define MAX_SPEED 16000
//	#define PAUSE 0;
//	#define RUN 1;
//	
//#endif


//typedef struct {
//	uint32_t maxStep;
//	uint32_t speed; //current freq of motor
//	uint32_t maxSpeed; // max freq of motor
//	uint32_t currentStep;
//	uint32_t lastStep;
//	
//	uint8_t isRunning;
//	uint8_t direction; //  0-CW - right, 1 = CCW - left
//}motor; 

//typedef struct {
//	E_DRVGPIO_FUNC PWM_func;
//	uint8_t enable;
//	uint8_t timer;
//	S_DRVPWM_TIME_DATA_T spt;
//	motor *c_motor;
//} motor_controller;

//extern motor motorX,motorY;
//extern motor_controller mcX,mcY;


////init func for motor and motor controller
//uint8_t InitMotor(motor *m,uint32_t max_step, uint32_t max_speed);
//uint8_t InitMotorController(motor_controller *m_controller,motor *m, E_DRVGPIO_FUNC m_func,uint8_t m_timer); 

////cac ham xu ly motor
//uint8_t MoveMotor(motor_controller *mc, uint8_t dir,uint32_t step);
//uint8_t Reset(motor_controller *mc);
//uint8_t SetHome(motor_controller *mc_x, motor_controller *mc_y);

