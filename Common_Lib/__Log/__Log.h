#pragma once
#include <Windows.h>
using namespace std;

#define dfLOG_DEVELOP	0	//	그냥 정상적인 활동
#define dfLOG_WARNING	1	//	서버가 죽는 에러는 아니지만, 비정상적인 상황이다.
#define dfLOG_ERROR		2	//	비정상적인 클라이언트이다. 어뷰징이거나 해킹의 가능성이 있다. 클라이언트 종료
#define dfLOG_SHUTDOWN  3	//	말도 안되는 에러. 서버 종료된다...
#define dfLOG_NOTPRINT	4	//
//-------------------------------------------------------
//  17.01.18
//	로그 작성용
//
//	DWORD in_dwLevel : #define 된 로그의 레벨
//	format, 가변인자
//
//	서버 콘솔창에 찍고, 로그 텍스트파일에 찍는다.
//-------------------------------------------------------
void __Log(DWORD in_dwLevel, const WCHAR* format, ...);