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
Purpose : USB OTG stack API
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USB_OTG_H__
#define USB_OTG_H__

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include "SEGGER.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#define USB_OTG_ID_PIN_STATE_IS_HOST          0
#define USB_OTG_ID_PIN_STATE_IS_DEVICE        1
#define USB_OTG_ID_PIN_STATE_IS_INVALID     (-1)

/*********************************************************************
*
*       Types / structures
*/

/*********************************************************************
*
*       USB_OTG_IDPIN_FUNC
*
*  Description
*    Returns the current state of the USB OTG ID pin.
*
*  Return value
*     == 0  : ID pin is low
*     == 1  : otherwise
*/
typedef int USB_OTG_IDPIN_FUNC (void);

typedef struct {
  void     (*pfInit)           (void);
  int      (*pfGetSessionState)(void);
  void     (*pfDeInit)         (void);
} USB_OTG_HW_DRIVER;

/*********************************************************************
*
*       Public API functions
*
*/
void     USB_OTG_DeInit              (void);
void     USB_OTG_Init                (void);
int      USB_OTG_GetSessionState     (void);
int      USB_OTG_GetIdPin            (void);
void     USB_OTG_SetIdPinFunc        (USB_OTG_IDPIN_FUNC * pfGetIdPin);

/*********************************************************************
*
*       Obsolete API functions
*
*/
int      USB_OTG_GetIdState          (void);
int      USB_OTG_GetVBUSState        (void);
int      USB_OTG_IsSessionValid      (void);

/*********************************************************************
*
*       Setting USB target driver
*
*/
void USB_OTG_AddDriver(const USB_OTG_HW_DRIVER * pDriver);


/*********************************************************************
*
*       Function that has to be supplied by the customer
*
*/
void USB_OTG_X_Config(void);


/*********************************************************************
*
*       Configurations for USB target driver
*
*/
void USB_OTG_DRIVER_STM32F2xxFS_ConfigAddr(U32 BaseAddr);
void USB_OTG_DRIVER_XMC4500_ConfigAddr(U32 BaseAddr);
void USB_OTG_DRIVER_EHCI_ConfigAddr(U32 BaseAddr);

/*********************************************************************
*
*       Available target USB drivers
*
*/

extern const USB_OTG_HW_DRIVER USB_OTG_Driver_Renesas_RX62N;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_Renesas_RX63N;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_Renesas_RX72N;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_ST_STM32F2xxFS;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_ST_STM32F7xxFS;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_ST_STM32L4xx;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_XMC4500;
extern const USB_OTG_HW_DRIVER USB_OTG_Driver_EHCI;

#if defined(__cplusplus)
  }              /* Make sure we have C-declarations in C++ programs */
#endif

#endif /* USB_OTG_H__ */
