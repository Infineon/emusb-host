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
File    : USBH_ConfDefaults.h
Purpose : This file contains the default values for USB-Host
          configuration parameters. To allow easy updates please
          do not change the parameters here, but add them in the
          USBH_Conf.h file, the defines there will replace the default
          values.
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef   USBH_CONFDEFAULTS_H
#define   USBH_CONFDEFAULTS_H

#include "USBH_Conf.h"

/*********************************************************************
*
*       USBH_DEBUG
*
*  Description
*    emUSB-Host can be configured to display debug messages and warnings
*    to locate an error or potential problems. This can be useful for debugging.
*    In a release (production) build of a target system, they are typically not required
*    and should be switches off.
*
*    To output the messages, emUSB-Host uses the logging routines contained
*    in \file{USBH_ConfigIO.c} which can be customized.
*
*    USBH_DEBUG can be set to the following values:
*    * 0 - Used for release builds. Includes no debug options.
*    * 1 - Used in debug builds to include support for "panic" checks.
*    * 2 - Used in debug builds to include warning, log messages and "panic" checks.
*          This significantly increases the code size.
*/
#ifndef     USBH_DEBUG
  #define   USBH_DEBUG    0
#endif

/*********************************************************************
*
*       USBH_PANIC, USBH_SUPPORT_LOG + USBH_SUPPORT_WARN
*/
#ifndef     USBH_PANIC
  #if       USBH_DEBUG
    #define USBH_PANIC(s) USBH_Panic(s)
  #else
    #ifdef _lint
      //lint -emacro({9036}, USBH_PANIC)  N:100
      #define USBH_PANIC(p)  do {} while(0)
    #else
      #define USBH_PANIC(p)
    #endif
  #endif
#endif

#ifndef     USBH_SUPPORT_LOG
  #if       USBH_DEBUG > 1
    #define USBH_SUPPORT_LOG   1
  #else
    #define USBH_SUPPORT_LOG   0
  #endif
#endif

#ifndef     USBH_SUPPORT_WARN
  #if       USBH_DEBUG > 1
    #define USBH_SUPPORT_WARN  1
  #else
    #define USBH_SUPPORT_WARN  0
  #endif
#endif

#if USBH_DEBUG < 2 && (USBH_SUPPORT_LOG != 0 || USBH_SUPPORT_WARN != 0)
  #error "Setting of USBH_SUPPORT_LOG / USBH_SUPPORT_WARN is allowed only if USBH_DEBUG == 2"
#endif

/*********************************************************************
*
*       USBH_LOG_BUFFER_SIZE
*
*  Description
*    Maximum size of a debug / warning message (in characters) that can be output.
*    A buffer of this size is created on the stack when a message is output.
*/
#ifndef   USBH_LOG_BUFFER_SIZE
  #define USBH_LOG_BUFFER_SIZE     200
#endif

/*********************************************************************
*
*       Trace/SystemView related configuration defaults.
*/
#define USBH_SUPPORT_TRACE            0     // not yet supported

/*********************************************************************
*
*    Use of the standard C-library
*/

//
// Macro overwrite for memcpy.
//
#ifndef   USBH_MEMCPY
  #include <string.h>
  #define USBH_MEMCPY  memcpy                          /*lint -e{9087,9005} D:105[b] */
#endif

//
// Macro overwrite for memset.
//
#ifndef   USBH_MEMSET
  #include <string.h>
  #define USBH_MEMSET  memset                          /*lint -e{9087,9005} D:105[b] */
#endif

//
// Macro overwrite for memcmp.
//
#ifndef   USBH_MEMCMP
  #include <string.h>
  #define USBH_MEMCMP  memcmp                          /*lint -e{9087,9005} D:105[b] */
  //lint -sem(memcmp, pure)          N:100
#endif

//
// Macro overwrite for memmove
//
#ifndef   USBH_MEMMOVE
  #include <string.h>
  #define USBH_MEMMOVE  memmove                         /*lint -e{9087,9005} D:105[b] */
#endif

//
// Macro overwrite for strlen
//
#ifndef   USBH_STRLEN
  #include <string.h>
  #define USBH_STRLEN   strlen
#endif

//
// Macro overwrite for strcat
//
#ifndef   USBH_STRCAT
  #include <string.h>
  #define USBH_STRCAT   strcat
#endif

