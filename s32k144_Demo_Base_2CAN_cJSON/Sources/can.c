#include "cpu.h"
#include "can.h"


/*����������Ϊȫ�ֱ�����ÿ�ζ����ҵ�����ֲ������ᵼ�±�ΪԶ��֡*/
/*���ͻ���������*/
can_buff_config_t Tx_buffCfg={
		.enableBRS=false,
		.enableFD=false,
		.fdPadding=0U,
		.idType=CAN_MSG_ID_STD,
		.isRemote=false
};
/*���ջ���������*/
can_buff_config_t Rx_buffCfg={
		.enableBRS=false,
		.enableFD=false,
		.fdPadding=0U,
		.idType=CAN_MSG_ID_STD,
		.isRemote=false
};



/*���屨����Ϣ��ʽ*/
can_message_t *CANx;
can_message_t recvMsg_CAN0;
can_message_t recvMsg_CAN1;
can_message_t recvMsg_CAN2;

can_message_t sendFDMsg;
can_message_t sendMsg;

can_instance_t p;



/*���պͷ����ж�*/
char IRQ_CAN0_RX;
char IRQ_CAN0_TX;

char IRQ_CAN1_RX;
char IRQ_CAN1_TX;

char IRQ_CAN2_RX;
char IRQ_CAN2_TX;

/*�ص�����*/
void CAN0_CallBack_Function(uint32_t instance,can_event_t eventType,uint32_t buffIdx){
	(void) instance;
	(void) buffIdx;

	CAN_Receive(&can_pal0_instance,Rx_Message_buffer_CAN0,&recvMsg_CAN0);   //ָ�����ջ������ͽ�������
	switch(eventType){
	case CAN_EVENT_RX_COMPLETE:
		IRQ_CAN0_RX=1;
		break;
	case CAN_EVENT_TX_COMPLETE:
		IRQ_CAN0_TX=1;
		break;
	}


}

void CAN1_CallBack_Function(uint32_t instance,can_event_t eventType,uint32_t buffIdx){
	(void) instance;
	(void) buffIdx;

	CAN_Receive(&can_pal1_instance,Rx_Message_buffer_CAN1,&recvMsg_CAN1);
	switch(eventType){
	case CAN_EVENT_RX_COMPLETE:
		IRQ_CAN1_RX=1;
		break;
	case CAN_EVENT_TX_COMPLETE:
		IRQ_CAN1_TX=1;
		break;
	}
}

void CAN2_CallBack_Function(uint32_t instance,can_event_t eventType,uint32_t buffIdx){
	(void) instance;
	(void) buffIdx;

	CAN_Receive(&can_pal2_instance,Rx_Message_buffer_CAN2,&recvMsg_CAN2);
	switch(eventType){
	case CAN_EVENT_RX_COMPLETE:
		IRQ_CAN2_RX=1;
		break;
	case CAN_EVENT_TX_COMPLETE:
		IRQ_CAN2_TX=1;
		break;
	}
}


/*CAN�˿ڳ�ʼ��*/
void CAN0_Init(){
	CAN_Init(&can_pal0_instance, &can_pal0_Config0);                    //��ʼ��
	CAN_ConfigTxBuff(&can_pal0_instance,Tx_Message_buffer_CAN0,&Tx_buffCfg);             //���÷��ͻ�����
	CAN_ConfigRxBuff(&can_pal0_instance,Rx_Message_buffer_CAN0,&Rx_buffCfg,Rx_Filter);   //���ý��ջ�����
	CAN_SetRxFilter(&can_pal0_instance,CAN_MSG_ID_STD,Rx_Message_buffer_CAN0,0);         //���ý��ܹ���
	CAN_Receive(&can_pal0_instance,Rx_Message_buffer_CAN0,&recvMsg_CAN0);                //ָ�����ջ������ͽ������䣨����ȥ������
	//CAN_Send(&can_pal0_instance,);
	CAN_InstallEventCallback(&can_pal0_instance,&CAN0_CallBack_Function,(void*)0);       //��װ�ص�����

}

