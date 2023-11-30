#pragma once

// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ Ư�� ���� ������ ��� �ִ�
// ���� �����Դϴ�.
//

#define _CRT_SECURE_NO_WARNINGS

//#pragma comment (lib, "fmod_vc.lib")
#include "targetver.h"
#include"fmod.hpp"
#include"fmod_errors.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
#include <atlimage.h>
#include <mmsystem.h>
#include <fmod.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <windef.h>
#include <assert.h>

using namespace std;
using namespace std::chrono;

#include "Define.h"
#include "vector.h"
#include "scene.h"
#include "menuScene.h"
#include "stage2Scene.h"
#include "overScene.h"
#include "clearScene.h"
#include "resource.h"
#include "Camera.h"
#include "NetWork.h"
#include "GameFramework.h"

// ���⼭ ���α׷��� �ʿ��� �߰� ����� �����մϴ�.