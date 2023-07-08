#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

static void clock_setup(void){
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void gpio_setup(void){
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

static void systick_setup(void){
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_clear();
	systick_set_reload(rcc_ahb_frequency / 1000 -1); // reload systick every millisecond
	systick_interrupt_enable();
	systick_counter_enable();
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
	clock_setup();
	gpio_setup();
	systick_setup();
	while(1){
		gpio_toggle(GPIOC, GPIO13);
		delay(500);
	}
	return 0;
}
