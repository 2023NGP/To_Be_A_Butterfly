#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

// define
#define WINCX 1280
#define WINCY 720

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr; }

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define PI 3.141592f

#define TILECX 145 //bmp크기
#define TILECY 145

#define TILE_RECTCX 50 //Rect크기
#define TILE_RECTCY 50

#define TILEX 30
#define TILEY 30

#define CHAR_CX 100
#define CHAR_CY 100

#define PLAYER_ATTACK 10

#define SERVERPORT 9000
#define BUFSIZE    512

#define MAP_EDGE 50
#define	REVIVE_TIME 10000

#define POTION_TIME 10
#define START_TIME 5

// enum
namespace OBJID
{
	enum ID { PLAYER, COIN, HEART, EFFECT, INGAME_UI, UI, MOUSE, END };
}

namespace GROUPID
{
	enum ID { BACKGROUND, OBJECT, EFFECT, UI, END };
}

namespace ENDING
{
	enum END_TYPE { ING, WIN, LOSE, END };
}

//// extern
//extern HWND g_hWnd;
//extern POTIONRES g_tHpPotionRes;
//extern PLAYER_INIT_SEND g_tPlayerInit;
//extern HANDLE hServerProcess;
//extern DWORD WINAPI ServerProcess(LPVOID arg);
//

#endif // !__DEFINE_H__