#pragma once

class Item
{
	int type;		// 1 하트, 2 코인
	bool isGot = 0;	// 0 아직 안먹음, 1 먹음

public:
	int ix, iy;

public:
	void SetType(int value);
	int GetType();

	void SetIsGot(bool value);
	bool GetIsGot();
};

