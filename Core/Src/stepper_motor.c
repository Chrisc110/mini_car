#include "stepper_motor.h"

typedef enum 
{
    STEP_MOTOR_IDLE_STATE = 0,
    STEP_MOTOR_RUN_STATE,
    STEP_MOTOR_CHANGE_SPEED_STATE,
    STEP_MOTOR_CHANGE_DIRECTION_STATE
} stepper_motor_state_e;

stepper_motor_state_e stepper_motor_state = STEP_MOTOR_IDLE_STATE;

void run_stepper_motor_task()
{

    switch (stepper_motor_state)
    {

        case STEP_MOTOR_IDLE_STATE: break; //do nothing

        case STEP_MOTOR_RUN_STATE:
        //go forward, both motors running at the same speed
        break;

        case STEP_MOTOR_CHANGE_SPEED_STATE:
        // change the variable that is holding the speed to what ever value we want
        break;

        case STEP_MOTOR_CHANGE_DIRECTION_STATE:
        // make only the left motor go, so we start to turn, continue to do this until the ultrasonic sensor says it is clear
        stepper_motor_state = STEP_MOTOR_RUN_STATE;
        break;

    }

}