# SEGGER emUSB-Host for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html) for a complete description of the emUSB-Host Middleware.

## What's Included?

* Updated the emUSB-Host stack to 2.36.1
* Extended the support of RTOS
* Improved the Debug Message Output experience
* Other minor improvements

To view the full list of changes, refer to [Changelog section](https://infineon.github.io/emusb-host/html/index.html#section_emusb_host_changelog).

## Known Issues and Limitations

The Host suspend and resume for CAT1A devices do not work because the controller cannot generate the correct signaling on the USB bus.

Possible workarounds include:
- Reset the device with the USB application. For example:
```
USBH_URB Urb;
Urb.Header.Function = USBH_FUNCTION_RESET_DEVICE;
USBH_SubmitUrb(USBH_CDC_GetInterfaceHandle(CDC_Handle), &Urb);
USBH_CDC_Close(CDC_Handle);
```
- Physically unplug the device, and then plug it back in.

## Supported Software and Tools

This version of the emUSB-Host was validated for the compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.0.0   |
| MTB CAT1 Peripheral Driver Library                      | 3.2.0   |
| Hardware Abstraction Layer                              | 2.3.0   |
| RTOS Abstraction                                        | 1.7.0   |
| Retarget IO                                             | 1.4.0   |
| GCC Compiler                                            | 10.3.1  |
| IAR Compiler                                            | 9.3.1   |
| ARM Compiler 6                                          | 6.16    |

## More information

For more information, refer to the following documents:

* [emUSB-Host README.md](./README.md)
* [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022-2023, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
