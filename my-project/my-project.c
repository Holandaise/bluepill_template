#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <stdio.h>
#include <common/setup.h>

#define BAUDRATE 115200
#define ENC_ADDR 0x36
#define RAW_ANGLE_ADDRH 0x0E
#define RAW_ANGLE_ADDRL 0x0F


static void systick_setup(void)
{
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_frequency(1000, rcc_ahb_frequency); // this calls systick_set_reload
	systick_clear();
	systick_interrupt_enable();
	systick_counter_enable();
}

static volatile uint64_t _millis = 0;

void sys_tick_handler(void)
{
	_millis++;
}

static void delay(uint32_t ms)
{
	const uint64_t until = _millis + ms;
	while (_millis < until)
		;
}


int main(void)
{

	int i, j = 0, c = 0;
	clock_setup();
	systick_setup();
	uart_setup();
	iic_setup();

	usart_wait_send_ready(USART1);

	uint8_t write_buf = 0;
	uint8_t read_h = 0, read_l = 0;
	uint32_t raw_encoder;
	unsigned int encoder_val = 0;
	static char to_send[7];
	usart_write_string(USART1, "Angle\n");
	float scale = (360.0f/4096.0f);
	// set hysteresis bits
	uint16_t hyst = (0x08 << 8 | 0x03 << 2);
	i2c_transfer7(I2C1, ENC_ADDR, (uint8_t*)&hyst, 2, &read_h, 0);

	while (1)
	{
		write_buf = RAW_ANGLE_ADDRH;
		i2c_transfer7(I2C1, ENC_ADDR, &write_buf, 1, &read_h, 1);
		write_buf = RAW_ANGLE_ADDRL;
		i2c_transfer7(I2C1, ENC_ADDR, &write_buf, 1, &read_l, 1);
		raw_encoder = (uint16_t)(read_h << 8 | read_l);
		encoder_val = (unsigned int)raw_encoder;

		snprintf(to_send, 7, "%04i\n", encoder_val);
		usart_write_string(USART1, to_send);
		//usart_write(USART1, to_send, 6);
		delay(30);
	}
	return 0;
}