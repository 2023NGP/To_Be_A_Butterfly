#include "lobbyScene.h"
#include "stdafx.h"

// 이곳에서 플레이어 접속 확인, 3명 모두 접속하면 게임 시작

extern WGameFramework framework;

lobbyScene::~lobbyScene()
{

}

void lobbyScene::init()
{
	//메뉴 화면에서 그릴 리소스 위치 초기화
	background.Load(TEXT("image/로비배경.png"));
	
}
void lobbyScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		switch (wParam) {

		case VK_RETURN:
			Scene* scene = framework.CurScene;   ////현재 씬을 tmp에 넣고 지워줌
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

void lobbyScene::Update(const float frameTime)
{
	//애니메이션 있으면 여기서 업데이트
}

void lobbyScene::Render(HDC hdc)
{
	background.Draw(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, 0, 0, background.GetWidth(), background.GetHeight());
	//--- 메모리 DC에 배경 그리기
}