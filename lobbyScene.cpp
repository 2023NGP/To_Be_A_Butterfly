#include "lobbyScene.h"
#include "stdafx.h"

// �̰����� �÷��̾� ���� Ȯ��, 3�� ��� �����ϸ� ���� ����

extern WGameFramework framework;

lobbyScene::~lobbyScene()
{

}

void lobbyScene::init()
{
	//�޴� ȭ�鿡�� �׸� ���ҽ� ��ġ �ʱ�ȭ
	background.Load(TEXT("image/�κ���.png"));
	
}
void lobbyScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		switch (wParam) {

		case VK_RETURN:
			Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
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
	//�ִϸ��̼� ������ ���⼭ ������Ʈ
}

void lobbyScene::Render(HDC hdc)
{
	background.Draw(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, 0, 0, background.GetWidth(), background.GetHeight());
	//--- �޸� DC�� ��� �׸���
}