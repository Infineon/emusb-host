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
File        : USBH_CDC.h
Purpose     : API of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_CDC_H
#define USBH_CDC_H

#include "USBH.h"
#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

//
// Baud Rates
//
#define USBH_CDC_BAUD_300         300
#define USBH_CDC_BAUD_600         600
#define USBH_CDC_BAUD_1200       1200
#define USBH_CDC_BAUD_2400       2400
#define USBH_CDC_BAUD_4800       4800
#define USBH_CDC_BAUD_9600       9600
#define USBH_CDC_BAUD_14400     14400
#define USBH_CDC_BAUD_19200     19200
#define USBH_CDC_BAUD_38400     38400
#define USBH_CDC_BAUD_57600     57600
#define USBH_CDC_BAUD_115200   115200
#define USBH_CDC_BAUD_230400   230400
#define USBH_CDC_BAUD_460800   460800
#define USBH_CDC_BAUD_921600   921600

//
// Word Lengths
//
#define USBH_CDC_BITS_8        (U8)8
#define USBH_CDC_BITS_7        (U8)7
#define USBH_CDC_BITS_6        (U8)6
#define USBH_CDC_BITS_5        (U8)5

//
// Stop Bits
//
#define USBH_CDC_STOP_BITS_1   (U8)0
#define USBH_CDC_STOP_BITS_1_5 (U8)1
#define USBH_CDC_STOP_BITS_2   (U8)2

//
// Parity
//
#define USBH_CDC_PARITY_NONE   (U8)0
#define USBH_CDC_PARITY_ODD    (U8)1
#define USBH_CDC_PARITY_EVEN   (U8)2
#define USBH_CDC_PARITY_MARK   (U8)3
#define USBH_CDC_PARITY_SPACE  (U8)4

//
// Flow Control
//
#define USBH_CDC_FLOW_NONE     0x0000
#define USBH_CDC_FLOW_RTS_CTS  0x0100
#define USBH_CDC_FLOW_DTR_DSR  0x0200
#define USBH_CDC_FLOW_XON_XOFF 0x0400

//
// Setup Flags for CDC
//
#define USBH_CDC_IGNORE_INT_EP           (1uL << 0)          // Ignore, that an Interrupt EP is available. It can also be used where no Interrupt EP is available.
#define USBH_CDC_DISABLE_INTERFACE_CHECK (1uL << 1)          // Disable, that the data interface no. in the ACM interface descriptor is not valid and though is not checked.


// CDC specific descriptor types
#define USBH_CDC_CS_INTERFACE_DESCRIPTOR_TYPE          0x24u
#define USBH_CDC_CS_ENDPOINT_DESCRIPTOR_TYPE           0x25u


// CDC specific descriptor sub types
#define USBH_CDC_DESC_SUBTYPE_HEADER                   0x00u
#define USBH_CDC_DESC_SUBTYPE_CALL_MANAGEMENT          0x01u // Call Management Functional Descriptor
#define USBH_CDC_DESC_SUBTYPE_ACM                      0x02u // Abstract Control Management Functional Descriptor
#define USBH_CDC_DESC_SUBTYPE_UNION_FUCTIONAL          0x06u // Union Functional Descriptor
#define USBH_CDC_DESC_SUBTYPE_ETH_NET_FUNC_DESC        0x0Fu // Ethernet Networking Functional Descriptor

#define USBH_CDC_ABSTRACT_LINE_CONTROL_MODEL           0x02u
#define USBH_CDC_COMMON_AT_COMMANDS                    0x01u

#define USBH_CDC_REQ_SEND_ENCAPSULATED_COMMAND         0x00u
#define USBH_CDC_REQ_GET_ENCAPSULATED_RESPONSE         0x01u
#define USBH_CDC_REQ_SET_LINE_CODING                   0x20u
#define USBH_CDC_REQ_GET_LINE_CODING                   0x21u
#define USBH_CDC_REQ_SET_CONTROL_LINE_STATE            0x22u
#define USBH_CDC_REQ_SEND_BREAK                        0x23u

