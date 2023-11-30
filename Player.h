#pragma once

#define MAX_HP	498.f

enum PLAYER_STATUS { IDLE, COLLIDED, CLEAR, DEAD };

class Player {
	int id;
	int status = IDLE;
	float hp = MAX_HP;
	int coinCount;
	RECT animation[56];				// 평상시 애니메이션 (리소스 위치)

public:
	CImage image;
	int px, py;
	int jumpForce;
	int animIndex;				//충돌이면 20~27, 평상시면 0~19
	bool isShocked;
	float shockTime;

public:
	void SetID(int value);
	int GetID() const;

	void SetStatus(int value);
	int GetStatus() const;

	float IncreaseHp(float amount);
	float DecreaseHp(float amount);
	float GetHp() const;
	
	void IncreaseCoinCount();
	int GetCoinCount() const;

	void Draw(HDC hdc);
	void Update(const float frameTime);

	void InitAnimation();
};

