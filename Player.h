#pragma once

#define MAX_HP	498.f

enum PLAYER_STATUS { IDLE, COLLIDED, CLEAR, DEAD };

class Player {
	int id;
	int status = IDLE;
	float hp = MAX_HP;
	int coinCount;

public:
	int px, py;
	int jumpForce;
	int animIndex;

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
};

