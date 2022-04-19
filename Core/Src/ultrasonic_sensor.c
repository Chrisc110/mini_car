#include "ultrasonic_sensor.h"
#include "ultrasonic_sensor_driver.h"

typedef enum
{
    ULTRASONIC_IDLE_STATE = 0,
    ULTRASONIC_SEND_PULSE_STATE
} ultrasonic_sensor_state_e;

ultrasonic_sensor_state_e ultrasonic_state = ULTRASONIC_IDLE_STATE;

uint8_t isBlocked = NOT_BLOCKED;

void run_ultrasonic_task()
{
    switch (ultrasonic_state)
    {
        case ULTRASONIC_IDLE_STATE: break; //do nothing

        case ULTRASONIC_SEND_PULSE_STATE:
        //send a couple of pulses and average out the distance to make a decision
        ultrasonic_send_pulse_state();
        break;
    }
}

void get_isBlocked()
{
    return isBlocked;
}

/**********************************************************************************/

void start_ultrasonic_sensor_task(void)
{
    ultrasonic_state = ULTRASONIC_SEND_PULSE_STATE;
}

void stop_ultrasonic_sensor_task(void)
{
    ultrasonic_state = ULTRASONIC_IDLE_STATE;
}

void update_blocked(double distance)
{
    if (distance < 20)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
        isBlocked = NOT_BLOCKED;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
        isBlocked = BLOCKED;
    }
}

void ultrasonic_send_pulse_state()
{
    const uint8_t inst = 0; 
    double distance = ultrasonic_get_distance(inst);
    
    update_blocked (distance);

    ultrasonic_state = ULTRASONIC_SEND_PULSE_STATE;
}