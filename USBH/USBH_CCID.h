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
File        : USBH_CCID.h
Purpose     : Smart card interface class of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_CCID_H
#define USBH_CCID_H

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

#define USBH_CCID_INVALID_HANDLE              NULL

#define USBH_CCID_PROTOCOL_T0                 0x01u
#define USBH_CCID_PROTOCOL_T1                 0x02u

#define USBH_CCID_EXLVL_TPDU                  0x01u
#define USBH_CCID_EXLVL_APDU                  0x02u
#define USBH_CCID_EXLVL_APDU_EX               0x04u

/*********************************************************************
*
*       USBH_CCID_HANDLE
*/

typedef struct _USBH_CCID_INST * USBH_CCID_HANDLE;

/*********************************************************************
*
*       USBH_CCID_DEVICE_INFO
*
*  Description
*    Structure containing information about a CCID device.
*/
typedef struct {
  U16         VendorId;             // The Vendor ID of the device.
  U16         ProductId;            // The Product ID of the device.
  USBH_SPEED  Speed;                // The USB speed of the device, see USBH_SPEED.
  U8          InterfaceNo;          // Interface index (from USB descriptor).
  U8          Class;                // The Class value field of the interface
  U8          SubClass;             // The SubClass value field of the interface
  U8          Protocol;             // The Protocol value field of the interface
  U8          ICCProtocols;         // Supported ICC protocols (bit field), see USBH_CCID_PROTOCOL_... macros
  U8          ExchangeLevel;        // Host exchange level, see USBH_CCID_EXLVL_... macros
  USBH_INTERFACE_ID InterfaceID;    // ID of the interface.
} USBH_CCID_DEVICE_INFO;

/*********************************************************************
*
*       USBH_CCID_CMD_PARA
*
*  Description
*    Structure describing all parameters to issue a command to the CCID device.
*/
typedef struct {
  U8         CmdType;         // [IN] CCID message type.
  U8         RespType;        // [IN] Response message type.
  U8         Slot;            // [IN] Slot index (counting from 0).
  U8         Param[3];        // [IN] Message specific parameters.
  U32        LenData;         // [IN] Length of command data in bytes.
  const U8 * pData;           // [IN] Pointer to the command data.
  U32        BuffSize;        // [IN] Size of buffer pointed to by pAnswer.
  U8       * pAnswer;         // [IN] Pointer to buffer that receives the answer data.
  U32        LenAnswer;       // [OUT] Length of answer data stored into pAnswer. May be truncated to BuffSize.
  U32        LenAnswerOrig;   // [OUT] Length of answer data received from the CCID. May be greater than BuffSize.
  U8         bStatus;         // [OUT] Status code returned by the CCID device.
  U8         bError;          // [OUT] Error code returned by the CCID device.
  U8         bInfo;           // [OUT] Additional info returned by the CCID device.
} USBH_CCID_CMD_PARA;

/*********************************************************************
*
*       USBH_CCID_SLOT_CHANGE_CALLBACK
*
*  Description
*    Function called on a reception of a slot change event.
*    Used by the function USBH_CCID_SetOnSlotChange().
*
*  Parameters
*    pContext  : Pointer to the user-provided user context.
*    SlotState : Bit mask of slot states. Bit n indicates state of slot n (n = 0 ... NumSlots-1).
*                If a smart card is present in the slot, the respective bit has a value of 1.
*/
typedef void USBH_CCID_SLOT_CHANGE_CALLBACK(void *pContext, U32 SlotState);


