#pragma once
namespace CarminoJ
{
	unsigned __stdcall AcceptThreadProc(void *PARAM);	// Accept 전용 Thread;
	unsigned __stdcall WorkerThreadProc(void *PARAM);	// Worker Thread;
	unsigned __stdcall PrintThreadProc(void *PARAM);	// Dialog Print 전용 Thread;
}

