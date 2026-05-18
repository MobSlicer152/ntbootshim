// whatever the fuck i need to interface with the nt loader

#pragma once

// 0x38 bytes (sizeof)
struct _EFI_FIRMWARE_INFORMATION
{
	ULONG FirmwareVersion;											 // 0x0
	struct _VIRTUAL_EFI_RUNTIME_SERVICES* VirtualEfiRuntimeServices; // 0x8
	LONG SetVirtualAddressMapStatus;								 // 0x10
	ULONG MissedMappingsCount;										 // 0x14
	struct _LIST_ENTRY FirmwareResourceList;						 // 0x18
	VOID* EfiMemoryMap;												 // 0x28
	ULONG EfiMemoryMapSize;											 // 0x30
	ULONG EfiMemoryMapDescriptorSize;								 // 0x34
};

// 0x4 bytes (sizeof)
struct _PCAT_FIRMWARE_INFORMATION
{
	ULONG PlaceHolder; // 0x0
};

// 0x40 bytes (sizeof)
struct _FIRMWARE_INFORMATION_LOADER_BLOCK
{
	ULONG FirmwareTypeUefi : 1;					 // 0x0
	ULONG EfiRuntimeUseIum : 1;					 // 0x0
	ULONG EfiRuntimePageProtectionSupported : 1; // 0x0
	ULONG Reserved1 : 1;						 // 0x0
	ULONG SoftRebootMpwrResume : 1;				 // 0x0
	ULONG Reserved2 : 27;						 // 0x0
	union {
		struct _EFI_FIRMWARE_INFORMATION EfiInformation;   // 0x8
		struct _PCAT_FIRMWARE_INFORMATION PcatInformation; // 0x8
	} u;												   // 0x8
};

// 0x10 bytes (sizeof)
struct _I386_LOADER_BLOCK
{
	VOID* CommonDataArea; // 0x0
	ULONG MachineType;	  // 0x8
	ULONG VirtualBias;	  // 0xc
};

// 0x4 bytes (sizeof)
struct _ARM_LOADER_BLOCK
{
	ULONG PlaceHolder; // 0x0
};

// 0x10 bytes (sizeof)
struct _PROFILE_PARAMETER_BLOCK
{
	USHORT Status;		 // 0x0
	USHORT Reserved;	 // 0x2
	USHORT DockingState; // 0x4
	USHORT Capabilities; // 0x6
	ULONG DockID;		 // 0x8
	ULONG SerialNumber;	 // 0xc
}; 

// 0x60 bytes (sizeof)
struct _LOADER_PERFORMANCE_DATA
{
	ULONGLONG StartTime;				  // 0x0
	ULONGLONG EndTime;					  // 0x8
	ULONGLONG PreloadEndTime;			  // 0x10
	ULONGLONG TcbLoaderStartTime;		  // 0x18
	ULONGLONG LoadHypervisorTime;		  // 0x20
	ULONGLONG LaunchHypervisorTime;		  // 0x28
	ULONGLONG LoadVsmTime;				  // 0x30
	ULONGLONG LaunchVsmTime;			  // 0x38
	ULONGLONG ExecuteTransitionStartTime; // 0x40
	ULONGLONG ExecuteTransitionEndTime;	  // 0x48
	ULONGLONG LoadDriversTime;			  // 0x50
	ULONGLONG CleanupVsmTime;			  // 0x58
}; 

// 0x68 bytes (sizeof)
struct _BOOT_ENTROPY_SOURCE_LDR_RESULT
{
	enum _BOOT_ENTROPY_SOURCE_ID SourceId;			  // 0x0
	ULONGLONG Policy;								  // 0x8
	enum _BOOT_ENTROPY_SOURCE_RESULT_CODE ResultCode; // 0x10
	LONG ResultStatus;								  // 0x14
	ULONGLONG Time;									  // 0x18
	ULONG EntropyLength;							  // 0x20
	UCHAR EntropyData[64];							  // 0x24
}; 

