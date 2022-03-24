#include "stepper_motor_driver.h"

stepper_config_s STEPPER_CONFIG[UNITS] = 
{
    {


    },
    {

        
    }
};

stepper_info_s STEPPER_INFO[UNITS] = {0};

static uint8_t UNIPOLAR_WD_PATTERN[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
};
static uint8_t UNIPOLAR_FS_PATTERN[4][4] = {
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 1},
		{1, 0, 0, 1}
};
static uint8_t UNIPOLAR_HS_PATTERN[8][4] = {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 1},
		{1, 0, 0, 1}
}; 

void stepper_init(void){

    for (int i = 0; i < UNITS; i++)
    {
        //GPIOS are already initalized by MX CUBE

        //setting default values
        STEPPER_INFO[i].steps = 0;
        STEPPER_INFO[i].ticks = 0;
        STEPPER_INFO[i].max_ticks = 0;
        STEPPER_INFO[i].rpm = 0;
        STEPPER_INFO[i].step_index = 0;
        STEPPER_INFO[i].dir = DIR_CW;

        //setting max index
        if (STEPPER_CONFIG[i].stepping_mode == FULL_STEP || STEPPER_CONFIG[i].stepping_mode == WAVE_STEP){
            STEPPER_INFO[i].max_index = 4 - 1;
        }
        else if (STEPPER_CONFIG[i].stepping_mode == HALF_STEP){
            STEPPER_INFO[i].max_index = 8 - 1;
        }
    }
}

void stepper_set_speed(uint8_t inst, uint16_t rpm){

    uint32_t total_steps = 0;

    STEPPER_INFO[inst].rpm = rpm;

    if (STEPPER_CONFIG[inst].stepping_mode == HALF_STEP){
        total_steps = STEPPER_CONFIG[inst].steps_per_rev * 2;
    }
    else{
        total_steps = STEPPER_CONFIG[inst].steps_per_rev;
    }

    STEPPER_INFO[inst].max_ticks = 1; //TODO, FIGURE OUT THIS CALCULATION WITH THE TIMER CONFIG CHOSEN
}

//since this function is only being used within the driver, declare it as static
static void stepper_step_once(uint8_t inst){
    //taking a step
    if (STEPPER_CONFIG[inst].stepping_mode == FULL_STEP){
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[0], STEPPER_CONFIG[inst].pin[0], UNIPOLAR_FS_PATTERN[STEPPER_INFO[inst].step_index][0]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[1], STEPPER_CONFIG[inst].pin[1], UNIPOLAR_FS_PATTERN[STEPPER_INFO[inst].step_index][1]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[2], STEPPER_CONFIG[inst].pin[2], UNIPOLAR_FS_PATTERN[STEPPER_INFO[inst].step_index][2]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[3], STEPPER_CONFIG[inst].pin[3], UNIPOLAR_FS_PATTERN[STEPPER_INFO[inst].step_index][3]);
    }

    else if (STEPPER_CONFIG[inst].stepping_mode = HALF_STEP){
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[0], STEPPER_CONFIG[inst].pin[0], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][0]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[1], STEPPER_CONFIG[inst].pin[1], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][1]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[2], STEPPER_CONFIG[inst].pin[2], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][2]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[3], STEPPER_CONFIG[inst].pin[3], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][3]);
    }

    else if (STEPPER_CONFIG[inst].stepping_mode = WAVE_STEP){
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[0], STEPPER_CONFIG[inst].pin[0], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][0]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[1], STEPPER_CONFIG[inst].pin[1], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][1]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[2], STEPPER_CONFIG[inst].pin[2], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][2]);
        HAL_GPIO_WritePin(STEPPER_CONFIG[inst].gpio[3], STEPPER_CONFIG[inst].pin[3], UNIPOLAR_HS_PATTERN[STEPPER_INFO[inst].step_index][3]);
    }

    //updating the step_index based on direction

    if (STEPPER_INFO[inst].dir == DIR_CW){
        
        if (STEPPER_INFO[inst].step_index == STEPPER_INFO[inst].max_index){
            STEPPER_INFO[inst].step_index = 0;
        }

        STEPPER_INFO[inst].step_index++;
    } 

    if(STEPPER_INFO[inst].dir = DIR_CCW){
        
        if(STEPPER_INFO[inst].step_index == 0){
            STEPPER_INFO[inst].step_index = STEPPER_INFO[inst].max_index;
        }

        STEPPER_INFO[inst].step_index--;
    }
}

void stepper_step_nonblocking(uint8_t inst, uint32_t steps, uint8_t dir){
    STEPPER_INFO[inst].steps = steps;
    STEPPER_INFO[inst].dir = dir;
}

void stepper_stop(uint8_t inst){
    STEPPER_INFO[inst].steps = 0;
}

//TODO: CALL THIS FUNCTION IN HAL TIM OVERFLOW CALLBACK FUNCTION
void stepper_TIM_OVF_ISR(TIM_HandleTypeDef* htim){

    if (htim->Instance == STEPPER_TIMER){
        
        for (int i = 0; i < UNITS; i++){
            if ((STEPPER_INFO[i].ticks >= STEPPER_INFO[i].max_ticks) && (STEPPER_INFO[i].steps > 0)){
                stepper_step_once(i);
                STEPPER_INFO[i].steps--;
                STEPPER_INFO[i].ticks = 0;
            }
            
            else{
                STEPPER_INFO[i].ticks++;
            }
        }      
    }
} 