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
	// ������ Max ������ ������
	//
	// Param : ����
	// Return : int ������ Max ������.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetBufferSize(void)
	{
		return _iMax;
	}

	/////////////////////////////////////////////////////////////////////////
	// ���� ������� �뷮 ���.
	//
	// Parameters: ����.
	// Return: (int)������� �뷮.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetCurrentUsingSize(void)
	{
		// Interlock �迭�� ���������.
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		// �� ������ �ε����� �޾ƿͼ� ��Ƽ������ ȯ�� ȣ�⿡�� �����ϵ��� �Ѵ�.
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
	// ���� ���ۿ� ���� �뷮 ���.
	//
	// Parameters: ����.
	// Return: (int)�����뷮.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetFreeSize(void)
	{
		return (_iMax - GetCurrentUsingSize()) - 1;
	}

	/////////////////////////////////////////////////////////////////////////
	// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
	// (������ ���� ����)
	//
	// Parameters: ����.
	// Return: (int)��밡�� �뷮.
	////////////////////////////////////////////////////////////////////////
	int	RingQueue::GetNotBrokenGetSize(void)
	{
		// Interlock �迭�� ���������.
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
	// ���� �� �ִ� ������.
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
	// WritePos �� ����Ÿ ����.
	//
	// Parameters: (char *)����Ÿ ������. (int)ũ��. 
	// Return: (int)���� ũ��.
	//
	// �����ڵ� -2 �� �迭�� ���̺��� iSize�� ũ�� �־��� �� ��´�.
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
	// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
	//
	// Parameters: (char *)����Ÿ ������. (int)ũ��. bool bPeek == TRUE �� Peek �̹Ƿ� _idxRead �Ȱǵ帱 ����
	// Return: (int)������ ũ��.
	//
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::Dequeue(char *chpDest, int iSize, bool bPeek)
	{
		LONG idxWrite, idxRead;
		InterlockedExchange(&idxWrite, _idxWrite);
		InterlockedExchange(&idxRead, _idxRead);
		int iCurrentUsingSize = GetCurrentUsingSize();

		// ����ִ� ť���� �� ���������� �ϴ°Ŵ�??
		if (iCurrentUsingSize == 0)
		{
			return 0;
		}
		// �����Ͱ� ���� ��Ȳ�Դϴٸ�...?
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
		//���������� �ش� ������ ��ŭ ���ִٰ� ������ ��...
		else if (iSize <= iCurrentUsingSize)
		{
			//©�� ���輺�� �ִ� ���.
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
			//���� ���
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
	// ReadPos ���� ����Ÿ �о��. ReadPos ����.
	//
	// Parameters: (char *)����Ÿ ������. (int)ũ��.
	// Return: (int)������ ũ��.
	/////////////////////////////////////////////////////////////////////////
	int	RingQueue::Peek(char *chpDest, int iSize)
	{
		int ret = Dequeue(chpDest, iSize, true);
		return ret;
	}


	/////////////////////////////////////////////////////////////////////////
	// ���ϴ� ���̸�ŭ �б���ġ ���� ���� / ���� ��ġ �̵�
	//
	// Parameters: ����.
	// Return: ����.
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
	// ������ ��� ����Ÿ ����.
	//
	// Parameters: ����.
	// Return: ����.
	/////////////////////////////////////////////////////////////////////////
	void	RingQueue::ClearBuffer(void)
	{
		_idxRead = 0;
		_idxWrite = 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ ������ ����.
	//
	// Parameters: ����.
	// Return: (char *) ���� ������.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetBufferPtr(void)
	{
		return _arrQueue;
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ ReadPos ������ ����.
	//
	// Parameters: ����.
	// Return: (char *) ���� ������.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetReadBufferPtr(void)
	{
		LONG idxRead;
		InterlockedExchange(&idxRead, _idxRead);
		return &_arrQueue[idxRead];
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ WritePos ������ ����.
	//
	// Parameters: ����.
	// Return: (char *) ���� ������.
	/////////////////////////////////////////////////////////////////////////
	char*	RingQueue::GetWriteBufferPtr(void)
	{
		LONG idxWrite;
		InterlockedExchange(&idxWrite, _idxWrite);
		return &_arrQueue[idxWrite];
	}
}
