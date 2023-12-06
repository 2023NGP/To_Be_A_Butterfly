#pragma once
#include "HPBar.h"
#include "scene.h"

class stage2Scene : public Scene {

public:
	RECT cloud_ani[75];
	RECT rain_ani[35];

	int item_index;		//��Ʈ�� ���� ����
	int bar_startY;
	float bar_w;

	int player_movestop;
	int player_startY;

	float gravity;
	bool fall;			//�������� ����

	RECT cRECT;			//���� �簢��
	RECT pRECT;			//�÷��̾� �簢��
	RECT tmp;			//�浹 �˻��� �簢��

	CImage background;
	CImage heart, stone;
	CImage normalCloud, rainCloud, darkCloud, rain, lightning;

	FMOD::System* pSystem;
	FMOD::Sound* bgSound;
	FMOD::Sound* effectSound[EFFECT_NUMBER];
	FMOD::Channel* Channel[3];

	Player player;
	std::vector<Cloud> vCloud;
	Item item[15];
	HPBar bar;

public:
	virtual ~stage2Scene() override;

	virtual void init() override;

	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Update(const float frameTime) override;

	virtual void Render(HDC hdc) override;

	void drawBackGround(HDC hdc);
	void drawCloud(HDC hdc);
	void drawBox(HDC hdc);
	void drawHPBar(HDC hdc);
	void drawItems(HDC hdc);

	void InitCloud();
	void InitAnimation();
	void InitHeart();

	void moveItem();
	bool getItemCheck();

	void InitSound();
};

