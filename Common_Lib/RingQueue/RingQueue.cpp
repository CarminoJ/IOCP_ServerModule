#include "../../stdafx.h"
#include "./RingQueue.h"
// 
namespace CarminoJ
{
	RingQueue::RingQueue() : _iMax(RingQueue::enMAX_QUEUE)
	{
		_arrQueue = new char[_iMax];
		_idxWrite = 0;
		_idxRead = 0;
	}

	RingQueue::RingQueue(int max) : _iMax(max)
	{
		_arrQueue = new char[_iMax];
		_idxWrite = 0;
		_idxRead = 0;
	}

	RingQueue::~RingQueue()
	{
		delete[] _arrQueue;
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 Max 사이즈 얻어오기
	//
	// Param : 없음
	// Return : int 버퍼의 Max 사이즈.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetBufferSize(void)
	{
		return _iMax;
	}

	/////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 용량 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 용량.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetCurrentUsingSize(void)
	{
		// Interlock 계열로 묶어버린다.
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		// 그 순간의 인덱스를 받아와서 멀티스레드 환경 호출에서 안전하도록 한다.
		if (idxWrite > idxRead) {
			return idxWrite - idxRead;
		}
		else if (idxWrite < idxRead)
		{
			return (_iMax - idxRead) + idxWrite;
		}
		else {
			return 0;	// EMPTY
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// 현재 버퍼에 남은 용량 얻기.
	//
	// Parameters: 없음.
	// Return: (int)남은용량.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetFreeSize(void)
	{
		return (_iMax - GetCurrentUsingSize()) - 1;
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	// (끊기지 않은 길이)
	//
	// Parameters: 없음.
	// Return: (int)사용가능 용량.
	////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetNotBrokenGetSize(void)
	{
		// Interlock 계열로 묶어버린다.
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		if (idxRead > idxWrite)
		{
			int iReturn;
			iReturn = _iMax - idxRead;
			return iReturn;
		}
		return GetCurrentUsingSize();
	}
	// 넣을 수 있는 사이즈.
	int	RingQueue::GetNotBrokenPutSize(void)
	{
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		if (idxWrite >= idxRead)
		{
			int iReturn;
			iReturn = _iMax - idxWrite;
			if (_idxRead == 0)
				iReturn = iReturn - 1;

			return iReturn;
		}
		return GetFreeSize();
	}

	/////////////////////////////////////////////////////////////////////////
	// WritePos 에 데이타 넣음.
	//
	// Parameters: (char *)데이타 포인터. (int)크기. 
	// Return: (int)넣은 크기.
	//
	// 에러코드 -2 는 배열의 길이보다 iSize를 크게 넣었을 때 뱉는다.
	/////////////////////////////////////////////////////////////////////////

	int	RingQueue::Enqueue(char *chpData, int iSize)
	{
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		int iFree = GetFreeSize();

		if (idxWrite >= _idxRead)
		{
			//
			int LeftSide;
			int RightSide;
			LeftSide = _iMax - idxWrite;
			RightSide = idxRead - 1;
			if (RightSide == -1)
			{
				LeftSide--;
			}
			//
			if (iFree >= iSize)
			{
				if (LeftSide >= iSize)
				{
					memcpy_s(&_arrQueue[idxWrite], LeftSide, chpData, iSize);
					//_idxWrite = (idxWrite + iSize) % _iMax;
					InterlockedExchange((LONG*)&_idxWrite, (idxWrite + iSize) % _iMax);
					return iSize;
				}
				else
				{
					memcpy_s(&_arrQueue[idxWrite], LeftSide, chpData, LeftSide);
					memcpy_s(_arrQueue, RightSide, &chpData[LeftSide], iSize - LeftSide);
					//_idxWrite = (idxWrite + iSize) % _iMax;
					InterlockedExchange((LONG*)&_idxWrite, _idxWrite = (idxWrite + iSize) % _iMax);
					return iSize;
				}
			}
			else
			{
				memcpy_s(&_arrQueue[idxWrite], LeftSide, chpData, LeftSide);
				if (idxRead == -1 || idxRead == 0)
				{
					//_idxWrite = (idxWrite + LeftSide) % _iMax;
					InterlockedExchange((LONG*)&_idxWrite, (idxWrite + LeftSide) % _iMax);
					return LeftSide;
				}
				memcpy_s(_arrQueue, RightSide, &chpData[LeftSide], RightSide);
				//_idxWrite = (idxWrite + LeftSide + RightSide) % _iMax;
				InterlockedExchange((LONG*)&_idxWrite, (idxWrite + LeftSide + RightSide) % _iMax);
				return LeftSide + RightSide;
			}
		}
		else
		{
			if (iFree >= iSize)
			{
				memcpy_s(&_arrQueue[idxWrite], iSize, chpData, iSize);
				//_idxWrite = (idxWrite + iSize) % _iMax;
				InterlockedExchange((LONG*)&_idxWrite, (idxWrite + iSize) % _iMax);
				return iSize;
			}
			else
			{
				memcpy_s(&_arrQueue[idxWrite], iFree, chpData, iFree);
				//_idxWrite = (idxWrite + iFree) % _iMax;
				InterlockedExchange((LONG*)&_idxWrite, (idxWrite + iFree) % _iMax);
				return iFree;
			}
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// ReadPos 에서 데이타 가져옴. ReadPos 이동.
	//
	// Parameters: (char *)데이타 포인터. (int)크기. bool bPeek == TRUE 면 Peek 이므로 _idxRead 안건드릴 것임
	// Return: (int)가져온 크기.
	//
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::Dequeue(char *chpDest, int iSize, bool bPeek)
	{
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		int iCurrentUsingSize = GetCurrentUsingSize();

		// 비어있는 큐에서 뭘 꺼내오려고 하는거니??
		if (iCurrentUsingSize == 0)
		{
			return 0;
		}
		// 데이터가 적은 상황입니다만...?
		if (iSize > iCurrentUsingSize)
		{
			if (idxRead > idxWrite)
			{
				int iLengthToEnd = _iMax - idxRead;
				memcpy_s(chpDest, iSize, &_arrQueue[idxRead], iLengthToEnd);
				if (idxWrite == 0)
				{
					if (!bPeek)
						_idxRead = (idxRead + iLengthToEnd) % _iMax;
					return iLengthToEnd;
				}
				else
				{
					memcpy_s(&chpDest[iLengthToEnd], iSize - iLengthToEnd, _arrQueue, idxWrite);
					if (!bPeek)
						_idxRead = (idxRead + iLengthToEnd + idxWrite) % _iMax;
					return iLengthToEnd + idxWrite;
				}

			}
			else
			{
				memcpy_s(chpDest, iSize, &_arrQueue[idxRead], iCurrentUsingSize);
				if (!bPeek)
					_idxRead = (idxRead + iCurrentUsingSize) % _iMax;

				return iCurrentUsingSize;
			}
		}
		//정상적으로 해당 데이터 만큼 들어가있다고 가정할 때...
		else if (iSize <= iCurrentUsingSize)
		{
			//짤릴 위험성이 있는 경우.
			if (idxRead > idxWrite)
			{
				int iLengthToEnd = _iMax - idxRead;
				int iLengthToWrite = idxWrite;
				if (iLengthToEnd >= iSize)
				{
					memcpy_s(chpDest, iSize, &_arrQueue[idxRead], iSize);
					if (!bPeek)
						_idxRead = (idxRead + iSize) % _iMax;
					return iSize;
				}
				else
				{
					memcpy_s(chpDest, iSize, &_arrQueue[idxRead], iLengthToEnd);
					//_idxRead = (_idxRead + iLengthToEnd) % _iMax;
					memcpy_s(&chpDest[iLengthToEnd], iSize - iLengthToEnd, _arrQueue, iSize - iLengthToEnd);
					if (!bPeek)
						_idxRead = (idxRead + iSize) % _iMax;
					return iSize;
				}
			}
			//없는 경우
			else
			{
				memcpy_s(chpDest, iSize, &_arrQueue[idxRead], iSize);
				if (!bPeek)
					_idxRead = (idxRead + iSize) % _iMax;
				return iSize;
			}
		}
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
	//
	// Parameters: (char *)데이타 포인터. (int)크기.
	// Return: (int)가져온 크기.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::Peek(char *chpDest, int iSize)
	{
		int ret = Dequeue(chpDest, iSize, true);
		return ret;
	}


	/////////////////////////////////////////////////////////////////////////
	// 원하는 길이만큼 읽기위치 에서 삭제 / 쓰기 위치 이동
	//
	// Parameters: 없음.
	// Return: 없음.
	/////////////////////////////////////////////////////////////////////////
	void	RingQueue::RemoveData(int iSize)
	{
		LONG idxRead;
		InterlockedExchange(&idxRead, _idxRead);
		if (iSize > GetCurrentUsingSize())
		{
			//__Log(dfLOG_SYSTEM, L"RemoveData() iSize [%d] > CurrentUsingSize [%d]", iSize, GetCurrentUsingSize());
			if (iSize <= GetCurrentUsingSize()) {
				goto kkk;
			}
			return;
		}
		//_idxRead = (idxRead + iSize) % _iMax;
	kkk:
		InterlockedExchange((LONG*)&_idxRead, (idxRead + iSize) % _iMax);
	}
	int		RingQueue::MoveWritePos(int iSize)
	{
		LONG idxWrite;
		InterlockedExchange(&idxWrite, _idxWrite);
		//_idxWrite = (idxWrite + iSize) % _iMax;
		InterlockedExchange((LONG*)&_idxWrite, (idxWrite + iSize) % _iMax);
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 모든 데이타 삭제.
	//
	// Parameters: 없음.
	// Return: 없음.
	/////////////////////////////////////////////////////////////////////////
	void	RingQueue::ClearBuffer(void)
	{
		_idxRead = 0;
		_idxWrite = 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetBufferPtr(void)
	{
		return _arrQueue;
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 ReadPos 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetReadBufferPtr(void)
	{
		LONG idxRead;
		InterlockedExchange(&idxRead, _idxRead);
		return &_arrQueue[idxRead];
	}

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 WritePos 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetWriteBufferPtr(void)
	{
		LONG idxWrite;
		InterlockedExchange(&idxWrite, _idxWrite);
		return &_arrQueue[idxWrite];
	}
}
