#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__

// server에 필요한 구조체 정리
// 클래스로 할까 하다가
// 변수의 크기가 작거나, 수명이 짧고, 자주 할당되는 객체라서 구조체 사용
// 태그로 인용해 사용함

// 임시로 사용하는 ,x/y 구조체(float, int)
typedef struct Infomation
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

// 위치
typedef struct Position
{
	float	fX;
	float	fY;
}POS;

// 프레임 애니메이션 (시작 및 끝 인덱스, 장면 인덱스, 프레임 속도 및 경과 시간)
typedef struct Frame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;        
}FRAME;

// 프레임 애니메이션의 시작, 장면, 및 키 인덱스를 나타내는 변수 ... 재정의 오류나서 주석처리
//typedef struct FrameInfo
//{
//	int	iFrameStart;		// 
//	int	iFrameScene;		// 
//	int	iFrameKey;			//  ex)PLAYER_DOWN...애니메이션 유형이름			
//}FRAME_INFO;

// 플레이어 초기화 정보 (위치, 시작 여부, 플레이어 수, 및 인덱스)
typedef struct PlayerInit
{
	POS pos[3] = { 0 };
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT;

// 전송 플레이어 초기화 정보 (위치, 시작 여부, 플레이어 수, 및 인덱스)
typedef struct PlayerInitSend
{
	POS pos = { 0 };
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT_SEND;

// 플레이어 정보
typedef struct PlayerInfo
{
	POS pos = { 0 };						// 위치
	//FRAME_INFO	frameInfo;				// 애니메이션 프레임 정보

	int hp;									// 플레이어 hp
	int id;									// 플레이어 id
	bool start;									// 시작 여부
	bool button;								// 키입력도 전송받을 지는 모르겠음

	bool isHit = false;							// 충돌 판정
	bool isDead = false;						// 죽었다 살아나면 false로 변환 필요 / True일때 충돌x
	ENDING::END_TYPE eEnding = ENDING::ING;		// 엔드 확인

}PLAYER_INFO;

// 데이터 저장 (전송 용도)
typedef struct tagStoreData
{
	PLAYER_INFO playersInfo[3] = { 0 };
	int id = 0;
	int hp[3] = {};
	bool start;
}STORE_DATA;

//////////////////////////////////// 

// 체력 Up 아이템 (Heart)
typedef struct Heart
{
	int	index;
	POS	pos;
}HEART;

// Coin 아이템;
typedef struct Coin
{
	int	index;
	POS	pos;
}COIN;

//Heart + Coin Struct
struct Iteminfo
{
	int iSize = 0;
	HEART* heart;
	COIN* coin;
};


#endif // !__STRUCT_H__
