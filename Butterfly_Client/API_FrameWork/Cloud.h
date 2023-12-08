#pragma once
#include "Obj.h"
class CCloud : public CObj
{
public:
	int i_type;

public:
	CCloud();
	CCloud(int type);
	virtual ~CCloud();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	int GetIndex() { return m_lIndex; }
	void SetIndex(LONG lIndex) { m_lIndex = lIndex; }
	void SetType(int type) { i_type = type; }
	void CallBackCollision();
private:
	LONG m_lIndex;

};

