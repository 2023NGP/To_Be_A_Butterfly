#pragma once

class Item
{
	int type;		// 1 ��Ʈ, 2 ����
	bool isGot = 0;	// 0 ���� �ȸ���, 1 ����

public:
	int ix, iy;

public:
	void SetType(int value);
	int GetType();

	void SetIsGot(bool value);
	bool GetIsGot();
};

