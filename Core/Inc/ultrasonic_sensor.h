#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>
#include "main.h"

#define BLOCKED 0
#define NOT_BLOCKED 1

/**
  * @brief Runs the ultrasonic sensor state machine that controls the state of the ultrasonic sensor
  * @param None
  * @retval None
  */
void run_ultrasonic_task();

#endif