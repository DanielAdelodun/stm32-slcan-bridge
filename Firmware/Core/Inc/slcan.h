#ifndef _SLCAN_H
#define _SLCAN_H

#define SLCAN_MTU 30  // (sizeof("T1111222281122334455667788EA5F\r")+1)

#define SLCAN_STD_ID_LEN 3 // MAX VALUE 7FF
#define SLCAN_EXT_ID_LEN 8 // MAX VALUE 1FFFFFFF

typedef enum {
	SLCAN_OK,
	SLCAN_ERROR
} SLCAN_StatusTypeDef;

SLCAN_StatusTypeDef SLCAN_Parse_Frame(CAN_RxHeaderTypeDef *pHeader, uint8_t aData[], uint8_t *Buffer, uint32_t *Len);
SLCAN_StatusTypeDef SLCAN_Parse_Str(uint8_t *Buffer, uint32_t Len, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[]);
void ClearSLCAN();

extern uint32_t SLCAN_CurrentFilterID;
extern uint32_t SLCAN_CurrentFilterMask;
extern uint8_t SLCAN_CommandStringRx[SLCAN_MTU];
extern uint8_t SLCAN_CommandStringTx[SLCAN_MTU];

#endif // _SLCAN_H