//
// Macro overwrite for strrchr
//
#ifndef   USBH_STRRCHR
  #include <string.h>
  #define USBH_STRRCHR  strrchr
#endif

//
// Macro overwrite for strncpy
//
#ifndef   USBH_STRNCPY
  #include <string.h>
  #define USBH_STRNCPY  strncpy
#endif

//
// Macro overwrite for strcmp
//
#ifndef   USBH_STRCMP
  #include <string.h>
  #define USBH_STRCMP   strcmp
#endif

/*********************************************************************
*
*       USBH_MAX_NUM_HOST_CONTROLLERS
*
*  Description
*    Maximum number of host controllers the USB stack can handle.
*/
#ifndef USBH_MAX_NUM_HOST_CONTROLLERS
  #define USBH_MAX_NUM_HOST_CONTROLLERS  4u
#endif

/*********************************************************************
*
*       USBH_SUPPORT_ISO_TRANSFER
*
*  Description
*    Must be set to 1 if the USB stack shall support isochronous transfers
*    (e.g for audio and video applications).
*    If set to 0, all code that handles isochronous transfers is disabled,
*    which may significantly reduce the code size of the USB stack.
*/
#ifndef    USBH_SUPPORT_ISO_TRANSFER
  #define  USBH_SUPPORT_ISO_TRANSFER    0
#endif

/*********************************************************************
*
*       USBH_SUPPORT_VIRTUALMEM
*
*  Description
*    If set to 1 the USB stack allows translation of virtual to physical
*    memory addresses used for DMA operations (see USBH_Config_SetV2PHandler()).
*    If the target system does not have a MMU, it can be set to 0.
*/
#ifndef   USBH_SUPPORT_VIRTUALMEM
  #define USBH_SUPPORT_VIRTUALMEM       1
#endif

/*********************************************************************
*
*       USBH_REO_FREE_MEM_LIST
*
*  Description
*    The USB stack uses a memory heap to allocate data structures for each
*    connected USB device (see USBH_AssignMemory() and USBH_AssignTransferMemory()).
*    If USB devices are frequently connected and disconnected this may lead to
*    fragmentation of the heap memory. If this options is set, a reorganization
*    of all free memory areas in the heap is performed after each disconnection
*    of a USB device.
*/
#ifndef   USBH_REO_FREE_MEM_LIST
  #define USBH_REO_FREE_MEM_LIST        0
#endif

/*********************************************************************
*
*       USBH_USE_APP_MEM_PANIC
*
*  Description
*    The USB host stack calls the function "void USBH_MEM_Panic(void)", if memory allocation
*    fails during initialization of the host stack (USBH_Init()).
*    The stack contains a default implementation of the function USBH_MEM_Panic() which
*    halts the system, indicating a fatal error.
*    An application may implement its own USBH_MEM_Panic() function, when setting
*    USBH_USE_APP_MEM_PANIC to '1'.
*    After successful initialization using USBH_Init(), USBH_MEM_Panic() is never called.
*/
#ifndef USBH_USE_APP_MEM_PANIC
  #define USBH_USE_APP_MEM_PANIC  0
#endif

/*********************************************************************
*
*       USBH_ENABLE_V210_FEATURES
*
*  Description
*    Enable USB version 2.10 features (e.g. BOS descriptor).
*/
#ifndef USBH_ENABLE_V210_FEATURES
  #define USBH_ENABLE_V210_FEATURES  0
#endif

/*********************************************************************
*
*       USBH_MAX_INTERFACES_IN_IAD
*
*  Description
*    Maximum number of interface IDs inside a USBH_IAD_INFO structure.
*    See USBH_GetIADInfo().
*/
#ifndef USBH_MAX_INTERFACES_IN_IAD
  #define USBH_MAX_INTERFACES_IN_IAD  5u
#endif

/*********************************************************************
*
*       USBH_WAIT_AFTER_RESET
*
*  Description
*    The host controller waits this time after reset of a root hub port, before the device descriptor is requested or
*    the Set Address command is sent. Given in milliseconds.
*/
#ifndef USBH_WAIT_AFTER_RESET
  #define USBH_WAIT_AFTER_RESET        180
#endif

