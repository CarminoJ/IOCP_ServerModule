#pragma once
namespace CarminoJ
{
	unsigned __stdcall AcceptThreadProc(void *PARAM);	// Accept ���� Thread;
	unsigned __stdcall WorkerThreadProc(void *PARAM);	// Worker Thread;
	unsigned __stdcall PrintThreadProc(void *PARAM);	// Dialog Print ���� Thread;
}

