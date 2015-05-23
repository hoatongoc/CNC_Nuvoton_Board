
#include "DrvPWM.h"
#include "DrvGPIO.h"
#ifndef MOTOR_CTRL
	#define MOTOR_CTRL
	#define MAX_STEP_X 15000
	#define MAX_STEP_Y 15000
	#define CW 1
	#define CCW 0
	#define DEF_SPEED 5000
	#define MAX_SPEED 20000
	#define PAUSE 0
	#define RUN 1
	#define MC_X 0
	#define MC_Y 1
	#define SERVO_Z 3
	
	#define NOT_FINISHED 0
	#define FINISHED 1
	#define WAIT 2


typedef struct {
	uint32_t maxStep;
	uint32_t speed; //current freq of motor
	uint32_t maxSpeed; // max freq of motor
	uint32_t currentStep;
	uint32_t lastStep;
	uint8_t direction; //  1-CW - right, 0 = CCW - left
}motor; 

typedef struct {
	uint8_t name;
	E_DRVGPIO_FUNC PWM_func;
	uint8_t enable;
	uint8_t timer;
	uint8_t dir_port;
	uint32_t step_remain;
	S_DRVPWM_TIME_DATA_T spt;
	motor *c_motor;
} motor_controller;

extern motor motorX,motorY;
extern motor_controller mcX,mcY;
extern uint8_t SystemStatus;

//init func for motor and motor controller
uint8_t InitMotor(motor *m,uint32_t max_step, uint32_t max_speed);
uint8_t InitMotorController(motor_controller *m_controller,motor *m, E_DRVGPIO_FUNC m_func,uint8_t m_timer,uint8_t m_name,uint8_t dirP); 

//cac ham xu ly motor
uint8_t MoveMotor(motor_controller *mc, uint8_t dir,uint32_t step);
uint8_t PauseMotor (motor_controller *mc);
uint8_t RunMotor (motor_controller *mc);
uint8_t StopMotor (motor_controller *mc);
uint8_t Reset(motor_controller *mc);
uint8_t ChangeDir(motor_controller *mc, uint8_t dir);
uint8_t ChangeSpeed(motor_controller *mc, uint16_t speed);
uint8_t SetHome(motor_controller *mc_x, motor_controller *mc_y);
uint8_t MoveHome(motor_controller *mc_x,motor_controller *mc_y);
uint8_t EmergencyPause();


//cac ham xu ly du lieu
uint16_t Convert_u8_u16(uint8_t high, uint8_t low);



#endif
