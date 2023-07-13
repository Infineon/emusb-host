/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2023     SEGGER Microcontroller GmbH              *
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
*       emUSB-Host version: V2.36.3                                  *
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
File        : USBH_HW_XMC4xxxFS.h
Purpose     : Header for the XMC4500xx/F4xx FullSpeed emUSB Host driver
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_HW_XMC4xxxFS_H__
#define USBH_HW_XMC4xxxFS_H__

#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif


U32  USBH_XMC4xxx_FS_Add(void * pBase);

#if defined(__cplusplus)
  }
#endif

#endif /* USBH_HW_XMC4xxxFS_H__ */

/*************************** End of file ****************************/
