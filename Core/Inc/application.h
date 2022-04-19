#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>
#include "main.h"
#include "stepper_motor.h"
#include "ultrasonic_sensor.h"

/**
  * @brief Run application statemachine that starts/stops all other state machines
  * @param None
  * @retval None
  */
void run_application_task();

#endif