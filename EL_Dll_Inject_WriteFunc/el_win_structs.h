/*

I do not take credit for these stuctures, but rather give all credits to
the NTInternals Team. But, I do take credit fro taking the useful things
from the site and putting them into a easy to use place.

thanks and hope someone finds these useful

~sean reed

*/

#pragma once

#include <windows.h>
#include <wchar.h>

typedef void (*PELPEBLOCKROUTINE)( PVOID PebLock ); 

typedef struct _ELLSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
}ELLSA_UNICODE_STRING, *PELLSA_UNICODE_STRING, ELUNICODE_STRING, *PELUNICODE_STRING;

typedef struct _ELLIST_ENTRY 
{
  struct _ELLIST_ENTRY  *Flink;
  struct _ELLIST_ENTRY  *Blink;
} ELLIST_ENTRY, *PELLIST_ENTRY;

typedef struct _ELPEB_LDR_DATA 
{
	ULONG			Length; 
	BOOLEAN			Initialized; 
	PVOID			SsHandle; 
	ELLIST_ENTRY	InLoadOrderModuleList; 
	ELLIST_ENTRY	InMemoryOrderModuleList; 
	ELLIST_ENTRY	InInitializationOrderModuleList;

} ELPEB_LDR_DATA, *PELPEB_LDR_DATA;
	
typedef struct _ELRTL_DRIVE_LETTER_CURDIR 
{
	USHORT				Flags; 
	USHORT				Length; 
	ULONG				TimeStamp; 
	ELUNICODE_STRING	DosPath;

} ELRTL_DRIVE_LETTER_CURDIR, *PELRTL_DRIVE_LETTER_CURDIR;

typedef struct _ELRTL_USER_PROCESS_PARAMETERS 
{
	ULONG						MaximumLength; 
	ULONG						Length; 
	ULONG						Flags; 
	ULONG						DebugFlags; 
	PVOID						ConsoleHandle; 
	ULONG						ConsoleFlags; 
	HANDLE						StdInputHandle; 
	HANDLE						StdOutputHandle; 
	HANDLE						StdErrorHandle; 
	ELUNICODE_STRING			CurrentDirectoryPath; 
	HANDLE						CurrentDirectoryHandle; 
	ELUNICODE_STRING			DllPath; 
	ELUNICODE_STRING			ImagePathName; 
	ELUNICODE_STRING			CommandLine; 
	PVOID						Environment; 
	ULONG						StartingPositionLeft; 
	ULONG						StartingPositionTop; 
	ULONG						Width; 
	ULONG						Height; 
	ULONG						CharWidth; 
	ULONG						CharHeight; 
	ULONG						ConsoleTextAttributes; 
	ULONG						WindowFlags; 
	ULONG						ShowWindowFlags; 
	ELUNICODE_STRING			WindowTitle; 
	ELUNICODE_STRING			DesktopName; 
	ELUNICODE_STRING			ShellInfo; 
	ELUNICODE_STRING			RuntimeData; 
	ELRTL_DRIVE_LETTER_CURDIR	DLCurrentDirectory[0x20];

} ELRTL_USER_PROCESS_PARAMETERS, *PELRTL_USER_PROCESS_PARAMETERS;

typedef struct _ELPEB_FREE_BLOCK 
{
	struct _ELPEB_FREE_BLOCK *Next; 
	ULONG Size;

} ELPEB_FREE_BLOCK, *PELPEB_FREE_BLOCK;

typedef struct _ELLDR_MODULE {

	ELLIST_ENTRY		InLoadOrderModuleList;
	ELLIST_ENTRY		InMemoryOrderModuleList;
	ELLIST_ENTRY		InInitializationOrderModuleList;
	PVOID				BaseAddress;
	PVOID               EntryPoint;
	ULONG               SizeOfImage;
	ELUNICODE_STRING    FullDllName;
	ELUNICODE_STRING    BaseDllName;
	ULONG               Flags;
	SHORT               LoadCount;
	SHORT               TlsIndex;
	ELLIST_ENTRY        HashTableEntry;
	ULONG               TimeDateStamp;

} ELLDR_MODULE, *ELPLDR_MODULE;


