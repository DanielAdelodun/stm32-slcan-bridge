#include "stm32f4xx_hal.h"

#ifndef _CAN_H
#define _CAN_H

#define CAN_FilterRTR		(0x00000002U)
#define CAN_FilterIDE		(0x00000004U)
#define BitRateMagic		300				// Prescaler needed for 10kbit/s (Minimum Speed).

typedef enum {
    CAN_BITRATE_10K,
    CAN_BITRATE_20K,
    CAN_BITRATE_50K,
    CAN_BITRATE_100K,
    CAN_BITRATE_125K,
    CAN_BITRATE_250K,
    CAN_BITRATE_500K,
    CAN_BITRATE_750K,
    CAN_BITRATE_1000K,
} CAN_BitRate_TypeDef;

typedef enum {
	CAN_MODE_SET,
	CAN_MODE_UNSET,
	CAN_MODE_TOGGLE
} CAN_ModeState_TypeDef;

typedef enum {
	CAN_NORMAL 			= (0x00000000U),	// Mode 0
	CAN_LOOPBACK		= (0x00000001U),	// Mode 1
	CAN_LISTEN			= (0x00000002U),	// Mode 2 (AKA Silent)
	CAN_TEST			= (0x00000003U)		// Mode 3 (AKA Loopback + Listen)
} CAN_Mode_TypeDef;


HAL_CAN_StateTypeDef CAN_SetBitRate(CAN_HandleTypeDef *, CAN_BitRate_TypeDef);
HAL_CAN_StateTypeDef CAN_SetListen(CAN_HandleTypeDef *, CAN_ModeState_TypeDef);
HAL_CAN_StateTypeDef CAN_SetLoopback(CAN_HandleTypeDef *, CAN_ModeState_TypeDef);
HAL_CAN_StateTypeDef CAN_SetMode(CAN_HandleTypeDef *, CAN_Mode_TypeDef);
HAL_CAN_StateTypeDef CAN_SetFilter(CAN_HandleTypeDef *, uint32_t, uint32_t);

#endif // _CAN_H
