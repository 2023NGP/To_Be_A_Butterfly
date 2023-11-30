#include "stdafx.h"
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
		// TODO: status ó��
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

void Player::Draw(HDC hdc)
{
	//�÷��̾� �׸��� �Լ�
	image.Draw(hdc, px, py, PLAYER_WIDTH, PLAYER_HEIGHT, animation[animIndex].left, animation[animIndex].top,
		PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE);
}

void Player::InitAnimation()
{
	int j = 0;
	//14��
	//1�ִϸ��̼� 4��
	//0 ~ 3, ... ~39
	for (int i = 0; i < 14; i++) {
		animation[j] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 1] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 2] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		animation[j + 3] = { i * PLAYER_IMAGE_SIZE, 0, (i + 1) * PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE };
		j += 4;
	}
	// 0~4, 5~9, 10~14, 15~19, 20~24, |25~29, 30~34, 35~39, 40~44, 45~49,| 50 ~54, 55~59 ���� ���� �̹����� ���� ��
	// ������Ʈ �ӵ��� �ʹ� ���� ������ �����̰� �ϱ� ����
}
