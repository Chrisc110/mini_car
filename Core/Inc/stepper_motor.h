#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <stdint.h>
#include "main.h"

#define TURN_RIGHT 0
#define TURN_LEFT 1

#define DEG_TO_STEP_CONV 23

#define SLOW_RPM 5
#define NORMAL_RPM 8
#define FAST_RPM 10

/**
  * @brief Runs the stepper motor state machine that controls the state of the stepper motor
  * @param None
  * @retval None
  */
void run_stepper_motor_task();

void start_step_motor_task(void);

void stop_step_motor_task(void);

#endif