#pragma once

class Cloud
{
	int type;	// 1 ��������, 2 �񱸸�, 3 �Ϲݱ���

public:
	int cx, cy;
	int animIndex;

public:
	void SetType(int value);
	int GetType();
};

