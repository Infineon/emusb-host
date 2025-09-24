/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2025     SEGGER Microcontroller GmbH              *
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
*       emUSB-Host version: V2.48.0                                  *
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
the emUSB Host software for its Cortex M0, M0+, M4, M33 and M55 based devices.
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
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022 and Amendment Number Four, signed August 28th, 2023 
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4, M33 and M55
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2026-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : USBH_BULK.h
Purpose     : API of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_BULK_H
#define USBH_BULK_H

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       USBH_BULK_HANDLE
*/

#define USBH_BULK_INVALID_HANDLE             0u

typedef U32 USBH_BULK_HANDLE;

/*********************************************************************
*
*       USBH_BULK_EP_INFO
*
*  Description
*    Structure containing information about an endpoint.
*/
typedef struct {
  U8        Addr;                 // Endpoint Address.
  U8        Type;                 // Endpoint Type (see USB_EP_TYPE_... macros).
  U8        Direction;            // Endpoint direction (see USB_..._DIRECTION macros).
  U16       MaxPacketSize;        // Maximum packet size for the endpoint.
} USBH_BULK_EP_INFO;

/*********************************************************************
*
*       USBH_BULK_DEVICE_INFO
*
*  Description
*    Structure containing information about a BULK device.
*/
typedef struct {
  U16               VendorId;             // The Vendor ID of the device.
  U16               ProductId;            // The Product ID of the device.
  U8                Class;                // The interface class.
  U8                SubClass;             // The interface sub class.
  U8                Protocol;             // The interface protocol.
  U8                AlternateSetting;     // The current alternate setting
  USBH_SPEED        Speed;                // The USB speed of the device, see USBH_SPEED.
  U8                InterfaceNo;          // Index of the interface (from USB descriptor).
  U8                NumEPs;               // Number of endpoints.
  USBH_BULK_EP_INFO EndpointInfo[USBH_BULK_MAX_NUM_EPS]; // Obsolete. See USBH_BULK_GetEndpointInfo().
  USBH_DEVICE_ID    DeviceId;             // The unique device Id. This Id is assigned if the USB
                                          // device was successfully enumerated. It is valid until the
                                          // device is removed from the host. If the device is reconnected
                                          // a different device Id is assigned.
  USBH_INTERFACE_ID InterfaceID;          // Interface ID of the device.
} USBH_BULK_DEVICE_INFO;

/*********************************************************************
*
*       USBH_BULK_RW_CONTEXT
*
*  Description
*    Contains information about a completed, asynchronous transfers.
*    Is passed to the USBH_BULK_ON_COMPLETE_FUNC user
*    callback when using asynchronous write and read.
*    When this structure is passed to USBH_BULK_ReadAsync() or USBH_BULK_WriteAsync()
*    its member need not to be initialized.
*/
typedef struct {
  void        * pUserContext;           // Pointer to a user context. Can be arbitrarily used by the application.
  USBH_STATUS   Status;                 // Result status of the asynchronous transfer.
  I8            Terminated;             // * 1: Operation is terminated.
                                        // * 0: More data may be transfered and callback function may be called again
                                        // (ISO transfers only).
  U32           NumBytesTransferred;    // Number of bytes transferred.
  void        * pUserBuffer;            // For BULK and INT transfers:
                                        // Pointer to the buffer provided to USBH_BULK_ReadAsync() or USBH_BULK_WriteAsync().
                                        // For ISO IN transfers: Pointer to to data read.
  U32           UserBufferSize;         // For BULK and INT transfers:
                                        // Size of the buffer as provided to USBH_BULK_ReadAsync() or USBH_BULK_WriteAsync().
                                        // Not used For ISO transfers.
} USBH_BULK_RW_CONTEXT;

/*********************************************************************
*
*       USBH_BULK_ON_COMPLETE_FUNC
*
*  Description
*    Function called on completion of an asynchronous transfer.
*    Used by the functions USBH_BULK_ReadAsync() and USBH_BULK_WriteAsync().
*
*  Parameters
*    pRWContext : Pointer to a USBH_BULK_RW_CONTEXT structure.
*/
typedef void USBH_BULK_ON_COMPLETE_FUNC(USBH_BULK_RW_CONTEXT * pRWContext);

