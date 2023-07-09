#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#define BAUDRATE 115200

static void clock_setup(void){
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void gpio_setup(void){
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void systick_setup(void){
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_clear();
	systick_set_reload(rcc_ahb_frequency / 1000 -1); // reload systick every millisecond
	systick_interrupt_enable();
	systick_counter_enable();
}

static void uart_setup(void){
	rcc_periph_clock_enable(RCC_USART1);
	// setup gpio pins
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX|GPIO_USART1_RX);
	// set baudrate
	usart_set_baudrate(USART1, BAUDRATE);
	// set databits
	usart_set_databits(USART1, 8);
	// set stop bits
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	// set mode
	usart_set_mode(USART1, USART_MODE_TX_RX);
	// set parity
	usart_set_parity(USART1, USART_PARITY_NONE);
	// set flow control
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	// enable usart
	usart_enable(USART1);
}

static volatile uint64_t _millis = 0;

void sys_tick_handler(void){
	_millis++;
}

static void delay(uint32_t ms){
	const uint64_t until = _millis + ms;
	while(_millis < until);
}

int main(void){

	int i, j= 0, c= 0;
	clock_setup();
	gpio_setup();
	systick_setup();
	uart_setup();


	while(1){
		gpio_toggle(GPIOC, GPIO13);
		usart_wait_send_ready(USART1);
		usart_send_blocking(USART1, c + '0');
		c = (c == 9) ? 0 : c + 1;
		if((j++ % 80) == 0){
			usart_wait_send_ready(USART1);
			usart_send_blocking(USART1, '\r');
			usart_wait_send_ready(USART1);
			usart_send_blocking(USART1, '\n');
		}
		delay(100);
	}
	return 0;
}