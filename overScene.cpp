#pragma once
#include "stdafx.h"
#include "overScene.h"

extern WGameFramework framework;

FMOD::System* pSystem_o;
FMOD::Sound* bgSound_o;
FMOD::Channel* Channel_o;

overScene::~overScene()
{

}

void overScene::init()
{
    //�޴� ȭ�鿡�� �׸� ���ҽ� ��ġ �ʱ�ȭ
    background1.Load(TEXT("image/�������.png"));
    txt.Load(TEXT("image/gameover.png"));
    ren1 = 30;
    y = 0;

    FMOD::System_Create(&pSystem_o);
    pSystem_o->init(4, FMOD_INIT_NORMAL, NULL);
    pSystem_o->createSound("sound/over.wav", FMOD_LOOP_NORMAL, 0, &bgSound_o);		// �������
    pSystem_o->playSound(bgSound_o, NULL, 0, &Channel_o);
}
void overScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_RETURN:
            bgSound_o->release();
            Scene* scene = framework.CurScene;   //���� ���� tmp�� �ְ� ������
            framework.CurScene = new gameScene;
            framework.CurScene->init();
            framework.NowScene = GAME;
            delete scene;
            break;
        }
    }
    break;

    }
}

void overScene::Update(const float frameTime)
{

    if (y < 250)
        y += 15;

    //�ִϸ��̼� ������ ���⼭ ������Ʈ
}

void overScene::Render(HDC hdc)
{
    background1.Draw(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, 0, 0, background1.GetWidth(), background1.GetHeight());
    txt.Draw(hdc, ren1, ren1+y, FRAME_WIDTH-2*ren1, FRAME_HEIGHT-2*ren1, 0, 0, txt.GetWidth(), txt.GetHeight());
   
    //--- �޸� DC�� ��� �׸���
}