#define USBH_CDC_SET_LINE_CODING_LEN                   0x07u
#define USBH_CDC_GET_LINE_CODING_LEN                   0x07u

#define USBH_CDC_DTR_BIT                               0x00u
#define USBH_CDC_RTS_BIT                               0x01u

#define USBH_CDC_NOTIFICATION_REQUEST                  0xA1u

#define USBH_CDC_NOTIFICATION_TYPE_NETWORK_CONNECTION  0x00u
#define USBH_CDC_NOTIFICATION_TYPE_RESPONSE_AVAILABLE  0x01u
#define USBH_CDC_NOTIFICATION_TYPE_AUX_JACK_HOOK_STATE 0x08u
#define USBH_CDC_NOTIFICATION_TYPE_RING_DETECT         0x09u
#define USBH_CDC_NOTIFICATION_TYPE_SERIAL_STATE        0x20u
#define USBH_CDC_NOTIFICATION_TYPE_CALL_STATE_CHANGE   0x28u
#define USBH_CDC_NOTIFICATION_TYPE_LINE_STATE_CHANGE   0x23u

#define USBH_CDC_INVALID_HANDLE                        0u

/*********************************************************************
*
*       USBH_CDC_HANDLE
*/

typedef U32 USBH_CDC_HANDLE;

/*********************************************************************
*
*       USBH_CDC_DEVICE_INFO
*
*  Description
*    Structure containing information about a CDC device.
*/
typedef struct {
  U16         VendorId;             // The Vendor ID of the device.
  U16         ProductId;            // The Product ID of the device.
  USBH_SPEED  Speed;                // The USB speed of the device, see USBH_SPEED.
  U8          ControlInterfaceNo;   // Interface index of the ACM Control interface (from USB descriptor).
  U8          DataInterfaceNo;      // Interface index of the ACM Data interface (from USB descriptor).
  U16         MaxPacketSize;        // Maximum packet size of the device, usually 64 in full-speed
                                    // and 512 in high-speed.
  U16         ControlClass;         // The Class value field of the control interface
  U16         ControlSubClass;      // The SubClass value field of the control interface
  U16         ControlProtocol;      // The Protocol value field of the control interface
  U16         DataClass;            // The Class value field of the data interface
  U16         DataSubClass;         // The SubClass value field of the data interface
  U16         DataProtocol;         // The Protocol value field of the data interface
  USBH_INTERFACE_ID ControlInterfaceID;  // ID of the ACM control interface.
  USBH_INTERFACE_ID DataInterfaceID;     // ID of the ACM data interface.
} USBH_CDC_DEVICE_INFO;

/*********************************************************************
*
*       USBH_CDC_SERIALSTATE
*
*  Description
*    Structure describing the serial state of CDC device.
*    All members can have a value of 0 (= false/off) or 1 (= true/on).
*/
typedef struct {
  U8 bRxCarrier;  // State of receiver carrier detection mechanism of device. This signal
                  // corresponds to V.24 signal 109 and RS-232 signal DCD.
  U8 bTxCarrier;  // State of transmission carrier. This signal corresponds to V.24 signal 106 and
                  // RS-232 signal DSR.
  U8 bBreak;      // State of break detection mechanism of the device.
  U8 bRingSignal; // State of ring signal detection of the device.
  U8 bFraming;    // A framing error has occurred.
  U8 bParity;     // A parity error has occurred.
  U8 bOverRun;    // Received data has been discarded due to overrun in the device.
} USBH_CDC_SERIALSTATE;

/*********************************************************************
*
*       USBH_CDC_RW_CONTEXT
*
*  Description
*    Contains information about a completed, asynchronous transfers.
*    Is passed to the USBH_CDC_ON_COMPLETE_FUNC user
*    callback when using asynchronous write and read.
*    When this structure is passed to USBH_CDC_ReadAsync() or USBH_CDC_WriteAsync()
*    its member need not to be initialized.
*/
typedef struct {
  void        * pUserContext;           // Pointer to a user context. Can be arbitrarily used by the application.
  USBH_STATUS   Status;                 // Result status of the asynchronous transfer.
  U32           NumBytesTransferred;    // Number of bytes transferred.
  void        * pUserBuffer;            // Pointer to the buffer provided to USBH_CDC_ReadAsync() or USBH_CDC_WriteAsync().
  U32           UserBufferSize;         // Size of the buffer as provided to USBH_CDC_ReadAsync() or USBH_CDC_WriteAsync().
} USBH_CDC_RW_CONTEXT;

