#include "stdafx.h"
#include "HPBar.h"
#include "stage2Scene.h"

//memdc�� �׷��ִ� ����, frame
extern WGameFramework framework;

stage2Scene::~stage2Scene()
{

}
void  stage2Scene::InitSound()
{
    // ���� INIT
    FMOD::System_Create(&pSystem);
    pSystem->init(4, FMOD_INIT_NORMAL, NULL);
    pSystem->createSound("sound/mainsound.wav", FMOD_LOOP_NORMAL, 0, &bgSound);		// �������
    pSystem->createSound("effect/Coin01.wav", FMOD_DEFAULT, 0, &effectSound[0]);	// ���� ȹ��
    pSystem->createSound("effect/Rise02.wav", FMOD_DEFAULT, 0, &effectSound[1]);	// ��Ʈ ȹ��
    pSystem->createSound("effect/Rise05.wav", FMOD_DEFAULT, 0, &effectSound[2]);	// �浹
    pSystem->createSound("effect/Rise07.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// ����
    pSystem->createSound("effect/Downer01.wav", FMOD_DEFAULT, 0, &effectSound[3]);	// ����

    pSystem->playSound(bgSound, NULL, 0, &Channel[0]);
}
void stage2Scene::InitCloud() {       //txt���Ͽ��� ���� ���� �޾ƿ��� �Լ�
    //FILE* fp;
    //fopen_s(&fp, "image/map2.txt", "r");
    //random_device rd;
    //uniform_int_distribution <int> dis(0, 49);

    //int i = 0;
    //if (fp == NULL)//���� ������ ��
    //{
    //    perror("fopen ����");//���� �޽��� ���
    //    return;
    //}

    //while (!feof(fp)) {
    //    int t;
    //    fscanf_s(fp, "%d %d %d", &cloud[i].cx, &cloud[i].cy, &t);
    //    cloud[i].SetType(t);
    //    cloud[i].animIndex = dis(rd);
    //    ++i;
    //}

    //cloud_index = i;
    //fclose(fp);
}
void stage2Scene::InitHeart() {
    FILE* fp;
    fopen_s(&fp, "image/map2(heart).txt", "r");

    int i = 0;
    if (fp == NULL)
    {
        perror("fopen ����");
        return;
    }

    while (!feof(fp)) {
        int t;
        fscanf_s(fp, "%d %d %d", &item[i].ix, &item[i].iy, &t);
        item[i].SetType(t);
        ++i;
    }

    item_index = i;
    fclose(fp);
}
void stage2Scene::InitAnimation() {
    int j = 0;



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
void stage2Scene::init()
{
    startX = 0, startY = STAGE2_HEIGHT - (FRAME_HEIGHT);
    
    // �÷��̾� �ʱ�ȭ
    player.image.Load(TEXT("image/������.png"));
    player.InitAnimation();

    player.px = MEM_WIDTH / 2;
    player.py = (STAGE2_HEIGHT - (CLOUD_HEIGHT + 50));
    player.SetStatus(IDLE);
    player.jumpForce = 0;
    player.animIndex = 0;
    player.shockTime = 0.0f;
    player.isShocked = false;

    background.Load(TEXT("image/���ȭ��1.png"));

    normalCloud.Load(TEXT("image/�Ϲݱ���.png"));
    rainCloud.Load(TEXT("image/�񱸸�.png"));
    rain.Load(TEXT("image/��.png"));
    darkCloud.Load(TEXT("image/�Ա���.png"));
    lightning.Load(TEXT("image/����.png"));

    heart.Load(TEXT("image/heart.png"));
    stone.Load(TEXT("image/coin.png"));
    
    InitSound();
    InitAnimation();
    framework.net->RecvCloudData(vCloud);
    InitHeart();
    gravity = 1;

    bar_startY = (STAGE2_HEIGHT - (CLOUD_HEIGHT + 50)) + 100;
    fall = true;

    status = RUN;

    player.px = MEM_WIDTH / 2;
    player.py = (STAGE2_HEIGHT - (CLOUD_HEIGHT + 50));
    player.SetStatus(IDLE);
    player.jumpForce = 0;

    framework.mainCamera->setLookAt(POINT{ 0, STAGE2_HEIGHT - (FRAME_HEIGHT)});
    bar.y = framework.mainCamera->getLookAt().y + FRAME_HEIGHT - 100;
}

void stage2Scene::drawBackGround(HDC hdc) {
    //��� �׸��� �Լ�
    background.BitBlt(hdc, 0, 0, SRCCOPY);
}
void stage2Scene::drawCloud(HDC hdc) {
    //���� �׸��� �Լ�
    for (int j = 0; j < vCloud.size(); ++j) {
        switch (vCloud[j].GetType()) {
        case 1:
            if (vCloud[j].animIndex >= 25 && vCloud[j].animIndex <= 59) {
                darkCloud.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[vCloud[j].animIndex].left, cloud_ani[vCloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE + 45);
                lightning.Draw(hdc, vCloud[j].GetPosition().fY, vCloud[j].GetPosition().fY + (CLOUD_HEIGHT - 30), CLOUD_WIDTH, CLOUD_HEIGHT, rain_ani[vCloud[j].animIndex - 25].left, rain_ani[vCloud[j].animIndex - 25].top, CLOUD_IMAGE_SIZE, RAIN_IMAGE);
            }
            else
                darkCloud.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY, CLOUD_WIDTH, CLOUD_HEIGHT-30, cloud_ani[vCloud[j].animIndex].left, cloud_ani[vCloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE + 45);
            break;
        case 2:
            if (vCloud[j].animIndex >= 25 && vCloud[j].animIndex <= 59) {
                rainCloud.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[vCloud[j].animIndex].left, cloud_ani[vCloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
                rain.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY + (CLOUD_HEIGHT - 30), CLOUD_WIDTH, CLOUD_HEIGHT, rain_ani[vCloud[j].animIndex - 25].left, rain_ani[vCloud[j].animIndex - 25].top, CLOUD_IMAGE_SIZE, RAIN_IMAGE);
            }
            else
                rainCloud.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY, CLOUD_WIDTH, CLOUD_HEIGHT - 30, cloud_ani[vCloud[j].animIndex].left, cloud_ani[vCloud[j].animIndex].top, CLOUD_IMAGE_SIZE, RAINCLOUD_IMAGE);
            break;
        case 3:
            normalCloud.Draw(hdc, vCloud[j].GetPosition().fX, vCloud[j].GetPosition().fY, CLOUD_WIDTH, CLOUD_HEIGHT, cloud_ani[vCloud[j].animIndex].left, cloud_ani[vCloud[j].animIndex].top, CLOUD_IMAGE_SIZE, CLOUD_IMAGE_SIZE);
            break;
        }
    }
}
void stage2Scene::drawItems(HDC hdc) {
    for (int j = 0; j < item_index; ++j) {
        switch (item[j].GetType()) {
        case 1:
            heart.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, heart.GetWidth(), heart.GetHeight());
            break;
        case 2:
            stone.Draw(hdc, item[j].ix, item[j].iy, ITEM_SIZE, ITEM_SIZE, 0, 0, stone.GetWidth(), stone.GetHeight());
            break;
        }
    }
}
void stage2Scene::drawBox(HDC hdc) {
    Rectangle(hdc, pRECT.left, pRECT.top, pRECT.right, pRECT.bottom);
    for (int i = 0; i < vCloud.size(); ++i) {
        Rectangle(hdc, vCloud[i].GetPosition().fX + 30, vCloud[i].GetPosition().fY + 30, vCloud[i].GetPosition().fX + CLOUD_COLLIDE_WIDTH, vCloud[i].GetPosition().fY + CLOUD_COLLIDE_HEIGHT);
    }
}
void stage2Scene::moveItem() {
    for (int i = 0; i < item_index; ++i) {
        if (item[i].GetIsGot()) {
            item[i].iy = bar.y;
        }
    }
}
void stage2Scene::processKey(UINT iMessage, WPARAM wParam, LPARAM lParam)
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
bool stage2Scene::getItemCheck() {
    for (int i = 0; i < item_index; ++i) {
        if (item[i].GetIsGot() == false)
            return false;
    }
    return true;
}
//�ִϸ��̼� ������ ���⼭ ������Ʈ
void stage2Scene::Update(const float frameTime)
{
    bar.y = framework.mainCamera->getLookAt().y + FRAME_HEIGHT - 100;
    if (player.isShocked)
        player.shockTime += frameTime;

    if (player.shockTime >= 0.5) {
        player.shockTime = 0;
        player.isShocked = FALSE;
    }

    if (status == PAUSE)          //�Ͻ�����
        return;

    if (player.GetStatus() == IDLE) {          //�浹�� �ƴ� ��
        player.animIndex++;
    }

    if (player.animIndex >= 39)
        player.animIndex = 0;


    pRECT = { player.px + 18, player.py + 10, player.px + 18 + PLAYER_COLLIDE_WIDTH , player.py + PLAYER_HEIGHT };

    player.SetStatus(IDLE);

    for (int i = 0; i < vCloud.size(); ++i) {
        vCloud[i].animIndex++;
        if (vCloud[i].animIndex == 74)
            vCloud[i].animIndex = 0;
        cRECT = { vCloud[i].GetPosition().fX + 30, vCloud[i].GetPosition().fY + 30, vCloud[i].GetPosition().fX + CLOUD_COLLIDE_WIDTH, vCloud[i].GetPosition().fY + CLOUD_COLLIDE_HEIGHT };
        if (IntersectRect(&tmp, &cRECT, &pRECT) && i > 6) {                             //�浹 �˻�
            player.SetStatus(COLLIDED);
            player.animIndex = 50;
        }
        if (vCloud[i].GetType() != 3 && vCloud[i].animIndex >= 35 && vCloud[i].animIndex <= 59) {       //������ �� �浹 �˻�
            cRECT = { vCloud[i].GetPosition().fX + 30, vCloud[i].GetPosition().fY + (CLOUD_HEIGHT - 30),              //�� ����
                vCloud[i].GetPosition().fX + CLOUD_COLLIDE_WIDTH, vCloud[i].GetPosition().fY + (CLOUD_HEIGHT - 30) + CLOUD_HEIGHT };
            if (IntersectRect(&tmp, &cRECT, &pRECT)) {                                  //�浹 �˻�
                player.SetStatus(COLLIDED);
                player.animIndex = 50;
                if (vCloud[i].GetType() == 1) {
                    player.isShocked = TRUE;
                    player.shockTime = 0.0f;
                }
            }
        }
    }

    if (player.GetHp() <= 0) {
        pSystem->playSound(effectSound[4], NULL, 0, &Channel[0]);
        bgSound->release();

        Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
        framework.CurScene = new overScene;
        framework.CurScene->init();
        framework.NowScene = MENU;
        delete scene;
        return;
    }

    if (player.GetStatus() == COLLIDED) {           //�÷��̾ �浹�����̸� ü�� ����
        bar.barGauge = player.DecreaseHp(40 * frameTime);
        pSystem->playSound(effectSound[2], NULL, 0, &Channel[1]);
    }

    if (player.isShocked)
        return;

    bar.barGauge = player.DecreaseHp(0.5f * frameTime);       //�׻� ����

    for (int i = 0; i < item_index; ++i) {                                  //�÷��̾ ������ �Ծ����� �˻�
        cRECT = { item[i].ix, item[i].iy, item[i].ix + ITEM_SIZE, item[i].iy + ITEM_SIZE };
        if (IntersectRect(&tmp, &cRECT, &pRECT)) {
            item[i].ix = ITEM_START + i * 40;
            item[i].iy = bar.y;
            item[i].SetIsGot(true);
            if (item[i].GetType() == 1) {
                bar.barGauge = player.IncreaseHp(30);
                pSystem->playSound(effectSound[1], NULL, 0, &Channel[2]);	
            }
            else
                pSystem->playSound(effectSound[0], NULL, 0, &Channel[2]);
        }
    }

    if (fall && gravity >= -30)
        gravity -= frameTime * 12;

    if (fall && player.py <= (STAGE2_HEIGHT - (CLOUD_HEIGHT + 50))) {
        if (player.GetStatus() == COLLIDED)
            player.py -= (int)gravity / 3;
        else if (player.GetStatus() == IDLE)
            player.py -= (int)gravity;
        if (startY <= STAGE2_HEIGHT - (FRAME_HEIGHT) && player.py >= PLAYERMOVE_START) {
            if (player.GetStatus() == COLLIDED) {
                startY -= (int)gravity / 3;
                moveItem();
            }
            else if (player.GetStatus() == IDLE) {
                startY -= (int)gravity;
                moveItem();
            }
        }
    }

    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8001)) {
        fall = false;
        if (player.px < 0)
            return;
        if (player.py <= PLAYERMOVE_START || player.py >= (STAGE2_HEIGHT - (FRAME_HEIGHT / 2))) {
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
                moveItem();
                player.py -= 2;
                player.px -= 2;
            }
            else if (player.GetStatus() == IDLE) {
                startY -= 7;
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
        if (player.py <= PLAYERMOVE_START || player.py >= (STAGE2_HEIGHT - (FRAME_HEIGHT / 2))) {
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
                moveItem();
                player.py -= 2;
                player.px += 2;
            }
            else if (player.GetStatus() == IDLE) {
                startY -= 7;
                moveItem();
                player.py -= 7;
                player.px += 7;
            }
        }
        return;
    }
    if ((GetAsyncKeyState(VK_UP) & 0x8001)) {
        fall = false;
        if (player.py <= PLAYERMOVE_START || player.py >= (STAGE2_HEIGHT - (FRAME_HEIGHT / 2))) {
            if (player.GetStatus() == COLLIDED)
                player.py -= 2;
            else if (player.GetStatus() == IDLE)
                player.py -= 7;
        }
        else {
            if (player.GetStatus() == COLLIDED) {
                startY -= 2;
                moveItem();
                player.py -= 2;
            }
            else if (player.GetStatus() == IDLE) {
                startY -= 7;
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
        else if (player.GetStatus() == IDLE)
            player.px -= 5;
    }

    if (player.py <= 0) {
        //if (getItemCheck()==TRUE) {       // ���������� Ŭ�������� �����ص�
            pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
            bgSound->release();

            Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
            framework.CurScene = new clearScene;
            framework.CurScene->init();
            framework.NowScene = MENU;
            delete scene;
        //}
    }
    if (framework.mainCamera->m_vLookAt.y < player.py - FRAME_HEIGHT) {
        cout << framework.mainCamera->m_vLookAt.y << ", " << player.py << endl;
        //// mainCamera���� player�� ��ġ�� ������ ���ӿ��� �� ���
        pSystem->playSound(effectSound[3], NULL, 0, &Channel[0]);
        bgSound->release();
    
        Scene* scene = framework.CurScene;   ////���� ���� tmp�� �ְ� ������
        framework.CurScene = new overScene;
        framework.CurScene->init();
        framework.NowScene = MENU;
        delete scene;
    }

}
void stage2Scene::Render(HDC hdc)
{
    drawBackGround(hdc);
    // �浹 �ڽ�
    // drawBox(hdc); 
    player.Draw(hdc);
    //drawPlayer(hdc);
    drawCloud(hdc);
    drawItems(hdc);
    bar.Draw(hdc);
}

//void gameScene::Release() {
//
//
//}
