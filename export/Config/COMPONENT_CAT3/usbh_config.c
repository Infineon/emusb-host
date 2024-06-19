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
File        : usbh_config.c
Purpose     : emUSB-Host configuration file for CAT3 (XMC4xxx) device
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include <stdlib.h>
#include "USBH.h"
#include "USBH_HW_XMC4xxxFS.h"
#include "xmc_device.h"
#include "xmc_scu.h"
#include "xmc4_scu.h"
#include "cybsp.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
/* The size of memory (in bytes) dedicated to the stack. The size
 * of memory is selected to satisfy RAM usage of Driver and a
 * few connected devices. Update this value with the optimal
 * memory pool size (strongly recommended) for the application.
 * For details on selecting the optimal memory pool size, refer
 * to the emUSB-Host User Guide & Reference Manual Memory pools
 * chapter.
 **/
#define USBH_ALLOC_SIZE                         (0x8000U)

/* Define interrupt priority. In case of FreeRTOS ensure that
 * configMAX_SYSCALL_INTERRUPT_PRIORITY in FreeRTOSConfig.h is
 * configured in accordance to XMC4xxx interrupts configuration
 */
#define USBH_ISR_PRIO                           (63U)

/* Enable OTG feature */
#if !defined(USBH_ENABLE_OTG)
#define USBH_ENABLE_OTG                         (0)
#endif /* #if !defined(USBH_ENABLE_OTG) */

#if (USBH_ENABLE_OTG) == 1
#include "USB_OTG.h"

#define OTG_ID_PORT                             (ioss_0_port_0_pin_9_PORT)
#define OTG_ID_PIN_NUM                          (ioss_0_port_0_pin_9_PIN)
#endif /* #if (USBH_ENABLE_OTG) == 1 */

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/* Memory area used by the stack */
static U32 memory_pool[USBH_ALLOC_SIZE / 4U];

/* Define categories of debug log messages that should be printed.
 * For possible categories, see USBH_MCAT_... definitions in USBH.h
 */
static const U8 log_categories[] =
{
    USBH_MCAT_INIT,
    USBH_MCAT_APPLICATION
};

typedef void USB_ISR_HANDLER  (void);

static USB_ISR_HANDLER * p_usb_isr_handler;

/*********************************************************************
*
*       USB0_0_IRQHandler
*
*  Function description
*    USB interrupt handler
*
*/
void USB0_0_IRQHandler(void)
{
    if (p_usb_isr_handler != NULL)
    {
        (p_usb_isr_handler)();
    }
}

/*********************************************************************
*
*       usbh_isr
*
*  Function description
*    The wrapper for the ISR handler provided by emUSB-Host. emUSB-Host
*    and emUSB-Device provide different prototypes of the ISR handler.
*    So, this wrapper is required to align the implementation between them.
*
*/
static void usbh_isr(void)
{
    USBH_ServiceISR(0);
}

/*********************************************************************
*
*       USB0_0_IRQHandler
*
*  Function description
*    Register function for interrupt handler and configure interrupt.
*
*/
void usb_enable_isr(IRQn_Type isr_index, USB_ISR_HANDLER * p_isr_handler, U32 isr_prio)
{
    p_usb_isr_handler = p_isr_handler;
    NVIC_SetPriority(isr_index, isr_prio);
    NVIC_EnableIRQ(isr_index);
}

/*********************************************************************
*
*       USBH_X_Config
*
*  Function description
*    Initialize USB hardware and configure the USB-Host stack. This function
*    is called by the startup code of the emUSB-Host stack from USBH_Init().
*    This is the place where a hardware driver can be added and configured.
*
*/
void USBH_X_Config(void)
{
    /* Assigning memory */
    USBH_AssignMemory((void *) &memory_pool, USBH_ALLOC_SIZE);

    /* Don't allow external hubs */
    USBH_ConfigSupportExternalHubs(0U);

    /* Wait 300ms for a new connected device */
    USBH_ConfigPowerOnGoodTime(300U);

    /* Define log and warn filter */
    /* Output all warnings */
    USBH_ConfigMsgFilter(USBH_WARN_FILTER_SET_ALL, 0, NULL);
    /* Set message categories in log filter */
    USBH_ConfigMsgFilter(USBH_LOG_FILTER_SET, sizeof(log_categories), log_categories);

    /* Enable power for USB */
    XMC_SCU_POWER_EnableUsb();
    /* De-assert Reset from USB controller */
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0);

    USBH_XMC4xxx_FS_Add((void*)USB0_BASE);

    usb_enable_isr(USB0_0_IRQn, usbh_isr, USBH_ISR_PRIO);
}

#if (USBH_ENABLE_OTG) == 1
/*********************************************************************
*
*       get_id_pin
*
*  Function description
*    Return the current status of ID pin.
*
*/
static int get_id_pin(void)
{
    return XMC_GPIO_GetInput(OTG_ID_PORT, OTG_ID_PIN_NUM);
}

/*********************************************************************
*
*       USBH_X_Config
*
*  Function description
*    Initialize USB hardware and configure the OTG driver. This function
*    is called by the startup code from USB_OTG_Init(). This is the place
*    where a hardware driver can be added and configured.
*
*/
void USB_OTG_X_Config(void)
{
    /* Enable power for USB */
    XMC_SCU_POWER_EnableUsb();

    /* De-assert Reset from USB controller */
    XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_USB0);

    USB_OTG_SetIdPinFunc(get_id_pin);
    USB_OTG_AddDriver(&USB_OTG_Driver_XMC4500);
}
#endif /* #if (USBH_ENABLE_OTG) == 1 */
/*************************** End of file ****************************/
