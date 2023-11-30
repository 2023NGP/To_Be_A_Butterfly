#pragma once

class Cloud
{
	int type;	// 1 번개구름, 2 비구름, 3 일반구름

public:
	int cx, cy;
	int animIndex;

public:
	void SetType(int value);
	int GetType();
};

