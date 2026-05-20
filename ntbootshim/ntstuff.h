#pragma once

#define PHNT_MODE PHNT_MODE_KERNEL
#include <ntifs.h>
#include <ntddk.h>
#include <wdm.h>

typedef UINT32 BOOL;
#define MAX_PATH 260

#include <phnt.h>
#include <ntimage.h>

// 0x18 bytes (sizeof)
struct _ETW_BOOT_CONFIG
{
	ULONG MaxLoggers;					// 0x0
	struct _LIST_ENTRY BootLoggersList; // 0x8
}; 

//0x10 bytes (sizeof)
struct _RTL_RB_TREE
{
	struct _RTL_BALANCED_NODE* Root; // 0x0
	union {
		UCHAR Encoded : 1;				// 0x8
		struct _RTL_BALANCED_NODE* Min; // 0x8
	};
}; 

#include "arc.h"

// 0xa0 bytes (sizeof)
typedef struct _KLDR_DATA_TABLE_ENTRY
{
	struct _LIST_ENTRY InLoadOrderLinks;			 // 0x0
	VOID* ExceptionTable;							 // 0x10
	ULONG ExceptionTableSize;						 // 0x18
	VOID* GpValue;									 // 0x20
	struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo; // 0x28
	VOID* DllBase;									 // 0x30
	VOID* EntryPoint;								 // 0x38
	ULONG SizeOfImage;								 // 0x40
	struct _UNICODE_STRING FullDllName;				 // 0x48
	struct _UNICODE_STRING BaseDllName;				 // 0x58
	ULONG Flags;									 // 0x68
	USHORT LoadCount;								 // 0x6c
	union {
		USHORT SignatureLevel : 4; // 0x6e
		USHORT SignatureType : 3;  // 0x6e
		USHORT Frozen : 2;		   // 0x6e
		USHORT HotPatch : 1;	   // 0x6e
		USHORT Unused : 6;		   // 0x6e
		USHORT EntireField;		   // 0x6e
	} u1;						   // 0x6e
	VOID* SectionPointer;		   // 0x70
	ULONG CheckSum;				   // 0x78
	ULONG CoverageSectionSize;	   // 0x7c
	VOID* CoverageSection;		   // 0x80
	VOID* LoadedImports;		   // 0x88
	union {
		VOID* Spare;									 // 0x90
		struct _KLDR_DATA_TABLE_ENTRY* NtDataTableEntry; // 0x90
	};
	ULONG SizeOfImageNotRounded; // 0x98
	ULONG TimeDateStamp;		 // 0x9c
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

extern "C" int _vsnprintf(char* buffer, size_t sizeOfBuffer, const char* format, va_list argptr);
