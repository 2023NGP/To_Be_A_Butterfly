#pragma once

#ifndef __EXTERN_H__
#define __EXTERN_H__

extern HWND g_hWnd;
extern POTIONRES g_tHeartRes;
extern COINRES g_tCoinRes;
extern CLOUDRES g_tCloudRes;
extern float g_CameraLookAt_Y;
extern PLAYER_INIT_SEND g_tPlayerInit;
extern HANDLE hServerProcess;
extern DWORD WINAPI ServerProcess(LPVOID arg);

#endif // !__EXTERN_H__
