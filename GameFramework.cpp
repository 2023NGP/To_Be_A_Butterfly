#include "stdafx.h"
#include "GameFramework.h"


WGameFramework::WGameFramework()
{
	
}

WGameFramework::~WGameFramework()
{
}

void WGameFramework::Reset()
{

}

void WGameFramework::Clear()
{	
	//프로그램 끝낼 때 현재 씬도 delete 해주기
	Scene* scene = CurScene;
	delete scene;

	delete mainCamera;

}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	AllocConsole();	// 콘솔창 띄우기
	_tfreopen(_T("CONOUT$"), _T("w"), stdout);
	_tfreopen(_T("CONIN$"), _T("r"), stdin);
	_tfreopen(_T("CONERR$"), _T("w"), stderr);

	// 시작 Scene = menuScene
	CurScene = new menuScene;
	NowScene = MENU;
	mainCamera = new Camera;

	CurScene->init();
}

void WGameFramework::OnDraw(HDC hdc)
{
	CurScene->Render(hdc);
}

void WGameFramework::OnUpdate(const float frameTime)
{
	CurScene->Update(frameTime);
	mainCamera->Update(frameTime);
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE) {
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	// 종료
			return;
		}
		else if (wParam == VK_Q) {
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	// 종료
			return;
		}
	}
	}
	CurScene->processKey(iMessage, wParam, lParam);	//현재 씬의 프로세스키
}

float WGameFramework::GetTick()
{
	return (float)(curFrameTime - prevFrameTime) / 1000;
}