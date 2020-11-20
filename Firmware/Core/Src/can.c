#define TOGGLE_BIT(REG,BIT) 			((REG) ^= (BIT))

#include "stm32f4xx_hal.h"
#include "can.h"
#include "led.h"

static CAN_FilterTypeDef Filter;
static uint32_t Prescaler;

/* Setting up a Filter is very convoluted.
 * See RM0390 Page 1042 for more information on Filtering.
 */
HAL_CAN_StateTypeDef CAN_SetFilter(CAN_HandleTypeDef *hcan, uint32_t ID, uint32_t Mask) {
    // See page 1042 of RM0390 for details on filters
    Filter.FilterIdHigh = (ID >> 16) & 0x0000FFFF;
    Filter.FilterIdLow = (ID & 0x0000FFFF);

    Filter.FilterMaskIdHigh = (Mask >> 16) & 0x0000FFFF;
    Filter.FilterMaskIdLow = (ID & 0x0000FFFF);

    Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    Filter.FilterBank = 0;
    Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    Filter.FilterScale = CAN_FILTERSCALE_32BIT;
    Filter.FilterActivation = ENABLE;
    Filter.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(hcan, &Filter) != HAL_OK){
		return HAL_CAN_STATE_ERROR;
	}

	return HAL_CAN_GetState(hcan);
}

/* For the following CAN functions, the CAN Peripheral must be in it's initialisation mode.
 * This is done by setting the INRQ Bit in the MCR Register to 1 (with SLEEP set to 0) (i.e. call HAL_CAN_Stop() first.)
 * We can then set the Mode (SILM and LBKM Bits) and the Prescaler (BRP[9:0]) values in the CAN_BTR Register.
 *
 * See RM0390 Page 1048 for more information on the CAN_BTR register
 *
 */


HAL_CAN_StateTypeDef CAN_SetBitRate(CAN_HandleTypeDef *hcan, CAN_BitRate_TypeDef BitRate) {
    if (HAL_CAN_GetState(hcan) != HAL_CAN_STATE_READY) {
        return HAL_CAN_STATE_ERROR;
    }

    // BitRateMagic defined in "can.h"
    switch (BitRate) {
    case CAN_BITRATE_10K:
    	Prescaler = BitRateMagic / 1;
        break;
    case CAN_BITRATE_20K:
    	Prescaler = BitRateMagic / 2;
        break;
    case CAN_BITRATE_50K:
    	Prescaler = BitRateMagic / 5;
        break;
    case CAN_BITRATE_100K:
        Prescaler = BitRateMagic / 10;
        break;
    case CAN_BITRATE_125K:
        Prescaler = BitRateMagic * 2 / 25 ;
        break;
    case CAN_BITRATE_250K:
        Prescaler = BitRateMagic / 25;
        break;
    case CAN_BITRATE_500K:
        Prescaler = BitRateMagic / 50;
        break;
    case CAN_BITRATE_750K:
        Prescaler = BitRateMagic / 75;
        break;
    case CAN_BITRATE_1000K:
        Prescaler = BitRateMagic / 100;
        break;
    default:
    	return HAL_CAN_STATE_ERROR;
    	break;
    }

    MODIFY_REG(hcan->Instance->BTR, CAN_BTR_BRP_Msk, Prescaler - 1);

	return HAL_CAN_GetState(hcan);
}

HAL_CAN_StateTypeDef CAN_SetListen(CAN_HandleTypeDef *hcan, CAN_ModeState_TypeDef State) {
	if (HAL_CAN_GetState(hcan) != HAL_CAN_STATE_READY) {
		return HAL_CAN_STATE_ERROR;
	}

	switch (State)
	{
	case CAN_MODE_SET:
		SET_BIT(hcan->Instance->BTR, CAN_MODE_SILENT);
		break;
	case CAN_MODE_UNSET:
		CLEAR_BIT(hcan->Instance->BTR, CAN_MODE_SILENT);
		break;
	case CAN_MODE_TOGGLE:
		TOGGLE_BIT(hcan->Instance->BTR, CAN_MODE_SILENT);
		break;
	default:
		return HAL_CAN_STATE_ERROR;
		break;
	}

	return HAL_CAN_GetState(hcan);
}

HAL_CAN_StateTypeDef CAN_SetLoopback(CAN_HandleTypeDef *hcan, CAN_ModeState_TypeDef State) {
    if (HAL_CAN_GetState(hcan) != HAL_CAN_STATE_READY) {
        return HAL_CAN_STATE_ERROR;
    }

	switch (State)
	{
	case CAN_MODE_SET:
		SET_BIT(hcan->Instance->BTR, CAN_MODE_LOOPBACK);
		break;
	case CAN_MODE_UNSET:
		CLEAR_BIT(hcan->Instance->BTR, CAN_MODE_LOOPBACK);
		break;
	case CAN_MODE_TOGGLE:
		TOGGLE_BIT(hcan->Instance->BTR, CAN_MODE_LOOPBACK);
		break;
	default:
		return HAL_CAN_STATE_ERROR;
		break;
	}

	return HAL_CAN_GetState(hcan);

}

HAL_CAN_StateTypeDef CAN_SetMode(CAN_HandleTypeDef *hcan, CAN_Mode_TypeDef Mode) {
    if (HAL_CAN_GetState(hcan) != HAL_CAN_STATE_READY) {
        return HAL_CAN_STATE_ERROR;
    }

	switch (Mode)
	{
	case CAN_NORMAL:
	    MODIFY_REG(hcan->Instance->BTR, (CAN_BTR_LBKM_Msk | CAN_BTR_SILM_Msk), CAN_MODE_NORMAL);
		break;
	case CAN_LOOPBACK:
	    MODIFY_REG(hcan->Instance->BTR, (CAN_BTR_LBKM_Msk | CAN_BTR_SILM_Msk), CAN_MODE_LOOPBACK);
		break;
	case CAN_LISTEN:
	    MODIFY_REG(hcan->Instance->BTR, (CAN_BTR_LBKM_Msk | CAN_BTR_SILM_Msk), CAN_MODE_SILENT);
		break;
	case CAN_TEST:
	    MODIFY_REG(hcan->Instance->BTR, (CAN_BTR_LBKM_Msk | CAN_BTR_SILM_Msk), CAN_MODE_SILENT_LOOPBACK);
		break;
	default:
		return HAL_CAN_STATE_ERROR;
		break;
	}

	return HAL_CAN_GetState(hcan);
}
