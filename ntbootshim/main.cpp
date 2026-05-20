#include "linux.h"
#include "ntstuff.h"
#include "serial.h"

// find an image in the loaded images
PKLDR_DATA_TABLE_ENTRY FindImageByName(PLOADER_PARAMETER_BLOCK loaderParams, PCWSTR name);

// find a section by name in an image
PIMAGE_SECTION_HEADER GetImageSection(PVOID imageBase, PCSTR name);

// translate the memory map
VOID TranslateMemoryMap(PLIST_ENTRY firstEntry);

LINUX_BOOT_PARAMS g_bootParams;
// prepare linux boot parameters
VOID PrepareBootParams(PLOADER_PARAMETER_BLOCK loaderParams, PKLDR_DATA_TABLE_ENTRY kernel, PVOID initrd, UINT32 initrdSize);

extern "C" VOID EnterKernel(ULONG_PTR entry);

extern "C" VOID NtBootShimStartup(PLOADER_PARAMETER_BLOCK loaderParams)
{
	InitializeSerial(SERIAL_115200_BAUD);

	DBG("ntbootshim start");

	auto vmlinuz = FindImageByName(loaderParams, VMLINUZ_NAME);
	auto initrd = FindImageByName(loaderParams, INITRD_NAME);

	auto initrdSection = GetImageSection(initrd->DllBase, ".initrd");
	auto initrdData = (PVOID)((ULONG_PTR)initrd->DllBase + initrdSection->VirtualAddress);
	auto initrdSize = initrdSection->Misc.VirtualSize;

	PrepareBootParams(loaderParams, vmlinuz, initrdData, initrdSize);

	DBG("entering linux kernel startup_64 at 0x%016llX", (ULONG_PTR)startup_64);
	EnterKernel((ULONG_PTR)vmlinuz->EntryPoint - 0x200);
}

PKLDR_DATA_TABLE_ENTRY FindImageByName(PLOADER_PARAMETER_BLOCK loaderParams, PCWSTR name)
{
	DBG("finding image %ls", name);
	auto head = &loaderParams->LoadOrderListHead;
	for (auto cur = head->Flink; cur != head; cur = cur->Flink)
	{
		auto entry = CONTAINING_RECORD(cur, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (wcsncmp(entry->BaseDllName.Buffer, name, entry->BaseDllName.Length / sizeof(WCHAR)) == 0)
		{
			DBG("found %ls at 0x%016llX", name, entry->DllBase);
			return entry;
		}
	}

	return nullptr;
}

PIMAGE_SECTION_HEADER GetImageSection(PVOID imageBase, PCSTR name)
{
	DBG("finding section %s in image 0x%016llX", name, imageBase);
	auto dosHdr = (PIMAGE_DOS_HEADER)imageBase;
	auto ntHdrs = (PIMAGE_NT_HEADERS)((ULONG_PTR)imageBase + dosHdr->e_lfanew);
	auto sections = (PIMAGE_SECTION_HEADER)((ULONG_PTR)&ntHdrs->OptionalHeader + ntHdrs->FileHeader.SizeOfOptionalHeader);

	for (USHORT i = 0; i < ntHdrs->FileHeader.NumberOfSections; i++)
	{
		if (strncmp((PCSTR)sections[i].Name, name, IMAGE_SIZEOF_SHORT_NAME) == 0)
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
		return E820_RESERVED;
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

VOID TranslateMemoryMap(PLIST_ENTRY head)
{
	DBG("translating memory map");
	auto& i = g_bootParams.e820_entries;
	for (auto cur = head->Flink; cur != head; cur = cur->Flink)
	{
		auto desc = CONTAINING_RECORD(cur, MEMORY_ALLOCATION_DESCRIPTOR, ListEntry);

		if (i >= E820_MAX_ENTRIES_ZEROPAGE)
		{
			// should this be an error? probably but who cares
			return;
		}

		auto& entry = g_bootParams.e820_table[i++];
		entry.addr = desc->BasePage * PAGE_SIZE;
		entry.size = desc->PageCount * PAGE_SIZE;
		entry.type = TranslateMemoryType(desc->MemoryType);
	}
}

#define WRITE_SPLIT_FIELD(name, val)                                                                                             \
	g_bootParams.hdr.name = ((ULONG_PTR)(val) & 0xFFFFFFFF);                                                                     \
	g_bootParams.ext_##name = (((ULONG_PTR)(val) >> 32) & 0xFFFFFFFF);

VOID PrepareBootParams(PLOADER_PARAMETER_BLOCK loaderParams, PKLDR_DATA_TABLE_ENTRY kernel, PVOID initrd, UINT32 initrdSize)
{
	DBG("initializing linux struct boot_params");

	// read setup header from kernel
	auto setupHeader = (PLINUX_SETUP_HEADER)((ULONG_PTR)kernel->DllBase + SETUP_HEADER_OFFSET);
	// as stated in the docs, the jump instruction's offset can be used to compute the header size. limine does this too.
	auto setupHeaderSize = ((setupHeader->jump >> 8) & 0xFF) - JUMP_OFFSET;
	memcpy(&g_bootParams.hdr, setupHeader, setupHeaderSize);

	g_bootParams.acpi_rsdp_addr = (UINT64)loaderParams->Extension->AcpiTable;

	WRITE_SPLIT_FIELD(ramdisk_image, initrd);
	WRITE_SPLIT_FIELD(ramdisk_size, initrdSize);
	g_bootParams.hdr.type_of_loader = 0xF0; // "special" is definitely one word for this
	// from limine: boot_params->secure_boot = secure_boot_active ? 3 : 2;
	g_bootParams.secure_boot = 2;

	TranslateMemoryMap(&loaderParams->MemoryDescriptorListHead);
}
