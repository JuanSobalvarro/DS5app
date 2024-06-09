#include "Device.h"

NTSTATUS
EvtDeviceAdd(
    _In_ WDFDRIVER Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    NTSTATUS status;
    WDFDEVICE device;
    WDF_OBJECT_ATTRIBUTES attributes;
    PDEVICE_CONTEXT deviceContext;

    UNREFERENCED_PARAMETER(Driver);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    deviceContext = DeviceContextGet(device);

    status = InitializeUsbDevice(device);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Couldnt Initialize USB device\n"));
        return status;
    }
        
    KdPrint(("USB Device Initialized\n"));
    return status;
}

NTSTATUS InitializeUsbDevice(_In_ WDFDEVICE Device)
{
    NTSTATUS status;
    WDF_USB_DEVICE_SELECT_CONFIG_PARAMS configParams;
    WDFUSBDEVICE usbDevice;
    WDF_USB_DEVICE_INFORMATION deviceInfo;
    WDF_USB_DEVICE_CREATE_CONFIG createParams;
    PDEVICE_CONTEXT deviceContext = DeviceContextGet(Device);

    WDF_USB_DEVICE_CREATE_CONFIG_INIT(&createParams, USBD_CLIENT_CONTRACT_VERSION_600);
    status = WdfUsbTargetDeviceCreateWithParameters(Device, &createParams, WDF_NO_OBJECT_ATTRIBUTES, &usbDevice);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    WDF_USB_DEVICE_INFORMATION_INIT(&deviceInfo);
    status = WdfUsbTargetDeviceRetrieveInformation(usbDevice, &deviceInfo);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Couldnt retrieve Information from USB device\n"));
        return status;
    }

    if (deviceInfo.DeviceDescriptor.idVendor == DS5_VENDOR_ID &&
        deviceInfo.DeviceDescriptor.idProduct == DS5_PRODUCT_ID) {

        deviceContext->UsbDevice = usbDevice;

        WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_SINGLE_INTERFACE(&configParams);
        status = WdfUsbTargetDeviceSelectConfig(usbDevice, WDF_NO_OBJECT_ATTRIBUTES, &configParams);
        if (!NT_SUCCESS(status)) {
            WdfObjectDelete(usbDevice);
            return status;
        }

        KdPrint(("DualSense 5 USB device initialized successfully\n"));
    }
    else {
        WdfObjectDelete(usbDevice);
        KdPrint(("USB DEVICE not recognized\n"));
        return STATUS_DEVICE_NOT_CONNECTED;
    }

    return STATUS_SUCCESS;
}
