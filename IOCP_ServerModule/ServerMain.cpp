// IOCP_ServerModule.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <process.h>
#include "./Common_Lib/PacketBuffer/PacketBuffer.h"
#include "./ServerModule/IOCPWorkerFunction.h"

using namespace CarminoJ;
int main()
{
	__Log(dfLOG_DEVELOP, L"Hello World?\n");
	HANDLE hPrintTh = (HANDLE)_beginthreadex(NULL, 0, PrintThreadProc, NULL, 0, NULL);		//	Print Thread;
	WaitForSingleObject(hPrintTh, INFINITE);
    return 0;
}

