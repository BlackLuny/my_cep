#include "cekp.h"

UNICODE_STRING  GetFilePathByFileObject(PVOID FileObject)
{
	POBJECT_NAME_INFORMATION ObjetNameInfor;
	
	if (!NT_SUCCESS(IoQueryFileDosDeviceName((PFILE_OBJECT)FileObject, &ObjetNameInfor)))
	{
		//���������־
	}
	return ObjetNameInfor->Name;
}
