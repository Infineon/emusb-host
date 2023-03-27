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
*       emUSB-Host version: V2.36.1                                  *
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
File        : USBH_MTP.h
Purpose     : API of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_MTP_H
#define USBH_MTP_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
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
#define USBH_MTP_DEFAULT_TIMEOUT      30000
#define USBH_MAX_NUM_STORAGES         4u
//
// Maximum length for strings which are retrieved when USBH_MTP_Open
// is called, longer strings are truncated to this size.
// Smaller values help to save memory.
// The value is in NumChars (U16, Unicode)
//
#define USBH_MTP_DEVINFO_MAX_STR_LEN                        256u
//
// Maximum lengths for other strings.
// All values are in NumChars (U16, Unicode)
//
#define USBH_MTP_STOR_INFO_STORAGE_DESCRIPTION_MAX_STR_LEN  128u
#define USBH_MTP_STOR_INFO_VOLUME_LABEL_MAX_STR_LEN         128u
#define OBJECT_INFO_FILE_NAME_MAX_STR_LEN                   256u
#define OBJECT_INFO_CAPTURE_DATE_MAX_STR_LEN                20u
#define OBJECT_INFO_MODIFICATION_DATE_MAX_STR_LEN           20u

#define USBH_MTP_ROOT_OBJECT_ID               0xFFFFFFFFUL
#define USBH_MTP_ANY_OBJECT_ID                0u
#define USBH_MTP_STORAGE_INDEX_ALL            0xFFu
//
// GetDeviceInfo related defines
//
#define USBH_MTP_DEVINFO_HEADER_SIZE          20u
#define USBH_MTP_DEVINFO_VED_NUM_CHAR_OFS     20u      // Offset for VendorExtensionDesc.NumChars
#define USBH_MTP_DEVINFO_NUM_NATIVE_OPS       35u      // This is fixed for MTP 1.1
#define USBH_MTP_DEVINFO_NUM_NATIVE_EVENTS    18u      // This is fixed for MTP 1.1
#define USBH_MTP_DEVINFO_NUM_NATIVE_DEV_PROPS 42u      // This is fixed for MTP 1.1
#define USBH_MTP_DEVINFO_NUM_NATIVE_CAP_FORM  35u      // This is fixed for MTP 1.1
#define USBH_MTP_DEVINFO_NUM_NATIVE_IMG_FORM  35u      // This is fixed for MTP 1.1

/*********************************************************************
*
*       Typedefs, enums and structs
*
**********************************************************************
*/
/*********************************************************************
*
*       USBH_MTP_DEVICE_HANDLE
*/
typedef U32 USBH_MTP_DEVICE_HANDLE;

/*********************************************************************
*
*       USBH_MTP_DEVICE_INFO
*
*  Description
*    Contains information about an MTP compatible device.
*/
typedef struct {
  U16          VendorId;        // Vendor identification number.
  U16          ProductId;       // Product identification number.
  U8           acSerialNo[255]; // Serial number string.
  USBH_SPEED   Speed;           // The USB speed of the device, see USBH_SPEED.
  const U16  * sManufacturer;   // Pointer to a Unicode string "manufacturer".
  const U16  * sModel;          // Pointer to a Unicode string "model".
  const U16  * sDeviceVersion;  // Pointer to a Unicode string "device version".
  const U16  * sSerialNumber;   // Pointer to a Unicode string "serial number".
} USBH_MTP_DEVICE_INFO;

/*********************************************************************
*
*       USBH_SEND_DATA_FUNC
*
*  Description
*    Definition of the callback which has to be specified when using USBH_MTP_CreateObject().
*
*  Parameters
*    pUserContext     : User context which is passed to the callback.
*    NumBytesSentTotal: This value contains the total number of bytes which have already been transferred
*    pNumBytesToSend  : The user has to set this value to the number of bytes which are inside the buffer.
*    ppNextBuffer     : The user can change this pointer to a different buffer. If this parameter remains
*                       NULL after the callback returns, the previous buffer is re-used (the application
*                       should put new data into the buffer first).
*/
typedef void USBH_SEND_DATA_FUNC(void * pUserContext, U32 NumBytesSentTotal, U32 * pNumBytesToSend, U8 ** ppNextBuffer);

/*********************************************************************
*
*       USBH_RECEIVE_DATA_FUNC
*
*  Description
*    Definition of the callback which has to be specified when using USBH_MTP_ReadFile().
*
*  Parameters
*    pUserContext     : User context which is passed to the callback.
*    NumBytesRemaining: This value contains the total number of bytes which still have to be read.
*    NumBytesInBuffer : The number of bytes which have been read in this transaction.
*    ppNextBuffer     : The user can change this pointer to a different buffer. If this parameter
*                       remains NULL after the callback returns, the previous buffer is re-used
*                       (the application should copy the data out of the buffer first, as it will
*                       be overwritten on the next transaction).
*    pNextBufferSize  : Size of the next buffer. This only needs to be changed when the pNextBuffer parameter is changed.
*/
typedef void USBH_RECEIVE_DATA_FUNC(void * pUserContext, U32 NumBytesRemaining, U32 NumBytesInBuffer, void ** ppNextBuffer, U32 * pNextBufferSize);

/*********************************************************************
*
*       USBH_EVENT_CALLBACK
*
*  Description
*    Definition of the callback which can be set via USBH_MTP_SetEventCallback().
*
*  Parameters
*    EventCode: Code of the MTP event, see USBH_MTP_EVENT_CODES.
*    Para1    : First parameter passed with the event.
*    Para2    : Second parameter passed with the event.
*    Para3    : Third parameter passed with the event.
*
*  Additional information
*    The events USBH_MTP_EVENT_STORE_ADDED and USBH_MTP_EVENT_STORE_REMOVED
*    are handled by the MTP module before being passed to the callback.
*    The storage information for the device is updated automatically
*    when one of these events is received. All events are passed to
*    the callback, this includes vendor specific events which are not
*    present in the USBH_MTP_EVENT_CODES enum. Parameters which are
*    not used with a specific event (e.g. USBH_MTP_EVENT_STORE_ADDED
*    has only one parameter) will be passed as zero.
*    The callback should not block.
*/
typedef void USBH_EVENT_CALLBACK(U16 EventCode, U32 Para1, U32 Para2, U32 Para3);

/*********************************************************************
*
*       USBH_MTP_STORAGE_INFO
*
*  Description
*    Contains information needed to create a new MTP object.
*/
typedef struct {
  U32                   ObjectID;       // Filled by the MTP Module after
                                        // the Object has been created.
                                        // This ID is provided by the device.
  U32                   ParentObjectID; // The ObjectID of the parent object.
                                        // For "root" use USBH_MTP_ROOT_OBJECT_ID.
  U32                   ObjectSize;     // Size of the file in bytes.
  USBH_SEND_DATA_FUNC * pfGetData;      // Pointer to a user-provided callback
                                        // which will provide the data. See USBH_SEND_DATA_FUNC.
  U32                   FileNameSize;   // The length of the file-name string (including termination).
  const U16           * sFileName;      // Pointer to the Unicode file-name string (must include zero-termination).
  U8                    isFolder;       // Flag indicating whether the new object should
                                        // be a folder or not. When creating a folder
                                        // pfGetData, pUserBuf, UserBufSize can be set to NULL.
  U8                  * pUserBuf;       // Pointer to a user buffer where the data is located.
                                        // Can be NULL, in this case the callback is
                                        // called immediately and the buffer has
                                        // to be set inside the callback.
  U32                   UserBufSize;    // Size of the user buffer in bytes.
  void                * pUserContext;   // User context which is passed to the callback.
} USBH_MTP_CREATE_INFO;

