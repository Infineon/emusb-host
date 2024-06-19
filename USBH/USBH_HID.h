/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2024     SEGGER Microcontroller GmbH              *
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
*       emUSB-Host version: V2.40.0                                  *
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
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : USBH_HID.h
Purpose     : API of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_HID_H__
#define USBH_HID_H__

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

//
// Device types (plug-in that handles the device).
//
#define USBH_HID_VENDOR           0u
#define USBH_HID_MOUSE            (1u << 0)
#define USBH_HID_KEYBOARD         (1u << 1)
#define USBH_HID_TOUCHPAD         (1u << 2)
#define USBH_HID_RC               (1u << 3)
#define USBH_HID_FT260            (1u << 4)

#define USBH_HID_INVALID_HANDLE           0u

//
// Flags for function USBH_HID_SetReportEx() and USBH_HID_GetReportEP0().
//
#define USBH_HID_USE_REPORT_ID      (1uL << 0)
#define USBH_HID_OUTPUT_REPORT      0uL
#define USBH_HID_INPUT_REPORT       0uL
#define USBH_HID_FEATURE_REPORT     (1uL << 1)

/*********************************************************************
*
*       USBH_HID_REPORT_INFO
*
*  Description
*    Structure containing information about a HID report.
*/
typedef struct {
  U8   ReportId;                  // Report Id
  U16  InputReportSize;           // Size of input report in bytes.
  U16  OutputReportSize;          // Size of output report in bytes.
} USBH_HID_REPORT_INFO;

/*********************************************************************
*
*       USBH_HID_DEVICE_INFO
*
*  Description
*    Structure containing information about a HID device.
*/
typedef struct {
  U16  InputReportSize;           // Deprecated. = ReportInfo[0].InputReportSize for compatibility.
  U16  OutputReportSize;          // Deprecated. = ReportInfo[0].OutputReportSize for compatibility.
  U16  ProductId;                 // The Product ID of the device.
  U16  VendorId;                  // The Vendor ID of the device.
  unsigned DevIndex;              // Device index.
  USBH_INTERFACE_ID InterfaceID;  // Interface ID of the HID device.
  unsigned NumReportInfos;        // Number of entries in ReportInfo.
  USBH_HID_REPORT_INFO ReportInfo[USBH_HID_MAX_REPORTS];  // Size and Report Ids of all reports of the interface.

  U8   DeviceType;                // Device type.
                                  // * USBH_HID_VENDOR - Vendor device
                                  // * USBH_HID_MOUSE - Mouse device
                                  // * USBH_HID_KEYBOARD - Keyboard device
  U8   InterfaceNo;               // Index of the interface (from USB descriptor).
} USBH_HID_DEVICE_INFO;

/*********************************************************************
*
*       USBH_HID_RW_CONTEXT
*
*  Description
*    Contains information about a completed, asynchronous transfers.
*    Is passed to the USBH_HID_ON_COMPLETE_FUNC user
*    callback when using asynchronous write and read.
*    When this structure is passed to USBH_HID_GetReport() or USBH_HID_SetReport()
*    its member need not to be initialized.
*/
typedef struct {
  void        * pUserContext;           // Pointer to a user context. Can be arbitrarily used by the application.
  USBH_STATUS   Status;                 // Result status of the asynchronous transfer.
  U32           NumBytesTransferred;    // Number of bytes transferred.
  void        * pUserBuffer;            // Pointer to the buffer provided to USBH_HID_GetReport() or USBH_HID_SetReport().
  U32           UserBufferSize;         // Size of the buffer as provided to USBH_HID_GetReport() or USBH_HID_SetReport().
} USBH_HID_RW_CONTEXT;

/*********************************************************************
*
*       USBH_HID_USER_FUNC
*
*  Description
*    Function called on completion of USBH_HID_GetReport() or USBH_HID_SetReport().
*
*  Parameters
*    pContext : Pointer to a USBH_HID_RW_CONTEXT structure.
*/
typedef void (USBH_HID_USER_FUNC)         (USBH_HID_RW_CONTEXT * pContext);

