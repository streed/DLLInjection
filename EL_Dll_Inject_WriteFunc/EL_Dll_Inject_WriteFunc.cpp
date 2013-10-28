// EL_Dll_Inject_WriteFunc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{

	DWORD pid = GetPid( "mspaint.exe" );

	InjectCode( pid );

	return 0;
}

