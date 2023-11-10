#pragma once
#include "scene.h"

class stage2Scene : public scene {
public:
	RECT animation[56];	//평상시 애니메이션(리소스 위치)
	RECT cloud_ani[75];
	RECT rain_ani[35];

	int ani_index;		//애니메이션 index(플레이어)
	int cloud_index;	//구름 개수
	int item_index;		//하트랑 스톤 개수
	int bar_startY;
	float bar_w;

	int player_movestop;
	int player_startY;

	float gravity;
	bool fall;			//떨어지는 상태
	bool shock;
	int shocktimer;

	RECT cRECT;			//구름 사각형
	RECT pRECT;			//플레이어 사각형
	RECT tmp;			//충돌 검사할 사각형

	CImage player_image, background;
	CImage heart, stone;
	CImage normalCloud, rainCloud, darkCloud, rain, lightning;

	FMOD::System* pSystem;
	FMOD::Sound* bgSound;
	FMOD::Sound* effectSound[EFFECT_NUMBER];
	FMOD::Channel* Channel[3];

	Player player;
	Cloud cloud[200];
	Item item[15];

public:
	virtual ~stage2Scene() override;

	virtual void init() override;

	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Update(const float frameTime) override;

	virtual void Render(HDC hdc) override;

	void drawPlayer(HDC hdc);
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