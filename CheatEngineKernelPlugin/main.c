#include "cekp.h"

#define ������� DriverEntry
#define ����ж�� DriverUnload
#define �豸���� L"\\Device\\cekp"
#define �豸���������� L"\\DosDevices\\cekp"

UNICODE_STRING DeviceName;
UNICODE_STRING SymbolicLinkName;


extern NTSTATUS Dispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
extern VOID ProcessNotify(_Inout_  PEPROCESS Process,_In_ HANDLE ProcessId,_In_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);
extern VOID NotifyImageLoadCallback(_In_opt_ PUNICODE_STRING FullImageName,_In_ HANDLE ProcessId,_In_ PIMAGE_INFO ImageInfo);
extern VOID UnprotectTheCurrentProcessFile();
extern VOID UnprotectTheCurrentProcess();


VOID ����ж��(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	UNREFERENCED_PARAMETER(DriverObject);

	UnprotectTheCurrentProcess();

	UnprotectTheCurrentProcessFile();

	IoDeleteSymbolicLink(&SymbolicLinkName);

	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS �������(
	_In_ struct _DRIVER_OBJECT *DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject;

	if (!NT_SUCCESS(status = PsSetCreateProcessNotifyRoutineEx(ProcessNotify, FALSE)))
		goto exit;

	if (!NT_SUCCESS(status = PsSetLoadImageNotifyRoutine(NotifyImageLoadCallback)))
		goto exit;

	RtlInitUnicodeString(&DeviceName, �豸����);
	if(!NT_SUCCESS(status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject)))
		goto exit;

	DriverObject->MajorFunction[IRP_MJ_CREATE] =
	DriverObject->MajorFunction[IRP_MJ_CLOSE] =
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Dispatch;
	DriverObject->DriverUnload = ����ж��;

	RtlInitUnicodeString(&SymbolicLinkName, �豸����������);
	if (!NT_SUCCESS(status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName)))
	{
		IoDeleteDevice(DeviceObject);
		goto exit;
	}
	exit:
	return status;
}