/*********************************************************************
*
*       USBH_HUB_WAIT_AFTER_RESET
*
*  Description
*   The host controller waits this time after reset of a external hub port, before the device descriptor is requested or
*   the Set Address command is sent. Given in milliseconds.
*/
#ifndef USBH_HUB_WAIT_AFTER_RESET
  #define USBH_HUB_WAIT_AFTER_RESET    180u
#endif

/*********************************************************************
*
*       WAIT_AFTER_SETADDRESS
*
*  Description
*    The USB stack waits this time before the next command is sent after Set Address. The device must answer to SetAddress on USB address 0 with the
*    handshake and than set the new address. This is a potential racing condition if this step is performed in the firmware.
*    Give the device this time to set the new address. Given in milliseconds.
*/
#ifndef WAIT_AFTER_SETADDRESS
  #define WAIT_AFTER_SETADDRESS         30u
#endif

/*********************************************************************
*
*       USBH_RESET_RETRY_COUNTER
*
*  Description
*    If an error is encountered during USB reset, set address or enumeration the process is repeated USBH_RESET_RETRY_COUNTER times
*    before the port is finally disabled.
*/
#ifndef USBH_RESET_RETRY_COUNTER
  #define USBH_RESET_RETRY_COUNTER       5u
#endif

/*********************************************************************
*
*       USBH_DELAY_FOR_REENUM
*
*  Description
*    Describes the time in milliseconds before a USB reset is restarted,
*    after the enumeration of the device (get descriptors, set configuration) has failed.
*/
#ifndef USBH_DELAY_FOR_REENUM
  #define USBH_DELAY_FOR_REENUM       1000u
#endif

/*********************************************************************
*
*       USBH_DELAY_BETWEEN_ENUMERATIONS
*
*  Description
*     On default, enumeration for multiple devices may be processed in parallel.
*     Setting USBH_DELAY_BETWEEN_ENUMERATIONS > 0 will serialize all enumerations using a delay
*     before a new enumeration is performed. The delay can be given in milliseconds.
*/
#ifndef USBH_DELAY_BETWEEN_ENUMERATIONS
  #define USBH_DELAY_BETWEEN_ENUMERATIONS   0
#endif

/*********************************************************************
*
*       USBH_DEFAULT_SETUP_TIMEOUT
*
*  Description
*    Default timeout (in milliseconds) for all setup requests during enumeration of a device.
*    After this time a not completed setup request is terminated. Windows gives 2 seconds to
*    answer to a setup request. Less than that some devices behave quite strange.
*    So it should >= 2000.
*/
#ifndef USBH_DEFAULT_SETUP_TIMEOUT
  #define USBH_DEFAULT_SETUP_TIMEOUT         2000
#endif

/*********************************************************************
*
*       USBH_URB_QUEUE_SIZE
*
*  Description
*    If not 0, queue URBs, when the driver reports USBH_STATUS_NO_CHANNEL
*    and retry them later. The value gives the maximum number of URBs that
*    can be queued. Only used for BULK transfers.
*/
#ifndef USBH_URB_QUEUE_SIZE
  #define USBH_URB_QUEUE_SIZE         0u
#endif

/*********************************************************************
*
*       USBH_URB_QUEUE_RETRY_INTV
*
*  Description
*    URB queue retry interval in ms. Only used, if USBH_URB_QUEUE_SIZE != 0.
*/
#ifndef USBH_URB_QUEUE_RETRY_INTV
  #define USBH_URB_QUEUE_RETRY_INTV   5u
#endif

/*********************************************************************
*
*       USBH_SUPPORT_HUB_CLEAR_TT_BUFFER
*
*  Description
*    If set, a CLEAR_TT_BUFFER request is send to the HUB after an URB was aborted.
*    Used only for USB devices that are using split transactions.
*    Not supported by all drivers.
*/
#ifndef   USBH_SUPPORT_HUB_CLEAR_TT_BUFFER
  #define USBH_SUPPORT_HUB_CLEAR_TT_BUFFER   0
#endif

/*********************************************************************
*
*       USBH_USE_PARA
*
*  Description
*    In order to avoid compiler warnings for undefined parameters
*/
#ifndef USBH_USE_PARA
  #define USBH_USE_PARA(para) (void)(para);
#endif

/*********************************************************************
*
*       USBH_MSD_MAX_DEVICES
*
*  Description
*    Maximum number of mass storage devices the USB stack can handle simultaneously.
*/
#ifndef USBH_MSD_MAX_DEVICES
  #define USBH_MSD_MAX_DEVICES            10u
