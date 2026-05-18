#include "linux.h"
#include "ntstuff.h"

// find an image in the loaded images
PKLDR_DATA_TABLE_ENTRY FindImageByName(PLOADER_PARAMETER_BLOCK loaderParams, PCWSTR name);

// find a section by name in an image
PIMAGE_SECTION_HEADER GetImageSection(PVOID imageBase, PCSTR name);

// translate the memory map
VOID TranslateMemoryMap(PLIST_ENTRY firstEntry);

// set up vga or something i guess? and also put that in the boot params
VOID SetupVideo();

LINUX_BOOT_PARAMS g_bootParams;
// prepare linux boot parameters
PFN_LINUX_STARTUP_64 PrepareBootParams(
	PLOADER_PARAMETER_BLOCK loaderParams, PKLDR_DATA_TABLE_ENTRY kernel, PVOID initrd, UINT32 initrdSize);

extern "C" VOID NtBootShimStartup(PLOADER_PARAMETER_BLOCK loaderParams)
{
	auto vmlinuz = FindImageByName(loaderParams, VMLINUZ_NAME);
	auto initrd = FindImageByName(loaderParams, INITRD_NAME);

	auto initrdSection = GetImageSection(initrd, ".initrd");
	auto initrdData = (PVOID)((ULONG_PTR)initrd->DllBase + initrdSection->VirtualAddress);
	auto initrdSize = initrdSection->Misc.VirtualSize;

	auto startup_64 = PrepareBootParams(loaderParams, vmlinuz, initrdData, initrdSize);

	// should be arch/x86/boot/compressed/head_64.S
	startup_64();
}

PKLDR_DATA_TABLE_ENTRY FindImageByName(PLOADER_PARAMETER_BLOCK loaderParams, PCWSTR name)
{
	for (auto cur = loaderParams->LoadOrderListHead.Flink; cur != nullptr; cur = cur->Flink)
	{
		auto entry = CONTAINING_RECORD(cur, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (wcsncmp(entry->BaseDllName.Buffer, name, entry->BaseDllName.Length) == 0)
		{
			return entry;
		}
	}

	return nullptr;
}

PIMAGE_SECTION_HEADER GetImageSection(PVOID imageBase, PCSTR name)
{
	auto dosHdr = (PIMAGE_DOS_HEADER)imageBase;
	auto ntHdrs = (PIMAGE_NT_HEADERS)((ULONG_PTR)imageBase + dosHdr->e_lfanew);
	auto sections = (PIMAGE_SECTION_HEADER)((ULONG_PTR)&ntHdrs->OptionalHeader + ntHdrs->FileHeader.SizeOfOptionalHeader);

	for (USHORT i = 0; i < ntHdrs->FileHeader.NumberOfSections; i++)
	{
		if (strncmp((PCSTR)sections[i].Name, name, IMAGE_SIZEOF_SHORT_NAME))
		{
			return &sections[i];
		}
	}

	return nullptr;
}

// i really hope nobody ever sees this
UINT32 TranslateMemoryType(TYPE_OF_MEMORY type)
{
	switch (type)
	{
	case LoaderExceptionBlock:
	case LoaderSystemBlock:
	case LoaderFree:
	case LoaderBad:
	case LoaderLoadedProgram:
	case LoaderOsloaderHeap:
	case LoaderOsloaderStack:
	case LoaderSystemCode:
	case LoaderHalCode:
	case LoaderBootDriver:
	case LoaderConsoleInDriver:
	case LoaderConsoleOutDriver:
	case LoaderStartupDpcStack:
	case LoaderStartupKernelStack:
	case LoaderStartupPanicStack:
	case LoaderStartupPcrPage:
	case LoaderStartupPdrPage:
	case LoaderRegistryData:
	case LoaderMemoryData:
	case LoaderNlsData:
		return E820_RAM;
	case LoaderFirmwareCode:
	case LoaderFirmwareData:
	case LoaderFirmwareTemporary:
		return E820_ACPI;
	case LoaderFirmwarePermanent:
		return E820_NVS;
	case LoaderSpecialMemory:
	case LoaderBBTMemory:
	case LoaderZero:
	case LoaderXIPRom:
	case LoaderHALCachedMemory:
	case LoaderLargePageFiller:
	case LoaderErrorLogMemory:
	case LoaderVsmMemory:
	case LoaderFirmwareReserved:
	case LoaderEnclaveMemory:
	case LoaderFirmwareKsr:
	case LoaderEnclaveKsr:
	case LoaderSkMemory:
	case LoaderSkFirmwareReserved:
	case LoaderIoSpaceMemoryZeroed:
	case LoaderIoSpaceMemoryFree:
	case LoaderIoSpaceMemoryKsr:
	case LoaderKernelShadowStack:
	case LoaderIsolatedHostVisible:
	case LoaderIsolatedKsr:
	case LoaderIoSpaceIsolatedKsr:
	case LoaderPersistentDatabase:
	case LoaderSyntheticFirmwareData:
		return E820_UNUSABLE;
	}

	return E820_RESERVED;
}

VOID TranslateMemoryMap(PLIST_ENTRY firstEntry)
{
	auto& i = g_bootParams.e820_entries;
	for (auto cur = firstEntry; cur != nullptr; cur = cur->Flink)
	{
		auto desc = CONTAINING_RECORD(cur, MEMORY_ALLOCATION_DESCRIPTOR, ListEntry);

		auto& entry = g_bootParams.e820_table[i++];
		entry.addr = desc->BasePage * PAGE_SIZE;
		entry.size = desc->PageCount * PAGE_SIZE;
		entry.type = TranslateMemoryType(desc->MemoryType);

		if (i >= E820_MAX_ENTRIES_ZEROPAGE)
		{
			// should this be an error? probably but who cares
			return;
		}
	}
}

VOID SetupVideo()
{
	// TODO: vga?
}

#define WRITE_SPLIT_FIELD(name, val)                                                                                             \
	g_bootParams.hdr.name = ((ULONG_PTR)(val) & 0xFFFFFFFF);                                                                     \
	g_bootParams.ext_##name = (((ULONG_PTR)(val) >> 32) & 0xFFFFFFFF);

PFN_LINUX_STARTUP_64 PrepareBootParams(
	PLOADER_PARAMETER_BLOCK loaderParams, PKLDR_DATA_TABLE_ENTRY kernel, PVOID initrd, UINT32 initrdSize)
{
	// read setup header from kernel
	auto setupHeader = (PLINUX_SETUP_HEADER)((ULONG_PTR)kernel->DllBase + SETUP_HEADER_OFFSET);
	auto setupHeaderSize = (setupHeader->jump >> 8) & 0xFF - JUMP_OFFSET;
	memcpy(&g_bootParams.hdr, setupHeader, setupHeaderSize);

	g_bootParams.acpi_rsdp_addr = (UINT64)loaderParams->Extension->AcpiTable;

	WRITE_SPLIT_FIELD(ramdisk_image, initrd);
	WRITE_SPLIT_FIELD(ramdisk_size, initrdSize);
	g_bootParams.hdr.type_of_loader = 0xF0; // "special" is definitely one word for this
	// boot_params->secure_boot = secure_boot_active ? 3 : 2;
	g_bootParams.secure_boot = 2; // copied from limine, means "no"

	TranslateMemoryMap(loaderParams->MemoryDescriptorListHead.Flink);
	SetupVideo();

	return (PFN_LINUX_STARTUP_64)&setupHeader->jump;
}
