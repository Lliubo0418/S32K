/*
 * 品智科技S32K144开发板
 * 01-基础工程
 * 建立一个常用的工程,包含数个常用函数
 *
 * IO配置
 * KEY1 PTC12
 * KEY2 PTC13
 * KEY3 PTB2
 * LED1 PTD16
 * LED2 PTD15
 * LED3 PTD1
 * LED4 PTD0
*/
#include "Cpu.h"
#include "delay.h"
#include "uart.h"
#include"key.h"
#include"oled.h"
#include "can.h"
  volatile int exit_code = 0;

#define LED1(x)  PINS_DRV_WritePin(PTD,16,!x);
#define LED2(x)  PINS_DRV_WritePin(PTD,15,!x);
#define LED3(x)  PINS_DRV_WritePin(PTD,1,!x);
#define LED4(x)  PINS_DRV_WritePin(PTD,0,!x);

extern char IRQ_CAN0_RX;
extern char IRQ_CAN0_TX;
extern char IRQ_CAN1_RX;
extern char IRQ_CAN1_TX;

extern can_message_t recvMsg_CAN0;
extern can_message_t recvMsg_CAN1;
extern can_message_t sendFDMsg_CAN0;
extern can_message_t sendMsg_CAN0;


int main(void)
{
  /* Write your local variable definition here */
//	uint8_t pinstate;
	int MCU_Freq;

	int i=0;
	int j=0;
	int flag =0;

	int cnt=0;



  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
	MCU_Freq = delay_init();//初始化delay函数
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr); //初始化IO
	I2C_MasterInit(&i2c1_instance, &i2c1_MasterConfig0);//初始化I2C外设,用于OLED通讯
	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0); //初始化串口

	oled_init(); //OLED配置参数初始化
	OLED_TITLE((uint8_t*)"S32K144",(uint8_t*)"01_BASE");//OLED显示标题

	CAN0_Init();
	CAN1_Init();

	u1_printf("初始化完毕,MCU运行频率为 %d Mhz \r\n",MCU_Freq);
    while(1)
    {
    	if(IRQ_CAN0_RX==1){
#if 0
    	u1_printf("CAN0 RECV ID:0x%x \r\n",recvMsg_CAN0.id);
    	for(i=0;i<recvMsg_CAN0.length;i++){
    		u1_printf("Data %d : %x\r\n",i,recvMsg_CAN0.data[i]);
    		if(i==recvMsg_CAN0.length-1) u1_printf("***************\r\n");

    	}
#endif
    	cnt++;
    	IRQ_CAN0_RX=0;

    	}


    	if(flag==0){
    	switch(cnt){
    	case 1 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j]=recvMsg_CAN0.data[j];
    		}
    		break;
    	case 2 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+8]=recvMsg_CAN0.data[j];
       		}
    		break;
    	case 3 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+16]=recvMsg_CAN0.data[j];
       		}
    		break;
    	case 4 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+24]=recvMsg_CAN0.data[j];
    		}
    		break;
    	case 5 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+32]=recvMsg_CAN0.data[j];
       		}
    		break;
    	case 6 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+40]=recvMsg_CAN0.data[j];
       		}
    		break;
    	case 7 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+48]=recvMsg_CAN0.data[j];
    		}
    		break;
    	case 8 :
    		for(j=0;j<8;j++){
    		sendFDMsg_CAN0.data[j+56]=recvMsg_CAN0.data[j];

       		}
    		break;
    	default:
    		break;

    	}
    	}


    	if(cnt==8&&(j+56)==64){
    		flag=1;
    		Send_CANFDMsg(0x555,sendFDMsg_CAN0.data,64);
    		flag=0;
    		cnt=0;
    	}



#if 0
    	/*LED闪烁*/
    	delay_ms(100);
    	PINS_DRV_TogglePins(PTD, 1 << 0);
    	PINS_DRV_TogglePins(PTD, 1 << 1);
    	PINS_DRV_TogglePins(PTD, 1 << 15);
    	PINS_DRV_TogglePins(PTD, 1 << 16);
#endif
    }
  /*** Don't write any code pass th5is line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