/*********************************************************************
*
*       USBH_CDC_ON_COMPLETE_FUNC
*
*  Description
*    Function called on completion of an asynchronous transfer.
*    Used by the functions USBH_CDC_ReadAsync() and USBH_CDC_WriteAsync().
*
*  Parameters
*    pRWContext : Pointer to a USBH_CDC_RW_CONTEXT structure.
*/
typedef void USBH_CDC_ON_COMPLETE_FUNC(USBH_CDC_RW_CONTEXT * pRWContext);

/*********************************************************************
*
*       USBH_CDC_SERIAL_STATE_CALLBACK
*
*  Description
*    Function called on a reception of a CDC ACM serial state change.
*    Used by the function USBH_CDC_SetOnSerialStateChange().
*
*  Parameters
*    hDevice      : Handle to an open device returned by USBH_CDC_Open().
*    pSerialState : Pointer to a structure of type USBH_CDC_SERIALSTATE showing the
*                   serial status from the device.
*/
typedef void USBH_CDC_SERIAL_STATE_CALLBACK(USBH_CDC_HANDLE hDevice, USBH_CDC_SERIALSTATE * pSerialState);

/*********************************************************************
*
*       USBH_CDC_INT_STATE_CALLBACK
*
*  Description
*    Function called on a reception of a CDC control interface state.
*    Used by the function USBH_CDC_SetOnIntStateChange().
*
*  Parameters
*    hDevice      : Handle to an open device returned by USBH_CDC_Open().
*    pSerialState : Pointer to the received data.
*    NumBytes     : The number of bytes that have been received..
*    pUserContext : Pointer to the user-provided user context.
*/
typedef void USBH_CDC_INT_STATE_CALLBACK(USBH_CDC_HANDLE hDevice, const U8 * pData, U32 Numbytes, void * pUserContext);

