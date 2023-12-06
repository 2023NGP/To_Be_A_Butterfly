#pragma once
#include "HPBar.h"
#include "scene.h"

class stage2Scene : public Scene {

public:
	RECT cloud_ani[75];
	RECT rain_ani[35];

	int item_index;		//하트랑 스톤 개수
	int bar_startY;
	float bar_w;

	int player_movestop;
	int player_startY;

	float gravity;
	bool fall;			//떨어지는 상태

	RECT cRECT;			//구름 사각형
	RECT pRECT;			//플레이어 사각형
	RECT tmp;			//충돌 검사할 사각형

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

