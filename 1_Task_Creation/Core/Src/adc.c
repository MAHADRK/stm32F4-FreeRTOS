#include "adc.h"

void adc_init(void)
{
	/* GPIOA for PA1 */
	RCC->AHB1ENR |= (1U<<0);

	/* ADC1 bus enable */
	RCC->APB2ENR |= (1U<<8);

	/* setting all pins to analog, causing
	 * UART connection to no response
	GPIOA->MODER &= 0xC; */

	/* Reseting PA1 */
	GPIOA->MODER &= ~(1U<<2); // set PA1 as Analog
	GPIOA->MODER &= ~(1U<<3);

	/* Setting PA1 to Analog mode */
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);

	/* turn on ADC */
	ADC1->CR2 = 0;

	/* sequence selection for ch 1 (as per order) */
	ADC1->SQR3 = 1;

	/* length of conversion seq */
	ADC1->SQR1 = 0;

	/* Continuous conversion */
	ADC1->CR2 |= 1;
}

uint32_t read_analog_sensor(void)
{
	ADC1->CR2 |= (1U<<30); //Start conversion of regular channels

	while(!(ADC1->SR & 2)){} //wait for end of conversion flag

	return ADC1->DR;
}