U8                USBH_CDC_Init                     (void);
void              USBH_CDC_Exit                     (void);
void              USBH_CDC_RegisterNotification     (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_CDC_AddNotification          (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_CDC_RemoveNotification       (const USBH_NOTIFICATION_HOOK * pHook);
void              USBH_CDC_ConfigureDefaultTimeout  (U32 ReadTimeout, U32 WriteTimeout);
USBH_CDC_HANDLE   USBH_CDC_Open                     (unsigned Index);
USBH_STATUS       USBH_CDC_Close                    (USBH_CDC_HANDLE hDevice);
int               USBH_CDC_GetIndex                 (USBH_INTERFACE_ID InterfaceID);

USBH_INTERFACE_HANDLE USBH_CDC_GetInterfaceHandle   (USBH_CDC_HANDLE hDevice);

USBH_STATUS       USBH_CDC_AllowShortRead           (USBH_CDC_HANDLE hDevice, U8 AllowShortRead);
USBH_STATUS       USBH_CDC_GetDeviceInfo            (USBH_CDC_HANDLE hDevice, USBH_CDC_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_CDC_Read                     (USBH_CDC_HANDLE hDevice,       U8 * pData,   U32 NumBytes,   U32 * pNumBytesRead);
USBH_STATUS       USBH_CDC_Write                    (USBH_CDC_HANDLE hDevice, const U8 * pData,   U32 NumBytes,   U32 * pNumBytesWritten);
USBH_STATUS       USBH_CDC_SetBreak                 (USBH_CDC_HANDLE hDevice, U16 Duration);
USBH_STATUS       USBH_CDC_SetBreakOn               (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetBreakOff              (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetCommParas             (USBH_CDC_HANDLE hDevice, U32 Baudrate,    U8 DataBits,   U8 StopBits,  U8 Parity);
USBH_STATUS       USBH_CDC_SetTimeouts              (USBH_CDC_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS       USBH_CDC_SetDtr                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_ClrDtr                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_SetRts                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_ClrRts                   (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_GetQueueStatus           (USBH_CDC_HANDLE hDevice, U32 * pRxBytes);
void              USBH_CDC_FlushBuffer              (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_GetSerialState           (USBH_CDC_HANDLE hDevice, USBH_CDC_SERIALSTATE * pSerialState);
USBH_STATUS       USBH_CDC_GetSerialNumber          (USBH_CDC_HANDLE hDevice, U32 BuffSize, U8 *pSerialNumber, U32 *pSerialNumberSize);
USBH_STATUS       USBH_CDC_SetOnSerialStateChange   (USBH_CDC_HANDLE hDevice, USBH_CDC_SERIAL_STATE_CALLBACK * pfOnSerialStateChange);
USBH_STATUS       USBH_CDC_SetOnIntStateChange      (USBH_CDC_HANDLE hDevice, USBH_CDC_INT_STATE_CALLBACK * pfOnIntState, void * pUserContext);

USBH_STATUS       USBH_CDC_GetMaxTransferSize       (USBH_CDC_HANDLE hDevice, U32 * pMaxOutTransferSize, U32 * pMaxInTransferSize);
USBH_STATUS       USBH_CDC_ReadAsync                (USBH_CDC_HANDLE hDevice, void * pBuffer, U32 BufferSize, USBH_CDC_ON_COMPLETE_FUNC * pfOnComplete, USBH_CDC_RW_CONTEXT * pRWContext);
USBH_STATUS       USBH_CDC_WriteAsync               (USBH_CDC_HANDLE hDevice, void * pBuffer, U32 BufferSize, USBH_CDC_ON_COMPLETE_FUNC * pfOnComplete, USBH_CDC_RW_CONTEXT * pRWContext);
USBH_STATUS       USBH_CDC_CancelRead               (USBH_CDC_HANDLE hDevice);
USBH_STATUS       USBH_CDC_CancelWrite              (USBH_CDC_HANDLE hDevice);

USBH_STATUS       USBH_CDC_AddDevice                (USBH_INTERFACE_ID ControlInterfaceID, USBH_INTERFACE_ID DataInterfaceId, unsigned Flags);
USBH_STATUS       USBH_CDC_RemoveDevice             (USBH_INTERFACE_ID ControlInterfaceID, USBH_INTERFACE_ID DataInterfaceId);

void              USBH_CDC_SetConfigFlags           (U32 Flags);
USBH_STATUS       USBH_CDC_SendEncapsulatedCommand  (USBH_CDC_HANDLE hDevice, U8 * pBuffer, U32 * pNumBytes);
USBH_STATUS       USBH_CDC_GetEncapsulatedResponse  (USBH_CDC_HANDLE hDevice, U8 * pBuffer, U32 * pNumBytes);

USBH_STATUS       USBH_CDC_SetupRequest             (USBH_CDC_HANDLE hDevice, U8 RequestType, U8 Request, U16 wValue, U16 wIndex, void * pData, U32 * pNumBytesData);
USBH_STATUS       USBH_CDC_GetCSDesc                (USBH_CDC_HANDLE hDevice, U8 DescType, U8 DescSubType, void * pData, U32 * pNumBytesData);
USBH_STATUS       USBH_CDC_GetStringDesc            (USBH_CDC_HANDLE hDevice, U8 StringIndex, U8 * pBuffer, U32 * pNumBytesData);
USBH_STATUS       USBH_CDC_SetDataCommunication     (USBH_CDC_HANDLE hDevice, unsigned OnOff);
USBH_STATUS       USBH_CDC_SuspendResume            (USBH_CDC_HANDLE hDevice, U8 State);
#if defined(__cplusplus)
  }
#endif

#endif // USBH_CDC_H

/*************************** End of file ****************************/