/*********************************************************************
*
*       USBH_MTP_STORAGE_INFO
*
*  Description
*    Contains information about an MTP storage.
*/
typedef struct {
  U16      StorageType;         // + 0x0000 Undefined
                                // + 0x0001 Fixed ROM
                                // + 0x0002 Removable ROM
                                // + 0x0003 Fixed RAM
                                // + 0x0004 Removable RAM
                                //
                                // Note: This value is often unreliable
                                // as many devices return 0x0003 for everything.
  U16      FilesystemType;      // + 0x0000 Undefined
                                // + 0x0001 Generic flat
                                // + 0x0002 Generic hierarchical
                                // + 0x0003 DCF
  U16      AccessCapability;    // + 0x0000 Read-write
                                // + 0x0001 Read-only without object deletion
                                // + 0x0002 Read-only with object deletion
  U8       MaxCapacity[8];      // An U64 little-endian value which designates the maximum
                                // capacity of the storage in bytes. The value is declared as an
                                // array of U8, you will have to cast it into a U64 in your
                                // application. If the storage is read-only, this field is optional and
                                // may contain zero.
  U8       FreeSpaceInBytes[8]; // An U64 little-endian value which designates the free space on
                                // the storage in bytes. The value is declared as an array of U8,
                                // you will have to cast it into a U64 in your application. If the
                                // storage is read-only, this field is optional and may contain zero.
  U32      FreeSpaceInImages;   // Value describing the number of images which could still be
                                // fit into the storage. This is a PTP relevant value, for MTP it is
                                // normally zero or 0xFFFFFFFF.
  U16      sStorageDescription[USBH_MTP_STOR_INFO_STORAGE_DESCRIPTION_MAX_STR_LEN];  // Unicode string describing the storage.
  U16      sVolumeLabel[USBH_MTP_STOR_INFO_VOLUME_LABEL_MAX_STR_LEN];                // Unicode string which contains the volume label.
} USBH_MTP_STORAGE_INFO;

/*********************************************************************
*
*       USBH_MTP_OBJECT
*
*  Description
*    Contains basic information about an MTP object.
*/
typedef struct {
  U32 ObjectID;                         // Unique ID for the object, provided by the device.
  U16 ObjectFormat;                     // MTP Object format, see USBH_MTP_OBJECT_FORMAT
  U16 AssociationType;                  // MTP association type, see USBH_MTP_ASSOCIATION_TYPES
} USBH_MTP_OBJECT;

/*********************************************************************
*
*       USBH_MTP_OBJECT_INFO
*
*  Description
*    Contains extended information about an MTP object.
*/
typedef struct {
  U32 StorageID;                        // ID of the storage where the Object is located.
  U16 ObjectFormat;                     // MTP Object format, see USBH_MTP_OBJECT_FORMAT.
  U16 ProtectionStatus;                 // + 0x0000 - No protection
                                        // + 0x0001 - Read-only
                                        // + 0x8002 - Read-only data
                                        // + 0x8003 - Non-transferable data
  U32 ParentObject;                     // ObjectID of the parent Object. For "root" use the define USBH_MTP_ROOT_OBJECT_ID.
  U16 AssociationType;                  // MTP association type, see USBH_MTP_ASSOCIATION_TYPES
  U16 sFilename[OBJECT_INFO_FILE_NAME_MAX_STR_LEN];                   // File name buffer.
  U16 sCaptureDate[OBJECT_INFO_CAPTURE_DATE_MAX_STR_LEN];             // CaptureDate string buffer.
  U16 sModificationDate[OBJECT_INFO_MODIFICATION_DATE_MAX_STR_LEN];   // ModificationDate string buffer.
} USBH_MTP_OBJECT_INFO;

/*********************************************************************
*
*       USBH_MTP_OBJECT_PROP_DESC
*
*  Description
*    Contains information about the data-type and accessibility of an object property.
*/
typedef struct {
  U16 PropertyCode;                     // MTP object property code, see USBH_MTP_OBJECT_PROPERTIES.
  U16 DataType;                         // Data type of the property.
  U8  GetSet;                           // 0 - Read-only, 1 - Read-write.
} USBH_MTP_OBJECT_PROP_DESC;

/*********************************************************************
*
*       USBH_MTP_DEVICE_PROP_DESC
*/
typedef struct {
  U16 PropertyCode;                     // MTP device property code, see USBH_MTP_DEVICE_PROPERTIES
  U16 DataType;                         // Data Type of the property.
  U8  GetSet;                           // 0 - Read-only, 1 - Read-write.
} USBH_MTP_DEVICE_PROP_DESC;

/*********************************************************************
*
*       Codes for data types
*/
#define USBH_MTP_DATA_TYPE_INT8                  0x0001u
#define USBH_MTP_DATA_TYPE_UINT8                 0x0002u
#define USBH_MTP_DATA_TYPE_INT16                 0x0003u
#define USBH_MTP_DATA_TYPE_UINT16                0x0004u
#define USBH_MTP_DATA_TYPE_INT32                 0x0005u
#define USBH_MTP_DATA_TYPE_UINT32                0x0006u
#define USBH_MTP_DATA_TYPE_INT64                 0x0007u
#define USBH_MTP_DATA_TYPE_UINT64                0x0008u
#define USBH_MTP_DATA_TYPE_INT128                0x0009u
#define USBH_MTP_DATA_TYPE_UINT128               0x000Au
#define USBH_MTP_DATA_TYPE_AUINT8                0x4002u
#define USBH_MTP_DATA_TYPE_STR                   0xFFFFu

/*********************************************************************
*
*       USBH_MTP_FUNCTIONAL_MODE
*
*  Description
*    Other values are bit masks:
*    All other values with bit 15 set to 0 - Reserved
*    All other values with bit 15 set to 1 and bit 14 set to 0 - MTP vendor extension
*    All other values with bit 15 set to 1 and bit 14 set to 1 - MTP-defined
*/
typedef enum {
  USBH_MTP_FUNCTIONAL_MODE_STANDARD_MODE,
  USBH_MTP_FUNCTIONAL_MODE_SLEEP_STATE,
  USBH_MTP_FUNCTIONAL_MODE_NON_RESPONSIVE_PLAYBACK = 0xC001,
  USBH_MTP_FUNCTIONAL_MODE_RESPONSIVE_PLAYBACK
} USBH_MTP_FUNCTIONAL_MODE;

/*********************************************************************
*
*       USBH_MTP_EVENT_INDEX
*/
//lint -esym(621,USBH_MTP_EVENT_INDEX_*) N:100
typedef enum {
  USBH_MTP_EVENT_INDEX_UNDEFINED,
  USBH_MTP_EVENT_INDEX_CANCEL_TRANSACTION,
  USBH_MTP_EVENT_INDEX_OBJECT_ADDED,
  USBH_MTP_EVENT_INDEX_OBJECT_REMOVED,
  USBH_MTP_EVENT_INDEX_STORE_ADDED,
  USBH_MTP_EVENT_INDEX_STORE_REMOVED,
  USBH_MTP_EVENT_INDEX_DEVICE_PROP_CHANGED,
  USBH_MTP_EVENT_INDEX_OBJECT_INFO_CHANGED,
  USBH_MTP_EVENT_INDEX_DEVICE_INFO_CHANGED,
  USBH_MTP_EVENT_INDEX_REQUEST_OBJECT_TRANSFER,
  USBH_MTP_EVENT_INDEX_STORE_FULL,
  USBH_MTP_EVENT_INDEX_DEVICE_RESET,
  USBH_MTP_EVENT_INDEX_STORAGE_INFO_CHANGED,
  USBH_MTP_EVENT_INDEX_CAPTURE_COMPLETE,
  USBH_MTP_EVENT_INDEX_UNREPORTED_STATUS,
  USBH_MTP_EVENT_INDEX_OBJECT_PROP_CHANGED,
  USBH_MTP_EVENT_INDEX_OBJECT_PROP_DESC_CHANGED,
  USBH_MTP_EVENT_INDEX_OBJECT_REFERENCES_CHANGED
} USBH_MTP_EVENT_INDEX;

/*********************************************************************
*
*       USBH_MTP_EVENT_CODES
*
*  Description
*    Events are described by a 16-bit code.
*/
typedef enum {
  USBH_MTP_EVENT_UNDEFINED = 0x4000,
  USBH_MTP_EVENT_CANCEL_TRANSACTION,
  USBH_MTP_EVENT_OBJECT_ADDED,
  USBH_MTP_EVENT_OBJECT_REMOVED,
  USBH_MTP_EVENT_STORE_ADDED,
  USBH_MTP_EVENT_STORE_REMOVED,
  USBH_MTP_EVENT_DEVICE_PROP_CHANGED,
  USBH_MTP_EVENT_OBJECT_INFO_CHANGED,
  USBH_MTP_EVENT_DEVICE_INFO_CHANGED,
  USBH_MTP_EVENT_REQUEST_OBJECT_TRANSFER,
  USBH_MTP_EVENT_STORE_FULL,
  USBH_MTP_EVENT_DEVICE_RESET,
  USBH_MTP_EVENT_STORAGE_INFO_CHANGED,
  USBH_MTP_EVENT_CAPTURE_COMPLETE,
  USBH_MTP_EVENT_UNREPORTED_STATUS,
  USBH_MTP_EVENT_OBJECT_PROP_CHANGED = 0xC801,
  USBH_MTP_EVENT_OBJECT_PROP_DESC_CHANGED,
  USBH_MTP_EVENT_OBJECT_REFERENCES_CHANGED
} USBH_MTP_EVENT_CODES;

