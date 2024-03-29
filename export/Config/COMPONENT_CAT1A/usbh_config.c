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
File        : usbh_config.c
Purpose     : emUSB-Host configuration file for CAT1A device
---------------------------END-OF-HEADER------------------------------
*/

#include "USBH.h"
#include "USBH_HW_CYPRESS_PSoC.h"
#include "cyhal.h"

/* The size of memory (in bytes) dedicated to the stack. The size
 * of memory is selected to satisfy RAM usage of Driver and a
 * few connected devices. Update this value with the optimal
 * memory pool size (strongly recommended) for the application.
 * For details on selecting the optimal memory pool size, refer
 * to the emUSB-Host User Guide & Reference Manual Memory pools
 * chapter.
 **/
#define USBH_ALLOC_SIZE                         (0x8000U)
/* Define interrupt priority */
#define USBH_ISR_PRIO                           (3U)

/* Define interrupt source */
#if (COMPONENT_CM0P)
/* The number of CM0+ interrupt vectors is different for different devices,
 * so, change this macro for your selected device.
 */
#define USBH_INTERRUPT_NUM                      (NvicMux7_IRQn)
#else
#define USBH_INTERRUPT_NUM                      (usb_interrupt_med_IRQn)
#endif /* #if (COMPONENT_CM0P) */

/* The define pin, which controls the power distribution to a USB device
 * through VBUS line. The default value of the pin number is selected
 * only for the CY8CKIT-062-WIFI-BT kit and must be modified for other
 * boards.
 */
#define USBH_POWER_CONTROL_PIN                  (P13_2)


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/* Memory area used by the stack */
static U32 memory_pool[USBH_ALLOC_SIZE / 4U];

static bool vbus_control_pin_is_init = false;

/* Define categories of debug log messages that should be printed.
 * For possible categories, see USBH_MCAT_... definitions in USBH.h
 */
static const U8 log_categories[] =
{
    USBH_MCAT_INIT,
    USBH_MCAT_APPLICATION
};


/*********************************************************************
*
*       isr
*
*  Function description
*    USB interrupt handler
*
*/
static void isr(void)
{
    USBH_ServiceISR(0);
}


/*********************************************************************
*
*       on_port_power_control
*
*  Function description
*    A callback set by USBH_SetOnSetPortPower() is called when the
*    port power is changed.
*
*/
static void on_port_power_control(U32 HostControllerIndex, U8 Port, U8 PowerOn) {
    USBH_USE_PARA(HostControllerIndex);
    USBH_USE_PARA(Port);

    if (PowerOn != 0u)
    {
        cyhal_gpio_write(USBH_POWER_CONTROL_PIN, true);
    }
    else
    {
        cyhal_gpio_write(USBH_POWER_CONTROL_PIN, false);
    }
}


/*********************************************************************
*
*       init_power_control_pin
*
*  Function description
*    Initializes the pin, which controls the power distribution to a
*    USB device through VBUS line.
*
*/
void init_power_control_pin()
{
    if (!vbus_control_pin_is_init)
    {
        cy_rslt_t result;
        result = cyhal_gpio_init(USBH_POWER_CONTROL_PIN, CYHAL_GPIO_DIR_OUTPUT,
                                 CYHAL_GPIO_DRIVE_PULLUP, false);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        (void) result; /* To avoid the compiler warning in Release mode */

        vbus_control_pin_is_init = true;
    }
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
    init_power_control_pin();

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

    /* Add driver */
    (void) USBH_Cypress_PSoC_Add();

    USBH_SetOnSetPortPower(on_port_power_control);

    /* Install the interrupt service routine */
    cy_rslt_t result;
    result = cyhal_system_set_isr(USBH_INTERRUPT_NUM, usb_interrupt_med_IRQn, USBH_ISR_PRIO, isr);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */

    /* Enable USB interrupt */
    NVIC_EnableIRQ(USBH_INTERRUPT_NUM);
}