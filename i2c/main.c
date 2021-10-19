

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "IIC.H"
#include "stddef.h"
#include "uart.h"
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void deal_date_to_change_pwm(u8 *receive_date);  //详细注释看函数主体
/* global variable -----------------------------------------------------------*/
   u8 *receive_date;  //接收数据
   u8 test_data=0;
   u16 freq=0;  //初始频率
   u16 count=40000; //归一化后频率
   u16 th1_value_on=0;
   u16 th1_value_off=0;  //初始低电平有效值
   u16 th1_Compare=0;   //归一化后低电平有效值
   u8 *scend1;
   u8 *scend2;
   u8 count_data[8]={0};
   u8 *read_num_iic;
void delay_us(uint16_t nCount)   //16M 晶振时  延时 1个微妙
{
    nCount*=3;//等同于 nCount=nCount*3  相当于把nCount变量扩大3倍
    while(--nCount);//nCount变量数值先减一，再判断nCount的数值是否大于0，大于0循环减一，等于0退出循环。
}

void delay_ms(uint16_t nCount)  //16M 晶振时  延时 1个毫秒
{
    while(nCount--)//先判断while()循环体里的nCount数值是否大于0，大于0循环，减一执行循环体，等于0退出循环。
    {
        delay_us(1000);//调用微妙延时函数，输入1000等译演示1毫秒。
    }
}


void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {   
    nCount--;
  }
}
uint8_t i=0;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*  名称 init_pwm
*    功能 初始化定时器一四个通道配置为pwm
*     入口参数：无
*      返回值：无
*/
void init_pwm_tim1(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1,ENABLE);    //使能定时器一时钟
 /*----------------------TIM1----------------------------*/
    GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);        //CH1
    GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_OUT_PP_LOW_FAST);        //2
    GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);        //4
    GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_SLOW);        //3

   // TIM1_UpdateDisableConfig(ENABLE);
    TIM1_TimeBaseInit(0x0007,TIM1_COUNTERMODE_UP,39999,0x00);    // 一分频，向上计数，周期2k
    
    TIM1_OC1Init(TIM1_OCMODE_PWM1,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,0,
                 TIM1_OCPOLARITY_HIGH,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET ); //定时器通道一  输出使能，互补输出关闭
    
    TIM1_OC2Init(TIM1_OCMODE_PWM1,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,0,
                 TIM1_OCPOLARITY_HIGH,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET );
    
    TIM1_OC3Init(TIM1_OCMODE_PWM1,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,0,
                 TIM1_OCPOLARITY_HIGH,TIM1_OCNPOLARITY_LOW,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET );
    
    TIM1_OC4Init(TIM1_OCMODE_PWM1,TIM1_OUTPUTSTATE_ENABLE,0,TIM1_OCPOLARITY_HIGH,TIM1_OCIDLESTATE_RESET);
    
    TIM1_ARRPreloadConfig(ENABLE);
    
    TIM1_OC1PreloadConfig(ENABLE);
    TIM1_OC2PreloadConfig(ENABLE);
    TIM1_OC3PreloadConfig(ENABLE);
    TIM1_OC4PreloadConfig(ENABLE);
    
    TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);

    TIM1_CtrlPWMOutputs(ENABLE);
    TIM1_Cmd(ENABLE);
}

/*  名称 init_pwm_tim2
*    功能 初始化定时器二 三个通道配置为pwm
*     入口参数：无
*      返回值：无
*/

void init_pwm_tim2(void)
{
   /*-----------------------TIM2------------------------------------------------------*/
    GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);          //CH1
    GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);         //CH3
    GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_FAST);          //CH2
    
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2,ENABLE);
    //TIM2_UpdateDisableConfig(ENABLE);
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_8,39999);
    
    TIM2_OC1Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
    TIM2_OC2Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,0,TIM2_OCPOLARITY_HIGH);
    
    TIM2_ARRPreloadConfig(ENABLE);
    
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC1PreloadConfig(ENABLE);
    
    TIM2_CCxCmd(TIM2_CHANNEL_1,ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_2,ENABLE);
    TIM2_CCxCmd(TIM2_CHANNEL_3,ENABLE);
    
    TIM2_Cmd(ENABLE);
      
}

/*  名称 Init_Timer4
*    功能 第八pwm测试
*     入口参数：无
*      返回值：无
*/


void Init_Timer4(void)
{
	/*TIM4_UpdateDisableConfig(ENABLE);//允许更新事件
	TIM4_ARRPreloadConfig(ENABLE);//自动重装
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//中断配置，更新中断
	TIM4_SetCounter(0xff);//计数器初值
	TIM4_SetAutoreload(0xFF);//计数器自动重装的初值
	TIM4_PrescalerConfig(TIM4_PRESCALER_128, TIM4_PSCRELOADMODE_UPDATE);//预分频值
	*/
  GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOA,GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);
	TIM4_TimeBaseInit(TIM4_PRESCALER_1, 16);      //  16M/16 =200khz      1us        
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	
	 /* Enable update interrupt */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
}

/*-------------------------------主函数--------------------------------------*/
void main(void)
{
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
   debug_uart_init();
   iic_init();
   init_pwm_tim1();
   init_pwm_tim2();
   //Init_Timer4();
   enableInterrupts();
   
  while (1)
  {   
    //GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
    
    
    receive_date = get_iic_data();  //获取数组首地址
    deal_date_to_change_pwm(receive_date);
  }
   
  }

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/*  名称 deal_date_to_change_pwm
*    功能 模拟PCA9685输出7路PWM
*     入口参数：IIC接收数据地址 u8 格式
*      返回值：无
*/
void deal_date_to_change_pwm(u8 *receive_date){
  
      u32tostr(count,count_data);
      send_debug_data(count_data);
      send_debug_data("bbbbb");
      send_debug_data("\r\n");
      u32tostr(test_data,count_data);
      send_debug_data(count_data);
      send_debug_data("cccc");
      send_debug_data("\r\n");
    switch(*(receive_date+0))
    {
    case 0x00:
      *(receive_date)= 0xab;
      read_num_iic=get_iic_read_num();
      *read_num_iic=0;
      break;
    case 0xfe:
      if(*(receive_date+1)!= NULL)
      {
      test_data=*(receive_date+1);
      freq = (25000000/4096/ *(receive_date+1)+1);
      count=(u16)(2000000/freq);
      TIM1_SetAutoreload(count); 
      TIM2_SetAutoreload(count);
      *(receive_date+1)= NULL;
      }
      else
      {
      scend1 = get_iic_send_address();
      *scend1 = 2000000/count;
      }
      read_num_iic=get_iic_read_num();
      *read_num_iic=0;
      break;
    case 0x06:
    //  th1_value_on=*(receive_date+1)|(*(receive_date+2)<<8);
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)(count)*th1_value_off/4096 );
      TIM1_SetCompare1(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x0a:
     th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM1_SetCompare2(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x0e:
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM1_SetCompare3(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x12:
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM1_SetCompare4(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x16:
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM2_SetCompare1(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x1a:
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM2_SetCompare2(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    case 0x1e:
      th1_value_off=*(receive_date+3)|(*(receive_date+4)<<8);
      th1_Compare=(u16)((float)count/4096 *th1_value_off);
      TIM2_SetCompare3(th1_Compare);
      *(receive_date+1)=NULL;
      break;
    default :

      break;
      
    }
   *(receive_date+0)=0xab;
   *(receive_date+1)=NULL;
   *(receive_date+2)=50;

}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
