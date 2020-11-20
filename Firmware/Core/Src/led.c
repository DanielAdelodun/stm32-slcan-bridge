//
// LED: Handles blinking of status light
// LD1 --> PB0 Green;
// LD2 --> PB7 Blue;
// LD3 --> PB14 Red
//

#include "stm32f4xx_hal.h"
#include "led.h"

static uint32_t LED_LastOn_Red = 0;
static uint32_t LED_LastOff_Red = 0;

static uint32_t LED_LastOn_Green = 0;
static uint32_t LED_LastOff_Green = 0;

static uint32_t LED_LastOn_Blue = 0;
static uint32_t LED_LastOff_Blue = 0;

void LED_RedForceOn(void)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
	LED_LastOn_Red = HAL_GetTick();
}

void LED_RedOn(void)
{
	// Make sure the LED has been off for at least LED_OFF_DURATION before turning on again
	// This prevents a solid status LED on a busy CAN bus
	if(LED_LastOn_Red == 0 && HAL_GetTick() - LED_LastOff_Red > LED_OFF_DURATION)
	{
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		LED_LastOn_Red = HAL_GetTick();
	}
}


void LED_GreenOn(void)
{
	// Make sure the LED has been off for at least LED_DURATION before turning on again
	if(LED_LastOn_Green == 0 && HAL_GetTick() - LED_LastOff_Green > LED_OFF_DURATION)
	{
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
		LED_LastOn_Green = HAL_GetTick();
	}
}

void LED_BlueOn(void)
{
	// Make sure the LED has been off for at least LED_DURATION before turning on again
	if(LED_LastOn_Blue == 0 && HAL_GetTick() - LED_LastOff_Blue > LED_OFF_DURATION)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
		LED_LastOn_Blue = HAL_GetTick();
	}
}

// Process time-based LED events
void LED_Process(void)
{
	// If red LED has been on for long enough, turn it off
	if(LED_LastOn_Red > 0 && HAL_GetTick() - LED_LastOn_Red > LED_RED_DURATION)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		LED_LastOn_Red = 0;
		LED_LastOff_Red = HAL_GetTick();
	}

	// If green LED has been on for long enough, turn it off
	if(LED_LastOn_Green > 0 && HAL_GetTick() - LED_LastOn_Green > LED_GREEN_DURATION)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
		LED_LastOn_Green = 0;
		LED_LastOff_Green = HAL_GetTick();
	}

	// If blue LED has been on for long enough, turn it off
	if(LED_LastOn_Blue > 0 && HAL_GetTick() - LED_LastOn_Blue > LED_BLUE_DURATION)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
		LED_LastOn_Blue = 0;
		LED_LastOff_Blue = HAL_GetTick();
	}
}