/*********************************************************************
*
*       USBH_MTP_ASSOCIATION_TYPES
*/
typedef enum {
  USBH_MTP_ASSOCIATION_TYPE_UNDEFINED,
  USBH_MTP_ASSOCIATION_TYPE_GENERIC_FOLDER
} USBH_MTP_ASSOCIATION_TYPES;

/*********************************************************************
*
*       USBH_MTP_OBJECT_PROPERTIES
*
*  Description
*    Object properties identify settings or state conditions of
*    files and folders (objects).
*/
//lint -esym(621,USBH_MTP_OBJECT_PROP_*) N:100
typedef enum {
  USBH_MTP_OBJECT_PROP_STORAGE_ID                            = 0xDC01,
  USBH_MTP_OBJECT_PROP_OBJECT_FORMAT                         = 0xDC02,
  USBH_MTP_OBJECT_PROP_PROTECTION_STATUS                     = 0xDC03,
  USBH_MTP_OBJECT_PROP_OBJECT_SIZE                           = 0xDC04,
  USBH_MTP_OBJECT_PROP_ASSOCIATION_TYPE                      = 0xDC05,
  USBH_MTP_OBJECT_PROP_ASSOCIATION_DESC                      = 0xDC06,
  USBH_MTP_OBJECT_PROP_OBJECT_FILE_NAME                      = 0xDC07,
  USBH_MTP_OBJECT_PROP_DATE_CREATED                          = 0xDC08,
  USBH_MTP_OBJECT_PROP_DATE_MODIFIED                         = 0xDC09,
  USBH_MTP_OBJECT_PROP_KEYWORDS                              = 0xDC0A,
  USBH_MTP_OBJECT_PROP_PARENT_OBJECT                         = 0xDC0B,
  USBH_MTP_OBJECT_PROP_ALLOWED_FOLDER_CONTENTS               = 0xDC0C,
  USBH_MTP_OBJECT_PROP_HIDDEN                                = 0xDC0D,
  USBH_MTP_OBJECT_PROP_SYSTEM_OBJECT                         = 0xDC0E,
  USBH_MTP_OBJECT_PROP_PERSISTENT_UNIQUE_OBJECT_IDENTIFIER   = 0xDC41,
  USBH_MTP_OBJECT_PROP_SYNCID                                = 0xDC42,
  USBH_MTP_OBJECT_PROP_PROPERTY_BAG                          = 0xDC43,
  USBH_MTP_OBJECT_PROP_NAME                                  = 0xDC44,
  USBH_MTP_OBJECT_PROP_CREATED_BY                            = 0xDC45,
  USBH_MTP_OBJECT_PROP_ARTIST                                = 0xDC46,
  USBH_MTP_OBJECT_PROP_DATE_AUTHORED                         = 0xDC47,
  USBH_MTP_OBJECT_PROP_DESCRIPTION                           = 0xDC48,
  USBH_MTP_OBJECT_PROP_URL_REFERENCE                         = 0xDC49,
  USBH_MTP_OBJECT_PROP_LANGUAGELOCALE                        = 0xDC4A,
  USBH_MTP_OBJECT_PROP_COPYRIGHT_INFORMATION                 = 0xDC4B,
  USBH_MTP_OBJECT_PROP_SOURCE                                = 0xDC4C,
  USBH_MTP_OBJECT_PROP_ORIGIN_LOCATION                       = 0xDC4D,
  USBH_MTP_OBJECT_PROP_DATE_ADDED                            = 0xDC4E,
  USBH_MTP_OBJECT_PROP_NON_CONSUMABLE                        = 0xDC4F,
  USBH_MTP_OBJECT_PROP_CORRUPTUNPLAYABLE                     = 0xDC50,
  USBH_MTP_OBJECT_PROP_PRODUCERSERIALNUMBER                  = 0xDC51,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_FORMAT          = 0xDC81,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_SIZE            = 0xDC82,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_HEIGHT          = 0xDC83,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_WIDTH           = 0xDC84,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_DURATION        = 0xDC85,
  USBH_MTP_OBJECT_PROP_REPRESENTATIVE_SAMPLE_DATA            = 0xDC86,
  USBH_MTP_OBJECT_PROP_WIDTH                                 = 0xDC87,
  USBH_MTP_OBJECT_PROP_HEIGHT                                = 0xDC88,
  USBH_MTP_OBJECT_PROP_DURATION                              = 0xDC89,
  USBH_MTP_OBJECT_PROP_RATING                                = 0xDC8A,
  USBH_MTP_OBJECT_PROP_TRACK                                 = 0xDC8B,
  USBH_MTP_OBJECT_PROP_GENRE                                 = 0xDC8C,
  USBH_MTP_OBJECT_PROP_CREDITS                               = 0xDC8D,
  USBH_MTP_OBJECT_PROP_LYRICS                                = 0xDC8E,
  USBH_MTP_OBJECT_PROP_SUBSCRIPTION_CONTENT_ID               = 0xDC8F,
  USBH_MTP_OBJECT_PROP_PRODUCED_BY                           = 0xDC90,
  USBH_MTP_OBJECT_PROP_USE_COUNT                             = 0xDC91,
  USBH_MTP_OBJECT_PROP_SKIP_COUNT                            = 0xDC92,
  USBH_MTP_OBJECT_PROP_LAST_ACCESSED                         = 0xDC93,
  USBH_MTP_OBJECT_PROP_PARENTAL_RATING                       = 0xDC94,
  USBH_MTP_OBJECT_PROP_META_GENRE                            = 0xDC95,
  USBH_MTP_OBJECT_PROP_COMPOSER                              = 0xDC96,
  USBH_MTP_OBJECT_PROP_EFFECTIVE_RATING                      = 0xDC97,
  USBH_MTP_OBJECT_PROP_SUBTITLE                              = 0xDC98,
  USBH_MTP_OBJECT_PROP_ORIGINAL_RELEASE_DATE                 = 0xDC99,
  USBH_MTP_OBJECT_PROP_ALBUM_NAME                            = 0xDC9A,
  USBH_MTP_OBJECT_PROP_ALBUM_ARTIST                          = 0xDC9B,
  USBH_MTP_OBJECT_PROP_MOOD                                  = 0xDC9C,
  USBH_MTP_OBJECT_PROP_DRM_STATUS                            = 0xDC9D,
  USBH_MTP_OBJECT_PROP_SUB_DESCRIPTION                       = 0xDC9E,
  USBH_MTP_OBJECT_PROP_IS_CROPPED                            = 0xDCD1,
  USBH_MTP_OBJECT_PROP_IS_COLOUR_CORRECTED                   = 0xDCD2,
  USBH_MTP_OBJECT_PROP_IMAGE_BIT_DEPTH                       = 0xDCD3,
  USBH_MTP_OBJECT_PROP_FNUMBER                               = 0xDCD4,
  USBH_MTP_OBJECT_PROP_EXPOSURE_TIME                         = 0xDCD5,
  USBH_MTP_OBJECT_PROP_EXPOSURE_INDEX                        = 0xDCD6,
  USBH_MTP_OBJECT_PROP_TOTAL_BITRATE                         = 0xDE91,
  USBH_MTP_OBJECT_PROP_BITRATE_TYPE                          = 0xDE92,
  USBH_MTP_OBJECT_PROP_SAMPLE_RATE                           = 0xDE93,
  USBH_MTP_OBJECT_PROP_NUMBER_OF_CHANNELS                    = 0xDE94,
  USBH_MTP_OBJECT_PROP_AUDIO_BITDEPTH                        = 0xDE95,
  USBH_MTP_OBJECT_PROP_SCAN_TYPE                             = 0xDE97,
  USBH_MTP_OBJECT_PROP_AUDIO_WAVE_CODEC                      = 0xDE99,
  USBH_MTP_OBJECT_PROP_AUDIO_BITRATE                         = 0xDE9A,
  USBH_MTP_OBJECT_PROP_VIDEO_FOURCC_CODEC                    = 0xDE9B,
  USBH_MTP_OBJECT_PROP_VIDEO_BITRATE                         = 0xDE9C,
  USBH_MTP_OBJECT_PROP_FRAMES_PER_THOUSAND_SECONDS           = 0xDE9D,
  USBH_MTP_OBJECT_PROP_KEYFRAME_DISTANCE                     = 0xDE9E,
  USBH_MTP_OBJECT_PROP_BUFFER_SIZE                           = 0xDE9F,
  USBH_MTP_OBJECT_PROP_ENCODING_QUALITY                      = 0xDEA0,
  USBH_MTP_OBJECT_PROP_ENCODING_PROFILE                      = 0xDEA1,
  USBH_MTP_OBJECT_PROP_DISPLAY_NAME                          = 0xDCE0,
  USBH_MTP_OBJECT_PROP_BODY_TEXT                             = 0xDCE1,
  USBH_MTP_OBJECT_PROP_SUBJECT                               = 0xDCE2,
  USBH_MTP_OBJECT_PROP_PRIORITY                              = 0xDCE3,
  USBH_MTP_OBJECT_PROP_GIVEN_NAME                            = 0xDD00,
  USBH_MTP_OBJECT_PROP_MIDDLE_NAMES                          = 0xDD01,
  USBH_MTP_OBJECT_PROP_FAMILY_NAME                           = 0xDD02,
  USBH_MTP_OBJECT_PROP_PREFIX                                = 0xDD03,
  USBH_MTP_OBJECT_PROP_SUFFIX                                = 0xDD04,
  USBH_MTP_OBJECT_PROP_PHONETIC_GIVEN_NAME                   = 0xDD05,
  USBH_MTP_OBJECT_PROP_PHONETIC_FAMILY_NAME                  = 0xDD06,
  USBH_MTP_OBJECT_PROP_EMAIL_PRIMARY                         = 0xDD07,
  USBH_MTP_OBJECT_PROP_EMAIL_PERSONAL_1                      = 0xDD08,
  USBH_MTP_OBJECT_PROP_EMAIL_PERSONAL_2                      = 0xDD09,
  USBH_MTP_OBJECT_PROP_EMAIL_BUSINESS_1                      = 0xDD0A,
  USBH_MTP_OBJECT_PROP_EMAIL_BUSINESS_2                      = 0xDD0B,
  USBH_MTP_OBJECT_PROP_EMAIL_OTHERS                          = 0xDD0C,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_PRIMARY                  = 0xDD0D,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_PERSONAL                 = 0xDD0E,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_PERSONAL_2               = 0xDD0F,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_BUSINESS                 = 0xDD10,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_BUSINESS_2               = 0xDD11,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_MOBILE                   = 0xDD12,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_MOBILE_2                 = 0xDD13,
  USBH_MTP_OBJECT_PROP_FAX_NUMBER_PRIMARY                    = 0xDD14,
  USBH_MTP_OBJECT_PROP_FAX_NUMBER_PERSONAL                   = 0xDD15,
  USBH_MTP_OBJECT_PROP_FAX_NUMBER_BUSINESS                   = 0xDD16,
  USBH_MTP_OBJECT_PROP_PAGER_NUMBER                          = 0xDD17,
  USBH_MTP_OBJECT_PROP_PHONE_NUMBER_OTHERS                   = 0xDD18,
  USBH_MTP_OBJECT_PROP_PRIMARY_WEB_ADDRESS                   = 0xDD19,
  USBH_MTP_OBJECT_PROP_PERSONAL_WEB_ADDRESS                  = 0xDD1A,
  USBH_MTP_OBJECT_PROP_BUSINESS_WEB_ADDRESS                  = 0xDD1B,
  USBH_MTP_OBJECT_PROP_INSTANT_MESSENGER_ADDRESS             = 0xDD1C,
  USBH_MTP_OBJECT_PROP_INSTANT_MESSENGER_ADDRESS_2           = 0xDD1D,
  USBH_MTP_OBJECT_PROP_INSTANT_MESSENGER_ADDRESS_3           = 0xDD1E,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_FULL          = 0xDD1F,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_LINE_1        = 0xDD20,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_LINE_2        = 0xDD21,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_CITY          = 0xDD22,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_REGION        = 0xDD23,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_POSTAL_CODE   = 0xDD24,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_PERSONAL_COUNTRY       = 0xDD25,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_FULL          = 0xDD26,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_LINE_1        = 0xDD27,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_LINE_2        = 0xDD28,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_CITY          = 0xDD29,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_REGION        = 0xDD2A,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_POSTAL_CODE   = 0xDD2B,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_BUSINESS_COUNTRY       = 0xDD2C,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_FULL             = 0xDD2D,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_LINE_1           = 0xDD2E,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_LINE_2           = 0xDD2F,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_CITY             = 0xDD30,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_REGION           = 0xDD31,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_POSTAL_CODE      = 0xDD32,
  USBH_MTP_OBJECT_PROP_POSTAL_ADDRESS_OTHER_COUNTRY          = 0xDD33,
  USBH_MTP_OBJECT_PROP_ORGANIZATION_NAME                     = 0xDD34,
  USBH_MTP_OBJECT_PROP_PHONETIC_ORGANIZATION_NAME            = 0xDD35,
  USBH_MTP_OBJECT_PROP_ROLE                                  = 0xDD36,
  USBH_MTP_OBJECT_PROP_BIRTHDATE                             = 0xDD37,
  USBH_MTP_OBJECT_PROP_MESSAGE_TO                            = 0xDD40,
  USBH_MTP_OBJECT_PROP_MESSAGE_CC                            = 0xDD41,
  USBH_MTP_OBJECT_PROP_MESSAGE_BCC                           = 0xDD42,
  USBH_MTP_OBJECT_PROP_MESSAGE_READ                          = 0xDD43,
  USBH_MTP_OBJECT_PROP_MESSAGE_RECEIVED_TIME                 = 0xDD44,
  USBH_MTP_OBJECT_PROP_MESSAGE_SENDER                        = 0xDD45,
  USBH_MTP_OBJECT_PROP_ACTIVITY_BEGIN_TIME                   = 0xDD50,
  USBH_MTP_OBJECT_PROP_ACTIVITY_END_TIME                     = 0xDD51,
  USBH_MTP_OBJECT_PROP_ACTIVITY_LOCATION                     = 0xDD52,
  USBH_MTP_OBJECT_PROP_ACTIVITY_REQUIRED_ATTENDEES           = 0xDD54,
  USBH_MTP_OBJECT_PROP_ACTIVITY_OPTIONAL_ATTENDEES           = 0xDD55,
  USBH_MTP_OBJECT_PROP_ACTIVITY_RESOURCES                    = 0xDD56,
  USBH_MTP_OBJECT_PROP_ACTIVITY_ACCEPTED                     = 0xDD57,
  USBH_MTP_OBJECT_PROP_OWNER                                 = 0xDD5D,
  USBH_MTP_OBJECT_PROP_EDITOR                                = 0xDD5E,
  USBH_MTP_OBJECT_PROP_WEBMASTER                             = 0xDD5F,
  USBH_MTP_OBJECT_PROP_URL_SOURCE                            = 0xDD60,
  USBH_MTP_OBJECT_PROP_URL_DESTINATION                       = 0xDD61,
  USBH_MTP_OBJECT_PROP_TIME_BOOKMARK                         = 0xDD62,
  USBH_MTP_OBJECT_PROP_OBJECT_BOOKMARK                       = 0xDD63,
  USBH_MTP_OBJECT_PROP_BYTE_BOOKMARK                         = 0xDD64,
  USBH_MTP_OBJECT_PROP_LAST_BUILD_DATE                       = 0xDD70,
  USBH_MTP_OBJECT_PROP_TIME_TO_LIVE                          = 0xDD71,
  USBH_MTP_OBJECT_PROP_MEDIA_GUID                            = 0xDD72
} USBH_MTP_OBJECT_PROPERTIES;

/*********************************************************************
*
*       USBH_MTP_DEVICE_PROP_INDEX
*/
//lint -esym(621,USBH_MTP_DEVICE_PROP_*) N:100
typedef enum {
  USBH_MTP_DEVICE_PROP_INDEX_UNDEFINED,
  USBH_MTP_DEVICE_PROP_INDEX_BATTERY_LEVEL,
  USBH_MTP_DEVICE_PROP_INDEX_FUNCTIONAL_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_IMAGE_SIZE,
  USBH_MTP_DEVICE_PROP_INDEX_COMPRESSION_SETTING,
  USBH_MTP_DEVICE_PROP_INDEX_WHITE_BALANCE,
  USBH_MTP_DEVICE_PROP_INDEX_RGB_GAIN,
  USBH_MTP_DEVICE_PROP_INDEX_F_NUMBER,
  USBH_MTP_DEVICE_PROP_INDEX_FOCAL_LENGTH,
  USBH_MTP_DEVICE_PROP_INDEX_FOCUS_DISTANCE,
  USBH_MTP_DEVICE_PROP_INDEX_FOCUS_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_EXPOSURE_METERING_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_FLASH_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_EXPOSURE_TIME,
  USBH_MTP_DEVICE_PROP_INDEX_EXPOSURE_PROGRAM_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_EXPOSURE_INDEX,
  USBH_MTP_DEVICE_PROP_INDEX_EXPOSURE_BIAS_COMPENSATION,
  USBH_MTP_DEVICE_PROP_INDEX_DATETIME,
  USBH_MTP_DEVICE_PROP_INDEX_CAPTURE_DELAY,
  USBH_MTP_DEVICE_PROP_INDEX_STILL_CAPTURE_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_CONTRAST,
  USBH_MTP_DEVICE_PROP_INDEX_SHARPNESS,
  USBH_MTP_DEVICE_PROP_INDEX_DIGITAL_ZOOM,
  USBH_MTP_DEVICE_PROP_INDEX_EFFECT_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_BURST_NUMBER,
  USBH_MTP_DEVICE_PROP_INDEX_BURST_INTERVAL,
  USBH_MTP_DEVICE_PROP_INDEX_TIMELAPSE_NUMBER,
  USBH_MTP_DEVICE_PROP_INDEX_TIMELAPSE_INTERVAL,
  USBH_MTP_DEVICE_PROP_INDEX_FOCUS_METERING_MODE,
  USBH_MTP_DEVICE_PROP_INDEX_UPLOAD_URL,
  USBH_MTP_DEVICE_PROP_INDEX_ARTIST,
  USBH_MTP_DEVICE_PROP_INDEX_COPYRIGHT_INFO,
  USBH_MTP_DEVICE_PROP_INDEX_SYNCHRONIZATION_PARTNER,
  USBH_MTP_DEVICE_PROP_INDEX_DEVICE_FRIENDLY_NAME,
  USBH_MTP_DEVICE_PROP_INDEX_VOLUME,
  USBH_MTP_DEVICE_PROP_INDEX_SUPPORTEDFORMATSORDERED,
  USBH_MTP_DEVICE_PROP_INDEX_DEVICEICON,
  USBH_MTP_DEVICE_PROP_INDEX_PLAYBACK_RATE,
  USBH_MTP_DEVICE_PROP_INDEX_PLAYBACK_OBJECT,
  USBH_MTP_DEVICE_PROP_INDEX_PLAYBACK_CONTAINER,
  USBH_MTP_DEVICE_PROP_INDEX_SESSION_INITIATOR_VERSION_INFO,
  USBH_MTP_DEVICE_PROP_INDEX_PERCEIVED_DEVICE_TYPE
} USBH_MTP_DEVICE_PROP_INDEX;

/*********************************************************************
*
*       USBH_MTP_DEVICE_PROPERTIES
*
*  Description
*    Device properties describe conditions or setting relevant to
*    the device itself. The properties are unrelated to objects.
*/
typedef enum {
  USBH_MTP_DEVICE_PROP_UNDEFINED                             = 0x5000,
  USBH_MTP_DEVICE_PROP_BATTERY_LEVEL                         = 0x5001,
  USBH_MTP_DEVICE_PROP_FUNCTIONAL_MODE                       = 0x5002,
  USBH_MTP_DEVICE_PROP_IMAGE_SIZE                            = 0x5003,
  USBH_MTP_DEVICE_PROP_COMPRESSION_SETTING                   = 0x5004,
  USBH_MTP_DEVICE_PROP_WHITE_BALANCE                         = 0x5005,
  USBH_MTP_DEVICE_PROP_RGB_GAIN                              = 0x5006,
  USBH_MTP_DEVICE_PROP_F_NUMBER                              = 0x5007,
  USBH_MTP_DEVICE_PROP_FOCAL_LENGTH                          = 0x5008,
  USBH_MTP_DEVICE_PROP_FOCUS_DISTANCE                        = 0x5009,
  USBH_MTP_DEVICE_PROP_FOCUS_MODE                            = 0x500A,
  USBH_MTP_DEVICE_PROP_EXPOSURE_METERING_MODE                = 0x500B,
  USBH_MTP_DEVICE_PROP_FLASH_MODE                            = 0x500C,
  USBH_MTP_DEVICE_PROP_EXPOSURE_TIME                         = 0x500D,
  USBH_MTP_DEVICE_PROP_EXPOSURE_PROGRAM_MODE                 = 0x500E,
  USBH_MTP_DEVICE_PROP_EXPOSURE_INDEX                        = 0x500F,
  USBH_MTP_DEVICE_PROP_EXPOSURE_BIAS_COMPENSATION            = 0x5010,
  USBH_MTP_DEVICE_PROP_DATETIME                              = 0x5011,
  USBH_MTP_DEVICE_PROP_CAPTURE_DELAY                         = 0x5012,
  USBH_MTP_DEVICE_PROP_STILL_CAPTURE_MODE                    = 0x5013,
  USBH_MTP_DEVICE_PROP_CONTRAST                              = 0x5014,
  USBH_MTP_DEVICE_PROP_SHARPNESS                             = 0x5015,
  USBH_MTP_DEVICE_PROP_DIGITAL_ZOOM                          = 0x5016,
  USBH_MTP_DEVICE_PROP_EFFECT_MODE                           = 0x5017,
  USBH_MTP_DEVICE_PROP_BURST_NUMBER                          = 0x5018,
  USBH_MTP_DEVICE_PROP_BURST_INTERVAL                        = 0x5019,
  USBH_MTP_DEVICE_PROP_TIMELAPSE_NUMBER                      = 0x501A,
  USBH_MTP_DEVICE_PROP_TIMELAPSE_INTERVAL                    = 0x501B,
  USBH_MTP_DEVICE_PROP_FOCUS_METERING_MODE                   = 0x501C,
  USBH_MTP_DEVICE_PROP_UPLOAD_URL                            = 0x501D,
  USBH_MTP_DEVICE_PROP_ARTIST                                = 0x501E,
  USBH_MTP_DEVICE_PROP_COPYRIGHT_INFO                        = 0x501F,
  USBH_MTP_DEVICE_PROP_SYNCHRONIZATION_PARTNER               = 0xD401,
  USBH_MTP_DEVICE_PROP_DEVICE_FRIENDLY_NAME                  = 0xD402,
  USBH_MTP_DEVICE_PROP_VOLUME                                = 0xD403,
  USBH_MTP_DEVICE_PROP_SUPPORTEDFORMATSORDERED               = 0xD404,
  USBH_MTP_DEVICE_PROP_DEVICEICON                            = 0xD405,
  USBH_MTP_DEVICE_PROP_PLAYBACK_RATE                         = 0xD410,
  USBH_MTP_DEVICE_PROP_PLAYBACK_OBJECT                       = 0xD411,
  USBH_MTP_DEVICE_PROP_PLAYBACK_CONTAINER                    = 0xD412,
  USBH_MTP_DEVICE_PROP_SESSION_INITIATOR_VERSION_INFO        = 0xD406,
  USBH_MTP_DEVICE_PROP_PERCEIVED_DEVICE_TYPE                 = 0xD407
} USBH_MTP_DEVICE_PROPERTIES;

