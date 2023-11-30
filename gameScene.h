#pragma once

class gameScene : public Scene
{
public:
	RECT animation[56];	//���� �ִϸ��̼�(���ҽ� ��ġ)
	RECT cloud_ani[75];
	RECT rain_ani[35];

	int ani_index;		//�ִϸ��̼� index(�÷��̾�)
	int cloud_index;	//���� ����
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

	CImage player_image, background;
	CImage heart, stone;
	CImage normalCloud, rainCloud, darkCloud, rain;

	// ���� ����
	FMOD::System* pSystem;
	FMOD::Sound* bgSound;
	FMOD::Sound* effectSound[EFFECT_NUMBER];
	FMOD::Channel* Channel[3];

	Player player;
	Cloud cloud[200];
	Item item[15];

public:
	virtual ~gameScene() override;

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