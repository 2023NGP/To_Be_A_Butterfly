#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

// 최대 접속 가능한 클라이언트 수
#define CLIENT_COUNT 3
#define CLOUD_COUNT 73

#define WINCX 1280
#define WINCY 720
//#define WINCX 800
//#define WINCY 600


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

#define MAP_EDGE 50
#define	REVIVE_TIME 10000

#define BUF_SIZE 1024

#endif // !__DEFINE_H__
