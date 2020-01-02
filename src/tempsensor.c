/**
 * Simplify usage of DS18B20 temperature sensor
 *
 */


#include "tempsensor.h"
#include "DS18B20.h"
#include "stm32f4xx_rcc.h"

// function declarations

/**
 * Initialize peripherals for DS18B20
 * return 1 if a device has been found
 */
void tempSensorInit(void)
{
  GPIO_InitTypeDef pg9;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	pg9.GPIO_Pin = GPIO_Pin_9;
	pg9.GPIO_Mode = GPIO_Mode_OUT;
	pg9.GPIO_OType = GPIO_OType_OD;
	pg9.GPIO_PuPd = GPIO_PuPd_UP;
	pg9.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &pg9);

	ds1820_init(GPIOG, GPIO_Pin_9);
}

/**
 * Get
 * return 1 if a device has been found
 */
float tempSensorGetTemperature()
{
	float temperature = -1e3;

  temperature = ds1820_read_temp(GPIOG, GPIO_Pin_9);
  return temperature;
}
