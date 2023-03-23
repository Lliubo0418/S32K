/*
 *
 *
*/
#include "Cpu.h"
#include "delay.h"
#include "uart.h"
#include"key.h"
#include"oled.h"
#include "can.h"
#include "cJSON.h"

  volatile int exit_code = 0;

#define LED1(x)  PINS_DRV_WritePin(PTD,16,!x);
#define LED2(x)  PINS_DRV_WritePin(PTD,15,!x);
#define LED3(x)  PINS_DRV_WritePin(PTD,1,!x);
#define LED4(x)  PINS_DRV_WritePin(PTD,0,!x);

extern char IRQ_CAN0_RX;
extern char IRQ_CAN0_TX;
extern char IRQ_CAN1_RX;
extern char IRQ_CAN1_TX;
extern char IRQ_CAN2_RX;
extern char IRQ_CAN2_TX;


extern can_message_t recvMsg_CANx;
extern can_message_t recvMsg_CAN0;
extern can_message_t recvMsg_CAN1;
extern can_message_t recvMsg_CAN2;
extern can_message_t sendFDMsg_CAN0;
extern can_message_t sendMsg_CAN0;

extern can_message_t sendMsg;


/*���ͻ���������*/
extern can_buff_config_t Tx_buffCfg;
/*���ջ���������*/
extern can_buff_config_t Rx_buffCfg;


//Ĭ��IDΪ16����ȥ����0x��ǰ׺
char *json="{                       \
    \"1\" : {                       \
        \"1\" : [2],            \
        \"2\" : [3],         \
        \"3\" : [2],         \
        \"123\" : [2, 3]    \
    },                              \
    \"2\" : {                       \
    	  \"4\" : [1],         \
        \"5\" : [1, 2],      \
        \"6\" : [3]             \
    },                               \
    \"3\" : {                       \
        \"7\" : [1, 3],      \
        \"789\" : [1]             \
    }                              \
}";


int main(void)
{
  /* Write your local variable definition here */

	int MCU_Freq;
	int j=0;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
	MCU_Freq = delay_init();//��ʼ��delay����
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr); //��ʼ��IO
	I2C_MasterInit(&i2c1_instance, &i2c1_MasterConfig0);//��ʼ��I2C����,����OLEDͨѶ
	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0); //��ʼ������

	oled_init(); //OLED���ò�����ʼ��
	OLED_TITLE((uint8_t*)"S32K144",(uint8_t*)"01_BASE");//OLED��ʾ����

	CAN0_Init();
	CAN1_Init();
	CAN2_Init();

	u1_printf("��ʼ�����,MCU����Ƶ��Ϊ %d Mhz \r\n",MCU_Freq);
    while(1)
    {
    	if(IRQ_CAN0_RX==1){

    		u1_printf("CAN0 RECV ID:0x%x \r\n",recvMsg_CAN0.id);
    		for(j=0;j<recvMsg_CAN0.length;j++){
    		    u1_printf("Data %d : %x\r\n",j,recvMsg_CAN0.data[j]);
    		    if(j==recvMsg_CAN0.length-1) u1_printf("***************\r\n");

    	}
            //����һ��json�ַ���
            cJSON *cjsonTest = cJSON_Parse(json);
            if (!cjsonTest) {
            	u1_printf("parse fail \r\n");
            }
            // ����key="1"��ȡjson����
            cJSON *config01 = cJSON_GetObjectItem(cjsonTest, "1");
            char str[16];
            char *target = itoa(recvMsg_CAN0.id,str,16);
            // ����Ĵ�����Ҫ�����ʱ��
            // begin
            cJSON *roules = cJSON_GetObjectItem(config01, target);
            int num = cJSON_GetArraySize(roules);
            u1_printf("%s of channel 1 ==>:\r\n", target);
            for (int i = 0; i < num; i++) {
                cJSON *ch = cJSON_GetArrayItem(roules, i);
                u1_printf("%d\r\n", ch->valueint);// �����ʵ����Ҫ�滻ΪCANͨ�����ͺ���
                //can���ͣ���Ŀ��ͨ��ch->valueint��

               Send_CANMsg(1,ch->valueint,recvMsg_CAN0.id,sendMsg.data,8);      //������Դͨ����Ŀ��ͨ�������յ�id����ָ��id�������ͱ������ݣ�8λ��

                }
            IRQ_CAN0_RX=0;

    	}

    	if(IRQ_CAN1_RX==1){

    		u1_printf("CAN1 RECV ID:0x%x \r\n",recvMsg_CAN1.id);
    		for(j=0;j<recvMsg_CAN1.length;j++){
    		    u1_printf("Data %d : %x\r\n",j,recvMsg_CAN1.data[j]);
    		    if(j==recvMsg_CAN1.length-1) u1_printf("***************\r\n");

    	}
            //����һ��json�ַ���
            cJSON *cjsonTest = cJSON_Parse(json);
            if (!cjsonTest) {
            	u1_printf("parse fail \r\n");
            }
            // ����key="1"��ȡjson����
            cJSON *config02 = cJSON_GetObjectItem(cjsonTest, "2");
            char str[16];
            char *target = itoa(recvMsg_CAN1.id,str,16);
            // ����Ĵ�����Ҫ�����ʱ��
            // begin
            cJSON *roules = cJSON_GetObjectItem(config02, target);
            int num = cJSON_GetArraySize(roules);
            u1_printf("%s of channel 2 ==>:\r\n", target);
            for (int i = 0; i < num; i++) {
                cJSON *ch = cJSON_GetArrayItem(roules, i);
                u1_printf("%d\r\n", ch->valueint);// �����ʵ����Ҫ�滻ΪCANͨ�����ͺ���
                //can���ͣ���Ŀ��ͨ��ch->valueint��

               Send_CANMsg(2,ch->valueint,recvMsg_CAN1.id,sendMsg.data,8);

                }
            IRQ_CAN1_RX=0;

    	}

#if 0
    	/*LED��˸*/
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
