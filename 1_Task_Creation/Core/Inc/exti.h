#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "stm32f4xx_hal.h"

void p13_interrupt_init(void);
void gpio_init(void);
uint8_t read_digital_sensor_data(void);


#endif /* INC_EXTI_H_ */
