#pragma once
#include <Windows.h>
#include <atlimage.h>
#include "stdafx.h"
#include "Player.h"
#include "Item.h"

enum GAME_STATUS{RUN, PAUSE, GAMEOVER};

class Scene {
public:
	GAME_STATUS status;
	int startX, startY;

public:
	virtual ~Scene();
	virtual void init() = 0;
	virtual void processKey(UINT iMessage, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Update(const float frameTime) = 0;
	virtual void Render(HDC hdc) = 0;
};