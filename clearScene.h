#pragma once
#include "scene.h"

class clearScene : public scene {
public:
	CImage back, txtmotion;
	int size;
public:
	virtual ~clearScene() override;

	virtual void init() override;

	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Update(const float frameTime) override;

	virtual void Render(HDC hdc) override;
};
