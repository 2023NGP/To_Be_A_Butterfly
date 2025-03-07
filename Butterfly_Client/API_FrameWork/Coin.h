#pragma once
#include "Obj.h"
class CCoin : public CObj
{
public:
	CCoin();
	virtual ~CCoin();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	int GetIndex() { return m_lIndex; }
	void SetIndex(LONG lIndex) { m_lIndex = lIndex; }
	void CallBackCollision();
private:
	LONG m_lIndex;

};

