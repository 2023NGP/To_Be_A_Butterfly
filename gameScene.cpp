﻿//#include "stdafx.h"
//#include "gameScene.h"
//
////memdc�� �׷��ִ� ����, frame
//extern WGameFramework framework;
//
//gameScene::~gameScene()
//{
//
//}
//
//void  gameScene::InitSound()
//{
//	// ���� INIT
//	FMOD::System_Create(&pSystem);
//	pSystem->init(4, FMOD_INIT_NORMAL, NULL);
//	pSystem->createSound("sound/mainsound.wav", FMOD_LOOP_NORMAL, 0, &bgSound);		// �������
//	pSystem->createSound("effect/Coin01.wav", FMOD_DEFAULT, 0, &effectSound[0]);	// ���� ȹ��
//	pSystem->createSound("effect/Rise02.wav", FMOD_DEFAULT, 0, &effectSound[1]);	// ��Ʈ ȹ��
//	pSystem->createSound("effect/Rise05.wav", FMOD_DEFAULT, 0, &effectSound[2]);	// �浹
//	pSystem->createSound("effect/Rise07.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// ����
//	pSystem->createSound("effect/Downer01.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// ����
//
//	pSystem->playSound(bgSound, NULL, 0, &Channel[0]);
//
//	// ���� ����
//
//}
//
//void gameScene::InitCloud() {       //txt���Ͽ��� ���� ���� �޾ƿ��� �Լ�
//	FILE* fp;
//	fopen_s(&fp, "image/map1.txt", "r");
//	random_device rd;
//	uniform_int_distribution <int> dis(0, 49);
//
//	int i = 0;
//	if (fp == NULL)//���� ������ ��
//	{
//		perror("fopen ����");//���� �޽��� ���
//		return;
//	}
//
//	while (!feof(fp)) {
//		int t;
//		fscanf_s(fp, "%d %d %d", &cloud[i].cx, &cloud[i].cy, &t);
//		cloud[i].SetType(t);
//		cloud[i].animIndex = dis(rd);
//		++i;
//	}
//
//	cloud_index = i;
//	fclose(fp);
//}
//void gameScene::InitHeart() {
//	FILE* fp;
//	fopen_s(&fp, "image/map1(heart).txt", "r");
//
//	int i = 0;
//	if (fp == NULL)
//	{
//		perror("fopen ����");
//		return;
//	}
//
//	while (!feof(fp)) {
//		int t;
//		fscanf_s(fp, "%d %d %d", &item[i].ix, &item[i].iy, &t);
//		item[i].SetType(t);
//		++i;
//	}
//
//	item_index = i;
//	fclose(fp);
//}
//void gameScene::InitAnimation() {
//	int j = 0;
//	//14��
//	//1�ִϸ��̼� 4��
//	//0 ~ 3, ... ~39
//	for (int i = 0; i < 14; i++) {
//		animation[j] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
//		animation[j + 1] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
//		animation[j + 2] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
//		animation[j + 3] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
//		j += 4;
//	}
//
//	j = 0;
//
//	//0~4, 5~9, 10~14, 15~19, 20~24, |25~29, 30~34, 35~39, 40~44, 45~49,| 50 ~54, 55~59 ���� ���� �̹����� ���� ��
//	//������Ʈ �ӵ��� �ʹ� ���� ������ �����̰� �ϱ� ����
//
//	for (int i = 0; i < 15; i++) {
//		if (i == 10 || i == 11) {
//			cloud_ani[j] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 1] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 2] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 3] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 4] = { 9 * CLOUD_IMAGE_SIZE, 0, 10 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//		}
//		else if (i == 12 || i == 13 || i == 14) {
//			cloud_ani[j] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 1] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 2] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 3] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 4] = { 0 * CLOUD_IMAGE_SIZE, 0, 1 * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//		}
//		else {
//			cloud_ani[j] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 1] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 2] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 3] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//			cloud_ani[j + 4] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE };
//		}
//		j += 5;
//	}
//	j = 0;
//	for (int i = 0; i < 7; i++) {
//		if (i == 5 || i == 6) {
//			rain_ani[j] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 1] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 2] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 3] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 4] = { 4 * CLOUD_IMAGE_SIZE, 0, 5 * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//		}
//		else {
//			rain_ani[j] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 1] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 2] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 3] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//			rain_ani[j + 4] = { i * CLOUD_IMAGE_SIZE, 0, (i + 1) * CLOUD_IMAGE_SIZE, RAIN_IMAGE };
//		}
//		j += 5;
//	}
//}
//void gameScene::init()
//{
//	startX = 0, startY = MEM_HEIGHT - (FRAME_HEIGHT);
//
//	player_image.Load(TEXT("image/������.png"));
//	background.Load(TEXT("image/���ȭ��1.png"));
//
//	normalCloud.Load(TEXT("image/�Ϲݱ���.png"));
//	rainCloud.Load(TEXT("image/�񱸸�.png"));
//	rain.Load(TEXT("image/��.png"));
//	darkCloud.Load(TEXT("image/�Ա���.png"));
//
//	heart.Load(TEXT("image/heart.png"));
//	stone.Load(TEXT("image/coin.png"));
//
//	InitSound();
//	InitAnimation();
//	InitCloud();
//	InitHeart();
//	ani_index = 0;      //�浹�̸� 20~27, ���ø� 0~19
//	gravity = 1;
//	bar_startY = PLAYER_FIRSTY + 100;
//	fall = true;
//
//	status = RUN;
//
//	player.px = MEM_WIDTH / 2;
//	player.py = PLAYER_FIRSTY;
//	player.SetStatus(IDLE);
//	player.jumpForce = 0;
//	cout << "stage 1" << endl;
//
//	framework.mainCamera->setLookAt(POINT{ 0, MEM_HEIGHT - (FRAME_HEIGHT) });
//}
//
//void gameScene::drawPlayer(HDC hdc) {
//	//�÷��̾� �׸��� �Լ�
//	player_image.Draw(hdc, player.px, player.py, PLAYER_WIDTH, PLAYER_HEIGHT, animation[ani_index].left, animation[ani_index].top,
//		PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE);
//
//
//}
//void gameScene::drawBackGround(HDC hdc) {
//	//��� �׸��� �Լ�
//	background.BitBlt(hdc, 0, 0, SRCCOPY);
//}
//void gameScene::drawCloud(HDC hdc) {
//	//���� �׸��� �Լ�
//	for (int j = 0; j < cloud_index; ++j) {
//		switch (cloud[j].GetType()) {
//		case 1:
//			darkCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT, cloud_ani[cloud[j].animIndex].left, cloud_ani[cloud[j].animIndex].top, CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE);
//			break;
//		case 2:
//			if (cloud[j].animIndex >= 25 && cloud[j].animIndex <= 59) {
//				rainCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[cloud[j].animIndex].left, cloud_ani[cloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
//				rain.Draw(hdc, cloud[j].cx, cloud[j].cy + (CLOUD_HEIGHT - 30), CLOUD_WIDTH, CLOUD_HEIGHT, rain_ani[cloud[j].animIndex - 25].left, rain_ani[cloud[j].animIndex - 25].top, CLOUD_IMAGE_SIZE, RAIN_IMAGE);
//			}
//			else
//				rainCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[cloud[j].animIndex].left, cloud_ani[cloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
//			break;
//		case 3:
//			normalCloud.Draw(hdc, cloud[j].cx, cloud[j].cy, CLOUD_WIDTH, CLOUD_HEIGHT, cloud_ani[cloud[j].animIndex].left, cloud_ani[cloud[j].animIndex].top, CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE);
//			break;
//		}
//	}
//}
//void gameScene::drawItems(HDC hdc) {
//	for (int j = 0; j < item_index; ++j) {
//		switch (item[j].GetType()) {
//		case 1:
//			heart.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, heart.GetWidth(), heart.GetHeight());
//			break;
//		case 2:
//			stone.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, stone.GetWidth(), stone.GetHeight());
//			break;
//		}
//	}
//}
//void gameScene::drawHPBar(HDC hdc) {
//	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(150, 50, 0));
//	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
//	Rectangle(hdc, 50, bar_startY + 1, (int)bar_w + 50, bar_startY + 29);
//	SelectObject(hdc, oldBrush);
//	DeleteObject(myBrush);
//
//	myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
//	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
//	HPEN hPen = CreatePen(PS_DOT, 2, RGB(0, 0, 0));
//	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
//	Rectangle(hdc, 50, bar_startY, HPBAR_WIDTH + 50, bar_startY + 30);
//	SelectObject(hdc, oldBrush);
//	DeleteObject(myBrush);
//	SelectObject(hdc, oldPen);
//	DeleteObject(hPen);
//}
//void gameScene::drawBox(HDC hdc) {
//	Rectangle(hdc, pRECT.left, pRECT.top, pRECT.right, pRECT.bottom);
//	for (int i = 0; i < cloud_index; ++i) {
//		Rectangle(hdc, cloud[i].cx + 30, cloud[i].cy + 30, cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + CLOUD_COLLIDE_HEIGHT);
//	}
//
//}
//
//void gameScene::moveItem() {
//	for (int i = 0; i < item_index; ++i) {
//		if (item[i].GetIsGot()) {
//			item[i].iy = bar_startY;
//		}
//	}
//}
//
//void gameScene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
//{
//	switch (iMessage)
//	{
//	case WM_KEYDOWN:
//	{
//		if (wParam == VK_P) {
//			if (status == PAUSE)
//				status = RUN;
//			else
//				status = PAUSE;
//			break;
//		}
//	}
//	break;
//	case WM_KEYUP:
//	{
//		if (wParam == VK_UP) {
//			fall = true;
//			gravity = 1.8f;
//		}
//		else if (wParam == VK_RIGHT || wParam == VK_LEFT) {
//			fall = true;
//			gravity = 0.5;
//		}
//
//	}
//	break;
//	}
//}
//
//bool gameScene::getItemCheck() {
//	for (int i = 0; i < item_index; ++i) {
//		if (item[i].GetIsGot() == false)
//			return false;
//	}
//	return true;
//}
//
////�ִϸ��̼� ������ ���⼭ ������Ʈ
//void gameScene::Update(const float frameTime)
//{
//
//	if (status == PAUSE)
//		return;
//
//	if (player.GetStatus() == IDLE) {          //�浹�� �ƴ� ��
//		ani_index++;
//	}
//	if (ani_index >= 39)
//		ani_index = 0;
//
//	pRECT = { player.px + 18,player.py + 10,player.px + 18 + PLAYER_COLLIDE_WIDTH ,player.py + PLAYER_HEIGHT };
//
//	player.SetStatus(IDLE);
//
//	for (int i = 0; i < cloud_index; ++i) {
//		cloud[i].animIndex++;
//		if (cloud[i].animIndex == 74)
//			cloud[i].animIndex = 0;
//		cRECT = { cloud[i].cx + 30, cloud[i].cy + 30, cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + CLOUD_COLLIDE_HEIGHT };
//		if (IntersectRect(&tmp, &cRECT, &pRECT) && i > 6) {                             //�浹 �˻�
//			player.SetStatus(COLLIDED);
//			ani_index = 50;
//		}
//		if (cloud[i].GetType() != 3 && cloud[i].animIndex >= 35 && cloud[i].animIndex <= 59) {       //������ �� �浹 �˻�
//			cRECT = { cloud[i].cx + 30, cloud[i].cy + (CLOUD_HEIGHT - 30),              //�� ����
//				cloud[i].cx + CLOUD_COLLIDE_WIDTH, cloud[i].cy + (CLOUD_HEIGHT - 30) + CLOUD_HEIGHT };
//			if (IntersectRect(&tmp, &cRECT, &pRECT)) {                             //�浹 �˻�
//				player.SetStatus(COLLIDED);
//				ani_index = 50;
//			}
//		}
//	}
//
//	if (player.GetStatus() == COLLIDED) {           //�÷��̾ �浹�����̸� ü�� ����
//		bar_w = player.DecreaseHp(40 * frameTime);
//		pSystem->playSound(effectSound[2], NULL, 0, &Channel[1]);	//�浹�ϸ� ȿ����
//	}
//
//	bar_w = player.DecreaseHp(0.5f * frameTime);       //�׻� ����
//
//	if (player.GetHp() <= 0) {
//
//		pSystem->playSound(effectSound[4], NULL, 0, &Channel[0]);
//		bgSound->release();
//
//		Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
//		framework.CurScene = new overScene;
//		framework.CurScene->init();
//		framework.NowScene = GAME;
//		delete scene;
//		return;
//	}
//
//	for (int i = 0; i < item_index; ++i) {                                  //�÷��̾ ������ �Ծ����� �˻�
//		cRECT = { item[i].ix, item[i].iy, item[i].ix + ITEM_SIZE, item[i].iy + ITEM_SIZE };
//		if (IntersectRect(&tmp, &cRECT, &pRECT)) {
//			item[i].ix = ITEM_START + i * 40;
//			item[i].iy = bar_startY;
//			item[i].SetIsGot(true);
//			if (item[i].GetType() == 1) {
//				bar_w = player.IncreaseHp(30);
//				pSystem->playSound(effectSound[1], NULL, 0, &Channel[2]);
//			}
//			else
//				pSystem->playSound(effectSound[0], NULL, 0, &Channel[2]);
//		}
//	}
//
//	if (fall && gravity >= -30)
//		gravity -= frameTime * 12;
//
//	if (fall && player.py <= PLAYER_FIRSTY) {
//		if (player.GetStatus() == COLLIDED)
//			player.py -= (int)gravity / 3;
//		else if (player.GetStatus() == IDLE)
//			player.py -= (int)gravity;
//		if (startY <= MEM_HEIGHT - (FRAME_HEIGHT) && player.py >= PLAYERMOVE_START) {
//			if (player.GetStatus() == COLLIDED) {
//				startY -= (int)gravity / 3;
//				bar_startY -= (int)gravity / 3;
//				moveItem();
//			}
//			else if (player.GetStatus() == IDLE) {
//				startY -= (int)gravity;
//				bar_startY -= (int)gravity;
//				moveItem();
//			}
//		}
//	}
//
//	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8001)) {
//		fall = false;
//		if (player.px < 0)
//			return;
//		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
//			if (player.GetStatus() == COLLIDED) {
//				player.py -= 2;
//				player.px -= 2;
//			}
//			else if (player.GetStatus() == IDLE) {
//				player.py -= 7;
//				player.px -= 7;
//			}
//
//		}
//		else {
//			if (player.GetStatus() == COLLIDED) {
//				startY -= 2;
//				bar_startY -= 2;
//				moveItem();
//				player.py -= 2;
//				player.px -= 2;
//			}
//			else if (player.GetStatus() == IDLE) {
//				startY -= 7;
//				bar_startY -= 7;
//				moveItem();
//				player.py -= 7;
//				player.px -= 7;
//			}
//		}
//		return;
//	}
//	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8001)) {
//		fall = false;
//		if (player.px + PLAYER_WIDTH > 1190)
//			return;
//		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
//			if (player.GetStatus() == COLLIDED) {
//				player.py -= 2;
//				player.px += 2;
//			}
//			else if (player.GetStatus() == IDLE) {
//				player.py -= 7;
//				player.px += 7;
//			}
//
//		}
//		else {
//			if (player.GetStatus() == COLLIDED) {
//				startY -= 2;
//				bar_startY -= 2;
//				moveItem();
//				player.py -= 2;
//				player.px += 2;
//			}
//			else if (player.GetStatus() == IDLE) {
//				startY -= 7;
//				bar_startY -= 7;
//				moveItem();
//				player.py -= 7;
//				player.px += 7;
//			}
//		}
//		return;
//	}
//	if ((GetAsyncKeyState(VK_UP) & 0x8001)) {
//		fall = false;
//		if (player.py <= PLAYERMOVE_START || player.py >= PLAYERMOVE_STOP) {
//			if (player.GetStatus() == COLLIDED)
//				player.py -= 2;
//			else if (player.GetStatus() == IDLE)
//				player.py -= 7;
//		}
//		else {
//			if (player.GetStatus() == COLLIDED) {
//				startY -= 2;
//				bar_startY -= 2;
//				moveItem();
//				player.py -= 2;
//			}
//			else if (player.GetStatus() == IDLE) {
//				startY -= 7;
//				bar_startY -= 7;
//				moveItem();
//				player.py -= 7;
//			}
//		}
//	}
//	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8001)) {
//		if (player.px + PLAYER_WIDTH > 1190)
//			return;
//		if (player.GetStatus() == COLLIDED)
//			player.px += 2;
//		else if (player.GetStatus() == IDLE)
//			player.px += 5;
//	}
//
//	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8001)) {
//		if (player.px < 0)
//			return;
//		if (player.GetStatus() == COLLIDED)
//			player.px -= 2;
//		else if (player.GetStatus() == IDLE)
//			player.px -= 5;
//	}
//	if (player.py <= 0 /* && getItemCheck()*/) {
//		pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
//		bgSound->release();
//		Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
//		framework.CurScene = new clearScene;
//		framework.CurScene->init();
//		framework.NowScene = MENU;
//		delete scene;
//	}
//	// mainCamera���� player�� ��ġ�� ������ Ŭ���� �� ���
//	if (framework.mainCamera->m_vLookAt.y < player.py - FRAME_HEIGHT) {
//		pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
//		bgSound->release();
//		Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
//		framework.CurScene = new overScene;
//		framework.CurScene->init();
//		framework.NowScene = MENU;
//		delete scene;
//	}
//	// ��ġ �� ������
//	framework.net->SendClientPos(player.px, player.py);
//
//	// ��ġ �� �ޱ�
//
//}
//void gameScene::Render(HDC hdc)
//{
//	drawBackGround(hdc);
//	//drawBox(hdc);
//	drawPlayer(hdc);
//	drawCloud(hdc);
//	drawItems(hdc);
//	drawHPBar(hdc);
//}
//
//
//
////void gameScene::Release() {
////
////
////}