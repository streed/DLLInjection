#include "stdafx.h"

typedef LONG (WINAPI *NTQIP)(HANDLE, ELPROCESS_INFORMATION_CLASS, PVOID, ULONG, PULONG);

static DWORD WINAPI ThreadFunc( ELTHREADINFO *info )
{
	/*

	Check if the stuff is null otherwise the injector code messed up

	*/
	if( info == NULL || info->mIat == NULL || info->mData == NULL )
		return 0;
	
	/*

	Cast out the needed structs to make it easier to read

	*/
	ELIAT *iat = (ELIAT *)info->mIat;
	ELDATA *data = (ELDATA *)info->mData;

	/*

	Load User32.dll

	*/
	HMODULE user32 = iat->mLoadLibA( (char *)data->mData[2] );

	/*

	Get the Address of MessaBoxA

	*/
	int (WINAPI *ELMsgBox)(HWND, LPCSTR, LPCSTR, UINT ) = (int (WINAPI *)(HWND, LPCSTR, LPCSTR, UINT )) iat->mGetProcAddr( user32, (char *)data->mData[3] );

	/*

	Create a MessageBox and say the text we wanted :D

	*/
	ELMsgBox( NULL, (char *)data->mData[0], (char *)data->mData[1], MB_OK );

	/*

	Successfully injected code into the running process and ran it

	*/

	return 0;
}

/*

Used to measure the length of the above function

*/
static void ThreadFuncEnd()
{}

DWORD GetPid( const char *mExe )
{
	HANDLE tSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	if( tSnap == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	
	PROCESSENTRY32 tSent;

	tSent.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( tSnap, &tSent ) )
	{
		return 0;
	}

	if( strstr( tSent.szExeFile, mExe ) )
	{
		CloseHandle( tSnap );
		
		return tSent.th32ProcessID;
	}

	while( Process32Next( tSnap, &tSent ) )
	{
		if( strstr( tSent.szExeFile, mExe ) )
		{
			return tSent.th32ProcessID;
		}
	}	

	return 0;
}

int InjectCode( DWORD pid )
{
	if( pid == 0 )
		return 0;

	HANDLE tProc = OpenProcess( PROCESS_CREATE_THREAD |  PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid );

	if( tProc == INVALID_HANDLE_VALUE )
		return 0;

	DWORD tCodeSize = (DWORD)&ThreadFuncEnd - (DWORD)&ThreadFunc;
	
	/*

	Load my "iat" with the fuctions you want to use
	here

	*/
	ELIAT tIAT;
	
	HMODULE tMod = LoadLibrary( "kernel32.dll" );

	tIAT.mLoadLibA = (LOADLIBA) GetProcAddress( tMod, "LoadLibraryA" );
	tIAT.mGetProcAddr = (GETPROCADDR) GetProcAddress( tMod, "GetProcAddress" ); 

	/*
	
	Allocate Memory for the "iat" in the other process

	*/
	LPVOID tIATMem = VirtualAllocEx( tProc, NULL, sizeof( ELIAT ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	DWORD tSize;

	/*
	
	Write the IAT to the other processes memory

	*/
	WriteProcessMemory( tProc, tIATMem, (LPVOID)&tIAT, sizeof( ELIAT ), &tSize );
	
	/*

	Create the ELDATA struct to hold the shit u plan on using in the other process
	as you have a max of 4kb for the thread stack

	*/
	ELDATA tDATA;

	LPVOID tDATAMem = VirtualAllocEx( tProc, NULL, sizeof( ELDATA ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	/*

	Set aside some space in other process and store the addresses to these spots 
	in the data struct

	*/
	tDATA.mData[0] = VirtualAllocEx( tProc, NULL, 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	tDATA.mData[1] = VirtualAllocEx( tProc, NULL, 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	tDATA.mData[2] = VirtualAllocEx( tProc, NULL, 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	tDATA.mData[3] = VirtualAllocEx( tProc, NULL, 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	char tmp[64];

	/*

	Write all the data u need for it to work here

	*/
	sprintf( tmp, ",,!,,( 0 _ 0 ),,!,," );

	WriteProcessMemory( tProc, tDATA.mData[0], (LPVOID)&tmp, strlen( tmp ), &tSize );

	sprintf( tmp, "Cool: NotePad MotherFucker" );

	WriteProcessMemory( tProc, tDATA.mData[1], (LPVOID)&tmp, strlen( tmp ), &tSize );

	sprintf( tmp, "user32.dll" );

	WriteProcessMemory( tProc, tDATA.mData[2], (LPVOID)&tmp, strlen( tmp ), &tSize );

	sprintf( tmp, "MessageBoxA" );

	WriteProcessMemory( tProc, tDATA.mData[3], (LPVOID)&tmp, strlen( tmp ), &tSize );

	/*

	Finally write the data struct here

	*/
	WriteProcessMemory( tProc, tDATAMem, (LPVOID) &tDATA, sizeof( ELDATA ), &tSize );

	/*

	Create the ELTHREADINFO struct and fill it with the addresses of the remote IAT
	and remote DATA structs

	*/
	ELTHREADINFO tInfo;

	tInfo.mIat = tIATMem;
	tInfo.mData = tDATAMem;
	
	/*

	Set aside some space there for the ELTHREADINFO struct

	*/
	LPVOID tInfoMem = VirtualAllocEx( tProc, NULL, sizeof( ELTHREADINFO ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	/*

	Copy the struct over

	*/
	WriteProcessMemory( tProc, tInfoMem, (LPVOID) &tInfo, sizeof( ELTHREADINFO ), &tSize );
	
	/*

	Allocate space for the function to be copied

	*/
	LPVOID tCode = VirtualAllocEx( tProc, NULL, tCodeSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );

	/*

	Copy the functions code over to the other process

	*/
	WriteProcessMemory( tProc, tCode, (LPVOID)&ThreadFunc, tCodeSize, &tSize );

	/*

	Run your code in other process

	*/
	CreateRemoteThread( tProc, NULL, 0, (LPTHREAD_START_ROUTINE) tCode, tInfoMem, 0, NULL );

	return 1;
}