#include "Player.h"

void Player::SetID(int value)
{
	id = value;
}

int Player::GetID() const
{
	return id;
}

void Player::SetStatus(int value)
{
	status = value;
}

int Player::GetStatus() const
{
	return status;
}

float Player::IncreaseHp(float amount)
{
	hp += amount;
	if (hp > MAX_HP) {
		hp = MAX_HP;
	}

	return hp;
}

float Player::DecreaseHp(float amount)
{
	hp -= amount;
	if (hp < 0) {
		hp = 0;
		// TODO: status Ã³¸®
	}

	return hp;
}

float Player::GetHp() const
{
	return hp;
}

void Player::IncreaseCoinCount()
{
}

int Player::GetCoinCount() const
{
	return coinCount;
}
