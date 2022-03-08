#include "ultrasonic_sensor.h"

typedef enum
{
    ULTRASONIC_IDLE_STATE = 0,
    ULTRASONIC_SEND_PULSE_STATE
} ultrasonic_sensor_state_e;

ultrasonic_sensor_state_e ultrasonic_sensor_state = ULTRASONIC_IDLE_STATE;

uint8_t isBlocked = NOT_BLOCKED;

void run_ultrasonic_task()
{
    switch (ultrasonic_sensor_state)
    {
        case ULTRASONIC_IDLE_STATE: break; //do nothing

        case ULTRASONIC_SEND_PULSE_STATE:
        //send a couple of pulses and average out the distance to make a decision
        break;
    }
}

void get_isBlocked()
{
    return isBlocked;
}

/**********************************************************************************/

