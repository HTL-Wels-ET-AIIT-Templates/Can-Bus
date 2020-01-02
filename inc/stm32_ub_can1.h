//--------------------------------------------------------------
// File     : stm32_ub_can1.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_CAN1_H
#define __STM32F4_UB_CAN1_H

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"
#include "misc.h"
#include <stdint.h>

//--------------------------------------------------------------
// CAN-Clock
//
//   Hinweis: Terminierung verwenden   
//
//--------------------------------------------------------------

// ToDo: choose Prescaler configuration
#define   CAN1_CLOCK_PRESCALER    1000

//--------------------------------------------------------------
// Struktur eines CAN-Pins
//--------------------------------------------------------------
typedef struct {
  GPIO_TypeDef* PORT;     // Port
  uint16_t PIN;     // Pin
  uint32_t CLK;     // Clock
  uint8_t SOURCE;   // Source
}CAN1_PIN_t;


//--------------------------------------------------------------
// Struktur vom CAN-Device
//--------------------------------------------------------------
typedef struct {
  CAN1_PIN_t  TX;       // TX-Pin
  CAN1_PIN_t  RX;       // RX-Pin
}CAN1_DEV_t;


//-------------------------------------
// Struktur eines CAN-TX-Frame
//-------------------------------------
typedef struct {
  uint32_t can_id;    // STD=11bit  EXT=29bit
  uint8_t nr_bytes;  // anzahl der daten [0...8]
  uint8_t data[8];    // datenbytes
}CAN1_TX_FRAME_t;


//-------------------------------------
// Mode von einem Frame
//-------------------------------------
typedef enum {
  CAN1_STD_DATA =0, // standard Daten-Frame
  CAN1_STD_REMOTE,  // standard Remote-Frame
  CAN1_EXT_DATA,    // extended Daten-Frame
  CAN1_EXT_REMOTE   // extended Remote-Frame
}CAN1_FRAME_MODE_t;


//-------------------------------------
// Struktur eines CAN-RX-Frame
//-------------------------------------
typedef struct {
  CAN1_FRAME_MODE_t frame_mode;    // Frame-Mode
  uint32_t can_id;    // STD=11bit  EXT=29bit
  uint8_t nr_bytes;  // anzahl der daten [0...8]
  uint8_t data[8];    // datenbytes
}CAN1_RX_FRAME_t;


//-------------------------------------
// Status beim receive
//-------------------------------------
typedef enum {
  CAN1_RX_EMPTY =0,  // noch nichts empfangen
  CAN1_RX_READY      // etwas wurde empfangen
}CAN1_STATUS_t;


//-------------------------------------
// Mode einer Maske
//-------------------------------------
typedef enum {
  CAN1_VALUE_IGNORE =0, // wert ist egal
  CAN1_VALUE_CHECK      // wert wird gepr�ft
}CAN1_MASKE_t;


//-------------------------------------
// Struktur eines Filters
// f�r ID und Mode
//-------------------------------------
typedef struct {
  uint32_t id;                  // ID
  CAN1_FRAME_MODE_t frame_mode; // mode
}CAN1_FILTER1_t;


//-------------------------------------
// Struktur einer Maske
// f�r ID und Mode
//-------------------------------------
typedef struct {
  uint32_t id_mask;      // maske [Bit Hi=check, Bit Lo=ignore]
  CAN1_MASKE_t rtr_bit;  // maske f�r Data/Remote
  CAN1_MASKE_t ide_bit;  // maske f�r std/ext
}CAN1_MASK1_t;


//-------------------------------------
// Struktur von standard Filter-Liste
//-------------------------------------
typedef struct {
  CAN1_FILTER1_t id1; // erster ID-Filter
  CAN1_FILTER1_t id2; // zweiter ID-Filter
  CAN1_FILTER1_t id3; // dritter ID-Filter
  CAN1_FILTER1_t id4; // vierter ID-Filter
}CAN1_STD_FL_t;


//-------------------------------------
// Struktur von standard Filter-Maske
//-------------------------------------
typedef struct {
  CAN1_FILTER1_t id1;   // erster ID-Filter
  CAN1_MASK1_t mask1;   // erste Maske
  CAN1_FILTER1_t id2;   // zweite ID-Filter
  CAN1_MASK1_t mask2;   // zweite Maske
}CAN1_STD_FM_t;


//-------------------------------------
// Struktur von extended Filter-Liste
//-------------------------------------
typedef struct {
  CAN1_FILTER1_t id1; // erster ID-Filter
  CAN1_FILTER1_t id2; // zweiter ID-Filter
}CAN1_EXT_FL_t;


//-------------------------------------
// Struktur von extended Filter-Maske
//-------------------------------------
typedef struct {
  CAN1_FILTER1_t id1;   // erster ID-Filter
  CAN1_MASK1_t mask1;   // erste Maske
}CAN1_EXT_FM_t;


//-------------------------------------
// interne Struktur f�r RX-Buffer
//-------------------------------------
typedef struct {
  CAN1_STATUS_t status;         // RX-Status
  CAN1_FRAME_MODE_t frame_mode; // Frame-Mode
  uint32_t can_id;              // STD=11bit  EXT=29bit
  uint8_t nr_bytes;            // anzahl der daten [0...8]
  uint8_t data[8];              // datenbytes
}CAN1_BUFFER_t;


//-------------------------------------
#define  CAN1_TX_TIMEOUT  500000  // > 5000



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_CAN1_Init(void);
ErrorStatus UB_CAN1_send_std_data(CAN1_TX_FRAME_t tx_frame);
ErrorStatus UB_CAN1_send_std_remote(CAN1_TX_FRAME_t tx_frame);
ErrorStatus UB_CAN1_send_ext_data(CAN1_TX_FRAME_t tx_frame);
ErrorStatus UB_CAN1_send_ext_remote(CAN1_TX_FRAME_t tx_frame);
CAN1_STATUS_t UB_CAN1_receive(CAN1_RX_FRAME_t *rx_frame);
// Filter Funktionen
void UB_CAN1_std_FilterList(CAN1_STD_FL_t filter, uint8_t nr);
void UB_CAN1_std_FilterMask(CAN1_STD_FM_t filter, uint8_t nr);
void UB_CAN1_ext_FilterList(CAN1_EXT_FL_t filter, uint8_t nr);
void UB_CAN1_ext_FilterMask(CAN1_EXT_FM_t filter, uint8_t nr);


#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------
#endif // __STM32F4_UB_CAN1_H
