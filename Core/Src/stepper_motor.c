#include "stepper_motor.h"
#include "stepper_motor_driver.h"
#include "ultrasonic_sensor.h"

typedef enum 
{
    STEP_MOTOR_IDLE_STATE = 0,
    STEP_MOTOR_RUN_STATE,
    STEP_MOTOR_CHANGE_SPEED_STATE,
    STEP_MOTOR_CHANGE_DIRECTION_STATE
} stepper_motor_state_e;

stepper_motor_state_e stepper_motor_state = STEP_MOTOR_IDLE_STATE;

typedef enum
{
    STEP_SPEED_SLOW = 0,
    STEP_SPEED_NORMAL,
    STEP_SPEED_FAST
} stepper_motor_speed_e;

stepper_motor_speed_e stepper_motor_speed = STEP_SPEED_SLOW;


void run_stepper_motor_task()
{
    switch (stepper_motor_state)
    {
        case STEP_MOTOR_IDLE_STATE: break; //do nothing

        case STEP_MOTOR_RUN_STATE:
        step_motor_run_state();
        break;

        case STEP_MOTOR_CHANGE_SPEED_STATE:
        //only a button press should put us in this state
        step_motor_change_speed_state();
        break;

        case STEP_MOTOR_CHANGE_DIRECTION_STATE:
        step_motor_change_direction_state();
        break;
    }
}

/********************************************************************/

void start_step_motor_task(void)
{
    stepper_motor_state = STEP_MOTOR_RUN_STATE;
}

void stop_step_motor_task(void)
{
    stepper_motor_state = STEP_MOTOR_IDLE_STATE;
}

void stepper_motor_init_all(void)
{
    stepper_init();
    for (uint8_t inst = 0; inst < UNITS; inst += 2)
    {
        stepper_set_speed(inst, FAST_RPM);
    }
}

void step_all(uint32_t steps, uint8_t dir)
{
    for (uint8_t inst = 0; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CCW);
    }

    for (uint8_t inst = 1; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CW);
    }
}

void step_motor_run_state()
{
    /*
        1. Take an additional x steps
        2. Check the ultrasonic isBlocked flag
        3. if isBlocked == true, then enter change direction state
        4. else, enter run state again
    */

    int32_t steps = 100;

    step_all(steps, DIR_CW);

    if (get_isBlocked() == BLOCKED)
    {
        stepper_motor_state = STEP_MOTOR_CHANGE_DIRECTION_STATE;
    }
    else
    {
        //stay in the same state if we are not blocked
        stepper_motor_state = STEP_MOTOR_RUN_STATE;
    }
}

/********************************************************************/

uint32_t calculate_turning_steps(uint16_t deg)
{
    uint32_t turning_steps = deg * DEG_TO_STEP_CONV;
    return turning_steps; 
}

void step_motor_turn_left(uint16_t deg)
{
    uint32_t steps = calculate_turning_steps(deg);

    for (uint8_t inst = 0; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CW);
    }

    for (uint8_t inst = 1; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CW);
    }
}

void step_motor_turn_right(uint16_t deg)
{
    uint32_t steps = calculate_turning_steps(deg);

    for (uint8_t inst = 0; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CCW); 
    }

    for (uint8_t inst = 1; inst < UNITS; inst += 2)
    {
        stepper_step_nonblocking(inst, steps, DIR_CCW);
    }
}

void step_motor_turn(uint8_t turn_dir, uint16_t deg)
{
    if (turn_dir == TURN_LEFT)
    {
        step_motor_turn_left(deg);
    }
    else
    {
        step_motor_turn_right(deg);
    }
}

void stop_all_motors()
{
    for (uint8_t inst = 0; inst < UNITS; inst++)
    {
        stepper_stop(inst);
    }
}

void step_motor_change_direction_state()
{
    //currently we will opt to always turn left
    uint8_t turn_dir = TURN_LEFT;
    uint16_t turn_deg = 10;

    stop_all_motors();
    
    step_motor_turn(turn_dir, turn_deg);

    if (get_isBlocked() == BLOCKED)
    {   
        //still blocked so we will continue to turn
        stepper_motor_state = STEP_MOTOR_CHANGE_DIRECTION_STATE;
    }
    else
    {
        stepper_motor_state = STEP_MOTOR_RUN_STATE;
    }
}
/********************************************************************/

void step_change_speed_all(uint16_t rpm)
{
    for (uint8_t inst = 0; inst < UNITS; inst++)
    {
        stepper_set_speed(inst, rpm);
    }
}

void step_motor_change_speed_state()
{
    switch (stepper_motor_speed)
    {
        case STEP_SPEED_SLOW:
        step_change_speed_all(SLOW_RPM);
        stepper_motor_speed = STEP_SPEED_NORMAL;
        break;

        case STEP_SPEED_NORMAL:
        step_change_speed_all(NORMAL_RPM);
        stepper_motor_speed = STEP_SPEED_FAST;
        break;

        case STEP_SPEED_FAST:
        step_change_speed_all(FAST_RPM);
        stepper_motor_speed = STEP_SPEED_SLOW;
        break;
    }

    stepper_motor_state = STEP_MOTOR_RUN_STATE;
}