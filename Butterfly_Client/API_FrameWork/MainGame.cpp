#include "stdafx.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "Player.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"

CMainGame::CMainGame()
	: m_iFPS(0), m_dwTime(GetTickCount())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map_long.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");
	//CBmpMgr::Get_Instance()->Insert_Bmp(__T(""), __T(""));

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();

}

void CMainGame::Render()
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Back");	// 게임 배경
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Bmp(L"BackBuffer");
	//BitBlt(hBackBuffer, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	StretchBlt(hBackBuffer, 0, 0, WINCX, WINCY,
		hMemDC, 0, g_CameraLookAt_Y, WINCX, WINCY, SRCCOPY);
	CSceneMgr::Get_Instance()->Render(hBackBuffer);

	// m_DC
	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
	
	// 프레임레이트 출력 
	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
}

void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();


	CObjMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}


