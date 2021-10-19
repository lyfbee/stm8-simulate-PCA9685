#ifndef __IIC_H
#define __IIC_H


void iic_init();
u8 get_iic_receive_data(void);
u8 *get_iic_data(void);

u8 *get_iic_send_address(void);

u8 *get_iic_read_num(void);


#endif


