#ifndef ROUTED_PROTOCOL_H
#define ROUTED_PROTOCOL_H
#include "Cpu.h"

//CAN0,CAN1,CAN2三个模块宏定义
#define CAN_0     0    //CAN0模块
#define CAN_1     1    //CAN1模块
#define CAN_2     2    //CAN2模块


//CAN_CALSSIC和CANFD两个模式
#define CAN_CLASSIC   0
#define CANFD         1


typedef enum MODE {
	CAN_NORMAL,
	CAN_FD,
	LIN
}MODE;


//定义该通道为源通道还是目标通道
#define MESSAGE_RX  0
#define MESSAGE_TX  1


/*掩码模式*/



/*转发规则*/
typedef enum F_Rule{
	Rule1,  //掩码模式
	Rule2,  //范围过滤
	Rule3,  //不做过滤
	Rule4    //reserve

}F_Rule;





/*源通道选择*/
void Source_Target_CAN_Init(uint8_t CAN_no,MODE Channal_Mode,uint8_t DIR);



/*目标通道选择*/
void TargetCAN_Init(uint8_t CAN_no,uint8_t CAN_Mode);

/*CAN模块使能*/
void CANx_Enable(uint8_t CAN_no);

/*转换通道选择*/
//void Source_To_Target(uint8_t CAN_no,uint8_t CAN_no);

/*过滤*/
void Filter_Rule_Init(F_Rule f);

#endif
