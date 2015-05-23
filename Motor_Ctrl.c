#include "Motor_Ctrl.h"

motor motorX , motorY;
motor_controller mcX , mcY;
uint8_t SystemStatus = WAIT;



uint8_t InitMotor(motor *m,uint32_t max_step, uint32_t max_speed) {
	m->maxStep = max_step;
	m->maxSpeed = max_speed;
	m->direction = CW;
	m->currentStep = 0;
	m->lastStep = 0;
	m->speed = 0;
	return 1;
}
uint8_t InitMotorController(motor_controller *m_controller,motor *m, E_DRVGPIO_FUNC m_func,uint8_t m_timer,uint8_t m_name,uint8_t dirP) {
	m_controller->c_motor = m;
	m_controller->name = m_name;
	m_controller->PWM_func = m_func;
	m_controller->enable = PAUSE;
	m_controller->timer = m_timer;
	m_controller->step_remain = 0;
	m_controller->spt.u8HighPulseRatio = 50;
	m_controller->spt.i32Inverter = 0;
	m_controller->spt.u32Frequency = m->speed;
	m_controller->spt.u8Mode = DRVPWM_AUTO_RELOAD_MODE;
	m_controller->dir_port = dirP;
	return 1;
}

uint8_t MoveMotor(motor_controller *mc, uint8_t dir,uint32_t step) {
	if (step <1) return 1;
	
	ChangeDir(mc,dir);
	
	if(dir == CW) {
		if(mc->name == MC_X) {
			mc->step_remain = ((mc->c_motor->currentStep + step) < MAX_STEP_X) ? step: (MAX_STEP_X - mc->c_motor->currentStep);
		}
		else if (mc->name == MC_Y) {
			mc->step_remain = ((mc->c_motor->currentStep + step) < MAX_STEP_Y) ? step: (MAX_STEP_Y - mc->c_motor->currentStep);
		}
	}
	else {
			if(mc->name == MC_X) {
				mc->step_remain = (mc->c_motor->currentStep > step) ? step: mc->c_motor->currentStep;
			}
			else if (mc->name == MC_Y) {
				mc->step_remain = (mc->c_motor->currentStep > step)  ? step: mc->c_motor->currentStep;
			}
	}
		
	mc->c_motor->direction  = dir;
	mc->c_motor->lastStep = mc->c_motor->currentStep;	
	RunMotor(mc);
	return 1;
};

uint8_t PauseMotor (motor_controller *mc) {
	mc->enable = PAUSE;
	//DrvPWM_SetTimerIO(mc->timer, 0);
	DrvPWM_Enable(mc->timer,mc->enable);
	return 1;
}
uint8_t RunMotor (motor_controller *mc) {
	if(mc->step_remain ==0) return 1;
	mc->enable = RUN;
	DrvPWM_SetTimerIO(mc->timer, 1);
	DrvPWM_Enable(mc->timer,mc->enable);
	return 1;
}

uint8_t ChangeSpeed(motor_controller *mc, uint16_t speed) {
	mc->c_motor->speed = speed;
	mc->spt.u32Frequency = speed;
	DrvPWM_SetTimerClk(mc->timer, &(mc->spt));
}
uint8_t StopMotor (motor_controller *mc){
	PauseMotor(mc);
	mc->step_remain = 0;
	return 1;
	
};

uint8_t SetHome(motor_controller *mc_x, motor_controller *mc_y){
	mc_x->c_motor->currentStep = 0;
	mc_x->c_motor->lastStep = 0;
	mc_y ->c_motor ->currentStep = 0;
	mc_y->c_motor->lastStep = 0;
	return 1;
};

uint8_t MoveHome(motor_controller *mc_x,motor_controller *mc_y) {

	
	MoveMotor(mc_x,CCW,mc_x->c_motor->currentStep);
	MoveMotor(mc_y,CCW,mc_y->c_motor->currentStep);
	return 1;
};

uint8_t ChangeDir(motor_controller *mc, uint8_t dir) {
		mc->c_motor->direction = dir;
		if(dir == CW) {
				DrvGPIO_SetBit(E_GPC,mc->dir_port);
		}
		else { //CCW
				DrvGPIO_ClrBit(E_GPC,mc->dir_port);
		}
		return 1;
		
}

uint8_t Reset(motor_controller *mc){ 
	
	PauseMotor(mc);
	
	if(mc->name == MC_Y) {
		mc->timer = DRVPWM_TIMER2;
		mc->c_motor->maxStep = MAX_STEP_Y;
	}
	else if(mc->name == MC_X) {
		mc->timer = DRVPWM_TIMER0;
		mc->c_motor->maxStep = MAX_STEP_X;
	}
	else return 0;
	
	
	mc->step_remain = 0;
	mc->c_motor->currentStep=0;
	mc->c_motor->lastStep = 0;
	mc->c_motor->direction = CW;
	mc->c_motor->speed = 0;
	mc->c_motor->maxSpeed = MAX_SPEED;
	return 1;
};

uint8_t EmergencyPause() {
	PauseMotor(&mcX);
	PauseMotor(&mcY);
}


uint16_t Convert_u8_u16(uint8_t high, uint8_t low) {
	uint16_t result = (uint16_t)(high<<8|low);
	return result;
}
