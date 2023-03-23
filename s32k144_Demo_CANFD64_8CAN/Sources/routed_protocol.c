#include "routed_protocol.h"
#include "cpu.h"
#include "can.h"

/*
 ***** Array initializer of CAN peripheral base pointers *****
		#define CAN_BASE_PTRS            { CAN0, CAN1, CAN2 }
 *
 ***** CAN - Register Layout Typedef ******
 	 	CAN_MemMapPtr
 * */
#if 0
static const CAN_MemMapPtr CAN_PTR[3] = CAN_BASE_PTRS;


void SourceCAN_Init(uint8_t CAN_no,uint8_t CAN_Mode){


	switch(CAN_no){
	case CAN_0:
		CAN0_Init();
		break;
	case CAN_1:
		CAN1_Init();
		break;
	case CAN_2:
		//CAN2_Init();
		break;

	}
#if 0
	//�����λ
	can_ptr->MCR |= CAN_MCR_SOFTRST_MASK;
	can_ptr->MCR &= ~CAN_MCR_SOFTRST_MASK;
#endif


	switch(CAN_Mode){
	case CAN_CLASSIC:
		CAN_Mode_Config(CAN_CLASSIC);
		break;
	case CANFD:
		CAN_Mode_Config(CANFD);
		break;
	case CAN_FIFO:
		CAN_Mode_Config(CAN_FIFO);
		break;
	}


#if 0
    //�ȴ�����ģʽ����
    while ((can_ptr->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT);
    //�ȴ�׼�����
    while ((can_ptr->MCR && CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT);
#endif

}


void TargetCAN_Init(uint8_t CAN_no,uint8_t CAN_Mode){


	switch(CAN_no){
	case CAN_0:
		CAN0_Init();
		break;
	case CAN_1:
		CAN1_Init();
		break;
	case CAN_2:
		//CAN2_Init();
		break;

	}


	switch(CAN_Mode){
	case CAN_CLASSIC:
		CAN_Mode_Config(CAN_CLASSIC);
		break;
	case CANFD:
		CAN_Mode_Config(CANFD);
		break;
	case CAN_FIFO:
		CAN_Mode_Config(CAN_FIFO);
		break;
	}




}




void CANx_Enable(uint8_t CAN_no){
	switch(CAN_no){
	case CAN_0:
		//���ý��շ�������ΪPTE4��PTE5
		PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[4] |= PORT_PCR_MUX(5);
		PORTE->PCR[5] |= PORT_PCR_MUX(5);
		PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_1:
		//���ý��շ�������ΪPTC7��PTC6
		PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[7] |= PORT_PCR_MUX(3);
		PORTC->PCR[6] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN1_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_2:
		//���ý��շ�������ΪPTC17��PTC16
		PORTC->PCR[17] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[16] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[17] |= PORT_PCR_MUX(3);
		PORTC->PCR[16] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN2_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
}
}

void CAN_Mode_Config(uint8_t CAN_Mode){
	int i;
	//CAN_MemMapPtr can_ptr = CAN_PTR[CAN_no];

	switch(CAN_Mode)
		{
		case CAN_CLASSIC:
			//��ֹCANģ��
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
			//����ʱ��Դ ѡ��SOSʱ��Դ 8MHz
			can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
			//ʹ��ģ��
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
			//�ȴ����붳��ģʽ
			while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
			//���ÿ��ƼĴ���1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
			can_ptr->CTRL1 = 0x00DB0006;
			//�������
			if(CAN_no == CAN_0)
			{
				for(i = 0 ; i < 128; i++)
				{
					can_ptr->RAMn[i] = 0x0;
				}
			}
			else
			{
				for(i = 0 ; i < 64; i++)
				{
					can_ptr->RAMn[i] = 0x0;
				}
			}
			//���ý�������Ĵ���
			for(i = 0; i < CAN_RXIMR_COUNT; i++)
			{
				can_ptr->RXIMR[i] =  0xFFFFFFFF;
			}
			//��������ȫ������Ĵ���
			can_ptr->RXMGMASK = 0x1FFFFFFF;
			can_ptr->RX14MASK = 0x1FFFFFFF;
			can_ptr->RX15MASK = 0x1FFFFFFF;
			//���ý��������С
			can_ptr->MCR = CAN_MCR_MAXMB(7);
			break;
		case CANFD:
			//��ֹCANģ��
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		    //����ʱ��Դ ѡ������ʱ�Ӽ�ѡ��ϵͳʱ��
			can_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;
		    //ʹ��ģ��
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		    //�ȴ����붳��ģʽ
		    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> \
		    		                                 CAN_MCR_FRZACK_SHIFT));
		    //����CBT  BTF=0x1;ERJW=0xF;EPROPSEG=0x2f;EPSEG1=0x1E;EPSEG2=0xF;
		    can_ptr->CBT = 0x800FBDEF;
		    //����FDCBT
		    can_ptr->FDCBT = 0x00035CE7;
		    //�������
		    for(i = 0 ; i < CAN_RAMn_COUNT; i++)
		    {
		    	can_ptr->RAMn[i] = 0x0;
		    }
		    //���ý�������Ĵ���
		    for(i = 0; i < CAN_RXIMR_COUNT; i++)
		    {
		    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
		    }
		    //��������ȫ������Ĵ���
		    can_ptr->RXMGMASK = 0xFFFFFFFF;
		    can_ptr->RX14MASK = 0xFFFFFFFF;
		    can_ptr->RX15MASK = 0xFFFFFFFF;
		    //����ISO-11898-1
		    can_ptr->CTRL2 |= CAN_CTRL2_ISOCANFDEN_MASK;
		    //����FDCTRL�Ĵ���
		    can_ptr->FDCTRL = CAN_FDCTRL_FDRATE_MASK | \
		    	            CAN_FDCTRL_MBDSR0(3) | CAN_FDCTRL_TDCEN(1) | \
			                CAN_FDCTRL_TDCFAIL(1) | CAN_FDCTRL_TDCOFF(0X13);
		    //���ý��������СΪ16���֣�ʹ��FDģʽ
		    can_ptr->MCR = CAN_MCR_MAXMB_MASK | CAN_MCR_FDEN_MASK;
		    break;
		case CAN_FIFO:
			//��ֹCANģ��
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		    //����ʱ��Դ ѡ��OSCʱ��Դ ʱ��Ƶ��Ϊ8MHz
			can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
		    //ʹ��ģ��
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		    //�ȴ����붳��ģʽ
		    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
		    //���ÿ��ƼĴ���1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
		    can_ptr->CTRL1 = 0x00DB0006;
		    //�������
		    if(CAN_no == CAN_0)
		    {
			    for(i = 0 ; i < 128; i++)
			    {
			    	can_ptr->RAMn[i] = 0x0;
			    }
		    }
		    else
		    {
			    for(i = 0 ; i < 64; i++)
			    {
			    	can_ptr->RAMn[i] = 0x0;
			    }
		    }
		    //���ý�������Ĵ���
		    for(i = 0; i < CAN_RXIMR_COUNT; i++)
		    {
		    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
		    }
		    //��������ȫ������Ĵ���
		    can_ptr->RXFGMASK = 0x7FFFFFFE;

		    //���ý��������С ����FIFOʹ��
		    can_ptr->MCR = CAN_MCR_MAXMB_MASK | CAN_MCR_RFEN_MASK | \
		    		                         CAN_MCR_IDAM(0) |CAN_MCR_IRMQ(1);
		    break;
		}
}






