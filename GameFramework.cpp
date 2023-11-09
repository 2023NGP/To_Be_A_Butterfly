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
	Scene* scene = curScene;
	delete scene;
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	// ���� Scene = menuScene
	curScene = new menuScene;
	nowScene = MENU;
	mainCamera = new Camera;

	curScene->init();
}

void WGameFramework::OnDraw(HDC hdc)
{
	curScene->Render(hdc);
}

void WGameFramework::OnUpdate(const float frameTime)
{
	curScene->Update(frameTime);
	mainCamera->Update(frameTime);
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_Q) {
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);	//Q ������ ����
			return;
		}
	}
	}
	curScene->processKey(iMessage, wParam, lParam);	//���� ���� ���μ���Ű
}

float WGameFramework::GetTick()
{
	return (float)(curFrameTime - prevFrameTime) / 1000;
}