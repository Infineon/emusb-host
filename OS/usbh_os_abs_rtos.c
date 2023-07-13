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
File        : usbh_os_abs_rtos.c
Purpose     : OS Layer for the emUSB-Host.
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/

#include "USBH.h"
#include "USBH_Util.h"
#include "USBH_MEM.h"

#include "cyabs_rtos.h"

#if defined (COMPONENT_CAT1A) || defined (COMPONENT_CAT3)
    /* Do nothing */
#else
    #error "Unsupported Device Family"
#endif /* #if defined (COMPONENT_CAT1A) || defined (COMPONENT_CAT3) */

/*********************************************************************
*
*       Definitions, non-configurable
*
**********************************************************************
*/
#define NET_EVENT_BITS_MASK                     (0x01U)
#define ISR_EVENT_BITS_MASK                     (0x01U)
#define CUSTOM_EVENT_BITS_MASK                  (0x01U)
#define CRITICAL_SECTION_MAX_COUNT              (6U)

/*********************************************************************
*
*       Type definitions
*
**********************************************************************
*/
struct _USBH_OS_EVENT_OBJ
{
    USBH_DLIST list_entry;
    cy_event_t event_task;
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static cy_mutex_t usbh_mutex[USBH_MUTEX_COUNT];
static cy_event_t net_event;
static cy_event_t isr_event;
static USBH_DLIST user_event_list;
static volatile U32  isr_mask;

static uint32_t int_state[CRITICAL_SECTION_MAX_COUNT];
static uint32_t int_state_cnt;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       USBH_OS_DisableInterrupt
*
*  Function description
*    Enter a critical region for the USB stack: Increments interrupt disable count and
*    disables interrupts.
*
*  Additional information
*    The USB stack will perform nested calls to USBH_OS_DisableInterrupt() and USBH_OS_EnableInterrupt().
*/
__WEAK void USBH_OS_DisableInterrupt(void)
{
#if defined (COMPONENT_CAT1A)
    int_state[int_state_cnt] = cyhal_system_critical_section_enter();
#elif defined (COMPONENT_CAT3)
    int_state[int_state_cnt] = XMC_EnterCriticalSection();
#endif /* #if defined (COMPONENT_CAT1A) */
    int_state_cnt++;
}

/*********************************************************************
*
*       USBH_OS_EnableInterrupt
*
*  Function description
*    Leave a critical region for the USB stack: Decrements interrupt disable count and
*    enable interrupts if counter reaches 0.
*
*  Additional information
*    The USB stack will perform nested calls to USBH_OS_DisableInterrupt() and USBH_OS_EnableInterrupt().
*/
__WEAK void USBH_OS_EnableInterrupt(void)
{
    /* Check possibility of getting negative value for unsigned variable */
    CY_ASSERT(0U != int_state_cnt);
    int_state_cnt--;
#if defined (COMPONENT_CAT1A)
    cyhal_system_critical_section_exit(int_state[int_state_cnt]);
#elif defined (COMPONENT_CAT3)
    XMC_ExitCriticalSection(int_state[int_state_cnt]);
#endif /* #if defined (COMPONENT_CAT1A) */
}

/*********************************************************************
*
*       USBH_OS_Init
*
*  Function description
*    Initialize (create) all objects required for task synchronization.
*/
void USBH_OS_Init(void)
{
    cy_rslt_t result;

    USBH_DLIST_Init(&user_event_list);

    result = cy_rtos_event_init(&net_event);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    result = cy_rtos_event_init(&isr_event);
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    for (uint32_t i = 0U; i < USBH_MUTEX_COUNT; i++)
    {
        result = cy_rtos_mutex_init(&usbh_mutex[i], true);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
    }

    (void) result; /* To avoid the compiler warning in Release mode */

    int_state_cnt = 0U;
}

/*********************************************************************
*
*       USBH_OS_Lock
*
*  Function description
*    This function locks a mutex object, guarding sections of the stack code
*    against other threads. Mutexes are recursive.
*
*  Parameters
*    Idx:     Index of the mutex to be locked (0 .. USBH_MUTEX_COUNT-1).
*/
void USBH_OS_Lock(unsigned Idx)
{
    cy_rslt_t result = cy_rtos_mutex_get(&usbh_mutex[Idx], CY_RTOS_NEVER_TIMEOUT);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_Unlock
*
*  Function description
*    Unlocks the mutex used by a previous call to USBH_OS_Lock().
*    Mutexes are recursive.
*
*  Parameters
*    Idx:     Index of the mutex to be released (0 .. USBH_MUTEX_COUNT-1).
*/
void USBH_OS_Unlock(unsigned Idx)
{
    cy_rslt_t result = cy_rtos_mutex_set(&usbh_mutex[Idx]);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_GetTime32
*
*  Function description
*    Return the current system time in ms.
*    The value will wrap around after app. 49.7 days. This is taken into
*    account by the stack.
*
*  Return value
*    Current system time.
*/
USBH_TIME USBH_OS_GetTime32(void)
{
    cy_time_t time;
    cy_rslt_t result = cy_rtos_time_get(&time);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
    return (USBH_TIME) time;
}

/*********************************************************************
*
*       USBH_OS_Delay
*
*  Function description
*    Blocks the calling task for a given time.
*
*  Parameters
*    ms:     Delay in milliseconds.
*/
void USBH_OS_Delay(unsigned ms)
{
    cy_rslt_t result = cy_rtos_delay_milliseconds((cy_time_t) ms);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_WaitNetEvent
*
*  Function description
*    Blocks until the timeout expires or a USBH-event occurs.
*    Called from USBH_MainTask() only.
*    A USBH-event is signaled with USBH_OS_SignalNetEvent() called from an other task or ISR.
*
*  Parameters
*    ms:     Timeout in milliseconds.
*/
void USBH_OS_WaitNetEvent(unsigned ms)
{
    uint32_t bits = NET_EVENT_BITS_MASK;
    cy_rslt_t result = cy_rtos_event_waitbits(&net_event, &bits, true, true, (cy_time_t ) ms);
    CY_ASSERT((CY_RSLT_SUCCESS == result) || (CY_RTOS_TIMEOUT == result));
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_SignalNetEvent
*
*  Function description
*    Wakes the USBH_MainTask() if it is waiting for a event or timeout
*    in the function USBH_OS_WaitNetEvent().
*/
void USBH_OS_SignalNetEvent(void)
{
    cy_rslt_t result = cy_rtos_event_setbits(&net_event, NET_EVENT_BITS_MASK);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_WaitISR
*
*  Function description:
*    Blocks until USBH_OS_SignalISR() is called (from ISR).
*    Called from USBH_ISRTask() only.
*
*  Return value:
*    An ISR mask, where each bit set corresponds to a host controller index.
*/
U32 USBH_OS_WaitISR(void)
{
    U32 ret_val;
    uint32_t bits = ISR_EVENT_BITS_MASK;

    cy_rslt_t result = cy_rtos_event_waitbits(&isr_event, &bits, true, true, CY_RTOS_NEVER_TIMEOUT);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */

    USBH_OS_DisableInterrupt();
    ret_val = isr_mask;
    isr_mask = 0U;
    USBH_OS_EnableInterrupt();
    return ret_val;
}

/*********************************************************************
*
*       USBH_OS_SignalISREx
*
*  Function description
*    Wakes the USBH_ISRTask(). Called from ISR.
*
*  Parameters
*    DevIndex:  Zero-based index of the host controller that needs attention.
*/
void USBH_OS_SignalISREx(U32 DevIndex)
{
    CY_ASSERT(DevIndex <= sizeof(isr_mask) * 8U);
    isr_mask |= (1 << DevIndex);
    cy_rslt_t result = cy_rtos_event_setbits(&isr_event, ISR_EVENT_BITS_MASK);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    (void) result; /* To avoid the compiler warning in Release mode */
}

/*********************************************************************
*
*       USBH_OS_AllocEvent
*
*  Function description
*    Allocates and returns an event object.
*
*  Return value:
*    A pointer to a USBH_OS_EVENT_OBJ object on success or NULL on error.
*/
USBH_OS_EVENT_OBJ * USBH_OS_AllocEvent(void)
{
    USBH_OS_EVENT_OBJ * p;

    p = (USBH_OS_EVENT_OBJ *)USBH_TRY_MALLOC(sizeof(USBH_OS_EVENT_OBJ));
    if (p != NULL) {
        USBH_DLIST_Init(&p->list_entry);
        USBH_DLIST_InsertTail(&user_event_list, &p->list_entry);
        cy_rslt_t result = cy_rtos_event_init(&p->event_task);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        (void) result; /* To avoid the compiler warning in Release mode */
    }
    return p;
}

/*********************************************************************
*
*       USBH_OS_FreeEvent
*
*  Function description
*    Releases an object event.
*
*  Parameters
*    pEvent:  Pointer to an event object that was returned by USBH_OS_AllocEvent().
*/
void USBH_OS_FreeEvent(USBH_OS_EVENT_OBJ * pEvent)
{
    CY_ASSERT(pEvent != NULL);

    if (pEvent != NULL)
    {
        USBH_DLIST_RemoveEntry(&pEvent->list_entry);
        cy_rslt_t result = cy_rtos_event_deinit(&pEvent->event_task);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        (void) result; /* To avoid the compiler warning in Release mode */
        USBH_FREE(pEvent);
    }
}

/*********************************************************************
*
*       USBH_OS_SetEvent
*
*  Function description
*    Sets the state of the specified event object to signalled.
*
*  Parameters
*    pEvent:  Pointer to an event object that was returned by USBH_OS_AllocEvent().
*/
void USBH_OS_SetEvent(USBH_OS_EVENT_OBJ * pEvent)
{
    CY_ASSERT(pEvent != NULL);

    if (pEvent != NULL)
    {
        cy_rslt_t result = cy_rtos_event_setbits(&pEvent->event_task, CUSTOM_EVENT_BITS_MASK);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        (void) result; /* To avoid the compiler warning in Release mode */
    }
}

/*********************************************************************
*
*       USBH_OS_ResetEvent
*
*  Function description
*    Sets the state of the specified event object to non-signalled.
*
*  Parameters
*    pEvent:  Pointer to an event object that was returned by USBH_OS_AllocEvent().
*/
void USBH_OS_ResetEvent(USBH_OS_EVENT_OBJ* pEvent)
{
    CY_ASSERT(pEvent != NULL);
    cy_rslt_t result;
    uint32_t bits;
    if (pEvent != NULL)
    {
        result = cy_rtos_getbits_event(&pEvent->event_task, &bits);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        /* Clear bits only if they are set */
        if (bits == CUSTOM_EVENT_BITS_MASK)
        {
            result = cy_rtos_event_clearbits(&pEvent->event_task, CUSTOM_EVENT_BITS_MASK);
            CY_ASSERT(CY_RSLT_SUCCESS == result);
        }
        (void)result; /* To avoid the compiler warning in Release mode */
    }
}

/*********************************************************************
*
*       USBH_OS_WaitEvent
*
*  Function description
*    Wait for the specific event.
*
*  Parameters
*    pEvent:  Pointer to an event object that was returned by USBH_OS_AllocEvent().
*/
void USBH_OS_WaitEvent(USBH_OS_EVENT_OBJ * pEvent)
{
    CY_ASSERT(pEvent != NULL);

    if (pEvent != NULL)
    {
        uint32_t bits = CUSTOM_EVENT_BITS_MASK;
        cy_rslt_t result = cy_rtos_event_waitbits(&pEvent->event_task, &bits, true, true, CY_RTOS_NEVER_TIMEOUT);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        (void) result; /* To avoid the compiler warning in Release mode */
    }
}

/*********************************************************************
*
*       USBH_OS_WaitEventTimed
*
*  Function description
*    Wait for the specific event within a given timeout.
*
*  Parameters
*    pEvent:        Pointer to an event object that was returned by USBH_OS_AllocEvent().
*    MilliSeconds:  Timeout in milliseconds.
*
*  Return value:
*    == USBH_OS_EVENT_SIGNALED:   Event was signaled.
*    == USBH_OS_EVENT_TIMEOUT:    Timeout occurred.
*/
int USBH_OS_WaitEventTimed(USBH_OS_EVENT_OBJ * pEvent, U32 MilliSeconds)
{
    CY_ASSERT(pEvent != NULL);
    uint32_t bits = CUSTOM_EVENT_BITS_MASK;

    if (pEvent != NULL)
    {
        cy_rslt_t result = cy_rtos_event_waitbits(&pEvent->event_task, &bits, true, true, (cy_time_t) MilliSeconds);
        CY_ASSERT(CY_RSLT_SUCCESS == result || CY_RTOS_TIMEOUT == result);
        (void) result; /* To avoid compiler warning in Release mode */
    }
    return (bits == CUSTOM_EVENT_BITS_MASK) ? USBH_OS_EVENT_SIGNALED : USBH_OS_EVENT_TIMEOUT;
}

/*********************************************************************
*
*       USBH_OS_DeInit
*
*  Function description
*    Deletes all objects required for task synchronization.
*/
void USBH_OS_DeInit(void)
{
    USBH_DLIST * p_list_head;
    USBH_DLIST * p_entry;
    cy_rslt_t result;

    p_list_head = &user_event_list;
    p_entry    = USBH_DLIST_GetNext(p_list_head);
    while (p_list_head != p_entry) {
        USBH_OS_EVENT_OBJ * p_event;

        p_event = STRUCT_BASE_POINTER(p_entry, USBH_OS_EVENT_OBJ, list_entry);
        result = cy_rtos_event_deinit(&p_event->event_task);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
        p_entry = USBH_DLIST_GetNext(p_entry);
        USBH_DLIST_RemoveEntry(&p_event->list_entry);
        USBH_FREE(p_event);
    }

    result = cy_rtos_event_deinit(&isr_event);
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    result = cy_rtos_event_deinit(&net_event);
    CY_ASSERT(CY_RSLT_SUCCESS == result);

    for (uint32_t i = 0U; i < USBH_MUTEX_COUNT; i++)
    {
        result = cy_rtos_mutex_deinit(&usbh_mutex[i]);
        CY_ASSERT(CY_RSLT_SUCCESS == result);
    }

    (void) result; /* To avoid the compiler warning in Release mode */
}

/*************************** End of file ****************************/
