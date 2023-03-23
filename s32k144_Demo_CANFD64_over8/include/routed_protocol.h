#ifndef ROUTED_PROTOCOL_H
#define ROUTED_PROTOCOL_H
#include "Cpu.h"

//CAN0,CAN1,CAN2����ģ��궨��
#define CAN_0     0    //CAN0ģ��
#define CAN_1     1    //CAN1ģ��
#define CAN_2     2    //CAN2ģ��


//CAN_CALSSIC��CANFD����ģʽ
#define CAN_CLASSIC   0
#define CANFD         1


typedef enum MODE {
	CAN_NORMAL,
	CAN_FD,
	LIN
}MODE;


//�����ͨ��ΪԴͨ������Ŀ��ͨ��
#define MESSAGE_RX  0
#define MESSAGE_TX  1


/*����ģʽ*/



/*ת������*/
typedef enum F_Rule{
	Rule1,  //����ģʽ
	Rule2,  //��Χ����
	Rule3,  //��������
	Rule4    //reserve

}F_Rule;





/*Դͨ��ѡ��*/
void Source_Target_CAN_Init(uint8_t CAN_no,MODE Channal_Mode,uint8_t DIR);



/*Ŀ��ͨ��ѡ��*/
void TargetCAN_Init(uint8_t CAN_no,uint8_t CAN_Mode);

/*CANģ��ʹ��*/
void CANx_Enable(uint8_t CAN_no);

/*ת��ͨ��ѡ��*/
//void Source_To_Target(uint8_t CAN_no,uint8_t CAN_no);

/*����*/
void Filter_Rule_Init(F_Rule f);

#endif
