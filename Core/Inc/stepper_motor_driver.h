#ifndef STEPPER_MOTOR_DRIVER_H
#define STEPPER_MOTOR_DRIVER_H

#include <stdint.h>
#include <math.h>
#include "main.h"

#define HALF_STEP 0
#define FULL_STEP 1
#define WAVE_STEP 2

#define DIR_CW 0
#define DIR_CCW 1

#define STEPPER_TIMER TIM3
#define MAX_TICKS_PER_MIN 60000.0 //This value is based TIM3... ((Frequency/Prescaler) / Period) * 60

#define UNITS 2

#define STEPPER_0 0
#define STEPPER_1 1


typedef struct stepper_info_s{
    uint32_t steps;
    uint32_t ticks;
    uint32_t max_ticks;
    uint16_t rpm;
    uint8_t step_index;
    uint8_t max_index;
    uint8_t dir;
}stepper_info_s;

typedef struct stepper_config_s{
    GPIO_TypeDef * gpio[4];
    uint16_t pin[4];
    uint16_t steps_per_rev;
    uint8_t stepping_mode;
}stepper_config_s;

void stepper_init(void);

void stepper_set_speed(uint8_t inst, uint16_t rpm);

static void stepper_step_once(uint8_t inst);

void stepper_step_nonblocking(uint8_t inst, uint32_t steps, uint8_t dir);

void stepper_stop(uint8_t inst);

#endif