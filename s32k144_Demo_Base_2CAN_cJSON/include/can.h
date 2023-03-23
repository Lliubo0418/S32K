#ifndef CAN_H_
#define CAN_H_

/*������պͷ�������*/
#define Rx_Message_buffer_CAN0 (0UL)
#define Tx_Message_buffer_CAN0 (1UL)
#define Rx_Message_buffer_CAN1 (2UL)
#define Tx_Message_buffer_CAN1 (3UL)
#define Rx_Message_buffer_CAN2 (4UL)
#define Tx_Message_buffer_CAN2 (5UL)
/*������ܹ���*/
#define Rx_Filter 0x0

/*CAN0��ʼ��*/
void CAN0_Init();

/*CAN1��ʼ��*/
void CAN1_Init();

/*CAN2��ʼ��*/
void CAN2_Init();

/*CAN0�ص�����*/
void CAN0_CallBack_Function(uint32_t ,can_event_t ,uint32_t );

/*CAN1�ص�����*/
void CAN1_CallBack_Function(uint32_t ,can_event_t ,uint32_t );

/*CAN���ķ���*/
void Send_CANMsg(uint8_t SCH_No,uint8_t CH_no,uint16_t canId,uint16_t canData[8],uint16_t Length);//

/*CANFD���ķ���*/
void Send_CANFDMsg(uint8_t CH_no,uint16_t canId,uint16_t canData[64],uint16_t Length);// �˺�����δ����

void Rule_Send_CANMsg(uint8_t CH_no);
void Rule_Send_CANFDMsg(uint8_t CH_no);
#endif
