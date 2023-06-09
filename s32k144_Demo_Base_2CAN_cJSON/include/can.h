#ifndef CAN_H_
#define CAN_H_

/*定义接收和发送邮箱*/
#define Rx_Message_buffer_CAN0 (0UL)
#define Tx_Message_buffer_CAN0 (1UL)
#define Rx_Message_buffer_CAN1 (2UL)
#define Tx_Message_buffer_CAN1 (3UL)
#define Rx_Message_buffer_CAN2 (4UL)
#define Tx_Message_buffer_CAN2 (5UL)
/*定义接受过滤*/
#define Rx_Filter 0x0

/*CAN0初始化*/
void CAN0_Init();

/*CAN1初始化*/
void CAN1_Init();

/*CAN2初始化*/
void CAN2_Init();

/*CAN0回调函数*/
void CAN0_CallBack_Function(uint32_t ,can_event_t ,uint32_t );

/*CAN1回调函数*/
void CAN1_CallBack_Function(uint32_t ,can_event_t ,uint32_t );

/*CAN报文发送*/
void Send_CANMsg(uint8_t SCH_No,uint8_t CH_no,uint16_t canId,uint16_t canData[8],uint16_t Length);//

/*CANFD报文发送*/
void Send_CANFDMsg(uint8_t CH_no,uint16_t canId,uint16_t canData[64],uint16_t Length);// 此函数暂未更改

void Rule_Send_CANMsg(uint8_t CH_no);
void Rule_Send_CANFDMsg(uint8_t CH_no);
#endif
