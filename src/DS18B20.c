/*
**
**                           DS18B20.c
** Source: https://www.mikrocontroller.net/topic/378438
**
**********************************************************************/

/* Includes
**********************************************************************/
#include "DS18B20.h"
#include "math.h"

#define OW_Delay_A          (uint32_t) 6
#define OW_Delay_B          (uint32_t) 64
#define OW_Delay_C          (uint32_t) 60
#define OW_Delay_D          (uint32_t) 10
#define OW_Delay_E          (uint32_t) 9
#define OW_Delay_F          (uint32_t) 55
#define OW_Delay_G          (uint32_t) 0
#define OW_Delay_H          (uint32_t) 480
#define OW_Delay_I          (uint32_t) 70
#define OW_Delay_J          (uint32_t) 410

static void    ds1820_wr_bit(uint8_t wrbit, GPIO_TypeDef * port, uint16_t used_pin);    //write one bit to device
static void    ds1820_wr_byte(uint8_t wrbyte, GPIO_TypeDef * port, uint16_t used_pin);  //write one byte to device
static uint8_t ds1820_re_bit(GPIO_TypeDef * port, uint16_t used_pin);                   //read one bit from device
static uint8_t ds1820_re_byte(GPIO_TypeDef * port, uint16_t used_pin);                  //read one byte from device

/* Functions
**********************************************************************/

static void Delay_us(__IO uint16_t us){
  volatile uint16_t usStart = (uint16_t)TIM3->CNT;
  while((TIM3->CNT - usStart) < us){
    __asm("nop");
  }
}

//reset device
uint8_t ds1820_reset(GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t err=100;

  Delay_us(OW_Delay_G);                             // 0us (standard)
  port->ODR &= ~used_pin;                           // Pull low
  Delay_us(OW_Delay_H);							    // 480us (standard)
  port->ODR |= used_pin;                            // Pull high over extern pullup
  Delay_us(OW_Delay_I-10);							    // 70us (standard)
  if((port->IDR & used_pin) > 0){
    err = 1;
  }else{
    err = 0;
  }
  Delay_us(OW_Delay_J);							    // 410us (standard)

  if((port->IDR & used_pin) == 0 ){			// short circuit --> err=2
    err = 2;
  }

  return err;
}

//write one bit to device
static void ds1820_wr_bit(uint8_t wrbit, GPIO_TypeDef * port, uint16_t used_pin){
  if (wrbit == 0){
    port->ODR &= ~used_pin;                         // Pull low
    Delay_us(OW_Delay_C);                           // 60us (standard)
    port->ODR |= used_pin;                          // Pull high over extern pullup
    Delay_us(OW_Delay_D);                           // 10us (standard)
  }
  if (wrbit == 1){
    port->ODR &= ~used_pin;                         // Pull low
    Delay_us(OW_Delay_A);                           // 6us (standard)
    port->ODR |= used_pin;                          // Pull high over extern pullup
    Delay_us(OW_Delay_B);                           // 64us (standard)
  }
}

//write one byte to device
static void ds1820_wr_byte(uint8_t wrbyte, GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t i;
  for (i=0; i<8; i++){ // writes byte, one bit at a time
    ds1820_wr_bit((wrbyte & 0x01), port, used_pin);
    wrbyte = wrbyte >> 1;
  }
  Delay_us(5);
}

//read one bit from device
static uint8_t ds1820_re_bit(GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t rebit;
  port->ODR &= ~used_pin;                           // Pull low
  Delay_us(OW_Delay_A);                             // 6us (standard)
  port->ODR |= used_pin;                            // Pull high over extern pullup
  Delay_us(OW_Delay_E);							    // 9us (standard)
  if((port->IDR & used_pin) > 0){
    rebit = 1;                     // Read bit
  }else{
    rebit = 0;
  }
  Delay_us(OW_Delay_F);							    // 55us (standard)
  return rebit;
}

//read one byte from device
static uint8_t ds1820_re_byte(GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t rebyte =0x00;
  uint8_t rebit;
  uint8_t i;

  for (i=0;i<8;i++){
    rebit=ds1820_re_bit(port, used_pin);
	Delay_us(2);									//be on the save side
	if (rebit==1){
      rebyte|=(1<<i);
    }
  }
  return(rebyte);
}

//read temperature from device
float   ds1820_read_temp(GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t error,i;
  uint16_t j = 0;
  uint8_t scratchpad[9] = {0};
  float temp = 0;
  error=ds1820_reset(port, used_pin);               //1. Reset
  if (error==0){
    ds1820_wr_byte(0xCC, port, used_pin);  					//2. skip ROM
    ds1820_wr_byte(0x44, port, used_pin);  					//3. ask for temperature conversion
    while ((port->IDR & used_pin)==0){			        //4. wait until conversion is finished
      j++;
    }
    error=ds1820_reset(port, used_pin);							//5. Reset
    ds1820_wr_byte(0xCC, port, used_pin);  					//6. skip ROM
    ds1820_wr_byte(0xBE, port, used_pin);  					//7. Read entire scratchpad 9 bytes

    for (i=0; i<2; i++){         									  //8. Get scratchpad byte by byte
      scratchpad[i]=ds1820_re_byte(port, used_pin); //9. read one DS18S20 byte
    }
  }
  //Umrechnung von Scratchpad zu Temperatur
  temp = ((scratchpad[0])>>4)+((scratchpad[1] & 0x07)<<4);
  temp += ((scratchpad[0] & 0x08)>>3)*0.5;
  temp += ((scratchpad[0] & 0x04)>>2)*0.25;
  temp += ((scratchpad[0] & 0x02)>>1)*0.125;
  temp += ((scratchpad[0] & 0x01)>>0)*0.0625;
  if((scratchpad[1]&0xF8) > 0) {
    temp *= -1;
  }
  return temp;
}

//initialize device
void ds1820_init(GPIO_TypeDef * port, uint16_t used_pin){
  uint8_t error;
  uint16_t i =0;
  // configure timer
	TIM_TimeBaseInitTypeDef  tim3;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */
	tim3.TIM_Period = 0xFFFF; // 1 MHz down to 1 KHz (1 ms)
	tim3.TIM_Prescaler = 90; 
	tim3.TIM_ClockDivision = 0;
	tim3.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &tim3);
	TIM_Cmd(TIM3, ENABLE);
  
// Test delay function
//    GPIO_InitTypeDef pg9;
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//	pg9.GPIO_Pin = GPIO_Pin_9;
//	pg9.GPIO_Mode = GPIO_Mode_OUT;
//	pg9.GPIO_OType = GPIO_OType_PP;
//	pg9.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	pg9.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG, &pg9);
//  
//  while (1) {
//    Delay_us(1000);
//    GPIO_ToggleBits(GPIOG, GPIO_Pin_9);
//  }

  error=ds1820_reset(port, used_pin);									//1. Reset
  if (error==0){
    ds1820_wr_byte(0xCC, port, used_pin);  							//2. skip ROM
	ds1820_wr_byte(0x44, port, used_pin);  							//3. ask for temperature conversion
    while ((port->IDR & used_pin)==0){			//4. wait until conversion is finished
      i++;
    }
    Delay_us(20);
    error=ds1820_reset(port, used_pin);								//5. Reset
    ds1820_wr_byte(0xCC, port, used_pin);  							//6. skip ROM
	ds1820_wr_byte(0xBE, port, used_pin);  							//7. Read entire scratchpad 9 bytes
  }
  
}

