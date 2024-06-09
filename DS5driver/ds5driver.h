#pragma once
#ifndef DS5DRIVER_H
#define DS5DRIVER_H

#include <ntddk.h>
#include <wdf.h>
#include <wdfusb.h>
#include <usb.h>

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP EvtDriverContextCleanup;

#endif // DS5DRIVER_H