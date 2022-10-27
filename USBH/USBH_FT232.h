/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2022     SEGGER Microcontroller GmbH              *
*                                                                    *
*       www.segger.com     Support: www.segger.com/ticket            *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Host * USB Host stack for embedded applications        *
*                                                                    *
*       Please note: Knowledge of this file may under no             *
*       circumstances be used to write a similar product.            *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Host version: V2.36.0                                  *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
All Intellectual Property rights in the software belongs to SEGGER.
emUSB Host is protected by international copyright laws. This file
may only be used in accordance with the following terms:

The source code of the emUSB Host software has been licensed to Cypress
Semiconductor Corporation, whose registered office is 198 Champion
Court, San Jose, CA 95134, USA including the 
right to create and distribute the object code version of 
the emUSB Host software for its Cortex M0, M0+ and M4 based devices.
The object code version can be used by Cypress customers under the 
terms and conditions of the associated End User License Agreement.
Support for the object code version is provided by Cypress, 
full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              Cypress Semiconductor Corp, 198 Champion Ct., San Jose, CA 95134, USA
Licensed SEGGER software: emUSB-Host
License number:           USBH-00303
License model:            Cypress Services and License Agreement, signed June 9th/10th, 2009
                          and Amendment Number One, signed June 28th, 2019 and July 2nd, 2019
                          and Amendment Number Two, signed September 13th, 2021 and September 18th, 2021
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : USBH_FT232.h
Purpose     : API of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_FT232_H_
#define USBH_FT232_H_

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#define USBH_FT232_INVALID_HANDLE     0u

//
// Baud Rates
//
#define USBH_FT232_BAUD_300         300
#define USBH_FT232_BAUD_600         600
#define USBH_FT232_BAUD_1200       1200
#define USBH_FT232_BAUD_2400       2400
#define USBH_FT232_BAUD_4800       4800
#define USBH_FT232_BAUD_9600       9600
#define USBH_FT232_BAUD_14400     14400
#define USBH_FT232_BAUD_19200     19200
#define USBH_FT232_BAUD_38400     38400
#define USBH_FT232_BAUD_57600     57600
#define USBH_FT232_BAUD_115200   115200
#define USBH_FT232_BAUD_230400   230400
#define USBH_FT232_BAUD_460800   460800
#define USBH_FT232_BAUD_921600   921600

//
// Word Lengths
//
#define USBH_FT232_BITS_8         (U8)8
#define USBH_FT232_BITS_7         (U8)7

//
// Stop Bits
//
#define USBH_FT232_STOP_BITS_1    (U8)0
#define USBH_FT232_STOP_BITS_2    (U8)1

//
// Parity
//
#define USBH_FT232_PARITY_NONE    (U8)0
#define USBH_FT232_PARITY_ODD     (U8)1
#define USBH_FT232_PARITY_EVEN    (U8)2
#define USBH_FT232_PARITY_MARK    (U8)3
#define USBH_FT232_PARITY_SPACE   (U8)4

//
// Flow Control
//
#define USBH_FT232_FLOW_NONE        0x0000
#define USBH_FT232_FLOW_RTS_CTS     0x0100
#define USBH_FT232_FLOW_DTR_DSR     0x0200
#define USBH_FT232_FLOW_XON_XOFF    0x0400

//
// Purge rx and tx buffers
//
#define USBH_FT232_PURGE_RX         1u
#define USBH_FT232_PURGE_TX         2u

/*********************************************************************
*
*       USBH_FT232_HANDLE
*/
typedef U32 USBH_FT232_HANDLE;

/*********************************************************************
*
*       USBH_FT232_DEVICE_INFO
*
*  Description
*    Contains information about an FT232 device.
*/
typedef struct {
  U16         VendorId;       // USB Vendor Id.
  U16         ProductId;      // USB Product Id.
  U16         bcdDevice;      // The BCD coded device version.
  USBH_SPEED  Speed;          // Operation speed of the device. See USBH_SPEED.
  U16         MaxPacketSize;  // Maximum size of a single packet in bytes.
} USBH_FT232_DEVICE_INFO;

typedef void USBH_FT232_USER_FUNC(void * pContext);

U8                USBH_FT232_Init                     (void);
void              USBH_FT232_Exit                     (void);
void              USBH_FT232_RegisterNotification     (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_FT232_AddNotification          (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_FT232_RemoveNotification       (const USBH_NOTIFICATION_HOOK * pHook);
USBH_STATUS       USBH_FT232_AddCustomDeviceMask      (const U16 * pVendorIds, const U16 * pProductIds, U16 NumIds);
void              USBH_FT232_ConfigureDefaultTimeout  (U32 ReadTimeout, U32 WriteTimeout);
USBH_FT232_HANDLE USBH_FT232_Open                     (unsigned Index);
USBH_STATUS       USBH_FT232_Close                    (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_GetDeviceInfo            (USBH_FT232_HANDLE hDevice, USBH_FT232_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_FT232_ResetDevice              (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetTimeouts              (USBH_FT232_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS       USBH_FT232_SetBaudRate              (USBH_FT232_HANDLE hDevice, U32 BaudRate);
USBH_STATUS       USBH_FT232_Read                     (USBH_FT232_HANDLE hDevice,       U8 * pData, U32 NumBytes, U32 * pNumBytesRead);
USBH_STATUS       USBH_FT232_Write                    (USBH_FT232_HANDLE hDevice, const U8 * pData, U32 NumBytes, U32 * pNumBytesWritten);
USBH_STATUS       USBH_FT232_AllowShortRead           (USBH_FT232_HANDLE hDevice, U8 AllowShortRead);
USBH_STATUS       USBH_FT232_SetDataCharacteristics   (USBH_FT232_HANDLE hDevice, U8  Length, U8 StopBits,  U8 Parity);
USBH_STATUS       USBH_FT232_SetFlowControl           (USBH_FT232_HANDLE hDevice, U16 FlowControl, U8 XonChar,  U8 XoffChar);
USBH_STATUS       USBH_FT232_SetDtr                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_ClrDtr                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetRts                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_ClrRts                   (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_GetModemStatus           (USBH_FT232_HANDLE hDevice, U32 * pModemStatus);
USBH_STATUS       USBH_FT232_SetChars                 (USBH_FT232_HANDLE hDevice, U8 EventChar, U8 EventCharEnabled,  U8 ErrorChar, U8 ErrorCharEnabled);
USBH_STATUS       USBH_FT232_Purge                    (USBH_FT232_HANDLE hDevice, U32 Mask);
USBH_STATUS       USBH_FT232_GetQueueStatus           (USBH_FT232_HANDLE hDevice, U32 * pRxBytes);

USBH_INTERFACE_HANDLE USBH_FT232_GetInterfaceHandle   (USBH_FT232_HANDLE hDevice);

USBH_STATUS       USBH_FT232_SetBreakOn               (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetBreakOff              (USBH_FT232_HANDLE hDevice);
USBH_STATUS       USBH_FT232_SetLatencyTimer          (USBH_FT232_HANDLE hDevice, U8   Latency);
USBH_STATUS       USBH_FT232_GetLatencyTimer          (USBH_FT232_HANDLE hDevice, U8 * pLatency);
USBH_STATUS       USBH_FT232_SetBitMode               (USBH_FT232_HANDLE hDevice, U8   Mask, U8 Enable);
USBH_STATUS       USBH_FT232_GetBitMode               (USBH_FT232_HANDLE hDevice, U8 * pMode);

#if defined(__cplusplus)
  }
#endif

#endif // USBH_FT232_H_

/*************************** End of file ****************************/