// 0x868 bytes (sizeof)
struct _BOOT_ENTROPY_LDR_RESULT
{
	ULONG maxEntropySources;										// 0x0
	struct _BOOT_ENTROPY_SOURCE_LDR_RESULT EntropySourceResult[10]; // 0x8
	UCHAR SeedBytesForCng[48];										// 0x418
	UCHAR RngBytesForNtoskrnl[1024];								// 0x448
	UCHAR KdEntropy[32];											// 0x848
}; 

// 0x40 bytes (sizeof)
struct _LOADER_PARAMETER_HYPERVISOR_EXTENSION
{
	ULONG InitialHypervisorCrashdumpAreaPageCount; // 0x0
	ULONG HypervisorCrashdumpAreaPageCount;		   // 0x4
	ULONGLONG InitialHypervisorCrashdumpAreaSpa;   // 0x8
	ULONGLONG HypervisorCrashdumpAreaSpa;		   // 0x10
	ULONGLONG HypervisorLaunchStatus;			   // 0x18
	ULONGLONG HypervisorLaunchStatusArg1;		   // 0x20
	union {
		struct
		{
			ULONGLONG HypervisorLaunchStatusArg2; // 0x28
			ULONGLONG HypervisorLaunchStatusArg3; // 0x30
			ULONGLONG HypervisorLaunchStatusArg4; // 0x38
		};
		struct
		{
			VOID* RangeArray; // 0x28
			ULONG RangeCount; // 0x30
		};
	};
}; 

// 0x28 bytes (sizeof)
struct _LOADER_BUGCHECK_PARAMETERS
{
	ULONG BugcheckCode;			  // 0x0
	ULONGLONG BugcheckParameter1; // 0x8
	ULONGLONG BugcheckParameter2; // 0x10
	ULONGLONG BugcheckParameter3; // 0x18
	ULONGLONG BugcheckParameter4; // 0x20
};

// 0x14 bytes (sizeof)
struct _LOADER_HIVE_RECOVERY_INFO
{
	ULONG Recovered : 1;		  // 0x0
	ULONG LegacyRecovery : 1;	  // 0x0
	ULONG SoftRebootConflict : 1; // 0x0
	ULONG MostRecentLog : 3;	  // 0x0
	ULONG LoadedFromSnapshot : 1; // 0x0
	ULONG Spare : 27;			  // 0x4
	ULONG LogNextSequence;		  // 0x8
	ULONG LogMinimumSequence;	  // 0xc
	ULONG LogCurrentOffset;		  // 0x10
}; 

// 0x30 bytes (sizeof)
struct _LOADER_RESET_REASON
{
	UCHAR Supplied; // 0x0
	union {
		struct
		{
			UCHAR Pch;				  // 0x8
			UCHAR EmbeddedController; // 0x9
			UCHAR Reserved[6];		  // 0xa
		} Component;				  // 0x8
		ULONGLONG AsULONG64;		  // 0x8
		UCHAR AsBytes[8];			  // 0x8
	} Basic;						  // 0x8
	ULONG AdditionalInfo[8];		  // 0x10
}; 

// 0x40 bytes (sizeof)
struct _VSM_PERFORMANCE_DATA
{
	ULONGLONG LaunchVsmMark[8]; // 0x0
}; 

// 0x18 bytes (sizeof)
struct _LOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION
{
	UCHAR OriginalBootStatus;  // 0x0
	UCHAR NewBootStatus;	   // 0x1
	UCHAR ConfigurationLoaded; // 0x2
	UCHAR Spare;			   // 0x3
	union {
		ULONG AllFlags;								 // 0x4
		ULONG LkgSupported : 1;						 // 0x4
		ULONG FinalBootBeforeRecovery : 1;			 // 0x4
		ULONG ConfigurationComparisonAttempted : 1;	 // 0x4
		ULONG CurrentConfigurationLoadAttempted : 1; // 0x4
		ULONG LkgConfigurationLoadAttempted : 1;	 // 0x4
		ULONG UsageSubscriptionLoadAttempted : 1;	 // 0x4
		ULONG Spare : 26;							 // 0x4
	} Flags;										 // 0x4
	LONG ConfigurationComparisonStatus;				 // 0x8
	LONG CurrentConfigurationLoadStatus;			 // 0xc
	LONG LkgConfigurationLoadStatus;				 // 0x10
	LONG UsageSubscriptionLoadStatus;				 // 0x14
}; 

