#pragma once
#ifndef DEVICE_H
#define DEVICE_H

#include <ntddk.h>
#include <wdf.h>
#include <wdfusb.h>
#include <usb.h>

#define DS5_VENDOR_ID 0x054C  // Vendor ID Sony
#define DS5_PRODUCT_ID 0x0CE6 // Product ID DS5

typedef struct _DEVICE_CONTEXT {
    WDFUSBDEVICE UsbDevice;
    WDFQUEUE Queue;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceContextGet)

EVT_WDF_DRIVER_DEVICE_ADD EvtDeviceAdd;

#endif // DEVICE_H
