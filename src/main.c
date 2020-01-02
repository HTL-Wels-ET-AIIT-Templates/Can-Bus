/**
 * CAN Network
 * CAN1 is used, GPIO config: TX = PB9, RX = PB8 
 * (Those GPIOs are also used for display, that's why the display is flickering when
 * CAN is used)
 *
 * Temperature sensor DS18B20 has to be connected to 3V3, GND and PG9
 *
 * Exercises are marked with ToDo. There are 2 stages of ToDos:
 * 1) Implement CAN communication with a counter value
 * 2) Add temperature value to CAN payload. ( Marked with ToDo (2) )
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include "can.h"
#include "cancpp.h"

volatile int32_t SysTickCnt=0;

// function declarations
// ...
static int GetUserButtonPressed(void);

void SysTick_Handler(void) {	// SysTick ISR: will be executed every 10ms (configured by SysTick_Config())
  SysTickCnt++;
}

/**
* Check if User Button has been pressed
* @param none
* @return 1 if rising edge has been detected
*/
static int GetUserButtonPressed(void) {
  return (GPIOA->IDR & 0x0001);
}

/**
* main function
* @param none
* @return should never return
*/
int main(void) {
	static uint32_t SysTickCnt_Hold;

	RCC_ClocksTypeDef Clocks;
	SysTickCnt = 0;
	SysTickCnt_Hold = 0;

	RCC_GetClocksFreq(&Clocks);
	SysTick_Config( Clocks.HCLK_Frequency/100 - 1 );	// 100 Hz ( T=10ms)

	LCD_Init();
	LCD_LayerInit();
	LTDC_Cmd(ENABLE);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_BLUE);
	LCD_SetColors(LCD_COLOR_YELLOW, LCD_COLOR_BLUE);

	LCD_SetFont(&Font16x24);
	LCD_SetPrintPosition(1,0);
	printf("   HTL-WELS    ");
	LCD_SetPrintPosition(2,0);

	LCD_SetFont(&Font8x8);
	LCD_SetPrintPosition(39,0);
	LCD_SetColors(LCD_COLOR_MAGENTA, LCD_COLOR_BLUE); // TextColor,BackColor
	printf("  Copyright CAN Experts!  ");

  // ToDo: set up CAN peripherals



	while (1) {

		if( SysTickCnt - SysTickCnt_Hold < 10 ) {
			continue;  // 10 * 10ms = 100ms
		}
		SysTickCnt_Hold += 10;

    // ToDo: send data over CAN when user button has been pressed




    // ToDo: check if data has been received

    
    
    
		LCD_SetPrintPosition(36,1);
		printf("Time:%5.1f sec",(float)SysTickCnt/(float)100.0);

	}
} // main


