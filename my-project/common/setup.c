#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>

#include "setup.h"

#define BAUDRATE 115200

void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

void uart_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	// setup gpio pins
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX | GPIO_USART1_RX);
	usart_set_baudrate(USART1, BAUDRATE);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);
}

int usart_write(const uint32_t usart, const char *databuf, const uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		usart_send_blocking(usart, databuf[i]);
	}
	return 0;
}

void usart_write_string(const uint32_t usart, const char *str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		usart_send_blocking(usart, str[i++]);
	}
}

void iic_setup(void)
{
	// forums say to initialize clock THEN GPIO
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	i2c_set_speed(I2C1, i2c_speed_sm_100k, rcc_apb1_frequency/1000000UL);
	i2c_set_own_7bit_slave_address(I2C1, 0x11);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SDA | GPIO_I2C1_SCL);
	// maybe setting them will help
	gpio_set(GPIOB, GPIO_I2C1_SCL|GPIO_I2C1_SDA);
	i2c_peripheral_enable(I2C1);
}