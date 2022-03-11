#ifndef ULTRASONIC_SENSOR_DRIVER_H
#define ULTRASONIC_SENSOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define NUM_ULTRASONIC_CHANNELS 1
#define TIM2_FREQ_MHz 84

/**
  * @brief Get the distance
  * @param ultrasonic_channel The ultrasonic sensor to use
  * @retval The distance
  */
double ultrasonic_get_distance(uint8_t ultrasonic_channel);

/**
  * @brief Send a ultrasonic pulse
  * @param ultrasonic_channel The ultrasonic sensor to use
  * @retval None
  */
void ultrasonic_trigger(uint8_t ultrasonic_channel);

/**
  * @brief Delay function in microseconds
  * @param time The time to delay
  * @retval None
  */
void delay_us (uint16_t time);

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif