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
Purpose     : Video interface class of the USB host stack
-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef USBH_VIDEO_H
#define USBH_VIDEO_H

/*********************************************************************
*
*       #include Section
*
**********************************************************************
*/
#include "SEGGER.h"
#include "USBH.h"

#if defined(__cplusplus)
extern "C" { // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#ifndef USBH_VIDEO_MAX_NUM_INPUT_PINS
#define USBH_VIDEO_MAX_NUM_INPUT_PINS 5u
#endif

#ifndef USBH_VIDEO_MAX_BYTES_CONTROLS
#define USBH_VIDEO_MAX_BYTES_CONTROLS 8u
#endif

#ifndef USBH_VIDEO_MAX_FRAME_INTERVALS
#define USBH_VIDEO_MAX_FRAME_INTERVALS 10u
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
//
// Class codes.
//
#define USBH_VIDEO_CLASS_CONTROL                              0x0Eu
#define USBH_VIDEO_SUBCLASS_CONTROL                           0x01u
#define USBH_VIDEO_SUBCLASS_STREAMING                         0x02u
#define USBH_VIDEO_SUBCLASS_INTERFACE_COLLECTION              0x03u

#define USBH_VIDEO_PROBE_REQUEST                              0x0100u
#define USBH_VIDEO_COMMIT_REQUEST                             0x0200u
#define USBH_VIDEO_STILL_PROBE                                0x0300u
#define USBH_VIDEO_STILL_COMMIT                               0x0400u
#define USBH_VIDEO_STILL_TRIGGER                              0x0500u

#define USBH_VIDEO_GUID_LEN                                       16u

#define USBH_VIDEO_DESC_SUBTYPE_INDEX                              2u

//
// Video Input Terminal indexes
//
#define USBH_VIDEO_IT_TERM_ID_INDEX                                3u
#define USBH_VIDEO_IT_TERM_TYPE_INDEX                              4u
#define USBH_VIDEO_IT_ASSOC_TERM_INDEX                             6u
#define USBH_VIDEO_IT_I_STRING_INDEX                               7u

//
// Video Output Terminal indexes
//
#define USBH_VIDEO_OT_TERM_ID_INDEX                                3u
#define USBH_VIDEO_OT_TERM_TYPE_INDEX                              4u
#define USBH_VIDEO_OT_ASSOC_TERM_INDEX                             6u
#define USBH_VIDEO_OT_SOURCE_ID_INDEX                              7u
#define USBH_VIDEO_OT_I_STRING_INDEX                               8u

//
// Video Camera Terminal indexes
//
#define USBH_VIDEO_CT_TERM_ID_INDEX                                3u
#define USBH_VIDEO_CT_TERM_TYPE_INDEX                              4u
#define USBH_VIDEO_CT_ASSOC_TERM_INDEX                             6u
#define USBH_VIDEO_CT_I_STRING_INDEX                               7u
#define USBH_VIDEO_CT_FOCAL_MIN_INDEX                              8u
#define USBH_VIDEO_CT_FOCAL_MAX_INDEX                             10u
#define USBH_VIDEO_CT_FOCAL_INDEX                                 12u
#define USBH_VIDEO_CT_CTL_SIZE_INDEX                              14u
#define USBH_VIDEO_CT_1ST_CTL_INDEX                               15u

//
// Video Selector Unit indexes
//
#define USBH_VIDEO_SL_UNIT_ID_INDEX                                3u
#define USBH_VIDEO_SL_NR_INPUT_PINS_INDEX                          4u
#define USBH_VIDEO_SL_1ST_SOURCE_ID_INDEX                          5u

//
// Video Processing Unit indexes
//
#define USBH_VIDEO_PR_UNIT_ID_INDEX                                3u
#define USBH_VIDEO_PR_SOURCE_ID_INDEX                              4u
#define USBH_VIDEO_PR_MAX_MULT_INDEX                               5u
#define USBH_VIDEO_PR_CTL_SIZE_INDEX                               7u
#define USBH_VIDEO_PR_1ST_CTL_INDEX                                8u

//
// Video Extension Unit indexes
//
#define USBH_VIDEO_EX_UNIT_ID_INDEX                                3u
#define USBH_VIDEO_EX_GUID_INDEX                                   4u
#define USBH_VIDEO_EX_NR_CTL_INDEX                                20u
#define USBH_VIDEO_EX_NR_INPUT_PINS_INDEX                         21u
#define USBH_VIDEO_EX_1ST_SOURCE_ID_INDEX                         22u

//
// Video Input Header indexes
//
#define USBH_VIDEO_IH_NUM_FORM_INDEX                               3u
#define USBH_VIDEO_IH_EP_ADDR_INDEX                                6u
#define USBH_VIDEO_IH_BM_INFO_INDEX                                7u
#define USBH_VIDEO_IH_TERM_LINK_INDEX                              8u
#define USBH_VIDEO_IH_STILL_CAP_INDEX                              9u
#define USBH_VIDEO_IH_TRIG_SUP_INDEX                              10u
#define USBH_VIDEO_IH_TRIG_USE_INDEX                              11u
#define USBH_VIDEO_IH_CTL_SIZE_INDEX                              12u
#define USBH_VIDEO_IH_1ST_CTL_INDEX                               13u

//
// Uncompressed Video Format Descriptor indexes
//
#define USBH_VIDEO_VFO_UN_FORMAT_INDEX                             3u
#define USBH_VIDEO_VFO_UN_NUM_FRAME_INDEX                          4u
#define USBH_VIDEO_VFO_UN_GUID_INDEX                               5u
#define USBH_VIDEO_VFO_UN_BPP_INDEX                               21u
#define USBH_VIDEO_VFO_UN_DEF_FRAME_INDEX                         22u
#define USBH_VIDEO_VFO_UN_ASP_X_INDEX                             23u
#define USBH_VIDEO_VFO_UN_ASP_Y_INDEX                             24u
#define USBH_VIDEO_VFO_UN_INTERL_INDEX                            25u
#define USBH_VIDEO_VFO_UN_CP_INDEX                                26u

//
// Uncompressed Video Format Frame indexes
//
#define USBH_VIDEO_VFR_UN_TYPE_INDEX                               2u
#define USBH_VIDEO_VFR_UN_FRAME_INDEX                              3u
#define USBH_VIDEO_VFR_UN_CAPS_INDEX                               4u
#define USBH_VIDEO_VFR_UN_WIDTH_INDEX                              5u
#define USBH_VIDEO_VFR_UN_HEIGHT_INDEX                             7u
#define USBH_VIDEO_VFR_UN_MIN_BIT_INDEX                            9u
#define USBH_VIDEO_VFR_UN_MAX_BIT_INDEX                           13u
#define USBH_VIDEO_VFR_UN_DEF_INT_INDEX                           21u
#define USBH_VIDEO_VFR_UN_INT_TYPE_INDEX                          25u
#define USBH_VIDEO_VFR_UN_1ST_INT_INDEX                           26u
#define USBH_VIDEO_VFR_UN_MIN_INT_INDEX                           26u
#define USBH_VIDEO_VFR_UN_MAX_INT_INDEX                           30u
#define USBH_VIDEO_VFR_UN_INT_STEP_INDEX                          34u

//
// MJPEG Video Format Descriptor indexes
//
#define USBH_VIDEO_VFO_MJPEG_FORMAT_INDEX                          3u
#define USBH_VIDEO_VFO_MJPEG_NUM_FRAME_INDEX                       4u
#define USBH_VIDEO_VFO_MJPEG_FLAGS_INDEX                           5u
#define USBH_VIDEO_VFO_MJPEG_DEF_FRAME_INDEX                       6u
#define USBH_VIDEO_VFO_MJPEG_ASP_X_INDEX                           7u
#define USBH_VIDEO_VFO_MJPEG_ASP_Y_INDEX                           8u
#define USBH_VIDEO_VFO_MJPEG_INTERL_INDEX                          9u
#define USBH_VIDEO_VFO_MJPEG_CP_INDEX                             10u

//
// Frame Based Format Descriptor indexes
//
#define USBH_VIDEO_VFO_FB_FORMAT_INDEX                             3u
#define USBH_VIDEO_VFO_FB_NUM_FRAME_INDEX                          4u
#define USBH_VIDEO_VFO_FB_GUID_INDEX                               5u
#define USBH_VIDEO_VFO_FB_BPP_INDEX                               21u
#define USBH_VIDEO_VFO_FB_DEF_FRAME_INDEX                         22u
#define USBH_VIDEO_VFO_FB_ASP_X_INDEX                             23u
#define USBH_VIDEO_VFO_FB_ASP_Y_INDEX                             24u
#define USBH_VIDEO_VFO_FB_INTERL_INDEX                            25u
#define USBH_VIDEO_VFO_FB_CP_INDEX                                26u
#define USBH_VIDEO_VFO_FB_VAR_S_INDEX                             27u

//
// Frame Based Frame indexes (H.264)
// Only offsets different from uncompressed are defined.
//
#define USBH_VIDEO_VFR_FR_DEF_INT_INDEX                           17u
#define USBH_VIDEO_VFR_FR_INT_TYPE_INDEX                          21u
#define USBH_VIDEO_VFR_FR_B_PER_LINE_INDEX                        22u

//
// Color Matching Descriptor indexes
//
#define USBH_VIDEO_CM_COLOR_PRIM                                   3u
#define USBH_VIDEO_CM_TRANS_CHARA                                  4u
#define USBH_VIDEO_CM_MATRIX_COEFF                                 5u

// Video Class-Specific VideoControl Interface Descriptor Subtypes
// (USB_Video_Class_1.1.pdf, A.5 Video Class-Specific VC Interface Descriptor Subtypes)
#define USBH_VIDEO_VC_DESCRIPTOR_UNDEFINED                      0x00u
#define USBH_VIDEO_VC_HEADER                                    0x01u
#define USBH_VIDEO_VC_INPUT_TERMINAL                            0x02u
#define USBH_VIDEO_VC_OUTPUT_TERMINAL                           0x03u
#define USBH_VIDEO_VC_SELECTOR_UNIT                             0x04u
#define USBH_VIDEO_VC_PROCESSING_UNIT                           0x05u
#define USBH_VIDEO_VC_EXTENSION_UNIT                            0x06u
#define USBH_VIDEO_VC_ENCODING_UNIT                             0x07u


// Video Class-Specific VideoStreaming Interface Descriptor Subtypes
// (USB_Video_Class_1.1.pdf, A.6 Video Class-Specific VS Interface Descriptor Subtypes)
#define USBH_VIDEO_VS_UNDEFINED                                 0x00u
#define USBH_VIDEO_VS_INPUT_HEADER                              0x01u
#define USBH_VIDEO_VS_OUTPUT_HEADER                             0x02u
#define USBH_VIDEO_VS_STILL_IMAGE_FRAME                         0x03u
#define USBH_VIDEO_VS_FORMAT_UNCOMPRESSED                       0x04u
#define USBH_VIDEO_VS_FRAME_UNCOMPRESSED                        0x05u
#define USBH_VIDEO_VS_FORMAT_MJPEG                              0x06u
#define USBH_VIDEO_VS_FRAME_MJPEG                               0x07u
#define USBH_VIDEO_VS_FORMAT_MPEG2TS                            0x0Au
#define USBH_VIDEO_VS_FORMAT_DV                                 0x0Cu
#define USBH_VIDEO_VS_COLORFORMAT                               0x0Du
#define USBH_VIDEO_VS_FORMAT_FRAME_BASED                        0x10u
#define USBH_VIDEO_VS_FRAME_FRAME_BASED                         0x11u
#define USBH_VIDEO_VS_FORMAT_STREAM_BASED                       0x12u
#define USBH_VIDEO_VS_FORMAT_H264                               0x13u
#define USBH_VIDEO_VS_FRAME_H264                                0x14u
#define USBH_VIDEO_VS_FORMAT_H264_SIMULCAST                     0x15u
#define USBH_VIDEO_VS_FORMAT_VP8                                0x16u
#define USBH_VIDEO_VS_FRAME_VP8                                 0x17u
#define USBH_VIDEO_VS_FORMAT_VP8_SIMULCAST                      0x18u


// Video Class-Specific Request Codes
// (USB_Video_Class_1.1.pdf, A.8 Video Class-Specific Request Codes)
#define USBH_VIDEO_RC_UNDEFINED                                 0x00u
#define USBH_VIDEO_SET_CUR                                      0x01u
#define USBH_VIDEO_GET_CUR                                      0x81u
#define USBH_VIDEO_GET_MIN                                      0x82u
#define USBH_VIDEO_GET_MAX                                      0x83u
#define USBH_VIDEO_GET_RES                                      0x84u
#define USBH_VIDEO_GET_LEN                                      0x85u
#define USBH_VIDEO_GET_INFO                                     0x86u
#define USBH_VIDEO_GET_DEF                                      0x87u

// Request Error Code Control
// (USB_Video_Class_1.1.pdf, 4.2.1.2 Request Error Code Control)
#define USBH_VIDEO_VC_NO_ERROR_ERR                              0x00u
#define USBH_VIDEO_VC_NOT_READY_ERR                             0x01u
#define USBH_VIDEO_VC_WRONG_STATE_ERR                           0x02u
#define USBH_VIDEO_VC_POWER_ERR                                 0x03u
#define USBH_VIDEO_VC_OUT_OF_RANGE_ERR                          0x04u
#define USBH_VIDEO_VC_INVALID_UNIT_ERR                          0x05u
#define USBH_VIDEO_VC_INVALID_CONTROL_ERR                       0x06u
#define USBH_VIDEO_VC_INVALID_REQUEST_ERR                       0x07u
#define USBH_VIDEO_VC_UNKNOWN_ERR                               0xFFu

// USB Terminal Types
// (USB_Video_Class_1.1.pdf, B.1 USB Terminal Types)
#define USBH_VIDEO_TT_VENDOR_SPECIFIC                         0x0100u
#define USBH_VIDEO_TT_STREAMING                               0x0101u

// Input Terminal Types
// (USB_Video_Class_1.1.pdf, B.2 Input Terminal Types)
#define USBH_VIDEO_ITT_VENDOR_SPECIFIC                        0x0200u
#define USBH_VIDEO_ITT_CAMERA                                 0x0201u
#define USBH_VIDEO_ITT_MEDIA_TRANSPORT_INPUT                  0x0202u

// Output Terminal Types
// (USB_Video_Class_1.1.pdf, B.3 Output Terminal Types)
#define USBH_VIDEO_OTT_VENDOR_SPECIFIC                        0x0300u
#define USBH_VIDEO_OTT_DISPLAY                                0x0301u
#define USBH_VIDEO_OTT_MEDIA_TRANSPORT_OUTPUT                 0x0302u

// External Terminal Types
// (USB_Video_Class_1.1.pdf, B.4 External Terminal Types)
#define USBH_VIDEO_EXTERNAL_VENDOR_SPECIFIC                   0x0400u
#define USBH_VIDEO_COMPOSITE_CONNECTOR                        0x0401u
#define USBH_VIDEO_SVIDEO_CONNECTOR                           0x0402u
#define USBH_VIDEO_COMPONENT_CONNECTOR                        0x0403u

// Terminal Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.2 Terminal Control Selectors)
#define USBH_VIDEO_CS_TE_CONTROL_UNDEFINED                      0x00u

// Selector Unit Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.3 Selector Unit Control Selectors)
#define USBH_VIDEO_CS_SU_CONTROL_UNDEFINED                      0x00u
#define USBH_VIDEO_CS_SU_INPUT_SELECT_CONTROL                   0x01u

// Camera Terminal Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.4 Camera Terminal Control Selectors)
#define USBH_VIDEO_CS_CT_CONTROL_UNDEFINED                      0x00u
#define USBH_VIDEO_CS_CT_SCANNING_MODE_CONTROL                  0x01u
#define USBH_VIDEO_CS_CT_AE_MODE_CONTROL                        0x02u
#define USBH_VIDEO_CS_CT_AE_PRIORITY_CONTROL                    0x03u
#define USBH_VIDEO_CS_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL         0x04u
#define USBH_VIDEO_CS_CT_EXPOSURE_TIME_RELATIVE_CONTROL         0x05u
#define USBH_VIDEO_CS_CT_FOCUS_ABSOLUTE_CONTROL                 0x06u
#define USBH_VIDEO_CS_CT_FOCUS_RELATIVE_CONTROL                 0x07u
#define USBH_VIDEO_CS_CT_FOCUS_AUTO_CONTROL                     0x08u
#define USBH_VIDEO_CS_CT_IRIS_ABSOLUTE_CONTROL                  0x09u
#define USBH_VIDEO_CS_CT_IRIS_RELATIVE_CONTROL                  0x0Au
#define USBH_VIDEO_CS_CT_ZOOM_ABSOLUTE_CONTROL                  0x0Bu
#define USBH_VIDEO_CS_CT_ZOOM_RELATIVE_CONTROL                  0x0Cu
#define USBH_VIDEO_CS_CT_PANTILT_ABSOLUTE_CONTROL               0x0Du
#define USBH_VIDEO_CS_CT_PANTILT_RELATIVE_CONTROL               0x0Eu
#define USBH_VIDEO_CS_CT_ROLL_ABSOLUTE_CONTROL                  0x0Fu
#define USBH_VIDEO_CS_CT_ROLL_RELATIVE_CONTROL                  0x10u
#define USBH_VIDEO_CS_CT_PRIVACY_CONTROL                        0x11u

// Processing Unit Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.5 Processing Unit Control Selectors)
#define USBH_VIDEO_CS_PU_CONTROL_UNDEFINED                      0x00u
#define USBH_VIDEO_CS_PU_BACKLIGHT_COMPENSATION_CONTROL         0x01u
#define USBH_VIDEO_CS_PU_BRIGHTNESS_CONTROL                     0x02u
#define USBH_VIDEO_CS_PU_CONTRAST_CONTROL                       0x03u
#define USBH_VIDEO_CS_PU_GAIN_CONTROL                           0x04u
#define USBH_VIDEO_CS_PU_POWER_LINE_FREQUENCY_CONTROL           0x05u
#define USBH_VIDEO_CS_PU_HUE_CONTROL                            0x06u
#define USBH_VIDEO_CS_PU_SATURATION_CONTROL                     0x07u
#define USBH_VIDEO_CS_PU_SHARPNESS_CONTROL                      0x08u
#define USBH_VIDEO_CS_PU_GAMMA_CONTROL                          0x09u
#define USBH_VIDEO_CS_PU_WHITE_BALANCE_TEMPERATURE_CONTROL      0x0Au
#define USBH_VIDEO_CS_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL 0x0Bu
#define USBH_VIDEO_CS_PU_WHITE_BALANCE_COMPONENT_CONTROL        0x0Cu
#define USBH_VIDEO_CS_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL   0x0Du
#define USBH_VIDEO_CS_PU_DIGITAL_MULTIPLIER_CONTROL             0x0Eu
#define USBH_VIDEO_CS_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL       0x0Fu
#define USBH_VIDEO_CS_PU_HUE_AUTO_CONTROL                       0x10u
#define USBH_VIDEO_CS_PU_ANALOG_VIDEO_STANDARD_CONTROL          0x11u
#define USBH_VIDEO_CS_PU_ANALOG_LOCK_STATUS_CONTROL             0x12u

// Extension Unit Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.6 Extension Unit Control Selectors)
#define USBH_VIDEO_CS_XU_CONTROL_UNDEFINED                      0x00u

// VideoStreaming Interface Control Selectors
// (USB_Video_Class_1.1.pdf, A.9.7 VideoStreaming Interface Control Selectors)
#define USBH_VIDEO_CS_VS_CONTROL_UNDEFINED                      0x00u
#define USBH_VIDEO_CS_VS_PROBE_CONTROL                          0x01u
#define USBH_VIDEO_CS_VS_COMMIT_CONTROL                         0x02u
#define USBH_VIDEO_CS_VS_STILL_PROBE_CONTROL                    0x03u
#define USBH_VIDEO_CS_VS_STILL_COMMIT_CONTROL                   0x04u
#define USBH_VIDEO_CS_VS_STILL_IMAGE_TRIGGER_CONTROL            0x05u
#define USBH_VIDEO_CS_VS_STREAM_ERROR_CODE_CONTROL              0x06u
#define USBH_VIDEO_CS_VS_GENERATE_KEY_FRAME_CONTROL             0x07u
#define USBH_VIDEO_CS_VS_UPDATE_FRAME_SEGMENT_CONTROL           0x08u
#define USBH_VIDEO_CS_VS_SYNC_DELAY_CONTROL                     0x09u

#define USBH_VIDEO_INVALID_HANDLE NULL
#define USBH_VIDEO_INVALID_STREAM NULL

#define USBH_UVC_END_OF_FRAME (1uL << 1)
#define USBH_UVC_BIT_FRAME_ID (1uL << 0)

typedef struct _USBH_VIDEO_INST   *USBH_VIDEO_DEVICE_HANDLE;
typedef struct _USBH_VIDEO_STREAM *USBH_VIDEO_STREAM_HANDLE;

/*********************************************************************
*
*       USBH_VIDEO_INTERFACE_INFO
*
*  Description
*    Structure containing information about a video interface.
*/
typedef struct {
  U16               VendorId;    // The Vendor ID of the device.
  U16               ProductId;   // The Product ID of the device.
  U8                InterfaceNo; // Interface index (from USB descriptor).
  U8                NumTermUnits;// Number of terminals and units reported within the corresponding video control interface.
  USBH_INTERFACE_ID InterfaceID; // ID of the interface.
  USBH_DEVICE_ID    DeviceId;    // The unique device Id. This Id is assigned if the USB
                                 // device was successfully enumerated. It is valid until the
                                 // device is removed from the host. If the device is reconnected
                                 // a different device Id is assigned.
  USBH_SPEED        Speed;       // The USB speed of the device, see USBH_SPEED.
  U8                Class;       // The Class value field of the interface.
  U8                SubClass;    // The SubClass value field of the interface.
  U8                Protocol;    // The Protocol value field of the interface.
} USBH_VIDEO_INTERFACE_INFO;

/*********************************************************************
*
*       USBH_VIDEO_TERM_UNIT_INFO
*
*  Description
*    Structure containing information about a video terminal or a video unit.
*/
typedef struct {
  U8 Type;        // Terminal or Unit type. One of the following values:
                  // * USBH_VIDEO_VC_INPUT_TERMINAL
                  // * USBH_VIDEO_VC_OUTPUT_TERMINAL
                  // * USBH_VIDEO_VC_SELECTOR_UNIT
                  // * USBH_VIDEO_VC_PROCESSING_UNIT
                  // * USBH_VIDEO_VC_EXTENSION_UNIT
  U8 bTermUnitID; // Unique identifier (address space is shared between terminal and unit IDs).
  union {
    //
    // Structure containing information about a video selector unit.
    //
    struct {
      U16 TerminalType;
      union {
        //
        // Output Terminal
        //
        struct {
          U8 bAssocTerminal; // Constant, identifying the Input Terminal to which this Output Terminal is associated, or zero if no such association exists.
          U8 bSourceID;      // ID of the Unit or Terminal to which this Terminal is connected.
        } OutputTerm;
        //
        // Camera Terminal
        //
        struct {
          U8  bAssocTerminal;           // ID of the Output Terminal to which this (Camera) Input Terminal is associated.
          U8  iTerminal;                // Index of a string descriptor, describing the Camera Terminal.
          U16 wObjectiveFocalLengthMin; // Optical "L-min" value. 0 if not supported.
          U16 wObjectiveFocalLengthMax; // Optical "L-max" value. 0 if not supported.
          U16 wOcularFocalLength;       // Optical "L-ocular" value. 0 if not supported.
          U8  bControlSize;             // Size in bytes of the bmControls field.
          U8  bmControls[3];            // A bit set to 1 indicates that the mentioned Control is supported for the video stream.
                                        // * D0: Scanning Mode
                                        // * D1: Auto-Exposure Mode
                                        // * D2: Auto-Exposure Priority
                                        // * D3: Exposure Time (Absolute)
                                        // * D4: Exposure Time (Relative)
                                        // * D5: Focus (Absolute)
                                        // * D6 : Focus (Relative)
                                        // * D7: Iris (Absolute)
                                        // * D8 : Iris (Relative)
                                        // * D9: Zoom (Absolute)
                                        // * D10: Zoom (Relative)
                                        // * D11: PanTilt (Absolute)
                                        // * D12: PanTilt (Relative)
                                        // * D13: Roll (Absolute)
                                        // * D14: Roll (Relative)
                                        // * D15: Reserved
                                        // * D16: Reserved
                                        // * D17: Focus, Auto
                                        // * D18: Privacy
                                        // * D19..Reserved
        } CameraTerm;
      } u;
    } Terminal;
    //
    // Structure containing information about a video selector unit.
    //
    struct {
      U8 bNrInPins;                                 // Number of Input Pins of this Unit (number of valid elements inside baSourceID)
      U8 baSourceID[USBH_VIDEO_MAX_NUM_INPUT_PINS]; // IDs of the Units or Terminals to which the Input Pins of this Selector Unit are connected.
    } SelectUnit;
    //
    // Structure containing information about a video processing unit.
    //
    struct {
      U8  bSourceID;                                 // ID of the Unit or Terminal to which this Unit is connected.
      U16 wMaxMultiplier;                            // Maximum digital magnification, multiplied by 100. 0 if not supported.
      U8  bControlSize;                              // Size of the bmControls field, in bytes.
      U8  bmControls[USBH_VIDEO_MAX_BYTES_CONTROLS]; // A bit set to 1 indicates that the mentioned Control is supported for the video stream.
                                                     // * D0: Brightness
                                                     // * D1: Contrast
                                                     // * D2: Hue
                                                     // * D3: Saturation
                                                     // * D4: Sharpness
                                                     // * D5: Gamma
                                                     // * D6: White Balance Temperature
                                                     // * D7: White Balance Component
                                                     // * D8: Backlight Compensation
                                                     // * D9: Gain
                                                     // * D10: Power Line Frequency
                                                     // * D11: Hue, Auto
                                                     // * D12: White Balance Temperature, Auto
                                                     // * D13: White Balance Component, Auto
                                                     // * D14: Digital Multiplier
                                                     // * D15: Digital Multiplier Limit
                                                     // * D16: Analog Video Standard
                                                     // * D17: Analog Video Lock Status
                                                     // * D18.. Reserved.
      U8 bmVideoStandards;                           // A bitmap of all analog video standards supported by the Processing Unit.
                                                     // A value of zero indicates that this bitmap should be ignored.
                                                     // * D0: None
                                                     // * D1: NTSC - 525/60
                                                     // * D2: PAL - 625/50
                                                     // * D3: SECAM - 625/50
                                                     // * D4: NTSC - 625/50
                                                     // * D5: PAL - 525/60
                                                     // * D6-D7: Reserved.
    } ProcessUnit;
    //
    // Structure containing information about a video extension unit.
    //
    struct {
      U8 guidExtensionCode[USBH_VIDEO_GUID_LEN];    // Vendor-specific code identifying the Extension Unit
      U8 bNumControls;                              // Number of controls in this extension unit
      U8 bNrInPins;                                 // Number of Input Pins of this Unit
      U8 baSourceID[USBH_VIDEO_MAX_NUM_INPUT_PINS]; // IDs of the Units or Terminals to which the Input Pins of this Selector Unit are connected.
      U8 bControlSize;                              // Size of the bmControls field, in bytes
      U8 bmControls[USBH_VIDEO_MAX_BYTES_CONTROLS]; // A bit set to 1 indicates that the mentioned Control is supported
    } ExtUnit;
  } u;
} USBH_VIDEO_TERM_UNIT_INFO;

/*********************************************************************
*
*       USBH_VIDEO_INPUT_HEADER_INFO
*
*  Description
*    Structure containing information about a video stream.
*/
typedef struct {
  U8 bNumFormats;                                // Number of video payload formats.
  U8 bmInfo;                                     // Indicates the capabilities of this
                                                 // VideoStreaming interface:
                                                 // * D0: Dynamic Format Change supported
                                                 // * D7..1: Reserved
  U8 bTerminalLink;                              // The terminal ID of the Output Terminal to which the video endpoint of this interface is connected
  U8 bStillCaptureMethod;                        // Method of still image capture supported:
                                                 // * 0: None
                                                 // * 1: Method 1
                                                 // * 2: Method 2
                                                 // * 3: Method 3
  U8 bTriggerSupport;                            // Specifies if hardware triggering is supported through this interface
                                                 // * 0: Not supported
                                                 // * 1: Supported
  U8 bTriggerUsage;                              // Specifies how the host software shall respond to a hardware trigger interrupt
                                                 // event from this interface. This is ignored if the bTriggerSupport field is zero.
                                                 // * 0: Initiate still image capture
                                                 // * 1: General purpose button event.
  U8 bControlSize;                               // Size of each bmaControls(x) field, in bytes
  U8 bmaControls[USBH_VIDEO_MAX_BYTES_CONTROLS]; // Each byte in this array corresponds to one format (e.g. bmaControls[0] is for the first format)
                                                 // Each bit indicates support for the following capabilities:
                                                 // * D0: wKeyFrameRate
                                                 // * D1: wPFrameRate
                                                 // * D2: wCompQuality
                                                 // * D3: wCompWindowSize
                                                 // * D4: Generate Key Frame
                                                 // * D5: Update Frame Segment

} USBH_VIDEO_INPUT_HEADER_INFO;

/*********************************************************************
*
*       USBH_VIDEO_FORMAT_INFO
*
*  Description
*    Structure containing information about a video format.
*/
typedef struct {
  U16 FormatType;                         // Format type, one of the following:
                                          // * USBH_VIDEO_VS_FORMAT_UNCOMPRESSED
                                          // * USBH_VIDEO_VS_FORMAT_MJPEG
                                          // * USBH_VIDEO_VS_FORMAT_STREAM_BASED (H.264)
  union {
    //
    // Uncompressed Video Format
    //
    struct {
      U8 bFormatIndex;                    // Format index ID (this not the index used with USBH_VIDEO_GetFormatInfo()), this is the internal ID used by the USB device.
      U8 bNumFrameDescriptors;            // Number of frame descriptors contained within this format.
      U8 guidFormat[USBH_VIDEO_GUID_LEN]; // GUID identifying the format.
      U8 bBitsPerPixel;                   // Bits required to display a single pixel.
      U8 bDefaultFrameIndex;              // Default frame index ID.
      U8 bAspectRatioX;                   // X dimension of the picture aspect ratio.
      U8 bAspectRatioY;                   // Y dimension of the picture aspect ratio.
      U8 bmInterlaceFlags;                // Bitmap containing interlace information.
      U8 bCopyProtect;                    // Copy protection flag.
    } UncompressedFormat;
    //
    // MJPEG Video Format
    //
    struct {
      U8 bFormatIndex;         // Format index ID (this not the index used with USBH_VIDEO_GetFormatInfo()), this is the internal ID used by the USB device.
      U8 bNumFrameDescriptors; // Number of frame descriptors contained within this format.
      U8 bmFlags;              // Bits required to display a single pixel.
      U8 bDefaultFrameIndex;   // Default frame index ID.
      U8 bAspectRatioX;        // X dimension of the picture aspect ratio.
      U8 bAspectRatioY;        // Y dimension of the picture aspect ratio.
      U8 bmInterlaceFlags;     // Bitmap containing interlace information.
      U8 bCopyProtect;         // Copy protection flag.
    } MJPEG_Format;
    //
    // H.264 (Officially H.264 is only supported with UVC 1.5,
    // but some cameras seem to support it and still say they are UVC 1.00)
    //
    struct {
      U8 bFormatIndex;                    // Format index ID (this not the index used with USBH_VIDEO_GetFormatInfo()), this is the internal ID used by the USB device.
      U8 bNumFrameDescriptors;            // Number of frame descriptors contained within this format.
      U8 guidFormat[USBH_VIDEO_GUID_LEN]; // GUID identifying the format.
      U8 bBitsPerPixel;                   // Bits required to display a single pixel.
      U8 bDefaultFrameIndex;              // Default frame index ID.
      U8 bAspectRatioX;                   // X dimension of the picture aspect ratio.
      U8 bAspectRatioY;                   // Y dimension of the picture aspect ratio.
      U8 bmInterlaceFlags;                // Bitmap containing interlace information.
      U8 bCopyProtect;                    // Copy protection flag.
      U8 bVariableSize;                   // Specifies whether the data within the
                                          // frame is of variable length from frame
                                          // to frame.
    } H264_Format;
  } u;
} USBH_VIDEO_FORMAT_INFO;

/*********************************************************************
*
*       USBH_VIDEO_FRAME_INFO
*
*  Description
*    Structure containing information about a video frame.
*    The structure is used for all supported formats (Uncompressed, MJPEG or H.264 Video Frame).
*/
typedef struct {
  U16 FrameType;              // Frame type:
                              // * USBH_VIDEO_VS_FORMAT_UNCOMPRESSED
                              // * USBH_VIDEO_VS_FORMAT_MJPEG
                              // * USBH_VIDEO_VS_FORMAT_FRAME_BASED (H.264)
  U8 bFrameIndex;             // Frame index ID (this not the index used with USBH_VIDEO_GetFrameInfo()), this is the internal ID used by the USB device.
  U8 bmCapabilities;          // Capabilities bitmap:
                              // * D0: Still image supported
                              // * D1: Fixed frame-rate
                              // * D2-D7: Reserved.
  U16 wWidth;                 // X - frame width
  U16 wHeight;                // Y - frame height.
  U32 dwMinBitRate;           // Minimum bit rate
  U32 dwMaxBitRate;           // Maximum bit rate.
  U32 dwBytesPerLine;         // (H.264 only) Line stride alignment.
  U32 dwDefaultFrameInterval; // Default frame interval.
  U8  bFrameIntervalType;     // Frame interval type. A value of 0 indicates a continuous frame interval (very rarely used),
                              // a value > 0 indicates the number of discrete frame intervals supported (amount of valid entries inside the u.dwFrameInterval array).
  union {
    U32 dwMinFrameInterval;                              // Only valid for bFrameIntervalType == 0 (Continuous frame interval)
    U32 dwMaxFrameInterval;                              // Only valid for bFrameIntervalType == 0 (Continuous frame interval)
    U32 dwFrameIntervalStep;                             // Only valid for bFrameIntervalType == 0 (Continuous frame interval)
    U32 dwFrameInterval[USBH_VIDEO_MAX_FRAME_INTERVALS]; // For bFrameIntervalType > 0
  } u;

} USBH_VIDEO_FRAME_INFO;

/*********************************************************************
*
*       USBH_VIDEO_COLOR_INFO
*
*  Description
*    Structure containing information about a video format's color matching.
*/
typedef struct {
  U8 bColorPrimaries;           // Defines color primaries:
                                // * 1: BT.709, sRGB (default)
                                // * 2: BT.470-2 (M)
                                // * 3: BT.470-2 (B, G)
                                // * 4: SMPTE 170M
                                // * 5: SMPTE 240M
                                // * 6-255: Reserved
  U8 bTransferCharacteristics;  // Opto-electronic transfer characteristic:
                                // * 0: Unspecified (unknown)
                                // * 1: BT.709 (default)
                                // * 2: BT.470-2 M
                                // * 3: BT.470-2 B, G
  U8 bMatrixCoefficients;       // Matrix used to compute luma and chroma values from the color primaries:
                                // * 0: Unspecified
                                // * 1: BT. 709
                                // * 2: FCC
                                // * 3: BT.470-2 B, G
                                // * 4: SMPTE 170M (BT.601, default)
                                // * 5: SMPTE 240M
                                // * 6-255: Reserved
} USBH_VIDEO_COLOR_INFO;

/*********************************************************************
*
*       USBH_VIDEO_RX_CALLBACK
*
*  Description
*    Definition of the callback which is registered via USBH_VIDEO_OpenStream().
*    This callback is called by the stack when new video data is received.
*    To ensure good throughput this callback should block as little as possible.
*
*    After the data received has been handled the application must call USBH_VIDEO_Ack()
*    to allow for the internal buffer to be re-used by the stack.
*
*    If the function reports an error with Status != USBH_STATUS_SUCCESS, then
*    there is no data: pBuf == NULL and NumBytes == 0 and USBH_VIDEO_Ack() must not
*    be called.
*
*  Parameters
*    hDevice:  Device handle.
*    hStream:  Stream handle.
*    Status:   USBH_STATUS_SUCCESS on success or error code on failure.
*    pBuf:     Pointer to the last filled buffer.
*    NumBytes: Number of bytes inside the buffer.
*    Flags:    Bitfield containing flags related to the received data:
*              * USBH_UVC_END_OF_FRAME
*/
typedef void USBH_VIDEO_RX_CALLBACK(USBH_VIDEO_DEVICE_HANDLE hDevice, USBH_VIDEO_STREAM_HANDLE hStream, USBH_STATUS Status, const U8 *pData, unsigned NumBytes, U32 Flags, void * pUserDataContext);

/*********************************************************************
*
*       USBH_VIDEO_PAYLOAD_HEADER
*
*  Description
*    Structure containing the payload header.
*    Users must check bHeaderLength and bmHeaderInfo to makes sure all all fields are valid.
*/
typedef struct {
  U8  bHeaderLength;        // Length of the header.
  U8  bmHeaderInfo;         // Bitfield which provides information about the availability and validity of additional fields:
                            // * D0: Frame ID
                            // * D1: End of Frame flag
                            // * D2: Presentation Time field valid
                            // * D3: Source Clock Reference field valid
                            // * D4: Reserved
                            // * D5: Still Image flag
                            // * D6: Error flag
                            // * D7: End of header flag
  U16 SOFCounter;           // 1KHz SOF token counter
  U32 dwPresentationTime;   // Presentation Time Stamp
  U32 SourceTimeClock;      // Source Clock Reference
} USBH_VIDEO_PAYLOAD_HEADER;

/*********************************************************************
*
*       USBH_VIDEO_PAYLOAD_CALLBACK
*
*  Description
*    Definition of the callback which is registered via USBH_VIDEO_OpenStream().
*    This function is called with each received payload.
*    This function is only necessary if you are interested in the payload
*    header fields (see USBH_VIDEO_PAYLOAD_HEADER structure).
*    Payloads are normally received every 125 us in high-speed and every 1 ms in full-speed.
*
*  Parameters
*    hDevice:  Device handle.
*    Status:   USBH_STATUS_SUCCESS on success or error code on failure.
*    pHeader:  Pointer to the payload header.
*    NumBytes: Number of bytes inside the buffer.
*/
typedef void USBH_VIDEO_PAYLOAD_CALLBACK(USBH_VIDEO_DEVICE_HANDLE hDevice, USBH_STATUS Status, const USBH_VIDEO_PAYLOAD_HEADER *pHeader, void * pUserPayloadContext);

/*********************************************************************
*
*       USBH_VIDEO_STREAM_CONFIG
*
*  Description
*    Structure containing configuration with which the emUSB-Host Video class should start reading frame data from the device.
*/
typedef struct {
  U8                           Flags;               // Reserved
  U8                           FormatIdx;           // Index of the format to use
  U8                           FrameIdx;            // Index of the frame to use
  U8                           FrameIntervalIdx;    // Index of the Frame interval to use (0 if the frame interval type is "continuous").
  U32                          IntervalValue;       // (Only used for continuous frame interval!) Interval in 100ns units.
                                                    // The application must make sure the value is within the dwMinFrameInterval and dwMaxFrameInterval
                                                    // limits and that dwFrameIntervalStep is respected.
  USBH_VIDEO_RX_CALLBACK      *pfDataCallback;      // Function that is called when data is received from the device.
  void                        *pUserDataContext;    // Optional pointer to user data which is passed to the pfDataCallback function.
  USBH_VIDEO_PAYLOAD_CALLBACK *pfPayloadCallback;   // Optional function that is called with every received payload.
  void                        *pUserPayloadContext; // Optional pointer to user data which is passed to the pfPayloadCallback function.
} USBH_VIDEO_STREAM_CONFIG;

/*********************************************************************
*
*       USBH_VIDEO_GET_CONTROL
*
*  Description
*    Structure used with "Get" requests.
*/
typedef struct {
  U8 bGetControlType; // Should be one of the following types:
                      // * USBH_VIDEO_GET_CUR
                      // * USBH_VIDEO_GET_MIN
                      // * USBH_VIDEO_GET_MAX
                      // * USBH_VIDEO_GET_RES
                      // * USBH_VIDEO_GET_LEN
                      // * USBH_VIDEO_GET_INFO
                      // * USBH_VIDEO_GET_DEF
  U16  Selector;      // Control Selector, see USBH_VIDEO_CS_* macros.
  U8  *pData;         // Pointer to a buffer where the device response will be saved into.
  U32 *pDataLen;      // [IN] Number of bytes to requests from the device.
                      //      This value must match the requested control selector.
                      //      See "USB Device Class Definition for Video Devices revision 1.1" for details.
                      // [OUT] Number of bytes received.
} USBH_VIDEO_GET_CONTROL;

/*********************************************************************
*
*       USBH_VIDEO_SET_CONTROL
*
*  Description
*    Structure used with "Set Cur" requests.
*/
typedef struct {
  U16 Selector; // Control Selector, see USBH_VIDEO_CS_* macros.
  U8 *pData;    // Pointer to the SET_CUR request data.
  U32 DataLen;  // Length of the request.
} USBH_VIDEO_SET_CONTROL;

/*********************************************************************
*
*       Function prototypes
*/
USBH_STATUS USBH_VIDEO_Init(void);
void        USBH_VIDEO_Exit(void);
USBH_STATUS USBH_VIDEO_AddNotification(USBH_NOTIFICATION_HOOK *pHook, USBH_NOTIFICATION_FUNC *pfNotification, void *pContext);
USBH_STATUS USBH_VIDEO_RemoveNotification(const USBH_NOTIFICATION_HOOK *pHook);
USBH_STATUS USBH_VIDEO_Open(unsigned Index, USBH_VIDEO_DEVICE_HANDLE *pHandle);
void        USBH_VIDEO_Close(USBH_VIDEO_DEVICE_HANDLE hDevice);

int USBH_VIDEO_GetIndex(USBH_INTERFACE_ID InterfaceID);

USBH_STATUS USBH_VIDEO_GetInterfaceInfo(USBH_VIDEO_DEVICE_HANDLE hDevice, USBH_VIDEO_INTERFACE_INFO *pDevInfo);
USBH_STATUS USBH_VIDEO_GetTermUnitInfo(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned Index, USBH_VIDEO_TERM_UNIT_INFO *pTermUnitInfo);
USBH_STATUS USBH_VIDEO_TermUnitID2Index(USBH_VIDEO_DEVICE_HANDLE hDevice, U8 TermUnitID, unsigned * pIndex);

USBH_STATUS USBH_VIDEO_GetInputHeader(USBH_VIDEO_DEVICE_HANDLE hDevice, USBH_VIDEO_INPUT_HEADER_INFO *pInputHeaderInfo);
USBH_STATUS USBH_VIDEO_GetFormatInfo(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned FormatIdx, USBH_VIDEO_FORMAT_INFO *pFormatInfo);
USBH_STATUS USBH_VIDEO_GetFrameInfo(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned FormatIdx, unsigned FrameIdx, USBH_VIDEO_FRAME_INFO *pFrameInfo);
USBH_STATUS USBH_VIDEO_GetColorMatchingInfo(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned FormatIdx, USBH_VIDEO_COLOR_INFO *pColorInfo);

USBH_STATUS USBH_VIDEO_OpenStream(USBH_VIDEO_DEVICE_HANDLE hDevice, USBH_VIDEO_STREAM_CONFIG *pStreamInfo, USBH_VIDEO_STREAM_HANDLE *pHandle);
USBH_STATUS USBH_VIDEO_Ack(USBH_VIDEO_STREAM_HANDLE hStream);
USBH_STATUS USBH_VIDEO_GetStreamState(USBH_VIDEO_STREAM_HANDLE hStream, I8 * pIsStopped);
USBH_STATUS USBH_VIDEO_RestartStream(USBH_VIDEO_STREAM_HANDLE hStream);
USBH_STATUS USBH_VIDEO_CloseStream(USBH_VIDEO_STREAM_HANDLE hStream);

USBH_STATUS USBH_VIDEO_GetControlVal(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned Unit, const USBH_VIDEO_GET_CONTROL *pGetControl);
USBH_STATUS USBH_VIDEO_SetControlVal(USBH_VIDEO_DEVICE_HANDLE hDevice, unsigned Unit, const USBH_VIDEO_SET_CONTROL *pSetControl);

USBH_STATUS USBH_VIDEO_ReadStatus(USBH_VIDEO_DEVICE_HANDLE hDevice, U8 * pData, U32 NumBytes, U32 * pNumBytesRead, U32 Timeout);



#if defined(__cplusplus)
}
#endif

#endif // USBH_VIDEO_H

/*************************** End of file ****************************/