/*********************************************************************
*
*       USBH_MTP_OPERATION_INDEX
*/
//lint -esym(621,USBH_MTP_OPERATION_*) N:100
typedef enum {
  USBH_MTP_OPERATION_INDEX_GET_DEVICE_INFO,
  USBH_MTP_OPERATION_INDEX_OPEN_SESSION,
  USBH_MTP_OPERATION_INDEX_CLOSE_SESSION,
  USBH_MTP_OPERATION_INDEX_GET_STORAGE_IDS,
  USBH_MTP_OPERATION_INDEX_GET_STORAGE_INFO,
  USBH_MTP_OPERATION_INDEX_GET_NUM_OBJECTS,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_HANDLES,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_INFO,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT,
  USBH_MTP_OPERATION_INDEX_GET_THUMB,
  USBH_MTP_OPERATION_INDEX_DELETE_OBJECT,
  USBH_MTP_OPERATION_INDEX_SEND_OBJECT_INFO,
  USBH_MTP_OPERATION_INDEX_SEND_OBJECT,
  USBH_MTP_OPERATION_INDEX_INITIATECAPTURE,
  USBH_MTP_OPERATION_INDEX_FORMAT_STORE,
  USBH_MTP_OPERATION_INDEX_RESET_DEVICE,
  USBH_MTP_OPERATION_INDEX_SELF_TEST,
  USBH_MTP_OPERATION_INDEX_SET_OBJECT_PROTECTION,
  USBH_MTP_OPERATION_INDEX_POWER_DOWN,
  USBH_MTP_OPERATION_INDEX_GET_DEVICE_PROP_DESC,
  USBH_MTP_OPERATION_INDEX_GET_DEVICE_PROP_VALUE,
  USBH_MTP_OPERATION_INDEX_SET_DEVICE_PROP_VALUE,
  USBH_MTP_OPERATION_INDEX_RESET_DEVICE_PROP_VALUE,
  USBH_MTP_OPERATION_INDEX_TERMINATE_OPEN_CAPTURE,
  USBH_MTP_OPERATION_INDEX_MOVE_OBJECT,
  USBH_MTP_OPERATION_INDEX_COPY_OBJECT,
  USBH_MTP_OPERATION_INDEX_GET_PARTIAL_OBJECT,
  USBH_MTP_OPERATION_INDEX_INITIATE_OPEN_CAPTURE,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_PROPS_SUPPORTED,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_PROP_DESC,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_PROP_VALUE,
  USBH_MTP_OPERATION_INDEX_SET_OBJECT_PROP_VALUE,
  USBH_MTP_OPERATION_INDEX_SET_OBJECT_PROP_LIST,
  USBH_MTP_OPERATION_INDEX_SEND_OBJECT_PROP_LIST,
  USBH_MTP_OPERATION_INDEX_GET_OBJECT_REFERENCES,
  USBH_MTP_OPERATION_INDEX_SET_OBJECT_REFERENCES,
  USBH_MTP_OPERATION_INDEX_SKIP
} USBH_MTP_OPERATION_INDEX;

/*********************************************************************
*
*       USBH_MTP_OPERATION
*/
typedef unsigned int USBH_MTP_OPERATION;

/*********************************************************************
*
*       USBH_MTP_OPERATION_CODES
*
*  Description
*    16-bit codes used in operation requests.
*/
typedef enum {
  USBH_MTP_OPERATION_GET_DEVICE_INFO                         = 0x1001,
  USBH_MTP_OPERATION_OPEN_SESSION                            = 0x1002,
  USBH_MTP_OPERATION_CLOSE_SESSION                           = 0x1003,
  USBH_MTP_OPERATION_GET_STORAGE_IDS                         = 0x1004,
  USBH_MTP_OPERATION_GET_STORAGE_INFO                        = 0x1005,
  USBH_MTP_OPERATION_GET_NUM_OBJECTS                         = 0x1006,
  USBH_MTP_OPERATION_GET_OBJECT_HANDLES                      = 0x1007,
  USBH_MTP_OPERATION_GET_OBJECT_INFO                         = 0x1008,
  USBH_MTP_OPERATION_GET_OBJECT                              = 0x1009,
  USBH_MTP_OPERATION_GET_THUMB                               = 0x100A,
  USBH_MTP_OPERATION_DELETE_OBJECT                           = 0x100B,
  USBH_MTP_OPERATION_SEND_OBJECT_INFO                        = 0x100C,
  USBH_MTP_OPERATION_SEND_OBJECT                             = 0x100D,
  USBH_MTP_OPERATION_INITIATECAPTURE                         = 0x100E,
  USBH_MTP_OPERATION_FORMAT_STORE                            = 0x100F,
  USBH_MTP_OPERATION_RESET_DEVICE                            = 0x1010,
  USBH_MTP_OPERATION_SELF_TEST                               = 0x1011,
  USBH_MTP_OPERATION_SET_OBJECT_PROTECTION                   = 0x1012,
  USBH_MTP_OPERATION_POWER_DOWN                              = 0x1013,
  USBH_MTP_OPERATION_GET_DEVICE_PROP_DESC                    = 0x1014,
  USBH_MTP_OPERATION_GET_DEVICE_PROP_VALUE                   = 0x1015,
  USBH_MTP_OPERATION_SET_DEVICE_PROP_VALUE                   = 0x1016,
  USBH_MTP_OPERATION_RESET_DEVICE_PROP_VALUE                 = 0x1017,
  USBH_MTP_OPERATION_TERMINATE_OPEN_CAPTURE                  = 0x1018,
  USBH_MTP_OPERATION_MOVE_OBJECT                             = 0x1019,
  USBH_MTP_OPERATION_COPY_OBJECT                             = 0x101A,
  USBH_MTP_OPERATION_GET_PARTIAL_OBJECT                      = 0x101B,
  USBH_MTP_OPERATION_INITIATE_OPEN_CAPTURE                   = 0x101C,
  USBH_MTP_OPERATION_GET_OBJECT_PROPS_SUPPORTED              = 0x9801,
  USBH_MTP_OPERATION_GET_OBJECT_PROP_DESC                    = 0x9802,
  USBH_MTP_OPERATION_GET_OBJECT_PROP_VALUE                   = 0x9803,
  USBH_MTP_OPERATION_SET_OBJECT_PROP_VALUE                   = 0x9804,
  USBH_MTP_OPERATION_SET_OBJECT_PROP_LIST                    = 0x9806,
  USBH_MTP_OPERATION_SEND_OBJECT_PROP_LIST                   = 0x9808,
  USBH_MTP_OPERATION_GET_OBJECT_REFERENCES                   = 0x9810,
  USBH_MTP_OPERATION_SET_OBJECT_REFERENCES                   = 0x9811,
  USBH_MTP_OPERATION_SKIP                                    = 0x9820
} USBH_MTP_OPERATION_CODES;

/*********************************************************************
*
*       USBH_MTP_RESPONSE_CODES
*
*  Description
*    Possible response codes reported by the device upon completion of
*    an operation.
*/
//lint -esym(621,USBH_MTP_RESPONSE_*) N:100
typedef enum {
  USBH_MTP_RESPONSE_UNDEFINED                                = 0x2000,
  USBH_MTP_RESPONSE_OK                                       = 0x2001,
  USBH_MTP_RESPONSE_GENERAL_ERROR                            = 0x2002,
  USBH_MTP_RESPONSE_PARAMETER_NOT_SUPPORTED                  = 0x2006,
  USBH_MTP_RESPONSE_INVALID_STORAGE_ID                       = 0x2008,
  USBH_MTP_RESPONSE_INVALID_OBJECT_HANDLE                    = 0x2009,
  USBH_MTP_RESPONSE_DEVICEPROP_NOT_SUPPORTED                 = 0x200A,
  USBH_MTP_RESPONSE_STORE_FULL                               = 0x200C,
  USBH_MTP_RESPONSE_STORE_NOT_AVAILABLE                      = 0x2013,
  USBH_MTP_RESPONSE_SPECIFICATION_BY_FORMAT_NOT_SUPPORTED    = 0x2014,
  USBH_MTP_RESPONSE_NO_VALID_OBJECT_INFO                     = 0x2015,
  USBH_MTP_RESPONSE_DEVICE_BUSY                              = 0x2019,
  USBH_MTP_RESPONSE_INVALID_PARENT_OBJECT                    = 0x201A,
  USBH_MTP_RESPONSE_INVALID_PARAMETER                        = 0x201D,
  USBH_MTP_RESPONSE_SESSION_ALREADY_OPEN                     = 0x201E,
  USBH_MTP_RESPONSE_TRANSACTION_CANCELLED                    = 0x201F,
  USBH_MTP_RESPONSE_INVALID_OBJECT_PROP_CODE                 = 0xA801,
  USBH_MTP_RESPONSE_SPECIFICATION_BY_GROUP_UNSUPPORTED       = 0xA807,
  USBH_MTP_RESPONSE_OBJECT_PROP_NOT_SUPPORTED                = 0xA80A
} USBH_MTP_RESPONSE_CODES;

/*********************************************************************
*
*       Container Types
*/
#define USBH_MTP_CT_UNDEFINED 0u
#define USBH_MTP_CT_COMMAND   1u
#define USBH_MTP_CT_DATA      2u
#define USBH_MTP_CT_RESPONSE  3u
#define USBH_MTP_CT_EVENT     4u