USBH_STATUS       USBH_BULK_Init                    (const USBH_INTERFACE_MASK * pInterfaceMask);
void              USBH_BULK_Exit                    (void);
void              USBH_BULK_RegisterNotification    (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_BULK_AddNotification         (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext, const USBH_INTERFACE_MASK * pInterfaceMask);
USBH_STATUS       USBH_BULK_RemoveNotification      (const USBH_NOTIFICATION_HOOK * pHook);
USBH_BULK_HANDLE  USBH_BULK_Open                    (unsigned Index);
USBH_STATUS       USBH_BULK_Close                   (USBH_BULK_HANDLE hDevice);

USBH_STATUS       USBH_BULK_AllowShortRead          (USBH_BULK_HANDLE hDevice, U8 AllowShortRead);
USBH_STATUS       USBH_BULK_GetDeviceInfo           (USBH_BULK_HANDLE hDevice, USBH_BULK_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_BULK_GetEndpointInfo         (USBH_BULK_HANDLE hDevice, unsigned EPIndex, USBH_BULK_EP_INFO * pEPInfo);
USBH_STATUS       USBH_BULK_Read                    (USBH_BULK_HANDLE hDevice, U8 EPAddr, U8 * pData, U32 NumBytes, U32 * pNumBytesRead, U32 Timeout);
USBH_STATUS       USBH_BULK_Receive                 (USBH_BULK_HANDLE hDevice, U8 EPAddr, U8 * pData, U32 * pNumBytesRead, U32 Timeout);
USBH_STATUS       USBH_BULK_Write                   (USBH_BULK_HANDLE hDevice, U8 EPAddr, const U8 * pData, U32 NumBytes, U32 * pNumBytesWritten, U32 Timeout);
USBH_STATUS       USBH_BULK_GetNumBytesInBuffer     (USBH_BULK_HANDLE hDevice, U8 EPAddr, U32 * pRxBytes);
USBH_STATUS       USBH_BULK_GetSerialNumber         (USBH_BULK_HANDLE hDevice, U32 BuffSize, U8 *pSerialNumber, U32 *pSerialNumberSize);
int               USBH_BULK_GetIndex                (USBH_INTERFACE_ID InterfaceID);

USBH_INTERFACE_HANDLE USBH_BULK_GetInterfaceHandle  (USBH_BULK_HANDLE hDevice);

USBH_STATUS       USBH_BULK_GetMaxTransferSize      (USBH_BULK_HANDLE hDevice, U8 EPAddr, U32 * pMaxTransferSize);
USBH_STATUS       USBH_BULK_ReadAsync               (USBH_BULK_HANDLE hDevice, U8 EPAddr, void * pBuffer, U32 BufferSize, USBH_BULK_ON_COMPLETE_FUNC * pfOnComplete, USBH_BULK_RW_CONTEXT * pRWContext);
USBH_STATUS       USBH_BULK_WriteAsync              (USBH_BULK_HANDLE hDevice, U8 EPAddr, void * pBuffer, U32 BufferSize, USBH_BULK_ON_COMPLETE_FUNC * pfOnComplete, USBH_BULK_RW_CONTEXT * pRWContext);
USBH_STATUS       USBH_BULK_Cancel                  (USBH_BULK_HANDLE hDevice, U8 EPAddr);
USBH_STATUS       USBH_BULK_SetupRequest            (USBH_BULK_HANDLE hDevice, U8 RequestType, U8 Request, U16 wValue, U16 wIndex, void * pData, U32 * pNumBytesData, U32 Timeout);
USBH_STATUS       USBH_BULK_SetAlternateInterface   (USBH_BULK_HANDLE hDevice, U8 AltInterfaceSetting);
USBH_STATUS       USBH_BULK_IsoDataCtrl             (USBH_BULK_HANDLE hDevice, U8 EPAddr, USBH_ISO_DATA_CTRL *pIsoData);

#if defined(__cplusplus)
  }
#endif

#endif // USBH_BULK_H

/*************************** End of file ****************************/
