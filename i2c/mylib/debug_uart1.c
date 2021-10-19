#include "stm8s.h"
#include "uart.h"
#include "stdio.h"
/*
*  debug_uart_init
*  funtion :初始化串口
**/


void debug_uart_init(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
  GPIO_Init(GPIOD,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_IN_PU_NO_IT);
  UART1_DeInit();
  UART1_Init(115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,\
            UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,\
            UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(ENABLE);
  
}

int putchar(int c)
{
  if ('\n'==(char)c)
  {
  UART1_SendData8('\r');
  while (UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);
  }
  UART1_SendData8(c);
  while (UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);
  return (c);

}

void send_debug_data(uint8_t *Buffer)
{
  uint8_t *String;
        String=Buffer;
        while(*String!='\0')
       {
          UART1_SendData8(*String);
          while (UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);
          String++;
        }
}

/**********************************************************
** 函数名:u32tostr
** 功能描述: 将一个32位的变量dat转为字符串，比如把1234转为"1234"
** 输入参数: dat:带转的long型的变量
             str:指向字符数组的指针，转换后的字节串放在其中 
** 输出参数: 无
***********************************************************/
void u32tostr(unsigned long dat,char *str) 
{
    char temp[20];
    unsigned char i=0,j=0;
    i=0;
    while(dat)
    {
        temp[i]=dat%10+0x30;
        i++;
        dat/=10;
    }
    j=i;
    for(i=0;i<j;i++)
    {
          str[i]=temp[j-i-1];
    }
    if(!i) {str[i++]='0';}
    str[i]=0;
}
