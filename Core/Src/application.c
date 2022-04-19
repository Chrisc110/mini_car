#include "application.h"

typedef enum
{
    APP_IDLE_STATE = 0,
    APP_START_STATE,
    APP_STOP_STATE
} application_state_e;

static application_state_e application_state = APP_START_STATE;

void run_application_task()
{
    switch (application_state)
    {
        case APP_IDLE_STATE: break;//do nothing
        
        case APP_START_STATE:
        // Put all other state machines in their first state
        app_start_state();
        break;

        case APP_STOP_STATE:
        // We will go in this state if a button is pressed
        // This will put all the other state machines in the idle state, so nothing will happen
        application_state = APP_IDLE_STATE;
        break;

    }
}

/********************************************************************/

void app_start_state(void)
{
    start_step_motor_task();
    start_ultrasonic_sensor_task();
    application_state = APP_IDLE_STATE;
}

void app_stop_state(void)
{
    stop_step_motor_task();
    stop_ultrasonic_task();
    application_state = APP_IDLE_STATE;
}