/*********************************************************************
*
*       USBH_HID_ON_REPORT
*
*  Description
*    Function called on every report.
*
*  Parameters
*    InterfaceID     : Interface ID of the HID device that generated the event.
*    pReport         : Pointer to the report received.
*    ReportSize      : Size of the report in bytes.
*    Handled         : Is != 0, if the report was already handled by on of the other callback functions
*                      (USBH_HID_ON_KEYBOARD_FUNC, USBH_HID_ON_MOUSE_FUNC, USBH_HID_ON_GENERIC_FUNC).
*/
typedef void (USBH_HID_ON_REPORT) (USBH_INTERFACE_ID InterfaceID, const U8 *pReport, unsigned ReportSize, int Handled);

typedef U32 USBH_HID_HANDLE;

void            USBH_HID_Exit                        (void);
U8              USBH_HID_Init                        (void);
void            USBH_HID_SetOnReport                 (USBH_HID_ON_REPORT * pfOnReport);
USBH_STATUS     USBH_HID_Close                       (USBH_HID_HANDLE hDevice);
int             USBH_HID_GetNumDevices               (USBH_HID_DEVICE_INFO * pDevInfo, U32 NumItems);
int             USBH_HID_GetIndex                    (USBH_INTERFACE_ID InterfaceID);
USBH_STATUS     USBH_HID_GetDeviceInfo               (USBH_HID_HANDLE hDevice, USBH_HID_DEVICE_INFO * pDevInfo);
USBH_STATUS     USBH_HID_GetReportDescriptor         (USBH_HID_HANDLE hDevice, U8 * pReportDescriptor, unsigned NumBytes);
USBH_STATUS     USBH_HID_GetReportDesc               (USBH_HID_HANDLE hDevice, const U8 ** ppReportDescriptor, unsigned *pNumBytes);
USBH_STATUS     USBH_HID_GetReport                   (USBH_HID_HANDLE hDevice, U8 * pBuffer, U32 BufferSize, USBH_HID_USER_FUNC * pfFunc, USBH_HID_RW_CONTEXT * pRWContext);
USBH_STATUS     USBH_HID_GetReportEP0                (USBH_HID_HANDLE hDevice, U8 ReportID, unsigned Flags, U8 * pBuffer, U32 Length, U32 * pNumBytesRead);
USBH_HID_HANDLE USBH_HID_Open                        (unsigned Index);
USBH_STATUS     USBH_HID_SetReport                   (USBH_HID_HANDLE hDevice, const U8 * pBuffer, U32 BufferSize, USBH_HID_USER_FUNC * pfFunc, USBH_HID_RW_CONTEXT * pRWContext);
USBH_STATUS     USBH_HID_SetReportEx                 (USBH_HID_HANDLE hDevice, const U8 * pBuffer, U32 BufferSize, USBH_HID_USER_FUNC * pfFunc, USBH_HID_RW_CONTEXT * pRWContext, unsigned Flags);
USBH_STATUS     USBH_HID_CancelIo                    (USBH_HID_HANDLE hDevice);
void            USBH_HID_RegisterNotification        (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS     USBH_HID_AddNotification             (USBH_NOTIFICATION_HOOK * pHook, USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
USBH_STATUS     USBH_HID_RemoveNotification          (const USBH_NOTIFICATION_HOOK * pHook);
void            USBH_HID_ConfigureControlWriteRetries(U8 NumRetries);
void            USBH_HID_ConfigureControlWriteTimeout(U32 Timeout);

USBH_INTERFACE_HANDLE USBH_HID_GetInterfaceHandle    (USBH_HID_HANDLE hDevice);


/*********************************************************************
*
*   PLUGIN: Keyboard
*
**********************************************************************
*/
//
// Keyboard indicators (LEDs).
//
#define USBH_HID_IND_NUM_LOCK     (1 << 0)  // Indicates that Number Lock is enabled.
#define USBH_HID_IND_CAPS_LOCK    (1 << 1)  // Indicates that Capital Lock is enabled.
#define USBH_HID_IND_SCROLL_LOCK  (1 << 2)  // Indicates that Scroll Lock is enabled.
#define USBH_HID_IND_COMPOSE      (1 << 3)  // Indicates that composition mode is enabled.
#define USBH_HID_IND_KANA         (1 << 4)  // Indicates that Kana mode is enabled.
#define USBH_HID_IND_SHIFT        (1 << 5)  // Indicates that the Shift function is enabled.

/*********************************************************************
*
*       USBH_HID_KEYBOARD_DATA
*
*  Description
*    Structure containing information about a keyboard event.
*/
typedef struct {
  unsigned            Code;           // Contains the keycode.
  unsigned            Value;          // Keyboard state info. Refer to sample code for more information.
  USBH_INTERFACE_ID   InterfaceID;    // ID of the interface that caused the event.
} USBH_HID_KEYBOARD_DATA;

/*********************************************************************
*
*       USBH_HID_ON_KEYBOARD_FUNC
*
*  Description
*    Function called on every keyboard event.
*
*  Parameters
*    pKeyData : Pointer to a USBH_HID_KEYBOARD_DATA structure.
*/
typedef void (USBH_HID_ON_KEYBOARD_FUNC)  (USBH_HID_KEYBOARD_DATA * pKeyData);

void            USBH_HID_SetOnKeyboardStateChange    (USBH_HID_ON_KEYBOARD_FUNC * pfOnChange);
void            USBH_HID_SetOnExKeyboardStateChange  (USBH_HID_ON_KEYBOARD_FUNC * pfOnChange);
void            USBH_HID_ConfigureAllowLEDUpdate     (unsigned AllowLEDUpdate);
USBH_STATUS     USBH_HID_SetIndicators               (USBH_HID_HANDLE hDevice, U8 IndicatorMask);
USBH_STATUS     USBH_HID_GetIndicators               (USBH_HID_HANDLE hDevice, U8 * pIndicatorMask);


/*********************************************************************
*
*   PLUGIN: Mouse
*
**********************************************************************
*/

/*********************************************************************
*
*       USBH_HID_MOUSE_DATA
*
*  Description
*    Structure containing information about a mouse event.
*/
typedef struct {
  int                 xChange;       // Change of x-position since last event.
  int                 yChange;       // Change of y-position since last event.
  int                 WheelChange;   // Change of wheel-position since last event (if wheel is present).
  int                 ButtonState;   // Each bit corresponds to one button on the mouse.
                                     // If the bit is set, the corresponding button is pressed. Typically,
                                     // * bit 0 corresponds to the left mouse button
                                     // * bit 1 corresponds to the right mouse button
                                     // * bit 2 corresponds to the middle mouse button.
  USBH_INTERFACE_ID   InterfaceID;   // ID of the interface that caused the event.
} USBH_HID_MOUSE_DATA;

/*********************************************************************
*
*       USBH_HID_ON_MOUSE_FUNC
*
*  Description
*    Function called on every mouse event.
*
*  Parameters
*    pMouseData : Pointer to a USBH_HID_MOUSE_DATA structure.
*/
typedef void (USBH_HID_ON_MOUSE_FUNC)(USBH_HID_MOUSE_DATA * pMouseData);


void          USBH_HID_SetOnMouseStateChange(USBH_HID_ON_MOUSE_FUNC * pfOnChange);


/*********************************************************************
*
*   PLUGIN: Genric Input (Touchpad)
*
**********************************************************************
*/
//
// Special value of USAGE to allow storage of the device type into USBH_HID_GENERIC_DATA.
//
#define USBH_HID_USAGE_DEVICE_TYPE        0x000001u

/*********************************************************************
*
*  Description
*    Union to store either signed or unsigned data.
*/
typedef union {
  U32               u32;           // Unsigned value.
  I32               i32;           // Signed value.
} USBH_ANY_SIGNED;

/*********************************************************************
*
*       USBH_HID_GENERIC_DATA
*
*  Description
*    Structure containing information from a HID report event.
*/
typedef struct {
  U32                 Usage;         // HID usage code. Copied from the array given to
                                     // USBH_HID_SetOnGenericEvent(). Set to 0, if the usage code
                                     // was not found in any report descriptor.
  USBH_ANY_SIGNED     Value;         // Value of the field extracted from the report.
  U8                  Valid;         // = 1 if Value field contains valid value.
  U8                  Signed;        // = 1 if Value is signed, = 0 if unsigned.
  U8                  ReportID;      // ID of the report containing the field.
  USBH_ANY_SIGNED     LogicalMin;    // Logical minimum from report descriptor.
                                     // Contains signed value, if Signed == 1, unsigned value otherwise.
  USBH_ANY_SIGNED     LogicalMax;    // Logical maximum from report descriptor.
                                     // Contains signed value, if Signed == 1, unsigned value otherwise.
  USBH_ANY_SIGNED     PhysicalMin;   // Physical minimum from report descriptor.
                                     // Contains signed value, if PhySigned == 1, unsigned value otherwise.
  USBH_ANY_SIGNED     PhysicalMax;   // Physical maximum from report descriptor.
                                     // Contains signed value, if PhySigned == 1, unsigned value otherwise.
  U8                  PhySigned;     // = 1 if PhysicalMin / PhysicalMax are signed, = 0 if unsigned.
  U8                  NumBits;
  U16                 BitPosStart;
} USBH_HID_GENERIC_DATA;

/*********************************************************************
*
*       USBH_HID_ON_GENERIC_FUNC
*
*  Description
*    Function called on every generic HID event.
*
*  Parameters
*    InterfaceID     : Interface ID of the HID device that generated the event.
*    NumGenericInfos : Number of USBH_HID_GENERIC_DATA structures provided.
*    pGenericData    : Pointer to an array of USBH_HID_GENERIC_DATA structures.
*/
typedef void (USBH_HID_ON_GENERIC_FUNC) (USBH_INTERFACE_ID InterfaceID, unsigned NumGenericInfos, const USBH_HID_GENERIC_DATA * pGenericData);


void          USBH_HID_SetOnGenericEvent(U32 NumUsages, const U32 *pUsages, USBH_HID_ON_GENERIC_FUNC * pfOnEvent);


/*********************************************************************
*
*   PLUGIN: Remote Control
*
**********************************************************************
*/
/*********************************************************************
*
*       USBH_HID_RC_DATA
*
*  Description
*    Structure containing information about a remote control event.
*/
typedef struct {
  U8 VolumeIncrement;   // Indicates that the volume increment button was pressed (1 - pressed, 0 - unpressed).
  U8 VolumeDecrement;   // Indicates that the volume decrement button was pressed (1 - pressed, 0 - unpressed).
  I8 Mute;              // Indicates that the mute button was pressed (1 - pressed, 0 - unpressed OR
                        // -1 for "off", 0 for "no change" and 1 for "on", which selection variant
                        // is used depends on the device, but the second variant is rarely used).
  U8 PlayPause;         // Indicates that the play/pause button was pressed (1 - pressed, 0 - unpressed).
  U8 ScanNextTrack;     // Indicates that the scan next track button was pressed (1 - pressed, 0 - unpressed).
  U8 ScanPreviousTrack; // Indicates that the scan previous track button was pressed (1 - pressed, 0 - unpressed).
  U8 Repeat;            // Indicates that the repeat button was pressed (1 - pressed, 0 - unpressed).
  U8 RandomPlay;        // Indicates that the random play button was pressed (1 - pressed, 0 - unpressed).
  USBH_INTERFACE_ID   InterfaceID;   // ID of the interface that caused the event.
} USBH_HID_RC_DATA;


/*********************************************************************
*
*       USBH_HID_ON_RC_FUNC
*
*  Description
*    Function called on every remote control event.
*
*  Parameters
*    pMouseData : Pointer to a USBH_HID_RC_DATA structure.
*/
typedef void (USBH_HID_ON_RC_FUNC)(USBH_HID_RC_DATA * pMouseData);


void USBH_HID_SetOnRCStateChange(USBH_HID_ON_RC_FUNC * pfOnChange);


#if defined(__cplusplus)
  }
#endif

#endif // USBH_HID_H__

/*************************** End of file ****************************/
