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
*       emUSB-Host version: V2.36.0                                  *
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
License model:            Cypress Services and License Agreement, signed June 9th/10th, 2009
                          and Amendment Number One, signed June 28th, 2019 and July 2nd, 2019
                          and Amendment Number Two, signed September 13th, 2021 and September 18th, 2021
                          and Amendment Number Three, signed May 2nd, 2022 and May 5th, 2022
Licensed platform:        Cypress devices containing ARM Cortex M cores: M0, M0+, M4.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2022-05-12 - 2024-05-19
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : USBH_Util.h
Purpose     : Utility functions used by different modules of
              the emUSB-Host stack.
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_UTIL_H
#define USBH_UTIL_H

#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                 // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Utility macros
*/
#define USBH_MIN(x,y)    ((x)  <  (y)   ?  (x)   :  (y))
#define USBH_MAX(x,y)    ((x)  >  (y)   ?  (x)   :  (y))

//
// Calculate the pointer to the base of the structure given its type and a pointer to a field within the structure.
//
//lint --emacro((413,923,946,947,9033,9078),STRUCT_BASE_POINTER)  D:103[d] D:106
#define STRUCT_BASE_POINTER(fieldptr, type, field)            ((type *)(((char *)(fieldptr)) - ((char *)(&(((type *)0)->field)))))

/*********************************************************************
*
*       Type definitions
*/
//
// Double linked list structure. Can be used as either a list head, or as link words.
// The USBH_DLIST structure is the link element of the double linked list. It is used as either a list head, or as link entry.
// By means of such elements any structures may be handled as a double linked list. The USBH_DLIST structure is to be inserted
// into the structure which is to be handled. A pointer to the original structure can be obtained by means of the macro STRUCT_BASE_POINTER.
//
typedef struct _USBH_DLIST {
  struct _USBH_DLIST * pNext;
  struct _USBH_DLIST * pPrev;
} USBH_DLIST;

#define USBH_DLIST_GetNext(pEntry)  (pEntry)->pNext


typedef struct {
  U8       * pData;
  unsigned   Size;
  unsigned   NumBytesIn;
  unsigned   RdPos;
} USBH_BUFFER;

/*********************************************************************
*
*       Utility functions
*/

//
// Bitfield related functions.
//
unsigned      USBH_BITFIELD_CalcNumBitsUsed(U32 Value);
U32           USBH_BITFIELD_ReadEntry      (const U8 * pBase, U32 Index, unsigned NumBits);
void          USBH_BITFIELD_WriteEntry     (      U8 * pBase, U32 Index, unsigned NumBits, U32 v);
unsigned      USBH_BITFIELD_CalcSize       (U32 NumItems, unsigned BitsPerItem);
//
// Ring buffer read/write functions.
//
void          USBH_BUFFER_Init (USBH_BUFFER * pBuffer,     void * pData, U32      NumBytes);
unsigned      USBH_BUFFER_Read (USBH_BUFFER * pBuffer,       U8 * pData, unsigned NumBytesReq);
void          USBH_BUFFER_Write(USBH_BUFFER * pBuffer, const U8 * pData, unsigned NumBytes);
//
// Register read/write functions.
//
U8            USBH_ReadReg8  (const U8  * pAddr);
U16           USBH_ReadReg16 (const U16 * pAddr);
void          USBH_WriteReg32(      U8  * pAddr, U32 Value);
U32           USBH_ReadReg32 (const U8  * pAddr);
//
// Load/Store functions and number related functions.
//
//lint -sem(USBH_LoadU32LE, pure)    N:100
//lint -sem(USBH_LoadU32BE, pure)    N:100
//lint -sem(USBH_LoadU24LE, pure)    N:100
unsigned      USBH_CountLeadingZeros(U32 Value);
U32           USBH_LoadU32BE(const U8 * pData);
U32           USBH_LoadU32LE(const U8 * pData);
U32           USBH_LoadU24LE(const U8 * pData);
unsigned      USBH_LoadU16BE(const U8 * pData);
unsigned      USBH_LoadU16LE(const U8 * pData);
void          USBH_StoreU16BE(     U8 * p, unsigned v);
void          USBH_StoreU16LE(     U8 * p, unsigned v);
void          USBH_StoreU24LE(     U8 * p, U32      v);
void          USBH_StoreU32BE(     U8 * p, U32      v);
void          USBH_StoreU32LE(     U8 * p, U32      v);
U32           USBH_SwapU32(                U32      v);
//
// Double linked list related functions.
//
void          USBH_DLIST_Append        (      USBH_DLIST * pListHead,   USBH_DLIST * pList);
void          USBH_DLIST_InsertTail    (const USBH_DLIST * pListHead,   USBH_DLIST * pEntry);
void          USBH_DLIST_InsertHead    (      USBH_DLIST * pListHead,   USBH_DLIST * pEntry);
void          USBH_DLIST_InsertEntry   (      USBH_DLIST * pEntry,      USBH_DLIST * pNewEntry);
void          USBH_DLIST_RemoveTail    (const USBH_DLIST * pListHead,   USBH_DLIST ** ppEntry);
void          USBH_DLIST_RemoveHead    (const USBH_DLIST * pListHead,   USBH_DLIST ** ppEntry);
void          USBH_DLIST_RemoveEntry   (      USBH_DLIST * pEntry);
USBH_DLIST *  USBH_DLIST_GetPrev       (const USBH_DLIST * pEntry);
int           USBH_DLIST_IsEmpty       (const USBH_DLIST * pListHead);
void          USBH_DLIST_Init          (      USBH_DLIST * pListHead);
void          USBH_DLIST_Move          (      USBH_DLIST * pHead,       USBH_DLIST * pItem);
int           USBH_DLIST_Contains1Item (const USBH_DLIST * pList);
void          USBH_DLIST_MoveList      (const USBH_DLIST * pSrcHead,    USBH_DLIST * pDstHead);

const char  * USBH_Basename            (const char *pPath);
void          USBH_LogHexDump          (U32 Type, U32 Len, const void *pvData);
void          USBH_XXLog               (unsigned Tag, unsigned Data);

#if defined(__cplusplus)
  }
#endif

#endif // USBH_UTIL_H

/*************************** End of file ****************************/
