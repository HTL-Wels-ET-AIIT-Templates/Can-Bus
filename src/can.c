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
#include "tempsensor.h"


// function declarations
// ...

/**
* canInit function
* @param none
* @return none
*/
void canInit(void) {
	UB_CAN1_Init();

	LCD_SetFont(&Font8x8);
	LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_BLUE);
	LCD_SetPrintPosition(7,1);
	printf("CAN1: Send-Recv");

	LCD_SetColors(LCD_COLOR_GREEN, LCD_COLOR_BLUE);
  // ToDo: Show Send-Cnt, Recv-Cnt, Send-Data and Recv-Data on Display
  











  LCD_SetPrintPosition(30,1);
	printf("Bit-Timing-Register: %ux", CAN1->BTR);

  // ToDo (2): set up DS18B20 (temperature sensor)


}

/**
* sends a CAN frame
* @param none
* @return none
*/
void canSendTask(void) {
	// ToDo declare the required variables
	static unsigned int sendCnt = 0;
	int xpos;

  
  
  
  
  
  // ToDo (2): get temperature value

  
  
  
  
  // ToDo prepare send data

  
  
  
  
  
  // ToDo send CAN frame





  // ToDo display send counter and send data






}

/**
* checks if a can frame has been received
* @param none
* @return none
*/
void canReceiveTask(void) {
	// ToDo declare the required variables
	CAN1_STATUS_t ret;
	static unsigned int recvCnt = 0;
	int xpos;

  
  
  
  
  
  // ToDo: check if CAN frame has been received




  // ToDo: Process received CAN Frame (extract data)
  
  
  
  
  

  // ToDo display recv counter and recv data






}