// 0x48 bytes (sizeof)
struct _LOADER_FEATURE_CONFIGURATION_INFORMATION
{
	VOID* FeatureConfigurationBuffer;												   // 0x0
	ULONGLONG FeatureConfigurationBufferSize;										   // 0x8
	VOID* UsageSubscriptionBuffer;													   // 0x10
	ULONGLONG UsageSubscriptionBufferSize;											   // 0x18
	VOID* DelayedUsageReportBuffer;													   // 0x20
	ULONGLONG DelayedUsageReportBufferSize;											   // 0x28
	struct _LOADER_FEATURE_CONFIGURATION_DIAGNOSTIC_INFORMATION DiagnosticInformation; // 0x30
};

// 0x28 bytes (sizeof)
struct _OSLOADER_PAGE_TABLE_LOG
{
	VOID* LegacyPte;				 // 0x0
	ULONGLONG Pfn;					 // 0x8
	enum _TYPE_OF_MEMORY MemoryType; // 0x10
	ULONGLONG BasePage;				 // 0x18
	ULONGLONG PageCount;			 // 0x20
}; 

// 0x10 bytes (sizeof)
struct _LOADER_BOOT_APPLICATION_SVN_INFORMATION
{
	union _LARGE_INTEGER BootStlCreationTime; // 0x0
	ULONG HiberrsmMinSvn;					  // 0x8
}; 

// 0x10 bytes (sizeof)
struct _PHYSICAL_MEMORY_RUN
{
	ULONGLONG BasePage;	 // 0x0
	ULONGLONG PageCount; // 0x8
}; 

// 0x20 bytes (sizeof)
struct _PHYSICAL_MEMORY_DESCRIPTOR
{
	ULONG NumberOfRuns;					// 0x0
	ULONGLONG NumberOfPages;			// 0x8
	struct _PHYSICAL_MEMORY_RUN Run[1]; // 0x10
}; 

// 0x4 bytes (sizeof)
enum _TYPE_OF_MEMORY
{
	LoaderExceptionBlock = 0,
	LoaderSystemBlock = 1,
	LoaderFree = 2,
	LoaderBad = 3,
	LoaderLoadedProgram = 4,
	LoaderFirmwareTemporary = 5,
	LoaderFirmwarePermanent = 6,
	LoaderOsloaderHeap = 7,
	LoaderOsloaderStack = 8,
	LoaderSystemCode = 9,
	LoaderHalCode = 10,
	LoaderBootDriver = 11,
	LoaderConsoleInDriver = 12,
	LoaderConsoleOutDriver = 13,
	LoaderStartupDpcStack = 14,
	LoaderStartupKernelStack = 15,
	LoaderStartupPanicStack = 16,
	LoaderStartupPcrPage = 17,
	LoaderStartupPdrPage = 18,
	LoaderRegistryData = 19,
	LoaderMemoryData = 20,
	LoaderNlsData = 21,
	LoaderSpecialMemory = 22,
	LoaderBBTMemory = 23,
	LoaderZero = 24,
	LoaderXIPRom = 25,
	LoaderHALCachedMemory = 26,
	LoaderLargePageFiller = 27,
	LoaderErrorLogMemory = 28,
	LoaderVsmMemory = 29,
	LoaderFirmwareCode = 30,
	LoaderFirmwareData = 31,
	LoaderFirmwareReserved = 32,
	LoaderEnclaveMemory = 33,
	LoaderFirmwareKsr = 34,
	LoaderEnclaveKsr = 35,
	LoaderSkMemory = 36,
	LoaderSkFirmwareReserved = 37,
	LoaderIoSpaceMemoryZeroed = 38,
	LoaderIoSpaceMemoryFree = 39,
	LoaderIoSpaceMemoryKsr = 40,
	LoaderKernelShadowStack = 41,
	LoaderIsolatedHostVisible = 42,
	LoaderIsolatedKsr = 43,
	LoaderIoSpaceIsolatedKsr = 44,
	LoaderPersistentDatabase = 45,
	LoaderSyntheticFirmwareData = 46,
	LoaderMaximum = 47
};

