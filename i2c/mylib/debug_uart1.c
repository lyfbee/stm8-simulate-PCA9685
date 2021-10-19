#include "stm8s.h"
#include "uart.h"
#include "stdio.h"
/*
*  debug_uart_init
*  funtion :��ʼ������
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
** ������:u32tostr
** ��������: ��һ��32λ�ı���datתΪ�ַ����������1234תΪ"1234"
** �������: dat:��ת��long�͵ı���
             str:ָ���ַ������ָ�룬ת������ֽڴ��������� 
** �������: ��
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
