/////////////////////////////////////////////////////////////////////
// www.gamecodi.com						������ master@gamecodi.com
//
//
/////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------

Aya Library - Packet.

��Ʈ��ũ ��Ŷ�� Ŭ����.
�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

- ����.

CAyaPacket cPacket;

�ֱ�.
clPacket << 40030;	or	clPacket << iValue;	(int �ֱ�)
clPacket << 3;		or	clPacket << byValue;	(BYTE �ֱ�)
clPacket << 1.4;	or	clPacket << fValue;	(float �ֱ�)

����.
clPacket >> iValue;	(int ����)
clPacket >> byValue;	(BYTE ����)
clPacket >> fValue;	(float ����)

!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ����ϸ� �ȵȴ�.

2017.12.25
���� ������� ��������Ѵ�.
----------------------------------------------------------------*/
#include <Windows.h>
#ifndef  __AYA_PACKET__
#define  __AYA_PACKET__
namespace CarminoJ
{
	class PacketBuffer
	{
	public:
		/*---------------------------------------------------------------
		AyaPacket Enum.
		----------------------------------------------------------------*/
		enum enAYA_PACKET
		{
			eBUFFER_DEFAULT = 500		// ��Ŷ�� �⺻ ���� ������.
		};

		//////////////////////////////////////////////////////////////////////////
		// ������, �ı���.
		//
		// Return:
		//////////////////////////////////////////////////////////////////////////
		PacketBuffer();
		PacketBuffer(int iBufferSize);

		virtual	~PacketBuffer()
		{
		}
		//////////////////////////////////////////////////////////////////////////
		// ��Ŷ  �ı�.
		//
		// Parameters: ����.
		// Return: ����.
		//////////////////////////////////////////////////////////////////////////
		void	Release(void);

		//////////////////////////////////////////////////////////////////////////
		// ��Ŷ û��.
		//
		// Parameters: ����.
		// Return: ����.
		//////////////////////////////////////////////////////////////////////////
		void	Clear(void);

		//////////////////////////////////////////////////////////////////////////
		// ���� ������ ���.
		//
		// Parameters: ����.
		// Return: (int)��Ŷ ���� ������ ���.
		//////////////////////////////////////////////////////////////////////////
		int		GetBufferSize(void) { return m_iBufferSize; }

		//////////////////////////////////////////////////////////////////////////
		// ���� ������� ������ ���.
		//
		// Parameters: ����.
		// Return: (int)������� ����Ÿ ������.
		//////////////////////////////////////////////////////////////////////////
		int		GetDataSize(void) { return m_iDataSize; }

		//////////////////////////////////////////////////////////////////////////
		// ���� ������ ���.
		//
		// Parameters: ����.
		// Return: (char *)���� ������.
		//////////////////////////////////////////////////////////////////////////
		char	*GetBufferPtr(void) { return m_chaBufferDefault; }

		//////////////////////////////////////////////////////////////////////////
		// ���� Pos �̵�. (�����̵��� �ȵ�)
		// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
		//
		// Parameters: (int) �̵� ������.
		// Return: (int) �̵��� ������.
		//////////////////////////////////////////////////////////////////////////
		int		MoveWritePos(int iSize);
		int		MoveReadPos(int iSize);

		/* ============================================================================= */
		// ������ ���۷�����.
		/* ============================================================================= */
		PacketBuffer&	operator = (PacketBuffer &clSrcAyaPacket);

		//////////////////////////////////////////////////////////////////////////
		// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
		//////////////////////////////////////////////////////////////////////////
		PacketBuffer&	operator << (BYTE byValue);
		PacketBuffer&	operator << (char chValue);

		PacketBuffer&	operator << (short shValue);
		PacketBuffer&	operator << (WORD wValue);

		PacketBuffer&	operator << (int iValue);
		PacketBuffer&	operator << (DWORD dwValue);
		PacketBuffer&	operator << (float fValue);

		PacketBuffer&	operator << (__int64 iValue);
		PacketBuffer&	operator << (double dValue);

		//////////////////////////////////////////////////////////////////////////
		// ����.	�� ���� Ÿ�Ը��� ��� ����.
		//////////////////////////////////////////////////////////////////////////
		PacketBuffer	&operator >> (BYTE &byValue);
		PacketBuffer	&operator >> (char &chValue);

		PacketBuffer	&operator >> (short &shValue);
		PacketBuffer	&operator >> (WORD &wValue);

		PacketBuffer	&operator >> (int &iValue);
		PacketBuffer	&operator >> (DWORD &dwValue);
		PacketBuffer	&operator >> (float &fValue);

		PacketBuffer	&operator >> (__int64 &iValue);
		PacketBuffer	&operator >> (double &dValue);

		//////////////////////////////////////////////////////////////////////////
		// ����Ÿ ���.
		//
		// Parameters: (char *)Dest ������. (int)Size.
		// Return: (int)������ ������.
		//////////////////////////////////////////////////////////////////////////
		int		Enqueue(char *chpDest, int iSize);

		//////////////////////////////////////////////////////////////////////////
		// ����Ÿ ����.
		//
		// Parameters: (char *)Src ������. (int)SrcSize.
		// Return: (int)������ ������.
		//////////////////////////////////////////////////////////////////////////
		int		Dequeue(char *chpSrc, int iSrcSize);

	protected:
		//------------------------------------------------------------
		// ��Ŷ���� / ���� ������.
		//------------------------------------------------------------
		char	m_chaBufferDefault[eBUFFER_DEFAULT];
		int		m_iBufferSize;

		//------------------------------------------------------------
		// ������ ���� ��ġ, ���� ��ġ.
		//------------------------------------------------------------
		char	*m_chpReadPos;
		char	*m_chpWritePos;

		int		_idxRead;
		int		_idxWrite;
		//------------------------------------------------------------
		// ���� ���ۿ� ������� ������.
		//------------------------------------------------------------
		int		m_iDataSize;
	};
}
#endif