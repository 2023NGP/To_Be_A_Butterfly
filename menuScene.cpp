#include "menuScene.h"
#include "stdafx.h"
#include "server.h"

extern WGameFramework framework;

FMOD::System* pSystem_m;
FMOD::Sound* bgSound_m;
FMOD::Channel* Channel_m;

menuScene::~menuScene()
{

}

void menuScene::init()
{
	//�޴� ȭ�鿡�� �׸� ���ҽ� ��ġ �ʱ�ȭ
	background.Load(TEXT("image/����ȭ��1.png"));
	normalCloud.Load(TEXT("image/�Ϲݱ���.png"));
	rainCloud.Load(TEXT("image/�񱸸�1.png"));
	darkCloud.Load(TEXT("image/�Ա�������.png"));
	press.Load(TEXT("image/�����Ϸ��鴩������.png"));

	FMOD::System_Create(&pSystem_m);
	pSystem_m->init(4, FMOD_INIT_NORMAL, NULL);
	pSystem_m->createSound("sound/adventure.mp3", FMOD_LOOP_NORMAL, 0, &bgSound_m);		// �������

	pSystem_m->playSound(bgSound_m, NULL, 0, &Channel_m);
	
	cloudSizeCount = 0;
	cloudSize = 300;
	rainCloudSize = 250;
	darkCloudSize = 230;
	pressText = 0;
	ren = 0;
}
void menuScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		switch (wParam) {
		
		case VK_RETURN:
			bgSound_m->release();
			Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
			framework.CurScene = new lobbyScene;
			framework.CurScene->init();
			framework.NowScene = LOBBY;
			delete scene;
			break;

		}
	}
	break;

	}
}

void menuScene::Update(const float frameTime)
{
	ren++;
	if (ren >= 10)
		ren = 0;
	if (cloudSizeCount >= 10)
		cloudSizeCount = 0;
	if (cloudSizeCount % 2 == 0) {

		//cloudSize += 20;
		//rainCloudSize -= 10;
		//darkCloudSize += 40;
	}
	else {
		//cloudSize -= 20;
		//rainCloudSize += 10;
		//darkCloudSize -= 40;
	}
	//cloudSizeCount++;

	if (pressText % 2 == 0)
		pressText = 0;
	else if (pressText % 2 == 1)
		pressText = 1;
	pressText++;

	if (RecvInitData()) {
		Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
		framework.CurScene = new stage2Scene;
		framework.CurScene->init();
		framework.NowScene = STAGE2;
		delete scene;
	}
	//�ִϸ��̼� ������ ���⼭ ������Ʈ
}

void menuScene::Render(HDC hdc)
{
	background.Draw(hdc, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, 0, 0, background.GetWidth(), background.GetHeight());
	normalCloud.Draw(hdc, 0, 400, cloudSize, cloudSize, 512 * ren, 0, normalCloud.GetHeight(), normalCloud.GetHeight());
	press.Draw(hdc, 390, 550, 430, 45, 0, 0, press.GetWidth(), press.GetHeight());
	rainCloud.Draw(hdc, 900, 300, rainCloudSize, rainCloudSize+50, 512 * ren, 0, CLOUD_IMAGE_SIZE, rainCloud.GetHeight());
	darkCloud.Draw(hdc, 500, 0, darkCloudSize, darkCloudSize, 512 * ren, 0, darkCloud.GetHeight(), darkCloud.GetHeight());
	//--- �޸� DC�� ��� �׸���
}