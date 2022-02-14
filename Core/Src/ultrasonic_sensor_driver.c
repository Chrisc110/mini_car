#include "ultrasonic_sensor_driver.h"

#define TIME_DISTANCE_CONVERSION_CM 0.0003315

#define CHANNEL0 0

extern TIM_HandleTypeDef htim2;

bool u0_calculation_complete;
double u0_distance;
uint32_t u0_time0;
uint32_t u0_time1;
bool u0_is_time0_captured;

typedef struct ultrasonic_config_s
{
    GPIO_TypeDef * trig_port;
    uint16_t       trig_pin;
    TIM_HandleTypeDef *  timer;
    uint16_t       timer_channel;
    uint32_t       timer_freq_MHz;
    bool           calculation_complete;
    double         distance;
    uint32_t       time0;
    uint32_t       time1;
    bool           is_time0_captured;
}ultrasonic_config_s;

static ultrasonic_config_s ultrasonic_config[NUM_ULTRASONIC_CHANNELS] = 
{
    {GPIOA, GPIO_PIN_1, &htim2, TIM_CHANNEL_1, TIM2_FREQ_MHz, false, 0,0,0,0}
};

double ultrasonic_get_distance(uint8_t ultrasonic_channel)
{
    uint16_t distance = 0;

    /*
        Call the trig function
        Let the interrupts do its thing and collect the information
        Add a while loop that waits for a flag to get set then calculate the distance and distance
        Return distance

    */

   ultrasonic_trigger(ultrasonic_channel);

   while (!ultrasonic_config[ultrasonic_channel].calculation_complete);

   return ultrasonic_config[ultrasonic_channel].distance;
}

void ultrasonic_trigger(uint8_t ultrasonic_channel)
{
    HAL_GPIO_WritePin(ultrasonic_config[ultrasonic_channel].trig_port, ultrasonic_config[ultrasonic_channel].trig_pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay_us(10);  // wait for 10 us
	HAL_GPIO_WritePin(ultrasonic_config[ultrasonic_channel].trig_port, ultrasonic_config[ultrasonic_channel].trig_pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        if (!ultrasonic_config[CHANNEL0].is_time0_captured)
        {
            ultrasonic_config[CHANNEL0].time0 = HAL_TIM_ReadCapturedValue(htim, ultrasonic_config[CHANNEL0].timer_channel);
            ultrasonic_config[CHANNEL0].is_time0_captured = true;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, ultrasonic_config[CHANNEL0].timer_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
        }

        else
        {
            ultrasonic_config[CHANNEL0].time1 = HAL_TIM_ReadCapturedValue(htim, ultrasonic_config[CHANNEL0].timer_channel);
            __HAL_TIM_SET_COUNTER(htim, 0);

            if (ultrasonic_config[CHANNEL0].time1 > ultrasonic_config[CHANNEL0].time0)
            {
                ultrasonic_config[CHANNEL0].distance = ((ultrasonic_config[CHANNEL0].time1 - ultrasonic_config[CHANNEL0].time0) * TIME_DISTANCE_CONVERSION_CM) / 2.0;
            }

            else
            {
                ultrasonic_config[CHANNEL0].distance = ((0xFFFF - ultrasonic_config[CHANNEL0].time0 + ultrasonic_config[CHANNEL0].time1) * TIME_DISTANCE_CONVERSION_CM) / 2.0;
            }

            ultrasonic_config[CHANNEL0].calculation_complete = true;
            ultrasonic_config[CHANNEL0].is_time0_captured = false;

            __HAL_TIM_SET_CAPTUREPOLARITY(htim, ultrasonic_config[CHANNEL0].timer_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
            __HAL_TIM_DISABLE_IT(ultrasonic_config[CHANNEL0].timer, TIM_IT_CC1);
        }
    }
}

void delay_us (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER (&htim2) < time);
}