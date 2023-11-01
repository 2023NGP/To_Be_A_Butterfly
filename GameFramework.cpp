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

	scene* scene = curScene;
	delete scene;
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	curScene = new menuScene;	//처음에는 게임 시작 화면으로
	nowscene = MENU;
	curScene->init();
}

void WGameFramework::OnDraw(HDC hdc)
{
	curScene->Render(hdc);
}

void WGameFramework::OnUpdate(const float frameTime)
{
	curScene->Update(frameTime);
	//여기서 현재 씬 업데이트
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_Q) {
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	//Q 누르면 종료
			return;
		}
	}
	}
	curScene->processKey(iMessage, wParam, lParam);	//현재 씬의 프로세스키
}

float WGameFramework::GetTick()
{
	return (float)(curFrameTime - prevFrameTime) / 1000;
}