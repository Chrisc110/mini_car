#include "ultrasonic_sensor_driver.h"

typedef struct ultrasonic_config_s
{
    GPIO_TypeDef * trig_gpio;
    uint16_t       trig_pin;
    TIM_TypeDef *  timer;
    uint16_t       timer_channel;
    uint32_t       timer_freq_MHz;
}ultrasonic_config_s;

static const ultrasonic_config_s ULTRASONIC_CONFIG[NUM_ULTRASONIC_CHANNELS] = 
{

    {}

}