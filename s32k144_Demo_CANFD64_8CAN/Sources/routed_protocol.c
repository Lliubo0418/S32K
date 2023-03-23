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
	//软件复位
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
    //等待冻结模式结束
    while ((can_ptr->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT);
    //等待准备完成
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
		//配置接收发送引脚为PTE4和PTE5
		PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[4] |= PORT_PCR_MUX(5);
		PORTE->PCR[5] |= PORT_PCR_MUX(5);
		PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_1:
		//配置接收发送引脚为PTC7和PTC6
		PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[7] |= PORT_PCR_MUX(3);
		PORTC->PCR[6] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN1_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_2:
		//配置接收发送引脚为PTC17和PTC16
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
			//禁止CAN模块
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
			//设置时钟源 选择SOS时钟源 8MHz
			can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
			//使能模块
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
			//等待进入冻结模式
			while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
			//配置控制寄存器1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
			can_ptr->CTRL1 = 0x00DB0006;
			//清空数据
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
			//配置接收掩码寄存器
			for(i = 0; i < CAN_RXIMR_COUNT; i++)
			{
				can_ptr->RXIMR[i] =  0xFFFFFFFF;
			}
			//接收信箱全局掩码寄存器
			can_ptr->RXMGMASK = 0x1FFFFFFF;
			can_ptr->RX14MASK = 0x1FFFFFFF;
			can_ptr->RX15MASK = 0x1FFFFFFF;
			//配置接收信箱大小
			can_ptr->MCR = CAN_MCR_MAXMB(7);
			break;
		case CANFD:
			//禁止CAN模块
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		    //设置时钟源 选择外设时钟即选择系统时钟
			can_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;
		    //使能模块
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		    //等待进入冻结模式
		    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> \
		    		                                 CAN_MCR_FRZACK_SHIFT));
		    //配置CBT  BTF=0x1;ERJW=0xF;EPROPSEG=0x2f;EPSEG1=0x1E;EPSEG2=0xF;
		    can_ptr->CBT = 0x800FBDEF;
		    //配置FDCBT
		    can_ptr->FDCBT = 0x00035CE7;
		    //清空数据
		    for(i = 0 ; i < CAN_RAMn_COUNT; i++)
		    {
		    	can_ptr->RAMn[i] = 0x0;
		    }
		    //配置接收掩码寄存器
		    for(i = 0; i < CAN_RXIMR_COUNT; i++)
		    {
		    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
		    }
		    //接收信箱全局掩码寄存器
		    can_ptr->RXMGMASK = 0xFFFFFFFF;
		    can_ptr->RX14MASK = 0xFFFFFFFF;
		    can_ptr->RX15MASK = 0xFFFFFFFF;
		    //开启ISO-11898-1
		    can_ptr->CTRL2 |= CAN_CTRL2_ISOCANFDEN_MASK;
		    //配置FDCTRL寄存器
		    can_ptr->FDCTRL = CAN_FDCTRL_FDRATE_MASK | \
		    	            CAN_FDCTRL_MBDSR0(3) | CAN_FDCTRL_TDCEN(1) | \
			                CAN_FDCTRL_TDCFAIL(1) | CAN_FDCTRL_TDCOFF(0X13);
		    //配置接收信箱大小为16个字，使能FD模式
		    can_ptr->MCR = CAN_MCR_MAXMB_MASK | CAN_MCR_FDEN_MASK;
		    break;
		case CAN_FIFO:
			//禁止CAN模块
			can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		    //设置时钟源 选择OSC时钟源 时钟频率为8MHz
			can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
		    //使能模块
			can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		    //等待进入冻结模式
		    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
		    //配置控制寄存器1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
		    can_ptr->CTRL1 = 0x00DB0006;
		    //清空数据
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
		    //配置接收掩码寄存器
		    for(i = 0; i < CAN_RXIMR_COUNT; i++)
		    {
		    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
		    }
		    //接收信箱全局掩码寄存器
		    can_ptr->RXFGMASK = 0x7FFFFFFE;

		    //配置接收信箱大小 设置FIFO使能
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
	//局部变量声明
	int i;
	CAN_MemMapPtr can_ptr = CAN_PTR[CAN_No];
	//开时钟门配置引脚功能
	switch(CAN_No)
	{
	case CAN_0:
		//配置接收发送引脚为PTE4和PTE5
		PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	    PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
	    PORTE->PCR[4] |= PORT_PCR_MUX(5);
		PORTE->PCR[5] |= PORT_PCR_MUX(5);
		PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_1:
		//配置接收发送引脚为PTC7和PTC6
		PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[7] |= PORT_PCR_MUX(3);
		PORTC->PCR[6] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN1_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	case CAN_2:
		//配置接收发送引脚为PTC17和PTC16
		PORTC->PCR[17] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[16] &= ~PORT_PCR_MUX_MASK;
	    PORTC->PCR[17] |= PORT_PCR_MUX(3);
		PORTC->PCR[16] |= PORT_PCR_MUX(3);
		PCC->PCCn[PCC_FlexCAN2_INDEX] |= PCC_PCCn_CGC_MASK;
		break;
	}
	//软件复位
	can_ptr->MCR |= CAN_MCR_SOFTRST_MASK;
	can_ptr->MCR &= ~CAN_MCR_SOFTRST_MASK;
	switch(CAN_Mode)
	{
	case CAN_CLASSIC:
		//禁止CAN模块
		can_ptr->MCR |= CAN_MCR_MDIS_MASK;
		//设置时钟源 选择SOS时钟源 8MHz
		can_ptr->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
		//使能模块
		can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
		//等待进入冻结模式
		while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
		//配置控制寄存器1 RJW=0B11; PSG1=0B011; PSG2=0X011; PROPSEG=0B110;
		can_ptr->CTRL1 = 0x00DB0006;
		//清空数据
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
		//配置接收掩码寄存器
		for(i = 0; i < CAN_RXIMR_COUNT; i++)
		{
			can_ptr->RXIMR[i] =  0xFFFFFFFF;
		}
		//接收信箱全局掩码寄存器
		can_ptr->RXMGMASK = 0x1FFFFFFF;
		can_ptr->RX14MASK = 0x1FFFFFFF;
		can_ptr->RX15MASK = 0x1FFFFFFF;
		//配置接收信箱大小
		can_ptr->MCR = CAN_MCR_MAXMB(7);
		break;
	case CANFD:
		//禁止CAN模块
		can_ptr->MCR |= CAN_MCR_MDIS_MASK;
	    //设置时钟源 选择外设时钟即选择系统时钟
		can_ptr->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;
	    //使能模块
		can_ptr->MCR &= ~CAN_MCR_MDIS_MASK;
	    //等待进入冻结模式
	    while (!((can_ptr->MCR & CAN_MCR_FRZACK_MASK) >> \
	    		                                 CAN_MCR_FRZACK_SHIFT));
	    //配置CBT  BTF=0x1;ERJW=0xF;EPROPSEG=0x2f;EPSEG1=0x1E;EPSEG2=0xF;
	    can_ptr->CBT = 0x800FBDEF;
	    //配置FDCBT
	    can_ptr->FDCBT = 0x00035CE7;
	    //清空数据
	    for(i = 0 ; i < CAN_RAMn_COUNT; i++)
	    {
	    	can_ptr->RAMn[i] = 0x0;
	    }
	    //配置接收掩码寄存器
	    for(i = 0; i < CAN_RXIMR_COUNT; i++)
	    {
	    	can_ptr->RXIMR[i] =  0xFFFFFFFF;
	    }
	    //接收信箱全局掩码寄存器
	    can_ptr->RXMGMASK = 0xFFFFFFFF;
	    can_ptr->RX14MASK = 0xFFFFFFFF;
	    can_ptr->RX15MASK = 0xFFFFFFFF;
	    //开启ISO-11898-1
	    can_ptr->CTRL2 |= CAN_CTRL2_ISOCANFDEN_MASK;
	    //配置FDCTRL寄存器
	    can_ptr->FDCTRL = CAN_FDCTRL_FDRATE_MASK | \
	    	            CAN_FDCTRL_MBDSR0(3) | CAN_FDCTRL_TDCEN(1) | \
		                CAN_FDCTRL_TDCFAIL(1) | CAN_FDCTRL_TDCOFF(0X13);
	    //配置接收信箱大小为16个字，使能FD模式
	    can_ptr->MCR = CAN_MCR_MAXMB_MASK | CAN_MCR_FDEN_MASK;
	    break;

	}
    //等待冻结模式结束
    while ((can_ptr->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT);
    //等待准备完成
    while ((can_ptr->MCR && CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT);
}


typedef struct GateWay_Rule{
		F_Rule Filter_TypeNum;                         //网关类型
	//   Format Format_Converse;                         //格式转换
	   can_msg_id_type_t ID_Type;                      //ID类型
	   uint16_t  SrcChn[16];                            //源通道
	   uint16_t  TarChn[16];                            //目标通道


	   //联合体
	   union Filter_Type{
	   	uint16_t Filter_Mask;                   //掩码模式
	   	uint16_t Filter_Range_Start;            //范围过滤起始
	   	uint16_t Filter_None;                   //不做任何过滤
	   }Filter_Type;
	   uint16_t Filter_Range_End;               //范围过滤结束

	   char GateWay_Reserve[400];                //预留400个字节
}G_Rule;




#if 0
void Source_To_Target(uint8_t CAN_Source[8],uint8_t CAN_Target[8]){

	/*源通道转换完报文发送出来，关闭其他CAN模块，实现指定通道接收
	 *
	 * 多通道接收？
	 *
	 * CAN0:源通道   CAN1：目标通道    CAN3：关闭
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
	/*初始化对应的CAN模块，设置对应的模式，定义为源通道还是目标通道*/
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
