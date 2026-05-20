#include "ntstuff.h"

extern "C" VOID _invalid_parameter()
{
}

EXTERN_C
_IRQL_requires_max_(PASSIVE_LEVEL) NTSTATUS NTAPI RtlUnicodeToMultiByteN(
	_Out_writes_bytes_to_(MaxBytesInMultiByteString, *BytesInMultiByteString) PCHAR MultiByteString,
	_In_ ULONG MaxBytesInMultiByteString,
	_Out_opt_ PULONG BytesInMultiByteString,
	_In_reads_bytes_(BytesInUnicodeString) PCWCH UnicodeString,
	_In_ ULONG BytesInUnicodeString)
{
	// truncate it
	ULONG i = 0;
	for (; i < BytesInUnicodeString / sizeof(WCHAR) && i < MaxBytesInMultiByteString; i++)
	{
		MultiByteString[i] = (CHAR)UnicodeString[i];
	}

	if (BytesInMultiByteString)
	{
		*BytesInMultiByteString = i;
	}

	return STATUS_SUCCESS;
}