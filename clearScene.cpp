#pragma once
#include "clearScene.h"
#include "stdafx.h"

FMOD::System* pSystem_c;
FMOD::Sound* bgSound_c;
FMOD::Channel* Channel_c;

extern WGameFramework framework;

clearScene::~clearScene()
{

}

void clearScene::init()
{
	back.Load(TEXT("image/클리어화면.png"));
	txtmotion.Load(TEXT("image/stageclear.png"));
	FMOD::System_Create(&pSystem_c);
	pSystem_c->init(4, FMOD_INIT_NORMAL, NULL);
	pSystem_c->createSound("sound/clear.wav", FMOD_LOOP_NORMAL, 0, &bgSound_c);		// 배경음악
	pSystem_c->playSound(bgSound_c, NULL, 0, &Channel_c);
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
			bgSound_c->release();
			Scene* scene = framework.curScene;
			if (framework.nowScene == STAGE2) {
				framework.curScene = new menuScene;
				framework.curScene->init();
				framework.nowScene = MENU;
			}
			else {
				framework.curScene = new stage2Scene;
				framework.curScene->init();
				framework.nowScene = STAGE2;
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