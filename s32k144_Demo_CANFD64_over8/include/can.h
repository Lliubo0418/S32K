#ifndef CAN_H_
#define CAN_H_

/*������պͷ�������*/
#define Rx_Message_buffer_CAN0 (0UL)
#define Tx_Message_buffer_CAN0 (1UL)
#define Rx_Message_buffer_CAN1 (2UL)
#define Tx_Message_buffer_CAN1 (3UL)
/*������ܹ���*/
#define Rx_Filter 0x0

/*CAN0��ʼ��*/
void CAN0_Init();
/*CAN1��ʼ��*/
void CAN1_Init();
/*CAN0�ص�����*/
void CAN0_CallBack_Function(uint32_t ,can_event_t ,uint32_t );
/*CAN1�ص�����*/
void CAN1_CallBack_Function(uint32_t ,can_event_t ,uint32_t );

void Send_CANMsg(uint16_t canId,uint8_t canData[8],uint16_t Length);

void Send_CANFDMsg(uint16_t canId,uint16_t canData[64],uint16_t Length);

#endif
