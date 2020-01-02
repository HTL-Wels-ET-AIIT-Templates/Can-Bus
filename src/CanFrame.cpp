/*
 * CanFrame.cpp
 *
 *  Created on: 25.11.2019
 *      Author: ber
 */

#include "CanFrame.h"
#include "stm32_ub_can1.h"
#include "stm32f4xx_can.h"
#include "stm32f429i_discovery_lcd.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern volatile int32_t SysTickCnt;

CanFrame::CanFrame() : mDataSize(0), mId(0xFFFFFFFF) {

}

CanFrame::~CanFrame() {

}

bool CanFrame::setData(uint8_t *data, unsigned int len) {
  if (len > 8) {
    return false;
  }
	memcpy(mData, data, len);
	mDataSize = len;
  return true;
}

void CanFrame::setId(uint32_t id) {
	mId = id;
}

bool CanFrame::addData(uint8_t data) {
  if (mDataSize >= 8) {
    return false;
  }
  mData[mDataSize] = data;
  mDataSize++;
  return true;
}

bool CanFrame::txData(void) {
	  uint32_t timeout;
	  CanTxMsg msg;

	  if(!isValid()) {
	    return false;
	  }

	  // daten frame vorbereiten
	  msg.StdId = mId;
	  msg.ExtId = 0x00;
	  msg.RTR = CAN_RTR_DATA;
	  msg.IDE = CAN_ID_STD;
	  msg.DLC = mDataSize;

	  for(int i = 0; i < mDataSize; i++) {
	    msg.Data[i] = mData[i];
	  }

	  // warten bis ein Transmit Register frei ist
	  timeout=0;
	  while(!(CAN1->TSR & CAN_TSR_TME0 || CAN1->TSR & CAN_TSR_TME1 || CAN1->TSR & CAN_TSR_TME2)) {
	    timeout++;
	    if(timeout>CAN1_TX_TIMEOUT) {
	    	break;
	    }
	  }

	  // daten frame senden
	  uint8_t status = CAN_Transmit(CAN1, &msg);
	  if (status == CAN_TxStatus_NoMailBox) {
		  return false;
	  }

	  return true;
}

bool CanFrame::rxData(void) {
  bool ret = false;
  CAN1_STATUS_t s;
	CAN1_RX_FRAME_t myRXFrame1; // Puffer für RX-Daten
	uint32_t SysTickCntStart;

  SysTickCntStart = SysTickCnt;
  s = UB_CAN1_receive(&myRXFrame1);

  while ( (s != CAN1_RX_READY) &&
      ((SysTickCnt - SysTickCntStart) < 2) /* 20 ms */ ) {
    s = UB_CAN1_receive(&myRXFrame1);
  }

  if (s == CAN1_RX_READY) {
    mId = myRXFrame1.can_id;
    memcpy(mData, myRXFrame1.data, myRXFrame1.nr_bytes);
    mDataSize = myRXFrame1.nr_bytes;
    ret = true;
  }
  return ret;
}

void CanFrame::getData(uint8_t *data, unsigned int *len) {
	memcpy(data, mData, mDataSize);
	*len = mDataSize;
}

uint32_t CanFrame::getId(void) {
	return mId;
}

bool CanFrame::isValid(void) {
	if ((mId < (0x01 << 12)) && mDataSize > 0 && mDataSize < 9) {
		return true;
	}
	return false;
}

void CanFrame::printData(unsigned int ln, unsigned int col) {
	if (!isValid()) {
		return;
	}
	LCD_SetPrintPosition(ln, col);
	printf ("%03x", mId);
  col += 5;
	for (int i = 0; i < mDataSize; i++) {
		LCD_SetPrintPosition(ln, col);
    printf ("%02x", mData[i]);
    col = col + 3;
	}
}
