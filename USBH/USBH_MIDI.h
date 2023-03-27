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
*       emUSB-Host version: V2.36.1                                  *
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
License model:            Cypress Services and License Agreement, signed November 17th/18th, 2010
                          and Amendment Number One, signed December 28th, 2020 and February 10th, 2021
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : USBH_MIDI.h
Purpose     : MIDI class driver.
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_MIDI_H
#define USBH_MIDI_H

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

#define USBH_MIDI_INVALID_HANDLE                   NULL

/*********************************************************************
*
*       USBH_MIDI_HANDLE
*/

typedef struct _USBH_MIDI_INST * USBH_MIDI_HANDLE;

/*********************************************************************
*
*       USBH_MIDI_DEVICE_INFO
*
*  Description
*    Structure containing information about a MIDI device.
*/
typedef struct {
  U16                   VendorId;        // Vendor ID of the device.
  U16                   ProductId;       // Product ID of the device.
  unsigned              DevIndex;        // Device index.
  U16                   bcdDevice;       // BCD-coded device version.
  USBH_SPEED            Speed;           // USB speed of the device, see USBH_SPEED.
  USBH_INTERFACE_ID     InterfaceId;     // USBH interface ID.
  unsigned              NumInCables;     // Number of MIDI IN cables
  unsigned              NumOutCables;    // Number of MIDI OUT cables
} USBH_MIDI_DEVICE_INFO;

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Component management
*/
U8                USBH_MIDI_Init                     (void);
void              USBH_MIDI_Exit                     (void);

/*********************************************************************
*
*       Device management
*/
USBH_STATUS       USBH_MIDI_AddNotification          (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_MIDI_RemoveNotification       (const USBH_NOTIFICATION_HOOK * pHook);
void              USBH_MIDI_ConfigureDefaultTimeout  (U32 RdTimeout, U32 WrTimeout);
//
USBH_MIDI_HANDLE  USBH_MIDI_Open                     (unsigned Index);
USBH_STATUS       USBH_MIDI_Close                    (USBH_MIDI_HANDLE hDevice);
USBH_STATUS       USBH_MIDI_GetDeviceInfo            (USBH_MIDI_HANDLE hDevice, USBH_MIDI_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_MIDI_GetStatus                (USBH_MIDI_HANDLE hDevice);
USBH_STATUS       USBH_MIDI_SetTimeouts              (USBH_MIDI_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS       USBH_MIDI_SetBuffer                (USBH_MIDI_HANDLE hDevice, U8 *pBuffer, unsigned BufferLen);
int               USBH_MIDI_GetIndex                 (USBH_INTERFACE_ID InterfaceID);

/*********************************************************************
*
*       Device I/O
*/
USBH_STATUS       USBH_MIDI_RdEvent                  (USBH_MIDI_HANDLE hDevice, U32 *pEvent);
USBH_STATUS       USBH_MIDI_RdData                   (USBH_MIDI_HANDLE hDevice, U8 *pData, U32 DataLen, U32 *pRdDataLen);
USBH_STATUS       USBH_MIDI_WrEvent                  (USBH_MIDI_HANDLE hDevice, U32 Event);
USBH_STATUS       USBH_MIDI_WrData                   (USBH_MIDI_HANDLE hDevice, const U8 *pData, U32 DataLen, U32 *pWrDataLen);
USBH_STATUS       USBH_MIDI_Send                     (USBH_MIDI_HANDLE hDevice);
USBH_STATUS       USBH_MIDI_GetQueueStatus           (USBH_MIDI_HANDLE hDevice, U32 *pRxBytes);

#if defined(__cplusplus)
}
#endif

#endif // USBH_MIDI_H

/*************************** End of file ****************************/
