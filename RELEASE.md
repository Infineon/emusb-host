# SEGGER emUSB-Host for ModusToolbox Release Notes

Please refer to the [README.md](./README.md) and the [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html) for a complete description of the emUSB-Host Middleware.

## What's Included?

* Updating the LICENSE file

## Known Issues and Limitations

Host suspend and resume does not work, because the controller is not able to generate correct signaling on the USB bus.

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

| Software and Tools                                      | Version  |
| :---                                                    | :----:   |
| ModusToolbox Software Environment                       | 3.0.0    |
| MTB CAT1 Peripheral Driver Library                      | 3.0.0    |
| Hardware Abstraction Layer                              | 2.2.0    |
| FreeRTOS                                                | 10.4.304 |
| Retarget IO                                             | 1.3.0    |
| GCC Compiler                                            | 10.3.1   |
| IAR Compiler                                            | 9.3.1    |
| ARM Compiler 6                                          | 6.16     |

## More information

For more information, refer to the following documents:

* [emUSB-Host README.md](./README.md)
* [emUSB-Host API Reference Guide](https://infineon.github.io/emusb-host/html/index.html)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.cypress.com/products/modustoolbox-software-environment)
* [Infineon Technologies AG](https://www.infineon.com)

---
© 2022, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
