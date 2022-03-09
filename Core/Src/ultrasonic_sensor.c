#include "ultrasonic_sensor.h"
#include "ultrasonic_sensor_driver.h"

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

void update_blocked(double distance)
{
    if (distance < 20)
    {
        isBlocked = NOT_BLOCKED;
    }
    else
    {
        isBlocked = BLOCKED;
    }
}

void ultrasonic_send_pulse_state()
{
    const uint8_t inst = 0; 
    double distance = ultrasonic_get_distance(inst);
    
    update_blocked (distance);

    ultrasonic_sensor_state = ULTRASONIC_SEND_PULSE_STATE;
}