/*********************************************************************
*
*       Object format codes
*/
//lint -esym(621,USBH_MTP_OBJECT_FORMAT_*) N:100
typedef enum {
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED,
  USBH_MTP_OBJECT_FORMAT_INDEX_ASSOCIATION,
  USBH_MTP_OBJECT_FORMAT_INDEX_SCRIPT,
  USBH_MTP_OBJECT_FORMAT_INDEX_EXECUTABLE,
  USBH_MTP_OBJECT_FORMAT_INDEX_TEXT,
  USBH_MTP_OBJECT_FORMAT_INDEX_HTML,
  USBH_MTP_OBJECT_FORMAT_INDEX_DPOF,
  USBH_MTP_OBJECT_FORMAT_INDEX_AIFF,
  USBH_MTP_OBJECT_FORMAT_INDEX_WAV,
  USBH_MTP_OBJECT_FORMAT_INDEX_MP3,
  USBH_MTP_OBJECT_FORMAT_INDEX_AVI,
  USBH_MTP_OBJECT_FORMAT_INDEX_MPEG,
  USBH_MTP_OBJECT_FORMAT_INDEX_ASF,
  USBH_MTP_OBJECT_FORMAT_INDEX_DEFINED,
  USBH_MTP_OBJECT_FORMAT_INDEX_EXIF_JPEG,
  USBH_MTP_OBJECT_FORMAT_INDEX_TIFF_EP,
  USBH_MTP_OBJECT_FORMAT_INDEX_FLASHPIX,
  USBH_MTP_OBJECT_FORMAT_INDEX_BMP,
  USBH_MTP_OBJECT_FORMAT_INDEX_CIFF,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED2,
  USBH_MTP_OBJECT_FORMAT_INDEX_GIF,
  USBH_MTP_OBJECT_FORMAT_INDEX_JFIF,
  USBH_MTP_OBJECT_FORMAT_INDEX_CD,
  USBH_MTP_OBJECT_FORMAT_INDEX_PICT,
  USBH_MTP_OBJECT_FORMAT_INDEX_PNG,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED3,
  USBH_MTP_OBJECT_FORMAT_INDEX_TIFF,
  USBH_MTP_OBJECT_FORMAT_INDEX_TIFF_IT,
  USBH_MTP_OBJECT_FORMAT_INDEX_JP2,
  USBH_MTP_OBJECT_FORMAT_INDEX_JPX,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_FIRMWARE,
  USBH_MTP_OBJECT_FORMAT_INDEX_WINDOWS_IMAGE_FORMAT,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_AUDIO,
  USBH_MTP_OBJECT_FORMAT_INDEX_WMA,
  USBH_MTP_OBJECT_FORMAT_INDEX_OGG,
  USBH_MTP_OBJECT_FORMAT_INDEX_AAC,
  USBH_MTP_OBJECT_FORMAT_INDEX_AUDIBLE,
  USBH_MTP_OBJECT_FORMAT_INDEX_FLAC,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_VIDEO,
  USBH_MTP_OBJECT_FORMAT_INDEX_WMV,
  USBH_MTP_OBJECT_FORMAT_INDEX_MP4_CONTAINER,
  USBH_MTP_OBJECT_FORMAT_INDEX_MP2,
  USBH_MTP_OBJECT_FORMAT_INDEX_3GP_CONTAINER,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_MULTIMEDIA_ALBUM,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_IMAGE_ALBUM,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_AUDIO_ALBUM,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_VIDEO_ALBUM,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_AUDIO_VIDEO_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_CONTACT_GROUP,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_MESSAGE_FOLDER,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_CHAPTERED_PRODUCTION,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_AUDIO_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_VIDEO_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_MEDIACAST,
  USBH_MTP_OBJECT_FORMAT_INDEX_WPL_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_M3U_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_MPL_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_ASX_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_PLS_PLAYLIST,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_DOCUMENT,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_DOCUMENT,
  USBH_MTP_OBJECT_FORMAT_INDEX_XML_DOCUMENT,
  USBH_MTP_OBJECT_FORMAT_INDEX_MICROSOFT_WORD_DOCUMENT,
  USBH_MTP_OBJECT_FORMAT_INDEX_MHT_COMPILED_HTML_DOCUMENT,
  USBH_MTP_OBJECT_FORMAT_INDEX_MICROSOFT_EXCEL_SPREADSHEET,
  USBH_MTP_OBJECT_FORMAT_INDEX_MICROSOFT_POWERPOINT_PRESENTATION,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_MESSAGE,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_MESSAGE,
  USBH_MTP_OBJECT_FORMAT_INDEX_UNDEFINED_CONTACT,
  USBH_MTP_OBJECT_FORMAT_INDEX_ABSTRACT_CONTACT,
  USBH_MTP_OBJECT_FORMAT_INDEX_VCARD_2
} USBH_MTP_OBJECT_FORMAT_INDEX;

/*********************************************************************
*
*       USBH_MTP_OBJECT_FORMAT
*
*  Description
*    Identifiers describing the format type of a given object.
*/
typedef enum {
  USBH_MTP_OBJECT_FORMAT_UNDEFINED                           = 0x3000, // Undefined object
  USBH_MTP_OBJECT_FORMAT_ASSOCIATION                         = 0x3001, // Association (for example, a folder)
  USBH_MTP_OBJECT_FORMAT_SCRIPT                              = 0x3002, // Device model-specific script
  USBH_MTP_OBJECT_FORMAT_EXECUTABLE                          = 0x3003, // Device model-specific binary executable
  USBH_MTP_OBJECT_FORMAT_TEXT                                = 0x3004, // Text file
  USBH_MTP_OBJECT_FORMAT_HTML                                = 0x3005, // Hypertext Markup Language file (text)
  USBH_MTP_OBJECT_FORMAT_DPOF                                = 0x3006, // Digital Print Order Format file (text)
  USBH_MTP_OBJECT_FORMAT_AIFF                                = 0x3007, // Audio clip
  USBH_MTP_OBJECT_FORMAT_WAV                                 = 0x3008, // Audio clip
  USBH_MTP_OBJECT_FORMAT_MP3                                 = 0x3009, // Audio clip
  USBH_MTP_OBJECT_FORMAT_AVI                                 = 0x300A, // Video clip
  USBH_MTP_OBJECT_FORMAT_MPEG                                = 0x300B, // Video clip
  USBH_MTP_OBJECT_FORMAT_ASF                                 = 0x300C, // Microsoft Advanced Streaming Format (video)
  USBH_MTP_OBJECT_FORMAT_DEFINED                             = 0x3800, // Unknown image object
  USBH_MTP_OBJECT_FORMAT_EXIF_JPEG                           = 0x3801, // Exchangeable File Format, JEIDA standard
  USBH_MTP_OBJECT_FORMAT_TIFF_EP                             = 0x3802, // Tag Image File Format for Electronic Photography
  USBH_MTP_OBJECT_FORMAT_FLASHPIX                            = 0x3803, // Structured Storage Image Format
  USBH_MTP_OBJECT_FORMAT_BMP                                 = 0x3804, // Microsoft Windows Bitmap file
  USBH_MTP_OBJECT_FORMAT_CIFF                                = 0x3805, // Canon Camera Image File Format
  USBH_MTP_OBJECT_FORMAT_UNDEFINED2                          = 0x3806, // Reserved
  USBH_MTP_OBJECT_FORMAT_GIF                                 = 0x3807, // Graphics Interchange Format
  USBH_MTP_OBJECT_FORMAT_JFIF                                = 0x3808, // JPEG File Interchange Format
  USBH_MTP_OBJECT_FORMAT_CD                                  = 0x3809, // PhotoCD Image Pac
  USBH_MTP_OBJECT_FORMAT_PICT                                = 0x380A, // Quickdraw Image Format
  USBH_MTP_OBJECT_FORMAT_PNG                                 = 0x380B, // Portable Network Graphics
  USBH_MTP_OBJECT_FORMAT_UNDEFINED3                          = 0x380C, // Reserved
  USBH_MTP_OBJECT_FORMAT_TIFF                                = 0x380D, // Tag Image File Format
  USBH_MTP_OBJECT_FORMAT_TIFF_IT                             = 0x380E, // Tag Image File Format for Information Technology  (graphic arts)
  USBH_MTP_OBJECT_FORMAT_JP2                                 = 0x380F, // JPEG2000 Baseline File Format
  USBH_MTP_OBJECT_FORMAT_JPX                                 = 0x3810, // JPEG2000 Extended File Format
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_FIRMWARE                  = 0xB802, //
  USBH_MTP_OBJECT_FORMAT_WINDOWS_IMAGE_FORMAT                = 0xB881, //
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_AUDIO                     = 0xB900, //
  USBH_MTP_OBJECT_FORMAT_WMA                                 = 0xB901, //
  USBH_MTP_OBJECT_FORMAT_OGG                                 = 0xB902, //
  USBH_MTP_OBJECT_FORMAT_AAC                                 = 0xB903, //
  USBH_MTP_OBJECT_FORMAT_AUDIBLE                             = 0xB904, //
  USBH_MTP_OBJECT_FORMAT_FLAC                                = 0xB906, //
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_VIDEO                     = 0xB980, //
  USBH_MTP_OBJECT_FORMAT_WMV                                 = 0xB981, //
  USBH_MTP_OBJECT_FORMAT_MP4_CONTAINER                       = 0xB982, // ISO 14496-1
  USBH_MTP_OBJECT_FORMAT_MP2                                 = 0xB983, //
  USBH_MTP_OBJECT_FORMAT_3GP_CONTAINER                       = 0xB984, // 3GPP file format. http://www.3gpp.org/ftp/Specs/html-info/26244.htm
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_MULTIMEDIA_ALBUM           = 0xBA01, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_IMAGE_ALBUM                = 0xBA02, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_AUDIO_ALBUM                = 0xBA03, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_VIDEO_ALBUM                = 0xBA04, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_AUDIO_VIDEO_PLAYLIST       = 0xBA05, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_CONTACT_GROUP              = 0xBA06, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_MESSAGE_FOLDER             = 0xBA07, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_CHAPTERED_PRODUCTION       = 0xBA08, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_AUDIO_PLAYLIST             = 0xBA09, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_VIDEO_PLAYLIST             = 0xBA0A, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_MEDIACAST                  = 0xBA0B, // For use with mediacasts; references multimedia enclosures of RSS feeds or episodic content
  USBH_MTP_OBJECT_FORMAT_WPL_PLAYLIST                        = 0xBA10, //
  USBH_MTP_OBJECT_FORMAT_M3U_PLAYLIST                        = 0xBA11, //
  USBH_MTP_OBJECT_FORMAT_MPL_PLAYLIST                        = 0xBA12, //
  USBH_MTP_OBJECT_FORMAT_ASX_PLAYLIST                        = 0xBA13, //
  USBH_MTP_OBJECT_FORMAT_PLS_PLAYLIST                        = 0xBA14, //
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_DOCUMENT                  = 0xBA80, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_DOCUMENT                   = 0xBA81, //
  USBH_MTP_OBJECT_FORMAT_XML_DOCUMENT                        = 0xBA82, //
  USBH_MTP_OBJECT_FORMAT_MICROSOFT_WORD_DOCUMENT             = 0xBA83, //
  USBH_MTP_OBJECT_FORMAT_MHT_COMPILED_HTML_DOCUMENT          = 0xBA84, //
  USBH_MTP_OBJECT_FORMAT_MICROSOFT_EXCEL_SPREADSHEET         = 0xBA85, //
  USBH_MTP_OBJECT_FORMAT_MICROSOFT_POWERPOINT_PRESENTATION   = 0xBA86, //
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_MESSAGE                   = 0xBB00, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_MESSAGE                    = 0xBB01, //
  USBH_MTP_OBJECT_FORMAT_UNDEFINED_CONTACT                   = 0xBB80, //
  USBH_MTP_OBJECT_FORMAT_ABSTRACT_CONTACT                    = 0xBB81, //
  USBH_MTP_OBJECT_FORMAT_VCARD_2                             = 0xBB82  //
} USBH_MTP_OBJECT_FORMAT;


