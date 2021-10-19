#ifndef __uart_h
#define __uart_h

void debug_uart_init(void);
void send_debug_data(uint8_t *Buffer);
void u32tostr(unsigned long dat,char *str);

#endif
