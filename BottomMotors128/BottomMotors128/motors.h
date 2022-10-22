/*
 * step_motors.h
 *
 * Created: 10.09.2015 9:47:32
 *  Author: Dmitriy
 */ 

#ifndef STEP_MOTORS_H_
#define STEP_MOTORS_H_

// Справка по состояниям шаговых двигателей
// 0 состояние - шаговый двигатель не спозиционирован
// 1 состояние - позиционирование шагового двигателя
// 2 состояние - шаговый двигатель в рабочем режиме
// 3 состояние - завершение позиционирования (специально для "лифта")
#define STEP_MOTOR_UNKNOW 0
#define STEP_MOTOR_POSITIONING 1
#define STEP_MOTOR_READY 2
#define STEP_MOTOR_FINIFPOSITIONING 3

//#define LIFT_START_FREQ			1500
//#define LIFT_END_FREQ			2000

#define ROTATION_STOP_CLOCKWISE		0x04
#define ROTATION_STOP_ANTICLOCKWISE	0x08

void init_step_motors(void);
void init_lift_timer(void);
void init_rotation_timer(void);
void init_interrupt_stop(void);

void enable_motors(void);
void disable_motors(void);
void restart_motors(void);
void step_motors_get_ready(void);



uint16_t rotation_freq_to_ctc(uint16_t freq);
void set_rotation_start_freq(uint16_t freq);
void set_rotation_end_freq(uint16_t freq);
void set_rotation(float _rotation);
void positioning_rotation_async(void);
void enable_rotation(void);
void disable_rotation(void);
void stop_rotation(void);
void start_rotation(uint8_t by_step);
uint8_t rotation_is_max(void);
uint8_t rotation_is_min(void);
uint8_t rotation_must_stop(void);
void set_rotation_dir(uint8_t is_clockwise);
void set_rotation_step_count(uint16_t step);
uint16_t get_rotation_step_count(void);
uint8_t rotation_is_enabled(void);
uint8_t rotation_is_clockwise(void);
float get_rotation_degree_by_step(void);
void rotation_ready(void);

#endif /* STEP_MOTORS_H_ */