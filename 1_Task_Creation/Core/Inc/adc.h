#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32f4xx_hal.h"

void adc_init(void);
uint32_t read_analog_sensor(void);



#endif /* INC_ADC_H_ */