#endif

/*********************************************************************
*
*       USBH_MSD_MAX_SECTORS_AT_ONCE
*
*  Description
*    Maximum number of sectors to read with a single MSD read command.
*    Certain sticks have a limitation where they can not read or write
*    too many sectors in one command. For example:
*    DTSE9 16GB read limit ~ 4096, DT Ultimate G2 16GB write limit ~ 1024.
*    Windows uses max 128 sectors. Linux uses max 240 sectors.
*/
#ifndef USBH_MSD_MAX_SECTORS_AT_ONCE
  #define USBH_MSD_MAX_SECTORS_AT_ONCE       256u
#endif

/*********************************************************************
*
*       USBH_MSD_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a mass storage device, especially for 'GetMaxLun' and 'ClearFeatureHalt' commands.
*/
#ifndef USBH_MSD_EP0_TIMEOUT
  #define USBH_MSD_EP0_TIMEOUT          5000u
#endif

/*********************************************************************
*
*       USBH_MSD_CBW_WRITE_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds for sending a command block
*    to a mass storage device.
*/
#ifndef USBH_MSD_CBW_WRITE_TIMEOUT
  #define USBH_MSD_CBW_WRITE_TIMEOUT             3000
#endif

/*********************************************************************
*
*       USBH_MSD_CSW_READ_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds for reading a status block
*    from a mass storage device. 10 seconds is compatible to Windows.
*/
#ifndef USBH_MSD_CSW_READ_TIMEOUT
  #define USBH_MSD_CSW_READ_TIMEOUT       10000
#endif

/*********************************************************************
*
*       USBH_MSD_COMMAND_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds for reading answer data
*    from a mass storage device when not reading sector data.
*/
#ifndef USBH_MSD_COMMAND_TIMEOUT
  #define USBH_MSD_COMMAND_TIMEOUT      3000u
#endif

/*********************************************************************
*
*       USBH_MSD_DATA_READ_TIMEOUT
*
*  Description
*    Read timeout in milliseconds for the data phase when reading 'Length' bytes of sector data
*    from a mass storage device.
*/
#ifndef USBH_MSD_DATA_READ_TIMEOUT
  #define USBH_MSD_DATA_READ_TIMEOUT(Length)    (10000u + ((Length) / 512u) * 10u)
#endif

/*********************************************************************
*
*       USBH_MSD_DATA_WRITE_TIMEOUT
*
*  Description
*    Write timeout in milliseconds for the data phase when writing 'Length' bytes of sector data
*    to a mass storage device.
*/
#ifndef USBH_MSD_DATA_WRITE_TIMEOUT
  #define USBH_MSD_DATA_WRITE_TIMEOUT(Length)   (10000u + ((Length) / 512u) * 10u)
#endif

/*********************************************************************
*
*       USBH_MSD_MAX_TEST_READY_RETRIES
*
*  Description
*    Maximum number of retries executed for TestUnitReady / ReadCapacity commands
*    on failure during enumeration of a mass storage device. Value must be < 255.
*/
#ifndef USBH_MSD_MAX_TEST_READY_RETRIES
  #define USBH_MSD_MAX_TEST_READY_RETRIES 200u
#endif

/*********************************************************************
*
*       USBH_MSD_MAX_READY_WAIT_TIME
*
*  Description
*    Maximum time (in milliseconds) to wait for a LUN to become ready after enumeration
*    of a mass storage device, before the user notification callback is called.
*/
#ifndef USBH_MSD_MAX_READY_WAIT_TIME
  #define USBH_MSD_MAX_READY_WAIT_TIME    20000
#endif

/*********************************************************************
*
*       USBH_MSD_TEST_UNIT_READY_DELAY
*
*  Description
*    Minimum time (in milliseconds) between two TestUnitReady commands
*    send to mass storage device.
*/
#ifndef USBH_MSD_TEST_UNIT_READY_DELAY
  #define USBH_MSD_TEST_UNIT_READY_DELAY  5000
#endif

/*********************************************************************
*
*       USBH_HID_DISABLE_INTERFACE_PROTOCOL_CHECK
*
*  Description
*    Some HID devices, namely touch screens, report their interface protocol as "mouse" despite
*    being actual touch screens (normally bInterfaceProtocol is "None" for such devices).
*    If this happens with your touch screens you can enable this flag.
*/
#ifndef USBH_HID_DISABLE_INTERFACE_PROTOCOL_CHECK
  #define USBH_HID_DISABLE_INTERFACE_PROTOCOL_CHECK  0
