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
*       emUSB-Host version: V2.48.1                                  *
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
Purpose     : Header for the driver based on the Infineon PSC3M6 controller
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_HW_INFINEON_PSC3M6_H_
#define USBH_HW_INFINEON_PSC3M6_H_

#include "SEGGER.h"

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  I8             Enable;
  U8             Prio;
  PTR_ADDR       DmaChAddr;
  volatile U32 * RW1_DR;
  volatile U32 * RW2_DR;
  U32            DMADescr[6];
} USBH_Infineon_PSC3M6_DMA_INFO;


typedef struct {
  void (*pfSetupInDMA)(USBH_Infineon_PSC3M6_DMA_INFO *pDMA, U8 * pData, U32 NumBytes);
  void (*pfSetupOutDMA)(USBH_Infineon_PSC3M6_DMA_INFO *pDMA, U8 * pData, U32 NumBytes);
  void (*pfWaitDMA)(const USBH_Infineon_PSC3M6_DMA_INFO *pDMA);
  void (*pfStopDMA)(const USBH_Infineon_PSC3M6_DMA_INFO *pDMA);
} USBH_Infineon_PSC3M6_DMA_API;

extern const USBH_Infineon_PSC3M6_DMA_API USBH_INFINEON_PSoC_DMA;

/*********************************************************************
*
*       Function prototypes
*/
#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

U32 USBH_Infineon_PSC3M6_Add(U32 BaseAddress);
U32 USBH_Infineon_PSC3M6_DMA_Add(U32 BaseAddress, const USBH_Infineon_PSC3M6_DMA_API *pDMA_API, PTR_ADDR DMAChannelAddr, unsigned Priority);

#if defined(__cplusplus)
  }
#endif

#endif // USBH_HW_INFINEON_PSC3M6_H_

/*************************** End of file ****************************/
