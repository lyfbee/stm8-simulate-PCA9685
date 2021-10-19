#include "stm8s.h"
#include "IIC.H"

/****************************************************************************************
*iic��ʼ������
*��ڲ�������
*����ֵ����
*���ã�iic�ӻ���ʼ��
*****************************************************************************************/
void iic_init()    
{  
   CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
   //GPIO_Init(GPIOB,GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);    // ��ʼ��io 
   //GPIO_Init(GPIOB,GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST); 
   I2C_DeInit();    // ��ʧ��д��Ĵ���
   I2C_Init(100000,0x80,I2C_DUTYCYCLE_2,I2C_ACK_CURR,I2C_ADDMODE_7BIT,16);  //��ʼ��   �����ַ0xa0�� ��׼ģʽ100k 7Ϊ��ַ 16mƵ��ʱ��
   I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE); // ����iic�ж�
   I2C_Cmd(ENABLE);
}

/*************************************************************************
*INTERRUPT_HANDLER �жϷ�����
*I2C_IRQHandler��19
*���ã��ӻ����շ��͹���
**************************************************************************/
u8 Test_read[50] = {0};
u8 Test_r_num = 31;	//�ӻ����͸���
u8 Test_r_pos = 0;	  // ����ָ��
u8 Test_Write[50] = {0};   // �յ�50������
u8 Test_w_num = 0;   // �����������ݸ���
u8 Commucation_flag = 0;// ͨ����ɱ�־
static u8 temp=0;



INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
       //���շ���   
      if (I2C->SR1&0x02)//��ַ�Ѿ�ƥ�� ADDR��־
      {          
      	 // ��ADDR��־:��SR1����SR3
      	 // ���������ӻ������������DR�Ĵ���д����Խ��Խ��
      	 // �ж��Ƿ��Ƕ����� TRA
          if ((I2C->SR3)&0x04)
          {        
               if (Test_r_pos < Test_r_num)
               {
                      I2C->DR = Test_read[Test_r_pos ];
                      Test_r_pos ++;
                      if(Test_r_pos>23)
                        Test_r_pos=0;
               }
               else 
                    I2C->DR = 0XFF;  
          }
          else
          {
	      Test_r_pos=0; 
		  }
          Test_w_num= 0;
      }
      else if ((I2C->SR3)&0x04)//����ǽ���״̬��
      {        
      	  // �鿴TXE�Ƿ�DRΪ��
          if ((((I2C->SR1)&0x80) == 0X80))        
          {
               if (Test_r_pos < Test_r_num)
               {
                      I2C->DR = Test_read[Test_r_pos ];
                      Test_r_pos ++;

               }
              else 
              		 I2C->DR = 0XFF;           
          }              
      }
      else if (((I2C->SR1)&0x40)&&(!((I2C->SR3)&0x04)))
      {
      // ��DR������ձ�־
      		u8 data = I2C->DR;
                temp=data;
               
              
      	// ����������ͣ��ӻ����յ�����
          if (Test_w_num < 50)                         // 51 ������  
            Test_Write[Test_w_num ++] = temp ;
          
      }
      else if ((I2C->SR1)&0x10)
      {
      	 //��⵽ֹͣλ----���ֹͣλ---ͨ��дCR2
          I2C->CR2 = I2C->CR2;
          // ֹͣ��־λ��
          Test_r_pos = 0;
          Commucation_flag = 1;
      }  
      // �������
      if ((I2C->SR2)&0X0F)           
      {
          I2C->SR2 &= ~(0X0F); // ����
      }
      if(Test_w_num>=5)            //  0-48���ֽ�����
            Test_w_num=0;       // ȡ���е�52�������е�51����������   
                                //  1 ������cmd or  data 
                                //2  �� ��1 cmd ��2-49��ʾ�Զ���rgb��ɫ  ��1��data ��2 ��ʾѡ���ͼ��  ��3��ʾ ͼ�����ɫ��            
}


/******************************************************************************
*��һ���ֽ����ݴ�iic��
*��ڲ�����
*����ֵ��iic�����һ���ֽ�����
******************************************************************************/
u8 get_iic_receive_data(void)
{
    return temp;
} 

/*****************************************************************************
*����ָ�룬u8 *get_iic_data(u8*str,u8 n) ����ĳ������ĵڼ�������
*str ������
*n���ڼ�������
*******************************************************************************/
u8 *get_iic_data(void)
{
    u8 *p;
    p=Test_Write;
    return p;
}
/*****************************************************************************
*����ָ�룬*get_iic_send_address(void) 
*******************************************************************************/
u8 *get_iic_send_address(void)
{
  u8 *w;
  w=Test_read;
  return w;
}

u8 *get_iic_read_num(void)
{
   u8 *q;
   q=&Test_w_num;
   return q;
}