typedef enum _TYPE_OF_MEMORY TYPE_OF_MEMORY;

// 0x30 bytes (sizeof)
struct _MEMORY_ALLOCATION_DESCRIPTOR
{
	union {
		struct _LIST_ENTRY ListEntry;	// 0x0
		struct _RTL_BALANCED_NODE Node; // 0x0
	};
	enum _TYPE_OF_MEMORY MemoryType; // 0x18
	ULONGLONG BasePage;				 // 0x20
	ULONGLONG PageCount;			 // 0x28
};

typedef struct _MEMORY_ALLOCATION_DESCRIPTOR MEMORY_ALLOCATION_DESCRIPTOR, *PMEMORY_ALLOCATION_DESCRIPTOR;

// 0x1130 bytes (sizeof)
struct _LOADER_PARAMETER_EXTENSION
{
	ULONG Size;														   // 0x0
	struct _PROFILE_PARAMETER_BLOCK Profile;						   // 0x4
	VOID* EmInfFileImage;											   // 0x18
	ULONG EmInfFileSize;											   // 0x20
	VOID* TriageDumpBlock;											   // 0x28
	struct _HEADLESS_LOADER_BLOCK* HeadlessLoaderBlock;				   // 0x30
	struct _SMBIOS3_TABLE_HEADER* SMBiosEPSHeader;					   // 0x38
	VOID* DrvDBImage;												   // 0x40
	ULONG DrvDBSize;												   // 0x48
	VOID* DrvDBPatchImage;											   // 0x50
	ULONG DrvDBPatchSize;											   // 0x58
	struct _NETWORK_LOADER_BLOCK* NetworkLoaderBlock;				   // 0x60
	struct _LIST_ENTRY FirmwareDescriptorListHead;					   // 0x68
	VOID* AcpiTable;												   // 0x78
	ULONG AcpiTableSize;											   // 0x80
	ULONG LastBootSucceeded : 1;									   // 0x84
	ULONG LastBootShutdown : 1;										   // 0x84
	ULONG IumPersistentDatabaseEnabled : 1;							   // 0x84
	ULONG BootDebuggerActive : 1;									   // 0x84
	ULONG StrongCodeGuarantees : 1;									   // 0x84
	ULONG HardStrongCodeGuarantees : 1;								   // 0x84
	ULONG SidSharingDisabled : 1;									   // 0x84
	ULONG TpmInitialized : 1;										   // 0x84
	ULONG VsmConfigured : 1;										   // 0x84
	ULONG IumEnabled : 1;											   // 0x84
	ULONG IsSmbboot : 1;											   // 0x84
	ULONG BootLogEnabled : 1;										   // 0x84
	ULONG DriverVerifierEnabled : 1;								   // 0x84
	ULONG SuppressMonitorX : 1;										   // 0x84
	ULONG KernelCetEnabled : 1;										   // 0x84
	ULONG SuppressSmap : 1;											   // 0x84
	ULONG PointerAuthKernelIpEnabled : 1;							   // 0x84
	ULONG SplitLargeNumaNodes : 1;									   // 0x84
	ULONG KernelCetAuditModeEnabled : 1;							   // 0x84
	ULONG VerboseSELEnabled : 1;									   // 0x84
	ULONG EarlyCrashDumpEnabled : 1;								   // 0x84
	ULONG FeatureSimulations : 6;									   // 0x84
	ULONG MicrocodeSelfHosting : 1;									   // 0x84
	ULONG XhciLegacyHandoffSkip : 1;								   // 0x84
	ULONG DisableInsiderOptInHVCI : 1;								   // 0x84
	ULONG MicrocodeMinVerSupported : 1;								   // 0x84
	ULONG GpuIommuEnabled : 1;										   // 0x84
	struct _LOADER_PERFORMANCE_DATA LoaderPerformanceData;			   // 0x88
	struct _LIST_ENTRY BootApplicationPersistentData;				   // 0xe8
	VOID* WmdTestResult;											   // 0xf8
	struct _GUID BootIdentifier;									   // 0x100
	ULONG Reserved1;												   // 0x110
	VOID* DumpHeader;												   // 0x118
	VOID* BgContext;												   // 0x120
	VOID* NumaLocalityInfo;											   // 0x128
	VOID* NumaGroupAssignment;										   // 0x130
	struct _LIST_ENTRY AttachedHives;								   // 0x138
	ULONG MemoryCachingRequirementsCount;							   // 0x148
	VOID* MemoryCachingRequirements;								   // 0x150
	struct _BOOT_ENTROPY_LDR_RESULT BootEntropyResult;				   // 0x158
	ULONGLONG ProcessorCounterFrequency;							   // 0x9c0
	struct _LOADER_PARAMETER_HYPERVISOR_EXTENSION HypervisorExtension; // 0x9c8
	struct _GUID HardwareConfigurationId;							   // 0xa08
	struct _LIST_ENTRY HalExtensionModuleList;						   // 0xa18
	struct _LIST_ENTRY PrmUpdateModuleList;							   // 0xa28
	struct _LIST_ENTRY PrmFirmwareModuleList;						   // 0xa38
	union _LARGE_INTEGER SystemTime;								   // 0xa48
	ULONGLONG TimeStampAtSystemTimeRead;							   // 0xa50
	union {
		ULONGLONG BootFlags; // 0xa58
		struct
		{
			ULONGLONG DbgMenuOsSelection : 1;			   // 0xa58
			ULONGLONG DbgHiberBoot : 1;					   // 0xa58
			ULONGLONG DbgSoftRestart : 1;				   // 0xa58
			ULONGLONG DbgMeasuredLaunch : 1;			   // 0xa58
			ULONGLONG DbgMeasuredLaunchCapable : 1;		   // 0xa58
			ULONGLONG DbgSystemHiveReplace : 1;			   // 0xa58
			ULONGLONG DbgMeasuredLaunchSmmProtections : 1; // 0xa58
			ULONGLONG DbgMeasuredLaunchSmmLevel : 7;	   // 0xa58
			ULONGLONG DbgBugCheckRecovery : 1;			   // 0xa58
			ULONGLONG DbgFASR : 1;						   // 0xa58
			ULONGLONG DbgUseCachedBcd : 1;				   // 0xa58
		};
	};
	union {
		ULONGLONG InternalBootFlags; // 0xa60
		struct
		{
			ULONGLONG DbgUtcBootTime : 1;	   // 0xa60
			ULONGLONG DbgRtcBootTime : 1;	   // 0xa60
			ULONGLONG DbgNoLegacyServices : 1; // 0xa60
		};
	};
	VOID* WfsFPData;																	 // 0xa68
	ULONG WfsFPDataSize;																 // 0xa70
	struct _LOADER_BUGCHECK_PARAMETERS BugcheckParameters;								 // 0xa78
	VOID* ApiSetSchema;																	 // 0xaa0
	ULONG ApiSetSchemaSize;																 // 0xaa8
	struct _LIST_ENTRY ApiSetSchemaExtensions;											 // 0xab0
	struct _UNICODE_STRING AcpiBiosVersion;												 // 0xac0
	struct _UNICODE_STRING SmbiosVersion;												 // 0xad0
	struct _UNICODE_STRING EfiVersion;													 // 0xae0
	struct _DEBUG_DEVICE_DESCRIPTOR* KdDebugDevice;										 // 0xaf0
	struct _OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2 OfflineCrashdumpConfigurationTable; // 0xaf8
	struct _UNICODE_STRING ManufacturingProfile;										 // 0xb18
	VOID* BbtBuffer;																	 // 0xb28
	ULONGLONG XsaveAllowedFeatures;														 // 0xb30
	ULONG XsaveFlags;																	 // 0xb38
	VOID* BootOptions;																	 // 0xb40
	ULONG IumEnablement;																 // 0xb48
	ULONG IumPolicy;																	 // 0xb4c
	LONG IumStatus;																		 // 0xb50
	ULONG BootId;																		 // 0xb54
	struct _LOADER_PARAMETER_CI_EXTENSION* CodeIntegrityData;							 // 0xb58
	ULONG CodeIntegrityDataSize;														 // 0xb60
	struct _LOADER_HIVE_RECOVERY_INFO SystemHiveRecoveryInfo;							 // 0xb64
	ULONG SoftRestartCount;																 // 0xb78
	LONGLONG SoftRestartTime;															 // 0xb80
	VOID* HypercallCodeVa;																 // 0xb88
	VOID* HalVirtualAddress;															 // 0xb90
	ULONGLONG HalNumberOfBytes;															 // 0xb98
	struct _LEAP_SECOND_DATA* LeapSecondData;											 // 0xba0
	ULONG MajorRelease;																	 // 0xba8
	ULONG Reserved2;																	 // 0xbac
	CHAR NtBuildLab[224];																 // 0xbb0
	CHAR NtBuildLabEx[224];																 // 0xc90
	struct _LOADER_RESET_REASON ResetReason;											 // 0xd70
	ULONG MaxPciBusNumber;																 // 0xda0
	ULONG FeatureSettings;																 // 0xda4
	ULONG HotPatchReserveSize;															 // 0xda8
	ULONG KernelScpReserveSize;															 // 0xdac
	struct
	{
		VOID* CodeBase;																  // 0xdb0
		ULONGLONG CodeSize;															  // 0xdb8
	} MiniExecutive;																  // 0xdb0
	struct _VSM_PERFORMANCE_DATA VsmPerformanceData;								  // 0xdc0
	struct _NUMA_MEMORY_RANGE* NumaMemoryRanges;									  // 0xe00
	ULONG NumaMemoryRangeCount;														  // 0xe08
	ULONG IommuFaultPolicy;															  // 0xe0c
	struct _LOADER_FEATURE_CONFIGURATION_INFORMATION FeatureConfigurationInformation; // 0xe10
	struct _ETW_BOOT_CONFIG EtwBootConfig;											  // 0xe58
	struct _BOOT_FIRMWARE_RAMDISK_INFO* FwRamdiskInfo;								  // 0xe70
	VOID* IpmiHwContext;															  // 0xe78
	ULONGLONG IdleThreadShadowStack;												  // 0xe80
	ULONGLONG TransitionShadowStack;												  // 0xe88
	ULONGLONG* IstShadowStacksTable;												  // 0xe90
	ULONGLONG ReservedForKernelCet[2];												  // 0xe98
	struct _MEMORY_MIRRORING_DATA* MirroringData;									  // 0xea8
	union _LARGE_INTEGER Luid;														  // 0xeb0
	struct
	{
		struct _INSTALLED_MEMORY_RANGE* Ranges; // 0xeb8
		ULONG RangeCount;						// 0xec0
	} InstalledMemory;							// 0xeb8
	struct _LIST_ENTRY HotPatchList;			// 0xec8
	VOID* BSPMicrocodeData;						// 0xed8
	ULONG BSPMicrocodeDataSize;					// 0xee0
	struct
	{
		struct _GUID TargetVolume;				   // 0xee8
		struct _UNICODE_STRING* CimFiles;		   // 0xef8
		ULONG CimFilesCount;					   // 0xf00
	} CimfsInformation;							   // 0xee8
	union _LARGE_INTEGER HalSoftRebootDatabase;	   // 0xf08
	ULONG KasanEnabled : 1;						   // 0xf10
	ULONG InitialSystemPowerStatePresent : 1;	   // 0xf10
	ULONG Unused2 : 6;							   // 0xf10
	ULONG KasanFlags : 8;						   // 0xf10
	ULONG Unused3 : 16;							   // 0xf10
	ULONG KernelLargeStackSize;					   // 0xf14
	ULONGLONG PerformanceDataFrequency;			   // 0xf18
	ULONG DriverProxyReserveSize;				   // 0xf20
	VOID* FunctionOverrideCapabilityMask;		   // 0xf28
	VOID* FunctionOverrideOptinCapabilities;	   // 0xf30
	VOID* PrmFwHandlerTable;					   // 0xf38
	ULONG PrmFwHandlerCount;					   // 0xf40
	ULONGLONG InitialSystemPowerState;			   // 0xf48
	VOID* MicrocodeRecord;						   // 0xf50
	ULONG MicrocodeRecordSize;					   // 0xf58
	struct _BOOT_OSL_RAMDISK_INFO* OslRamdiskInfo; // 0xf60
	struct
	{
		struct _COMPOSITEFS_LAYER_INFORMATION** Layers;									 // 0xf68
		ULONG LayersCount;																 // 0xf70
	} CompositefsInformation;															 // 0xf68
	struct _LOADER_FEATURE_CONFIGURATION_INFORMATION2* FeatureConfigurationInformation2; // 0xf78
	ULONG PageTableLogCount;															 // 0xf80
	struct _OSLOADER_PAGE_TABLE_LOG PageTableLog[10];									 // 0xf88
	ULONGLONG EntropyAccumulatorConfig;													 // 0x1118
	struct _LOADER_BOOT_APPLICATION_SVN_INFORMATION BootAppSvnInfo;						 // 0x1120
};

