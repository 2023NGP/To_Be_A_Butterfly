#include "stdafx.h"
#include "gameScene.h"

//memdc에 그려주는 역할, frame
extern WGameFramework framework;

gameScene::~gameScene()
{

}

void  gameScene::InitSound()
{
	// 사운드 INIT
	FMOD::System_Create(&pSystem);
	pSystem->init(4, FMOD_INIT_NORMAL, NULL);
	pSystem->createSound("sound/mainsound.wav", FMOD_LOOP_NORMAL, 0, &bgSound);		// 배경음악
	pSystem->createSound("effect/Coin01.wav", FMOD_DEFAULT, 0, &effectSound[0]);	// 코인 획득
	pSystem->createSound("effect/Rise02.wav", FMOD_DEFAULT, 0, &effectSound[1]);	// 하트 획득
	pSystem->createSound("effect/Rise05.wav", FMOD_DEFAULT, 0, &effectSound[2]);	// 충돌
	pSystem->createSound("effect/Rise07.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// 성공
	pSystem->createSound("effect/Downer01.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// 실패

	pSystem->playSound(bgSound, NULL, 0, &Channel[0]);

	// 볼륨 설정

}

void gameScene::InitCloud() {       //txt파일에서 구름 정보 받아오는 함수
	FILE* fp;
	fopen_s(&fp, "image/map1.txt", "r");
	random_device rd;
	uniform_int_distribution <int> dis(0, 49);

	int i = 0;
	if (fp == NULL)//열기 실패일 때
	{
		perror("fopen 실패");//에러 메시지 출력
		return;
	}

	while (!feof(fp)) {
		fscanf_s(fp, "%d %d %d", &cloud[i].cx, &cloud[i].cy, &cloud[i++].type);
		cloud[i].index = dis(rd);
	}

	cloud_index = i;
	fclose(fp);
}
void gameScene::InitHeart() {
	FILE* fp;
	fopen_s(&fp, "image/map1(heart).txt", "r");

	int i = 0;
	if (fp == NULL)
	{
		perror("fopen 실패");
		return;
	}

	while (!feof(fp)) {
		fscanf_s(fp, "%d %d %d", &item[i].ix, &item[i].iy, &item[i].what);
		++i;
	}

	item_index = i;
	fclose(fp);
}
void gameScene::InitAnimation() {
	int j = 0;
	//14개
	//1애니메이션 4개
	//0 ~ 3, ... ~39
	for (int i = 0; i < 14; i++) {
		animation[j] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 1] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 2] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 3] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		j += 4;
	}

	j = 0;

	//0~4, 5~9, 10~14, 15~19, 20~24, |25~29, 30~34, 35~39, 40~44, 45~49,| 50 ~54, 55~59 까지 같은 이미지라 보면 됨
	//업데이트 속도가 너무 빨라서 느리게 움직이게 하기 위함

	for (int i = 0; i < 15; i++) {
		if (i == 10 || i == 11) {
			cloud_ani[j] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 1] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 2] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 3] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 4] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
		}
		else if (i == 12 || i == 13 || i == 14) {
			cloud_ani[j] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 1] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 2] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 3] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 4] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
		}
		else {
			cloud_ani[j] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 1] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 2] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 3] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
			cloud_ani[j + 4] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
		}
		j += 5;
	}
	j = 0;
	for (int i = 0; i < 7; i++) {
		if (i == 5 || i == 6) {
			rain_ani[j] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 1] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 2] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 3] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 4] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
		}
		else {
			rain_ani[j] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 1] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 2] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 3] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
			rain_ani[j + 4] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
		}
		j += 5;
	}
}
void gameScene::init()
{
	startX = 0, startY = MEM_HEIGHT - (FRAME_HEIGHT);

	player_image.Load(TEXT("image/꼬물이.png"));
	background.Load(TEXT("image/배경화면1.png"));

	normalCloud.Load(TEXT("image/일반구름.png"));
	rainCloud.Load(TEXT("image/비구름.png"));
	rain.Load(TEXT("image/비.png"));
	darkCloud.Load(TEXT("image/먹구름.png"));

	heart.Load(TEXT("image/heart.png"));
	stone.Load(TEXT("image/coin.png"));

	InitSound();
	InitAnimation();
	InitCloud();
	InitHeart();
	ani_index = 0;      //충돌이면 20~27, 평상시면 0~19
	gravity = 1;
	bar_startY = PLAYER_FIRSTY + 100;
	fall = true;

	status = RUN;

	player.px = MEM_WIDTH / 2;
	player.py = PLAYER_FIRSTY;
	player.SetStatus(IDLE);
	player.jumpForce = 0;
	cout << "stage 1" << endl;
}

