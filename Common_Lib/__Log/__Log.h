#pragma once
#include <Windows.h>
using namespace std;

#define dfLOG_DEVELOP	0	//	�׳� �������� Ȱ��
#define dfLOG_WARNING	1	//	������ �״� ������ �ƴ�����, ���������� ��Ȳ�̴�.
#define dfLOG_ERROR		2	//	���������� Ŭ���̾�Ʈ�̴�. ���¡�̰ų� ��ŷ�� ���ɼ��� �ִ�. Ŭ���̾�Ʈ ����
#define dfLOG_SHUTDOWN  3	//	���� �ȵǴ� ����. ���� ����ȴ�...
#define dfLOG_NOTPRINT	4	//
//-------------------------------------------------------
//  17.01.18
//	�α� �ۼ���
//
//	DWORD in_dwLevel : #define �� �α��� ����
//	format, ��������
//
//	���� �ܼ�â�� ���, �α� �ؽ�Ʈ���Ͽ� ��´�.
//-------------------------------------------------------
void __Log(DWORD in_dwLevel, const WCHAR* format, ...);