typedef struct _LOADER_PARAMETER_EXTENSION LOADER_PARAMETER_EXTENSION, *PLOADER_PARAMETER_EXTENSION;

// 0x170 bytes (sizeof)
struct _LOADER_PARAMETER_BLOCK
{
	ULONG OsMajorVersion;									 // 0x0
	ULONG OsMinorVersion;									 // 0x4
	ULONG Size;												 // 0x8
	ULONG OsLoaderSecurityVersion;							 // 0xc
	struct _LIST_ENTRY LoadOrderListHead;					 // 0x10
	struct _LIST_ENTRY MemoryDescriptorListHead;			 // 0x20
	struct _LIST_ENTRY BootDriverListHead;					 // 0x30
	struct _LIST_ENTRY EarlyLaunchListHead;					 // 0x40
	struct _LIST_ENTRY CoreDriverListHead;					 // 0x50
	struct _LIST_ENTRY CoreExtensionsDriverListHead;		 // 0x60
	struct _LIST_ENTRY TpmCoreDriverListHead;				 // 0x70
	ULONGLONG KernelStack;									 // 0x80
	ULONGLONG Prcb;											 // 0x88
	ULONGLONG Process;										 // 0x90
	ULONGLONG Thread;										 // 0x98
	ULONG KernelStackSize;									 // 0xa0
	ULONG RegistryLength;									 // 0xa4
	VOID* RegistryBase;										 // 0xa8
	struct _CONFIGURATION_COMPONENT_DATA* ConfigurationRoot; // 0xb0
	CHAR* ArcBootDeviceName;								 // 0xb8
	CHAR* ArcHalDeviceName;									 // 0xc0
	CHAR* NtBootPathName;									 // 0xc8
	CHAR* NtHalPathName;									 // 0xd0
	CHAR* LoadOptions;										 // 0xd8
	struct _NLS_DATA_BLOCK* NlsData;						 // 0xe0
	struct _ARC_DISK_INFORMATION* ArcDiskInformation;		 // 0xe8
	struct _LOADER_PARAMETER_EXTENSION* Extension;			 // 0xf0
	union {
		struct _I386_LOADER_BLOCK I386;							   // 0xf8
		struct _ARM_LOADER_BLOCK Arm;							   // 0xf8
	} u;														   // 0xf8
	struct _FIRMWARE_INFORMATION_LOADER_BLOCK FirmwareInformation; // 0x108
	CHAR* OsBootstatPathName;									   // 0x148
	CHAR* ArcOSDataDeviceName;									   // 0x150
	CHAR* ArcWindowsSysPartName;								   // 0x158
	struct _RTL_RB_TREE MemoryDescriptorTree;					   // 0x160
};

typedef struct _LOADER_PARAMETER_BLOCK LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;