void CAN1_Init(){
	CAN_Init(&can_pal1_instance, &can_pal1_Config0);
	CAN_ConfigTxBuff(&can_pal1_instance,Tx_Message_buffer_CAN1,&Tx_buffCfg);
	CAN_ConfigRxBuff(&can_pal1_instance,Rx_Message_buffer_CAN1,&Rx_buffCfg,Rx_Filter);
	CAN_SetRxFilter(&can_pal1_instance,CAN_MSG_ID_STD,Rx_Message_buffer_CAN1,0);
	CAN_Receive(&can_pal1_instance,Rx_Message_buffer_CAN1,&recvMsg_CAN1);
	//CAN_Send(&can_pal1_instance,);
	CAN_InstallEventCallback(&can_pal1_instance,&CAN1_CallBack_Function,(void*)0);
}

void CAN2_Init(){
	CAN_Init(&can_pal2_instance, &can_pal2_Config0);
	CAN_ConfigTxBuff(&can_pal2_instance,Tx_Message_buffer_CAN2,&Tx_buffCfg);
	CAN_ConfigRxBuff(&can_pal2_instance,Rx_Message_buffer_CAN2,&Rx_buffCfg,Rx_Filter);
	CAN_SetRxFilter(&can_pal2_instance,CAN_MSG_ID_STD,Rx_Message_buffer_CAN2,0);
	CAN_Receive(&can_pal2_instance,Rx_Message_buffer_CAN2,&recvMsg_CAN2);
	CAN_InstallEventCallback(&can_pal2_instance,&CAN2_CallBack_Function,(void*)0);

}



void Send_CANMsg(uint8_t SCH_No,uint8_t CH_no,uint16_t canId,uint16_t canData[8],uint16_t Length){   ///

	sendMsg.id=canId;
	sendMsg.length=Length;
	uint8_t tx_No;    //���������
	uint8_t rx_No;    //���������
	uint8_t i;

	switch(SCH_No){                            //Դͨ��
	case 1:
		CANx=&recvMsg_CAN0;
		break;
	case 2:
		CANx=&recvMsg_CAN1;
		break;
	case 3:
		CANx=&recvMsg_CAN2;
		break;
	default:
		break;
	}

	switch(CH_no){                          //Ŀ��ͨ��
	case 1:

		p=can_pal0_instance;                //ֻҪ�����ṹ��������ͬ���Ϳ���ʵ�ָ�ֵ���õȺ�
		tx_No=1;
		rx_No=0;
		break;
	case 2:

		p=can_pal1_instance;
		tx_No=3;
		rx_No=2;
		break;
	case 3:

		p=can_pal2_instance;
		tx_No=5;
		rx_No=4;
		break;
	default:
		break;
	}
	for(i=0;i<Length;i++){
		sendMsg.data[i]=i<Length?CANx->data[i]+1:0;
	}

	CAN_Send(&p,tx_No,&sendMsg);
	while(CAN_GetTransferStatus(&p,tx_No)==STATUS_BUSY);
}


void Send_CANFDMsg(uint8_t CH_no,uint16_t canId,uint16_t canData[64],uint16_t Length){        //

	sendFDMsg.id=canId;
	sendFDMsg.length=Length;
	uint8_t tx_No;    //���������
	uint8_t rx_No;    //���������
	uint8_t i;
	for(i=0;i<Length;i++){
		sendFDMsg.data[i]=i<Length?recvMsg_CAN0.data[i]+1:0;
	}
	switch(CH_no){
	case 1:
		p=can_pal0_instance;
		tx_No=1;                         //����Ӧ��������Զ��֡�������������⣺�ڴ������ֻ�ܴ��7��64λ���䣩
		rx_No=0;
		break;
	case 2:
		p=can_pal1_instance;
		tx_No=3;
		rx_No=2;
		break;
	case 3:
		p=can_pal2_instance;
		tx_No=5;
		rx_No=4;
		break;
	default:
		break;
	}
	CAN_Send(&p,tx_No,&sendFDMsg);
	while(CAN_GetTransferStatus(&p,tx_No)==STATUS_BUSY);                //�ж��Ƿ������
}








