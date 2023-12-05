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
	//���α׷� ���� �� ���� ���� delete ���ֱ�
	Scene* scene = CurScene;
	delete scene;

	delete mainCamera;

}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	AllocConsole();	// �ܼ�â ����
	_tfreopen(_T("CONOUT$"), _T("w"), stdout);
	_tfreopen(_T("CONIN$"), _T("r"), stdin);
	_tfreopen(_T("CONERR$"), _T("w"), stderr);

	// ���� Scene = menuScene
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
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	// ����
			return;
		}
		else if (wParam == VK_Q) {
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	// ����
			return;
		}
	}
	}
	CurScene->processKey(iMessage, wParam, lParam);	//���� ���� ���μ���Ű
}

float WGameFramework::GetTick()
{
	return (float)(curFrameTime - prevFrameTime) / 1000;
}