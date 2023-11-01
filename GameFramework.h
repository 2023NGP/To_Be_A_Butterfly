#pragma once

enum SCENE{MENU, GAME, STAGE2};

class WGameFramework
{
public:
	scene* curScene;
	SCENE nowscene;
	clock_t prevFrameTime;
	clock_t curFrameTime;

public:
	WGameFramework();
	~WGameFramework();

	void Reset();
	void Clear();

	void Create(HWND hWnd);
	void OnDraw(HDC hdc);
	void OnUpdate(const float frameTime = 0.17f);
	void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);

	float GetTick();

private:
	HWND	m_hWnd;
};