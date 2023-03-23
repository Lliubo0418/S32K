#include "cpu.h"
#include "can.h"

/*发送缓冲区配置*/
can_buff_config_t Tx_buffCfg={
		.enableBRS=false,
		.enableFD=true,
		.fdPadding=0U,
		.idType=CAN_MSG_ID_STD,
		.isRemote=false
};
/*接收缓冲区配置*/
can_buff_config_t Rx_buffCfg={
		.enableBRS=false,
		.enableFD=true,
		.fdPadding=0U,
		.idType=CAN_MSG_ID_STD,
		.isRemote=false
};



/*定义报文消息格式*/
can_message_t recvMsg_CAN0;
can_message_t recvMsg_CAN1;
can_message_t sendFDMsg_CAN0;
can_message_t sendMsg_CAN0;




/*接收和发送中断*/
char IRQ_CAN0_RX;
char IRQ_CAN0_TX;

char IRQ_CAN1_RX;
char IRQ_CAN1_TX;

void CAN0_Init(){
	CAN_Init(&can_pal0_instance, &can_pal0_Config0);
	CAN_ConfigTxBuff(&can_pal0_instance,Tx_Message_buffer_CAN0,&Tx_buffCfg);
	CAN_ConfigRxBuff(&can_pal0_instance,Rx_Message_buffer_CAN0,&Rx_buffCfg,Rx_Filter);
	CAN_SetRxFilter(&can_pal0_instance,CAN_MSG_ID_STD,Rx_Message_buffer_CAN0,0);
	CAN_Receive(&can_pal0_instance,Rx_Message_buffer_CAN0,&recvMsg_CAN0);
	//CAN_Send(&can_pal0_instance,);
	CAN_InstallEventCallback(&can_pal0_instance,&CAN0_CallBack_Function,(void*)0);

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

void CAN0_CallBack_Function(uint32_t instance,can_event_t eventType,uint32_t buffIdx){
	(void) instance;
	(void) buffIdx;

	CAN_Receive(&can_pal0_instance,Rx_Message_buffer_CAN0,&recvMsg_CAN0);
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


void Send_CANMsg(uint16_t canId,uint16_t canData[8],uint16_t Length){

	sendMsg_CAN0.id=canId;
	sendMsg_CAN0.length=Length;
	/*
	uint8_t i;
	for(i=0;i<Length;i++){
		sendMsg.data[i]=i<Length?recvMsg_CAN0.data[i]+1:0;
	}
	*/
	CAN_Send(&can_pal0_instance,Tx_Message_buffer_CAN0,&sendMsg_CAN0);
	while(CAN_GetTransferStatus(&can_pal0_instance,Tx_Message_buffer_CAN0)==STATUS_BUSY);
}


void Send_CANFDMsg(uint16_t canId,uint16_t canData[64],uint16_t Length){

	sendFDMsg_CAN0.id=canId;
	sendFDMsg_CAN0.length=Length;
/*
	uint8_t i;
	for(i=0;i<Length;i++){
		sendFDMsg.data[i]=i<Length?buffer[i]+1:0;
	}
*/
	CAN_Send(&can_pal0_instance,Tx_Message_buffer_CAN0,&sendFDMsg_CAN0);
	while(CAN_GetTransferStatus(&can_pal0_instance,Tx_Message_buffer_CAN0)==STATUS_BUSY);
}