/*!
 * @brief Enables/Disables the Self Reception feature.
 *
 * If enabled, FlexCAN is allowed to receive frames transmitted by itself.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Self Reception
 */
static inline void FLEXCAN_SetSelfReception(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_SRXDIS_MASK) | CAN_MCR_SRXDIS(enable? 0UL : 1UL);
}



#endif


static const CAN_MemMapPtr CAN_PTR[3] = CAN_BASE_PTRS;
void SourceCan_init(uint8_t CAN_No,uint8_t CAN_Mode)
{
	//�ֲ���������
	int i;
	CAN_MemMapPtr can_ptr = CAN_PTR[CAN_No];
	//��ʱ�����������Ź���
	switch(CAN_No)
	{
	case CAN_0:
		//���ý��շ�������ΪPTE4��PTE5
		PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	    PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
	    PORTE->PCR[4] |= PORT_PCR_MUX(5);
		PORTE->PCR[5] |= PORT_PCR_MUX(5);
		PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_1:
		//���ý��շ�������ΪPTC7��PTC6
		PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[7] |= PORT_PCR_MUX(3);
		PORTC->PCR[6] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN1_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_2:
		//���ý��շ�������ΪPTC17��PTC16
		PORTC->PCR[17] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[16] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[17] |= PORT_PCR_MUX(3);
		PORTC->PCR[16] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN2_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	}
	//�����λ
	can_ptr->MCR |= CAN_MCR_SOFTRST_MASK;
	can_ptr->MCR &= ~CAN_MCR_SOFTRST_MASK;
	switch(CAN_Mode)
	{
	case CAN_CLASSIC:
		//��ֹCANģ��
		can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		//����ʱ��Դ ѡ��SOSʱ��Դ 8MHz
		can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
		//ʹ��ģ��
		can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		//�ȴ����붳��ģʽ
		while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
		//���ÿ��ƼĴ���1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
		can_ptr->CTRL1 = 0x00DB0006;
		//�������
		if(CAN_No == CAN_0)
		{
			for(i = 0 ; i < 128; i++)
			{
				can_ptr->RAMn[i] = 0x0;
			}
		}
		else
		{
			for(i = 0 ; i < 64; i++)
			{
				can_ptr->RAMn[i] = 0x0;
			}
		}
		//���ý�������Ĵ���
		for(i = 0; i < CAN_RXIMR_COUNT; i++)
		{
			can_ptr->RXIMR[i] =  0xFFFFFFFF;
		}
		//��������ȫ������Ĵ���
		can_ptr->RXMGMASK = 0x1FFFFFFF;
		can_ptr->RX14MASK = 0x1FFFFFFF;
		can_ptr->RX15MASK = 0x1FFFFFFF;
		//���ý��������С
		can_ptr->MCR = CAN_MCR_MAXMB(7);
		break;
	case CANFD:
		//��ֹCANģ��
		can_ptr->MCR |= CAN_MCR_MDIS_MASK;
	    //����ʱ��Դ ѡ������ʱ�Ӽ�ѡ��ϵͳʱ��
		can_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;
	    //ʹ��ģ��
		can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
	    //�ȴ����붳��ģʽ
	    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> \
	    		                                 CAN_MCR_FRZACK_SHIFT));
	    //����CBT  BTF=0x1;ERJW=0xF;EPROPSEG=0x2f;EPSEG1=0x1E;EPSEG2=0xF;
	    can_ptr->CBT = 0x800FBDEF;
	    //����FDCBT
	    can_ptr->FDCBT = 0x00035CE7;
	    //�������
	    for(i = 0 ; i < CAN_RAMn_COUNT; i++)
	    {
	    	can_ptr->RAMn[i] = 0x0;
	    }
	    //���ý�������Ĵ���
	    for(i = 0; i < CAN_RXIMR_COUNT; i++)
	    {
	    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
	    }
	    //��������ȫ������Ĵ���
	    can_ptr->RXMGMASK = 0xFFFFFFFF;
	    can_ptr->RX14MASK = 0xFFFFFFFF;
	    can_ptr->RX15MASK = 0xFFFFFFFF;
	    //����ISO-11898-1
	    can_ptr->CTRL2 |= CAN_CTRL2_ISOCANFDEN_MASK;
	    //����FDCTRL�Ĵ���
	    can_ptr->FDCTRL = CAN_FDCTRL_FDRATE_MASK | \
	    	            CAN_FDCTRL_MBDSR0(3) | CAN_FDCTRL_TDCEN(1) | \
		                CAN_FDCTRL_TDCFAIL(1) | CAN_FDCTRL_TDCOFF(0X13);
	    //���ý��������СΪ16���֣�ʹ��FDģʽ
	    can_ptr->MCR = CAN_MCR_MAXMB_MASK | CAN_MCR_FDEN_MASK;
	    break;

	}
    //�ȴ�����ģʽ����
    while ((can_ptr->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT);
    //�ȴ�׼�����
    while ((can_ptr->MCR && CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT);
}


typedef struct GateWay_Rule{
		F_Rule Filter_TypeNum;                         //��������
	//   Format Format_Converse;                         //��ʽת��
	   can_msg_id_type_t ID_Type;                      //ID����
	   uint16_t  SrcChn[16];                            //Դͨ��
	   uint16_t  TarChn[16];                            //Ŀ��ͨ��


	   //������
	   union Filter_Type{
	   	uint16_t Filter_Mask;                   //����ģʽ
	   	uint16_t Filter_Range_Start;            //��Χ������ʼ
	   	uint16_t Filter_None;                   //�����κι���
	   }Filter_Type;
	   uint16_t Filter_Range_End;               //��Χ���˽���

	   char GateWay_Reserve[400];                //Ԥ��400���ֽ�
}G_Rule;




#if 0
void Source_To_Target(uint8_t CAN_Source[8],uint8_t CAN_Target[8]){

	/*Դͨ��ת���걨�ķ��ͳ������ر�����CANģ�飬ʵ��ָ��ͨ������
	 *
	 * ��ͨ�����գ�
	 *
	 * CAN0:Դͨ��   CAN1��Ŀ��ͨ��    CAN3���ر�
	 *
	 * */
	int i;
	for(i=0;i<8;i++){
		if(CAN_Source[i]&1==1){

		}

	}



}

#endif




void Source_Target_CAN_Init(uint8_t CAN_no,MODE Channal_Mode,uint8_t DIR){
	/*��ʼ����Ӧ��CANģ�飬���ö�Ӧ��ģʽ������ΪԴͨ������Ŀ��ͨ��*/
	switch(CAN_no){
	case CAN_0:
		CAN0_Init();
		break;
	case CAN_1:
		CAN1_Init();
		break;
	case CAN_2:
		CAN2_Init();
		break;
	}

	switch(Channal_Mode){
	//case CAN_CLASSIC:
	case CAN_NORMAL:

		break;
	case CAN_FD:

		break;
	case LIN:

		break;

	}
	switch(DIR){
	case MESSAGE_RX:

		break;
	case MESSAGE_TX:

		break;

	}



}



void Filter_Rule_Init(F_Rule f){

	G_Rule G_Rule;

	uint32_t Rule;

	G_Rule.Filter_TypeNum=f;

	switch(f){
		case Rule1:
			Rule = G_Rule.Filter_Type.Filter_Mask ;
			break;
		case Rule2:
			Rule >= G_Rule.Filter_Type.Filter_Range_Start &&  Rule <= G_Rule.Filter_Range_End;
			break;
		case Rule3:
			Rule=G_Rule.Filter_Type.Filter_None;
			break;
	}
}
