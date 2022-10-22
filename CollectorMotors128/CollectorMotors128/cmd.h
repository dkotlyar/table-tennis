/*
 * cmd.h
 *
 * Created: 06.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef CMD_H_
#define CMD_H_

enum command_codes {
	EMPTY = 0x00,
	ERROR = 0xFF,
	
	LED = 0x01,
	
	// Настройки
	LIFT_START_SPEED			= 0x10,
	LIFT_WORK_SPEED				= 0x11,
	LIFT_FINISH_SPEED			= 0x12,
	ROTATION_START_SPEED		= 0x13,
	ROTATION_WORK_SPEED			= 0x14,
	ROTATION_FINISH_SPEED		= 0x15,
	INCLINE_START_SPEED			= 0x16,
	INCLINE_WORK_SPEED			= 0x17,
	INCLINE_FINISH_SPEED		= 0x18,
	ROLLROTATION_START_SPEED	= 0x19,
	ROLLROTATION_WORK_SPEED		= 0x1A,
	ROLLROTATION_FINISH_SPEED	= 0x1B,
	
	// Установить координаты
	LIFT			= 0x20,
	ROTATION		= 0x21,
	INCLINE			= 0x22,
	ROLLROTATION	= 0x23,
	
	// Задаёт режим вращения роликов
	TR_SPEED		= 0x25,
	TR_WIDTH		= 0x26,
	TR_STOP			= 0x27,
	TR_DISABLE		= 0x28,
	BR_SPEED		= 0x29,
	BR_WIDTH		= 0x2A,
	BR_STOP			= 0x2B,
	BR_DISABLE		= 0x2C,
	
	// Команды управления
	ENABLE_MOTOR			= 0x30,
	ENABLE_ALL_MOTORS		= 0x31,
	DISABLE_MOTOR			= 0x32,
	DISABLE_ALL_MOTORS		= 0x33,
	POSITIONING_MOTOR		= 0x34,
	POSITIONING_ALL_MOTORS	= 0x35,
	
	// Получение данных от устройств управления
	GET_POSITIONS		= 0x40,
	GET_SPEED			= 0x41,
	GET_SETTINGS		= 0x42,
	
	TO_MAIN		= 0xF1,
	TO_BOTTOM	= 0xF2,
	TO_HEADER	= 0xF3,
	TO_ROLL		= 0xF4,
	TO_DISPLAY	= 0xF5,
	
	EXTENDED = 0xFE
};

enum led_codes {
	DISABLE	= 0,
	ENABLE	= 1,
	BLINK	= 2
};

enum motors {
	ALL_MOTORS			= 0,
	MOTOR_LIFT			= 1,
	MOTOR_ROTATION		= 2,
	MOTOR_INCLINE		= 3,
	MOTOR_ROLLROTATION	= 4
};

#endif /* CMD_H_ */