void gameScene::drawPlayer(HDC hdc) {
	//플레이어 그리는 함수
	player_image.Draw(hdc, player.px, player.py, PLAYER_WIDTH, PLAYER_HEIGHT, animation[ani_index].left, animation[ani_index].top,
		PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE);
}
void gameScene::drawBackGround(HDC hdc) {
	//배경 그리는 함수
	background.BitBlt(hdc, 0, 0, SRCCOPY);
}
void gameScene::drawCloud(HDC hdc) {
	//구름 그리는 함수
	for (int j = 0; j < cloud_index; ++j) {
		switch (cloud[j].type) {
		case 1:
			darkCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT, cloud_ani[cloud[j].index].left, cloud_ani[cloud[j].index].top, CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE);
			break;
		case 2:
			if (cloud[j].index >= 25 && cloud[j].index <= 59) {
				rainCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[cloud[j].index].left, cloud_ani[cloud[j].index].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
				rain.Draw(hdc, cloud[j].cx, cloud[j].cy + (CLOUD_HEIGHT - 30), CLOUD_WIDTH, CLOUD_HEIGHT, rain_ani[cloud[j].index - 25].left, rain_ani[cloud[j].index - 25].top, CLOUD_IMAGE_SIZE, RAIN_IMAGE);
			}
			else
				rainCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[cloud[j].index].left, cloud_ani[cloud[j].index].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
			break;
		case 3:
			normalCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT, cloud_ani[cloud[j].index].left, cloud_ani[cloud[j].index].top, CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE);
			break;
		}
	}
}
void gameScene::drawItems(HDC hdc) {
	for (int j = 0; j < item_index; ++j) {
		switch (item[j].what) {
		case 1:
			heart.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, heart.GetWidth(), heart.GetHeight());
			break;
		case 2:
			stone.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, stone.GetWidth(), stone.GetHeight());
			break;
		}
	}
}
void gameScene::drawHPBar(HDC hdc) {
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(150, 50, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	Rectangle(hdc, 50, bar_startY + 1, (int)bar_w + 50, bar_startY + 29);
	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);

	myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	HPEN hPen = CreatePen(PS_DOT, 2, RGB(0, 0, 0));
	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
	Rectangle(hdc, 50, bar_startY, HPBAR_WIDTH + 50, bar_startY + 30);
	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}
void gameScene::drawBox(HDC hdc) {
	Rectangle(hdc, pRECT.left, pRECT.top, pRECT.right, pRECT.bottom);
	for (int i = 0; i < cloud_index; ++i) {
		Rectangle(hdc, cloud[i].cx + 30, cloud[i].cy + 30, cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + CLOUD_COLLIDE_HEIGHT);
	}

}

void gameScene::moveItem() {
	for (int i = 0; i < item_index; ++i) {
		if (item[i].get == 1) {
			item[i].iy = bar_startY;
		}
	}
}

void gameScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_P) {
			if (status == PAUSE)
				status = RUN;
			else
				status = PAUSE;
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (wParam == VK_UP) {
			fall = true;
			gravity = 1.8f;
		}
		else if (wParam == VK_RIGHT || wParam == VK_LEFT) {
			fall = true;
			gravity = 0.5;
		}

	}
	break;
	}
}

bool gameScene::getItemCheck() {
	for (int i = 0; i < item_index; ++i) {
		if (item[i].get == 0)
			return false;
	}
	return true;
}

