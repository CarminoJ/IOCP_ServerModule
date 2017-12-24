#pragma once

#include "../stdafx.h"
#include "../Common_Lib/PacketBuffer/PacketBuffer.h"
#include "../Common_Lib/RingQueue/RingQueue.h"

namespace CarminoJ
{
	struct stSession
	{
		SOCKET				_hSocket;							//	Socket �ڵ鷯.
		SOCKADDR_IN			_addrClient;						//	����� Ŭ���̾�Ʈ�� ��Ʈ��ũ �ּ�.
		DWORD				_addrSize;							//	�ּ� ������.
		RingQueue			_recvQueue, _sendQueue;				//	������ ��ť.
		OVERLAPPED			_overLapRecv, _overLapSend;			//	IOCP �ڵ鷯?
		volatile	INT		_ioCount;							//	IOCOUNT, �׻� 1���� ũ��. 0�̸� Session ����.
		BOOL				_bSendAble;							//	WSASEND ȣ���� ������ �����ΰ�? Recv�� �������� ������, Send �� �ѹ��� ȣ���Ѵ�.

		DWORD				_recvBytes;
		DWORD				_sendBytes;

		CRITICAL_SECTION	_csRecv, _csSend;					//	������ ��ť�� ���� Lock �ڵ鷯.
	};
}