USBH_STATUS       USBH_CCID_Init                     (void);
void              USBH_CCID_Exit                     (void);
USBH_STATUS       USBH_CCID_AddNotification          (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS       USBH_CCID_RemoveNotification       (const USBH_NOTIFICATION_HOOK * pHook);
USBH_CCID_HANDLE  USBH_CCID_Open                     (unsigned Index);
void              USBH_CCID_Close                    (USBH_CCID_HANDLE hDevice);
USBH_STATUS       USBH_CCID_SetOnSlotChange          (USBH_CCID_HANDLE hDevice, USBH_CCID_SLOT_CHANGE_CALLBACK * pfOnSlotChange, void * pUserContext);
void              USBH_CCID_SetTimeout               (USBH_CCID_HANDLE hDevice, U32 Timeout);

USBH_STATUS       USBH_CCID_GetDeviceInfo            (USBH_CCID_HANDLE hDevice, USBH_CCID_DEVICE_INFO * pDevInfo);
USBH_STATUS       USBH_CCID_GetSerialNumber          (USBH_CCID_HANDLE hDevice, U32 BuffSize, U8 *pSerialNumber, U32 *pSerialNumberSize);
USBH_STATUS       USBH_CCID_GetNumSlots              (USBH_CCID_HANDLE hDevice, unsigned *pNumSlots);
USBH_STATUS       USBH_CCID_GetCSDesc                (USBH_CCID_HANDLE hDevice, U8 * pData, unsigned * pNumBytesData);
U16               USBH_CCID_GetLastDeviceStatus      (USBH_CCID_HANDLE hDevice);

USBH_INTERFACE_HANDLE USBH_CCID_GetInterfaceHandle   (USBH_CCID_HANDLE hDevice);

USBH_STATUS       USBH_CCID_Cmd                      (USBH_CCID_HANDLE hDevice, USBH_CCID_CMD_PARA *pCmd);
USBH_STATUS       USBH_CCID_PowerOn                  (USBH_CCID_HANDLE hDevice, U8 Slot, U32 BuffSize, U8 *pATR);
USBH_STATUS       USBH_CCID_PowerOnATR               (USBH_CCID_HANDLE hDevice, U8 Slot, U32 *pRespLen, U8 *pATR);
USBH_STATUS       USBH_CCID_PowerOff                 (USBH_CCID_HANDLE hDevice, U8 Slot);
USBH_STATUS       USBH_CCID_GetSlotStatus            (USBH_CCID_HANDLE hDevice, U8 Slot, U16 *pSlotStatus);
USBH_STATUS       USBH_CCID_APDU                     (USBH_CCID_HANDLE hDevice, U8 Slot, U32 CmdLen, const U8 *pCmd, U32 *pRespLen, U8 *pResp);
USBH_STATUS       USBH_CCID_GetResponse              (USBH_CCID_HANDLE hDevice, U8 Slot, U8 CLA, U8 Le, U32 *pRespLen, U8 *pResp);
USBH_STATUS       USBH_CCID_GetParameters            (USBH_CCID_HANDLE hDevice, U8 Slot, U32 *pParamLen, U8 *pParams, U8 *pProt);
USBH_STATUS       USBH_CCID_ResetParameters          (USBH_CCID_HANDLE hDevice, U8 Slot, U32 *pParamLen, U8 *pParams, U8 *pProt);
USBH_STATUS       USBH_CCID_SetParameters            (USBH_CCID_HANDLE hDevice, U8 Slot, U32 ParamLen, const U8 *pParams, U8 Prot);
USBH_STATUS       USBH_CCID_Abort                    (USBH_CCID_HANDLE hDevice, U8 Slot);
USBH_STATUS       USBH_CCID_GetClockFrequencies      (USBH_CCID_HANDLE hDevice, U32 *pBuff, U32 *pCount);
USBH_STATUS       USBH_CCID_GetDataRates             (USBH_CCID_HANDLE hDevice, U32 *pBuff, U32 *pCount);

USBH_STATUS       USBH_CCID_Read                     (USBH_CCID_HANDLE hDevice,       U8 * pData,   U32 NumBytes, U32 * pNumBytesRead, U32 Timeout);
USBH_STATUS       USBH_CCID_Write                    (USBH_CCID_HANDLE hDevice, const U8 * pData,   U32 NumBytes, U32 Timeout);
USBH_STATUS       USBH_CCID_GetNumBytesInBuffer      (USBH_CCID_HANDLE hDevice, U32 * pRxBytes);


#if defined(__cplusplus)
  }
#endif

#endif // USBH_CCID_H

/*************************** End of file ****************************/
