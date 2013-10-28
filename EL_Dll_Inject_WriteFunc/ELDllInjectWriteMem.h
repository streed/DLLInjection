/*

Another Method to do code injection

~streed

*/

#pragma once

#include "./stdafx.h"

typedef int (WINAPI *ELMsgBox)(HWND, LPCSTR, LPCSTR, UINT );
typedef LONG (WINAPI *NTQIP)(HANDLE, ELPROCESS_INFORMATION_CLASS, PVOID, ULONG, PULONG);
typedef HANDLE (WINAPI *GETPROC)();
typedef HMODULE (WINAPI *LOADLIBA)( const char *dll );
typedef LPVOID (WINAPI *GETPROCADDR)( HMODULE mod, const char *func );

typedef struct _ELIAT
{
	LOADLIBA mLoadLibA;
	GETPROCADDR mGetProcAddr;
}ELIAT;

typedef struct _ELADATA
{
	void *mData[256];
}ELDATA;

typedef struct _ELTHREADINFO
{
	LPVOID mIat;
	LPVOID mData;
}ELTHREADINFO;


DWORD GetPid( const char *exe );
int InjectCode( DWORD pid );