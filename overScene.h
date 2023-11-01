#pragma once
#include "scene.h"

class overScene : public scene {
public:
	CImage background1, txt;
	int ren1, y; //애니메이션을 위한 변수
public:
	virtual ~overScene() override;

	virtual void init() override;

	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Update(const float frameTime) override;

	virtual void Render(HDC hdc) override;
};