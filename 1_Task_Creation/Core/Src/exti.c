#include "exti.h"



void p13_interrupt_init(void)
{
	RCC->AHB1ENR |= (1U<<3);

	RCC->APB2ENR |= (1U<<14);

	GPIOC->MODER &= ~(0x0C000000);

	/* resetting PC13 interrupt configuration */
	SYSCFG->EXTICR[3] &= ~(0x00F0);

	/* setting PC13 interrupt configuration */
	SYSCFG->EXTICR[3] |= (0x0020);

	/* enable interrupt mask*/
	EXTI->IMR |= 0x2000;

	/* set interrupt for falling edge
	 * as button is active low */
	EXTI->FTSR |= 0x2000;

	/* set priority
	 * see Table 38. pg 236*/
	NVIC_SetPriority(EXTI15_10_IRQn, 6);

	NVIC_EnableIRQ(EXTI15_10_IRQn);

}


void gpio_init(void)
{
	RCC->AHB1ENR |= (1U<<2);
}

uint8_t read_digital_sensor_data(void)
{
	if (GPIOC->IDR & (0x2000))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
