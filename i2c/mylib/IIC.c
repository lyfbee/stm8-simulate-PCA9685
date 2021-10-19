#include "stm8s.h"
#include "IIC.H"

/****************************************************************************************
*iic初始化函数
*入口参数：无
*返回值：无
*作用：iic从机初始化
*****************************************************************************************/
void iic_init()    
{  
   CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
   //GPIO_Init(GPIOB,GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);    // 初始化io 
   //GPIO_Init(GPIOB,GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST); 
   I2C_DeInit();    // 先失能写入寄存器
   I2C_Init(100000,0x80,I2C_DUTYCYCLE_2,I2C_ACK_CURR,I2C_ADDMODE_7BIT,16);  //初始化   自身地址0xa0， 标准模式100k 7为地址 16m频率时钟
   I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE); // 开启iic中断
   I2C_Cmd(ENABLE);
}

/*************************************************************************
*INTERRUPT_HANDLER 中断服务函数
*I2C_IRQHandler，19
*作用：从机接收发送功能
**************************************************************************/
u8 Test_read[50] = {0};
u8 Test_r_num = 31;	//从机发送个数
u8 Test_r_pos = 0;	  // 发送指针
u8 Test_Write[50] = {0};   // 收到50个数据
u8 Test_w_num = 0;   // 主机发送数据个数
u8 Commucation_flag = 0;// 通信完成标志
static u8 temp=0;



INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
       //接收发送   
      if (I2C->SR1&0x02)//地址已经匹配 ADDR标志
      {          
      	 // 清ADDR标志:读SR1，读SR3
      	 // 主机读，从机发出的情况，DR寄存器写数据越快越好
      	 // 判断是否是读操作 TRA
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
      else if ((I2C->SR3)&0x04)//如果是接收状态：
      {        
      	  // 查看TXE是否DR为空
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
      // 读DR，清接收标志
      		u8 data = I2C->DR;
                temp=data;
               
              
      	// 如果主机发送，从机接收到数据
          if (Test_w_num < 50)                         // 51 个数据  
            Test_Write[Test_w_num ++] = temp ;
          
      }
      else if ((I2C->SR1)&0x10)
      {
      	 //检测到停止位----清除停止位---通过写CR2
          I2C->CR2 = I2C->CR2;
          // 停止标志位置
          Test_r_pos = 0;
          Commucation_flag = 1;
      }  
      // 错误管理
      if ((I2C->SR2)&0X0F)           
      {
          I2C->SR2 &= ~(0X0F); // 清零
      }
      if(Test_w_num>=5)            //  0-48个字节数据
            Test_w_num=0;       // 取其中的52个数组中的51个数据来用   
                                //  1 用来做cmd or  data 
                                //2  若 是1 cmd 则2-49表示自定义rgb颜色  若1是data 则2 表示选择的图像  ，3表示 图像的颜色。            
}


/******************************************************************************
*读一个字节数据从iic中
*入口参数无
*返回值：iic传输的一个字节数据
******************************************************************************/
u8 get_iic_receive_data(void)
{
    return temp;
} 

/*****************************************************************************
*返回指针，u8 *get_iic_data(u8*str,u8 n) 返回某个数组的第几个数据
*str ：数组
*n：第几个数据
*******************************************************************************/
u8 *get_iic_data(void)
{
    u8 *p;
    p=Test_Write;
    return p;
}
/*****************************************************************************
*返回指针，*get_iic_send_address(void) 
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
