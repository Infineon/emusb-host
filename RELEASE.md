# SEGGER emUSB-Host for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html) for a complete description of the emUSB-Host Middleware.

## What's Included?

* Add a new driver for PSoC 6 with DMA support
* Add OTG support for CAT3 device
* Updated the emUSB-Host stack to 2.40.0
* Other minor improvements

To view the full list of changes, refer to [Changelog section](https://infineon.github.io/emusb-host/html/index.html#section_emusb_host_changelog).

## Known Issues and Limitations

The Host suspend and resume for CAT1A devices do not work because the controller cannot generate the correct signaling on the USB bus.

Possible workarounds include:
- Reset the device with the USB application. For example:
```c
USBH_URB Urb;
Urb.Header.Function = USBH_FUNCTION_RESET_DEVICE;
USBH_SubmitUrb(USBH_CDC_GetInterfaceHandle(CDC_Handle), &Urb);
USBH_CDC_Close(CDC_Handle);
```
- Physically unplug the device, and then plug it back in.


The emUSB personality with DMA support for PSoC(TM) 6 is not updated in the mtb-pdl-cat1 3.10.1 library. So, the DMA must be configured in the user application. The updated emUSB personality with DMA support will be included in the next release of mtb-pdl-cat1.
The next code snippets show the DMA configuration for the PSoC 61 and PSoC 62 devices.

Note: This code is device-specific and cannot work for all devices. Also, some DMA channels can be reserved for other purposes.

Insert this code into the usbh_config.c file directly or save in another header file and include it into usbh_config.c.
For example: Insert this code under the first condition of `#if USBH_ENABLE_DMA == true` and before `#if !defined (USBH_DMA_INST_ADDRESS) || !defined (USBH_DMA_CHANNEL_NUM) || !defined (USBH_DMA_CHANNEL_PRIORITY)` (line 101 of usbh_config.c)

**For PSoC 61**
```c
#define USBH_DMA_INST_ADDRESS DW0
#define USBH_DMA_CHANNEL_NUM 2U
#define USBH_DMA_CHANNEL_PRIORITY 3U
```

**For PSoC 62**
```c
#define USBH_DMA_INST_ADDRESS DW0
#define USBH_DMA_CHANNEL_NUM 9U
#define USBH_DMA_CHANNEL_PRIORITY 3U
```

Execute this code before calling any of the emUSB-Host APIs or in the USBH_X_Config() function.
For example: Insert the code into USBH_X_Config(), under  `#if USBH_ENABLE_DMA == true` and before `Cy_DMA_Enable(USBH_DMA_INST_ADDRESS);` (line 223 of usbh_config.c).

**For PSoC 61**
```c
Cy_TrigMux_Connect(TRIG0_IN_TR_GROUP13_OUTPUT9, TRIG0_OUT_CPUSS_DW0_TR_IN2, false, TRIGGER_TYPE_LEVEL);
Cy_TrigMux_Connect(TRIG13_IN_USB_DMA_REQ1, TRIG13_OUT_TR_GROUP0_INPUT36, false, TRIGGER_TYPE_LEVEL);
```

**For PSoC 62**
```c
Cy_TrigMux_Select(TRIG_OUT_1TO1_5_USB_DMA1_TO_PDMA0_TR_IN9, false, TRIGGER_TYPE_EDGE);
```

## Supported Software and Tools

This version of the emUSB-Host was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.2.0   |
| MTB CAT1 Peripheral Driver Library                      | 3.10.1  |
| MTB CAT3 XMC Library                                    | 4.4.0   |
| Hardware Abstraction Layer                              | 2.6.0   |
| RTOS Abstraction                                        | 1.7.6   |
| Retarget IO                                             | 1.5.0   |
| Retarget IO (CAT3)                                      | 1.1.0   |
| GCC Compiler                                            | 11.3.1  |
| IAR Compiler                                            | 9.40.2  |
| ARM Compiler 6                                          | 6.16    |

**_NOTE:_** CAT3 device family supports only GCC_ARM compiler

## More information

For more information, refer to the following documents:

* [emUSB-Host README.md](./README.md)
* [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022-2024, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
