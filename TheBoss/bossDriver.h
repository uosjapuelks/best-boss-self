#include "commonHeads.h"

#ifndef DRIVER_H
#define DRIVER_H

enum move_t { STOP, FORWARD, REVERSE, CURVE_LEFT, CURVE_RIGHT, TURN_LEFT, TURN_RIGHT, REVERSE_LEFT, REVERSE_RIGHT };
//Store Drive Instructions

//driver functions
void InitMotor(void);
void FrontReverseRear(void);
void forward(void);
void reverse(void);
void motor_control(enum move_t move);
void stop(void);
void rewrite_driveMode(uint8_t optionNumber);
void rewrite_direction(uint8_t optionNumber);
void executeDrive(void);
int isDriving(void);
void driverless_mode(void);
void handleAutoSwitch(uint8_t option);

// Movement
// larger oveflow value => higher duty cycle => faster
#define FREQUENCY_TO_MOD(x) (375000 / x)
#define MOTOR_FAST (375000 / 50)
#define MOTOR_SLOW (375000 / 100)
#define MOTOR_SLOWER (375000 / 150)

//PWMs
#define PTD0_Pin 0										//PTD0_Pin
#define PTD1_Pin 1										//PTD1_Pin
#define PTD2_Pin 2										//PTD2_Pin
#define PTD3_Pin 3										//PTD3_Pin

//HIGH/LOW TO control TO and From in PTC8 and PTC9
#define FRONT_PIN 8
#define REAR_PIN 9
//#define RIGHT_PIN 8
//#define LEFT_PIN 9

//AUTO
#define USER_AUTO 0x02
#define END_AUTO 0x03

//UART rx_data parsing
#define USER_STOP 0x00
#define USER_FORWARD 0x01
#define USER_REVERSE 0x02

#define USER_TURN_LEFT 0x01
#define USER_TURN_RIGHT 0x02
#define USER_STRAIGHT 0x00

#define DRIVE_BACK_GO(x) (((uint32_t)(((uint32_t)(x))<<4))&0xf0)
#define DRIVE_WAY(x) (((uint32_t)((uint32_t)(x)))&0x0f)
#define MERGE_INSTRUCTIONS(x,y) (((uint32_t)((uint32_t)(x)))|y)

//driveInstructions directory
#define STAYSTILL 0x00
#define STILL_LEFT 0x01
#define STILL_RIGHT 0x02
#define FORWARD_STRAIGHT 0x10
#define FORWARD_LEFT 0x11
#define FORWARD_RIGHT 0x12
#define BACKWARD_STRAIGHT 0x20
#define BACKWARD_LEFT 0x21
#define BACKWARD_RIGHT 0x22

#endif // !DRIVER_H
