#pragma once
#include "clearScene.h"
#include "stdafx.h"

extern WGameFramework framework;

clearScene::~clearScene()
{

}

void clearScene::init()
{
	back.Load(TEXT("image/클리어화면.png"));
	txtmotion.Load(TEXT("image/stageclear.png"));
	PlaySound(TEXT("sound/clear.wav"), NULL, SND_ASYNC /*| SND_LOOP*/);
	size = 30;
}
void clearScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_RETURN:
			PlaySound(NULL, NULL, NULL);
			scene* scene = framework.curScene;
			if (framework.nowscene == STAGE2) {
				framework.curScene = new menuScene;
				framework.curScene->init();
				framework.nowscene = MENU;
			}
			else {
				framework.curScene = new stage2Scene;
				framework.curScene->init();
				framework.nowscene = STAGE2;
			}
			delete scene;
			break;
		}

	}
	break;
	}
}

void clearScene::Update(const float frameTime)
{
	if (size <= 15)
		size += 30;
	else 
		size -= 30;
}

void clearScene::Render(HDC hdc)
{
	back.Draw(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, 0, 0, back.GetWidth(), back.GetHeight());
	txtmotion.Draw(hdc, size, size, FRAME_WIDTH - 2*size, FRAME_HEIGHT - 2*size, 0, 0, txtmotion.GetWidth(), txtmotion.GetHeight());

}