//애니메이션 있으면 여기서 업데이트
void gameScene::Update(const float frameTime)
{

	if (status == PAUSE)
		return;

	if (player.GetStatus() == IDLE) {          //충돌이 아닐 때
		ani_index++;
	}
	if (ani_index >= 39)
		ani_index = 0;

	pRECT = { player.px + 18,player.py + 10,player.px + 18 + PLAYER_COLLIDE_WIDTH ,player.py + PLAYER_HEIGHT };

	player.SetStatus(IDLE);

	for (int i = 0; i < cloud_index; ++i) {
		cloud[i].index++;
		if (cloud[i].index == 74)
			cloud[i].index = 0;
		cRECT = { cloud[i].cx + 30, cloud[i].cy + 30, cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + CLOUD_COLLIDE_HEIGHT };
		if (IntersectRect(&tmp, &cRECT, &pRECT) && i > 6) {                             //충돌 검사
			player.SetStatus(COLLIDED);
			ani_index = 50;
		}
		if (cloud[i].type != 3 && cloud[i].index >= 35 && cloud[i].index <= 59) {       //번개나 비 충돌 검사
			cRECT = { cloud[i].cx + 30, cloud[i].cy + (CLOUD_HEIGHT - 30),              //비 범위
				cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + (CLOUD_HEIGHT - 30) + CLOUD_HEIGHT };
			if (IntersectRect(&tmp, &cRECT, &pRECT)) {                             //충돌 검사
				player.SetStatus(COLLIDED);
				ani_index = 50;
			}
		}
	}

	if (player.GetStatus() == COLLIDED) {           //플레이어가 충돌상태이면 체력 감소
		bar_w = player.DecreaseHp(40 * frameTime);
		pSystem->playSound(effectSound[2], NULL, 0, &Channel[1]);	//충돌하면 효과음
	}

	bar_w = player.DecreaseHp(0.5f * frameTime);       //항상 감소

	if (player.GetHp() <= 0) {
	
		pSystem->playSound(effectSound[4], NULL, 0, &Channel[0]);
		bgSound->release();
	
		Scene* scene = framework.CurScene;   ////현재 씬을 tmp에 넣고 지워줌
		framework.CurScene = new overScene;
		framework.CurScene->init();
		framework.NowScene = GAME;
		delete scene;
		return;
	}

	for (int i = 0; i < item_index; ++i) {                                  //플레이어가 아이템 먹었는지 검사
		cRECT = { item[i].ix, item[i].iy, item[i].ix + ITEM_SIZE, item[i].iy + ITEM_SIZE };
		if (IntersectRect(&tmp, &cRECT, &pRECT)) {
			item[i].ix = ITEM_START + i * 40;
			item[i].iy = bar_startY;
			item[i].get = 1;
			if (item[i].what == 1) {
				bar_w = player.IncreaseHp(30);
				pSystem->playSound(effectSound[1], NULL, 0, &Channel[2]);
			}
			else
				pSystem->playSound(effectSound[0], NULL, 0, &Channel[2]);
		}
	}

	if (fall && gravity >= -30)
		gravity -= frameTime * 12;

	if (fall && player.py <= PLAYER_FIRSTY) {
		if (player.GetStatus() == COLLIDED)
			player.py -= (int)gravity / 3;
		else if (player.GetStatus() == IDLE)
			player.py -= (int)gravity;
		if (startY <= MEM_HEIGHT - (FRAME_HEIGHT) && player.py >= PLAYERMOVE_START) {
			if (player.GetStatus() == COLLIDED) {
				startY -= (int)gravity / 3;
				bar_startY -= (int)gravity / 3;
				moveItem();
			}
			else if (player.GetStatus() == IDLE) {
				startY -= (int)gravity;
				bar_startY -= (int)gravity;
				moveItem();
			}
		}
	}

	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8001)) {
		fall = false;
		if (player.px < 0)
			return;
		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
			if (player.GetStatus() == COLLIDED) {
				player.py -= 2;
				player.px -= 2;
			}
			else if (player.GetStatus() == IDLE) {
				player.py -= 7;
				player.px -= 7;
			}

		}
		else {
			if (player.GetStatus() == COLLIDED) {
				startY -= 2;
				bar_startY -= 2;
				moveItem();
				player.py -= 2;
				player.px -= 2;
			}
			else if (player.GetStatus() == IDLE) {
				startY -= 7;
				bar_startY -= 7;
				moveItem();
				player.py -= 7;
				player.px -= 7;
			}
		}
		return;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8001)) {
		fall = false;
		if (player.px + PLAYER_WIDTH > 1190)
			return;
		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
			if (player.GetStatus() == COLLIDED) {
				player.py -= 2;
				player.px += 2;
			}
			else if (player.GetStatus() == IDLE) {
				player.py -= 7;
				player.px += 7;
			}

		}
		else {
			if (player.GetStatus() == COLLIDED) {
				startY -= 2;
				bar_startY -= 2;
				moveItem();
				player.py -= 2;
				player.px += 2;
			}
			else if (player.GetStatus() == IDLE) {
				startY -= 7;
				bar_startY -= 7;
				moveItem();
				player.py -= 7;
				player.px += 7;
			}
		}
		return;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8001)) {
		fall = false;
		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
			if (player.GetStatus() == COLLIDED)
				player.py -= 2;
			else if(player.GetStatus() == IDLE)
				player.py -= 7;
		}
		else {
			if (player.GetStatus() == COLLIDED) {
				startY -= 2;
				bar_startY -= 2;
				moveItem();
				player.py -= 2;
			}
			else if (player.GetStatus() == IDLE) {
				startY -= 7;
				bar_startY -= 7;
				moveItem();
				player.py -= 7;
			}
		}
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8001)) {
		if (player.px + PLAYER_WIDTH > 1190)
			return;
		if (player.GetStatus() == COLLIDED)
			player.px += 2;
		else if (player.GetStatus() == IDLE)
			player.px += 5;
	}

	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8001)) {
		if (player.px < 0)
			return;
		if (player.GetStatus() == COLLIDED)
			player.px -= 2;
		else if(player.GetStatus() == IDLE)
			player.px -= 5;
	}
	if (player.py <= 0 /* && getItemCheck()*/) {
		pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
		bgSound->release();
		Scene* scene = framework.CurScene;   ////현재 씬을 tmp에 넣고 지워줌
		framework.CurScene = new clearScene;
		framework.CurScene->init();
		framework.NowScene = MENU;
		delete scene;
	}
	// mainCamera보다 player의 위치가 낮으면 클리어 씬 출력
	if (framework.mainCamera->m_vLookAt.y < player.py - FRAME_HEIGHT) {
		pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
		bgSound->release();
		Scene* scene = framework.CurScene;   ////현재 씬을 tmp에 넣고 지워줌
		framework.CurScene = new overScene;
		framework.CurScene->init();
		framework.NowScene = MENU;
		delete scene;
	}
	// 위치 값 보내기
	framework.net->SendClientPos();

	// 위치 값 받기

}
void gameScene::Render(HDC hdc)
{
	drawBackGround(hdc);
	//drawBox(hdc);
	drawPlayer(hdc);
	drawCloud(hdc);
	drawItems(hdc);
	drawHPBar(hdc);
}



//void gameScene::Release() {
//
//
//}