typedef struct _ELPEB 
{
	BOOLEAN							InheritedAddressSpace; 
	BOOLEAN							ReadImageFileExecOptions; 
	BOOLEAN							BeingDebugged; 
	BOOLEAN							Spare; 
	HANDLE							Mutant; 
	PVOID							ImageBaseAddress; 
	PELPEB_LDR_DATA					LoaderData; 
	PELRTL_USER_PROCESS_PARAMETERS	ProcessParameters; 
	PVOID							SubSystemData; 
	PVOID							ProcessHeap; 
	PVOID							FastPebLock; 
	PELPEBLOCKROUTINE				FastPebLockRoutine; 
	PELPEBLOCKROUTINE				FastPebUnlockRoutine; 
	ULONG							EnvironmentUpdateCount; 
	PVOID							*KernelCallbackTable; 
	PVOID							EventLogSection; 
	PVOID							EventLog; 
	PELPEB_FREE_BLOCK				FreeList; 
	ULONG							TlsExpansionCounter; 
	PVOID							TlsBitmap; 
	ULONG							TlsBitmapBits[0x2]; 
	PVOID							ReadOnlySharedMemoryBase; 
	PVOID							ReadOnlySharedMemoryHeap; 
	PVOID							*ReadOnlyStaticServerData; 
	PVOID							AnsiCodePageData; 
	PVOID							OemCodePageData; 
	PVOID							UnicodeCaseTableData; 
	ULONG							NumberOfProcessors; 
	ULONG							NtGlobalFlag; 
	BYTE							Spare2[0x4]; 
	LARGE_INTEGER					CriticalSectionTimeout; 
	ULONG							HeapSegmentReserve; 
	ULONG							HeapSegmentCommit; 
	ULONG							HeapDeCommitTotalFreeThreshold; 
	ULONG							HeapDeCommitFreeBlockThreshold; 
	ULONG							NumberOfHeaps; 
	ULONG							MaximumNumberOfHeaps; 
	PVOID							**ProcessHeaps; 
	PVOID							GdiSharedHandleTable; 
	PVOID							ProcessStarterHelper; 
	PVOID							GdiDCAttributeList; 
	PVOID							LoaderLock; 
	ULONG							OSMajorVersion; 
	ULONG							OSMinorVersion; 
	ULONG							OSBuildNumber; 
	ULONG							OSPlatformId; 
	ULONG							ImageSubSystem; 
	ULONG							ImageSubSystemMajorVersion; 
	ULONG							ImageSubSystemMinorVersion; 
	ULONG							GdiHandleBuffer[0x22]; 
	ULONG							PostProcessInitRoutine; 
	ULONG							TlsExpansionBitmap; 
	BYTE							TlsExpansionBitmapBits[0x80];
	ULONG							SessionId;

} ELPEB, *PELPEB;

typedef struct _ELPROCESS_BASIC_INFORMATION 
{
    PVOID		Reserved1;
    PELPEB		PebBaseAddress;
    PVOID		Reserved2[2];
    ULONG_PTR	UniqueProcessId;
    PVOID		Reserved3;
} ELPROCESS_BASIC_INFORMATION;

typedef enum _ELPROCESS_INFORMATION_CLASS 
{
	ProcessBasicInformation, 
	ProcessQuotaLimits, 
	ProcessIoCounters, 
	ProcessVmCounters, 
	ProcessTimes, 
	ProcessBasePriority, 
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort, 
	ProcessAccessToken, 
	ProcessLdtInformation, 
	ProcessLdtSize, 
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers, 
	ProcessPooledUsageAndLimits, 
	ProcessWorkingSetWatch, 
	ProcessUserModeIOPL, 
	ProcessEnableAlignmentFaultFixup, 
	ProcessPriorityClass, 
	ProcessWx86Information, 
	ProcessHandleCount,
	ProcessAffinityMask, 
	ProcessPriorityBoost, 
	MaxProcessInfoClass
} ELPROCESS_INFORMATION_CLASS, *PELPROCESS_INFORMATION_CLASS;


//Gets the PEB for the current process
PELPEB EL_GetPeb();

//prints out the Loaded Modules for the current Peb
DWORD  EL_PrintModules( PELPEB peb );

//hides a module
DWORD EL_HideModule( PELPEB peb, const wchar_t *dll );