/*********************************************************************
*
*       API functions
*
**********************************************************************
*/
//
// Generic MTP module functions.
//
USBH_STATUS             USBH_MTP_Init                   (void);
void                    USBH_MTP_Exit                   (void);
void                    USBH_MTP_RegisterNotification   (USBH_NOTIFICATION_FUNC * pfNotification, void * pContext);
void                    USBH_MTP_ConfigEventSupport     (U8 OnOff);
U8                      USBH_MTP_GetEventSupport        (void);
//
// MTP device functions.
//
USBH_MTP_DEVICE_HANDLE  USBH_MTP_Open                   (U8 Index);
USBH_STATUS             USBH_MTP_Close                  (USBH_MTP_DEVICE_HANDLE hDevice);
USBH_STATUS             USBH_MTP_GetDeviceInfo          (USBH_MTP_DEVICE_HANDLE hDevice, USBH_MTP_DEVICE_INFO * pDevInfo);
USBH_STATUS             USBH_MTP_GetNumStorages         (USBH_MTP_DEVICE_HANDLE hDevice, U8 * pNumStorages);
USBH_STATUS             USBH_MTP_GetObjectPropsSupported(USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectFormatCode, U16 * pBuffer, U32 * pNumProps);
USBH_STATUS             USBH_MTP_GetObjectPropDesc      (USBH_MTP_DEVICE_HANDLE hDevice, U16 ObjectPropCode, U32 ObjectFormatCode, USBH_MTP_OBJECT_PROP_DESC * pDesc);
USBH_STATUS             USBH_MTP_GetDevicePropDesc      (USBH_MTP_DEVICE_HANDLE hDevice, U16 DevicePropCode, USBH_MTP_DEVICE_PROP_DESC * pDesc);
USBH_STATUS             USBH_MTP_GetDevicePropValue     (USBH_MTP_DEVICE_HANDLE hDevice, const USBH_MTP_DEVICE_PROP_DESC * pDesc, void * pData, U32 BufferSize);
USBH_STATUS             USBH_MTP_Reset                  (USBH_MTP_DEVICE_HANDLE hDevice);
USBH_STATUS             USBH_MTP_SetTimeouts            (USBH_MTP_DEVICE_HANDLE hDevice, U32 ReadTimeout, U32 WriteTimeout);
USBH_STATUS             USBH_MTP_CheckLock              (USBH_MTP_DEVICE_HANDLE hDevice);
USBH_STATUS             USBH_MTP_SetEventCallback       (USBH_MTP_DEVICE_HANDLE hDevice, USBH_EVENT_CALLBACK * cbOnUserEvent);
U16                     USBH_MTP_GetLastErrorCode       (USBH_MTP_DEVICE_HANDLE hDevice);
//
// MTP storage functions.
//
USBH_STATUS             USBH_MTP_GetStorageInfo         (USBH_MTP_DEVICE_HANDLE hDevice, U8 StorageIndex, USBH_MTP_STORAGE_INFO * pStorageInfo);
USBH_STATUS             USBH_MTP_Format                 (USBH_MTP_DEVICE_HANDLE hDevice, U8 StorageIndex);
//
// MTP object operation functions.
//
USBH_STATUS             USBH_MTP_GetNumObjects          (USBH_MTP_DEVICE_HANDLE hDevice, U8 StorageIndex, U32 DirObjectID, U32 * pNumObjects);
USBH_STATUS             USBH_MTP_GetObjectList          (USBH_MTP_DEVICE_HANDLE hDevice, U8 StorageIndex, U32 DirObjectID, USBH_MTP_OBJECT * pBuffer, U32 * pNumObjects);
USBH_STATUS             USBH_MTP_GetObjectInfo          (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID, USBH_MTP_OBJECT_INFO * pObjInfo);
USBH_STATUS             USBH_MTP_CreateObject           (USBH_MTP_DEVICE_HANDLE hDevice, U8 StorageIndex, USBH_MTP_CREATE_INFO * pInfo);
USBH_STATUS             USBH_MTP_DeleteObject           (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID);
USBH_STATUS             USBH_MTP_RenameObject           (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID, const U16 * sNewName, U32 NumChars);
USBH_STATUS             USBH_MTP_ReadFile               (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID, USBH_RECEIVE_DATA_FUNC * pfReadData, void * pUserContext, U8 * pUserBuf, U32 UserBufSize);
USBH_STATUS             USBH_MTP_GetObjectPropValue     (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID, const USBH_MTP_OBJECT_PROP_DESC * pDesc, void * pData, U32 BufferSize);
USBH_STATUS             USBH_MTP_SetObjectProperty      (USBH_MTP_DEVICE_HANDLE hDevice, U32 ObjectID, const USBH_MTP_OBJECT_PROP_DESC * pDesc, const void * pData, U32 NumBytes);

#if defined(__cplusplus)
  }
#endif

#endif // __USBH_MTP_H__

/*************************** End of file ****************************/
