#include "../stdafx.h"
#include "./IOCPWorkerFunction.h"
namespace CarminoJ
{
	unsigned __stdcall AcceptThreadProc(void *PARAM)
	{
		return 0;
	}
	unsigned __stdcall WorkerThreadProc(void *PARAM)
	{
		return 0;
	}
	unsigned __stdcall PrintThreadProc(void *PARAM)
	{
		while (1)
		{
			wprintf(L"I am PrintThread...\n");
			Sleep(1000);
		}
		return 0;
	}
}

