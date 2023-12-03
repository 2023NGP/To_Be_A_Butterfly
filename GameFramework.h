#pragma once

// 타이틀, 스테이지, 로비, 점수, 클리어, 오버
enum SCENELIST{MENU ,GAME, STAGE2, LOBBY, SCORE, GAME_CLEAR, GAME_OVER};

class WGameFramework
{
public:
	Camera*			mainCamera;
	Scene*			CurScene;
	SCENELIST		NowScene;
	clock_t			prevFrameTime;
	clock_t			curFrameTime;

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