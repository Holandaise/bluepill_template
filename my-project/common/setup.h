#ifndef _SETUP_H
#define _SETUP_H
void iic_setup(void);
void uart_setup(void);
void clock_setup(void);
void usart_write_string(const uint32_t usart, const char *str);
int usart_write(const uint32_t usart, const char *databuf, const uint8_t len);
#endif