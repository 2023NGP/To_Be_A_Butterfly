#pragma once
#include "scene.h"

class menuScene : public scene {
public:
	CImage background;
	CImage normalCloud, rainCloud, darkCloud, cloud;
	CImage press;
	int cloudSizeCount;
	int cloudSize;
	int rainCloudSize;
	int darkCloudSize;
	int pressText;
	int ren; //애니메이션을 위한 변수
public:
	virtual ~menuScene() override;

	virtual void init() override;

	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	virtual void Update(const float frameTime) override;

	virtual void Render(HDC hdc) override;
};