#pragma once

#include "../stdafx.h"
#include "../Common_Lib/PacketBuffer/PacketBuffer.h"
#include "../Common_Lib/RingQueue/RingQueue.h"

namespace CarminoJ
{
	struct stSession
	{
		SOCKET				_hSocket;							//	Socket 핸들러.
		SOCKADDR_IN			_addrClient;						//	연결된 클라이언트의 네트워크 주소.
		DWORD				_addrSize;							//	주소 사이즈.
		RingQueue			_recvQueue, _sendQueue;				//	데이터 링큐.
		OVERLAPPED			_overLapRecv, _overLapSend;			//	IOCP 핸들러?
		volatile	INT		_ioCount;							//	IOCOUNT, 항상 1보다 크다. 0이면 Session 종료.
		BOOL				_bSendAble;							//	WSASEND 호출이 가능한 상태인가? Recv는 다중으로 받지만, Send 는 한번만 호출한다.

		DWORD				_recvBytes;
		DWORD				_sendBytes;

		CRITICAL_SECTION	_csRecv, _csSend;					//	각각의 링큐에 대한 Lock 핸들러.
	};
}
