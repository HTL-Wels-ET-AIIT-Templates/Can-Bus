/**
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

#include "stm32_ub_can1.h"
#include "DS18B20.h"
#include "tempsensor.h"
#include "CanFrame.h"

// function declarations
// ...

/**
* canInit function
* @param none
* @return none
*/
extern "C" void cancppInit(void) {
	UB_CAN1_Init();

	// ToDo Show Send-Cnt and Recv-Cnt on Display
	// Send-Cnt: Recv-Cnt: Send-Data: Recv-Data:
	LCD_SetFont(&Font8x8);
	LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_BLUE);
	LCD_SetPrintPosition(7,1);
	printf("CAN1: Send-Recv");

	LCD_SetColors(LCD_COLOR_GREEN, LCD_COLOR_BLUE);
	LCD_SetPrintPosition(9,1);
	printf("Send-Cnt:");
	LCD_SetPrintPosition(9,15);
	printf("%5d", 0);
	LCD_SetPrintPosition(11,1);
	printf("Receive-Cnt:");
	LCD_SetPrintPosition(11,15);
	printf("%5d", 0);
	LCD_SetPrintPosition(13,1);
	printf("Send-Data:");
	LCD_SetPrintPosition(15,1);
	printf("Recv-Data:");

	LCD_SetPrintPosition(30,1);
	printf("Bit-Timing-Register: %ux", CAN1->BTR);

	tempSensorInit();  
} 

extern "C" void cancppSendTask(void) {
  CanFrame tx;
	static uint8_t sendCnt = 0;
  
  float t = tempSensorGetTemperature();
  LCD_SetPrintPosition(31,1);
  printf("T: %3.2f", t);
  uint16_t p = t * 10 ;
  
  tx.setId(0x0F5);
  tx.addData(0xAF);
  tx.addData(sendCnt);
  tx.addData(p);
  tx.addData(p << 8);
  
  if (tx.txData()) {
    sendCnt++;
  }
  tx.printData(13, 13);
  LCD_SetPrintPosition(9,15);
  printf("%5d", sendCnt);
}

extern "C" void cancppReceiveTask(void) {
  CanFrame rx;
  static uint8_t recvCnt = 0;
  
  if (rx.rxData()) {
    recvCnt++;
    LCD_SetPrintPosition(11,15);
    printf("%5d", recvCnt);

    rx.printData(15, 13);
  }
}