#endif

/*********************************************************************
*
*       USBH_HID_MAX_REPORTS
*
*  Description
*    Maximum number of reports (with different report ID's) of one HID interface that can be handled.
*/
#ifndef USBH_HID_MAX_REPORTS
  #define USBH_HID_MAX_REPORTS               6u
#endif

/*********************************************************************
*
*       USBH_HID_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a HID device, like 'GetReportDescriptor' command.
*/
#ifndef   USBH_HID_EP0_TIMEOUT
  #define USBH_HID_EP0_TIMEOUT            1000
#endif

/*********************************************************************
*
*       USBH_PRINTER_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a printer device, especially for 'GetPortStatus' and 'SendVendorRequest' commands.
*/
#ifndef   USBH_PRINTER_EP0_TIMEOUT
  #define USBH_PRINTER_EP0_TIMEOUT         1000
#endif

/*********************************************************************
*
*       USBH_BULK_MAX_NUM_EPS
*
*  Description
*    Maximum number of endpoints that can be handled for a BULK (Vendor) device.
*/
#ifndef   USBH_BULK_MAX_NUM_EPS
  #define USBH_BULK_MAX_NUM_EPS              5u
#endif

/*********************************************************************
*
*       USBH_BULK_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a vendor (BULK) device, especially for 'SetAlternateInterface' and 'ClearFeatureHalt' commands.
*/
#ifndef   USBH_BULK_EP0_TIMEOUT
  #define USBH_BULK_EP0_TIMEOUT           1000
#endif

/*********************************************************************
*
*       USBH_CDC_DISABLE_AUTO_DETECT
*
*  Description
*    Can be used to disable the automatic detection of CDC devices.
*    In that case the user must use USBH_CDC_AddDevice/USBH_CDC_RemoveDevice
*    for addition and removal of devices.
*/
#ifndef USBH_CDC_DISABLE_AUTO_DETECT
  #define USBH_CDC_DISABLE_AUTO_DETECT     0
#endif

/*********************************************************************
*
*       USBH_CDC_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a CDC device, like 'SetAlternateInterface', 'ClearFeatureHalt',
*    'SetLineCoding' and 'SetControlLineState' commands.
*/
#ifndef   USBH_CDC_EP0_TIMEOUT
  #define USBH_CDC_EP0_TIMEOUT           1000
#endif

/*********************************************************************
*
*       USBH_FT232_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a FT232 device, like 'ResetDevice', 'SetBaudRate', 'SetDtr',
*    'ClrDtr', 'SetRts', 'ClrRts', 'GetModemStatus', 'SetChars',
*    'Purge', 'SetBreakOn', 'SetBreakOff', 'SetLatencyTimer',
*    'GetLatencyTimer', 'SetBitMode', 'GetBitMode'
*    'SetDataCharacteristics' and 'SetFlowControl' commands.
*/
#ifndef   USBH_FT232_EP0_TIMEOUT
  #define USBH_FT232_EP0_TIMEOUT           1000
#endif

/*********************************************************************
*
*       USBH_CP210X_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to a CP210X device, like 'SetBaudRate', 'SetDataCharacteristics',
*    'SetModemHandshaking', 'GetModemStatus' and 'Purge' commands.
*/
#ifndef   USBH_CP210X_EP0_TIMEOUT
  #define USBH_CP210X_EP0_TIMEOUT           1000
#endif

/*********************************************************************
*
*       USBH_AUDIO_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to an AUDIO device.
*/
#ifndef   USBH_AUDIO_EP0_TIMEOUT
  #define USBH_AUDIO_EP0_TIMEOUT           1000
#endif
/*********************************************************************
*
*       USBH_VIDEO_EP0_TIMEOUT
*
*  Description
*    Specifies the timeout in milliseconds to be used for control requests
*    to an VIDEO device.
*/
#ifndef   USBH_VIDEO_EP0_TIMEOUT
  #define USBH_VIDEO_EP0_TIMEOUT           1000
#endif

#endif  /* USBH_CONFDEFAULTS_H */

/*************************** End of file ****************************/
