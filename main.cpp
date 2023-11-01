
#include "stdafx.h"
#include "Client.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
//static WGameFramework gGameFramework;
WGameFramework framework;
// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

//#ifdef _DEBUG
//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif
//#endif
//
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_APP_TITLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	framework.prevFrameTime = framework.curFrameTime = clock();

	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	framework.Clear();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(NULL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(NULL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(NULL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, FRAME_WIDTH, FRAME_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HBITMAP hBitmap;
	HDC mainHDC;
	HDC memdc;		//menuDC
	HDC gamedc;
	HDC game2dc;

	switch (message)
	{
	case WM_CREATE:
	{
		framework.Create(hWnd);
		SetTimer(hWnd, MAIN_TIMER, MAIN_TIMER_FRAME, NULL);
		SetTimer(hWnd, MENU_TIMER, MENU_TIMER_FRAME, NULL);
	}
	break;

	case WM_PAINT:
	{
		mainHDC = BeginPaint(hWnd, &ps);
		if (framework.nowscene == MENU) {
			hBitmap = CreateCompatibleBitmap(mainHDC, FRAME_WIDTH, FRAME_HEIGHT);
			memdc = CreateCompatibleDC(mainHDC);
			SelectObject(memdc, hBitmap);
			framework.OnDraw(memdc);
			BitBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, memdc, 0, 0, SRCCOPY);
			DeleteDC(memdc);
		}
		else if(framework.nowscene == GAME){
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, MEM_HEIGHT);		//gamescene일땐 memdc가 길어야 함
			gamedc = CreateCompatibleDC(mainHDC);
			SelectObject(gamedc, hBitmap);
			framework.OnDraw(gamedc);
			StretchBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, gamedc, framework.curScene->startX, framework.curScene->startY, FRAME_WIDTH, FRAME_HEIGHT, SRCCOPY);
			DeleteDC(gamedc);
		}
		else {
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, STAGE2_HEIGHT);		//gamescene일땐 memdc가 길어야 함
			game2dc = CreateCompatibleDC(mainHDC);
			SelectObject(game2dc, hBitmap);
			framework.OnDraw(game2dc);
			StretchBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, game2dc, framework.curScene->startX, framework.curScene->startY, FRAME_WIDTH, FRAME_HEIGHT, SRCCOPY);
			DeleteDC(game2dc);
		}

		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
			if (framework.nowscene == GAME || framework.nowscene == STAGE2) {
				framework.curFrameTime = clock();
				framework.OnUpdate(framework.GetTick());
				framework.prevFrameTime = framework.curFrameTime;
				InvalidateRgn(hWnd, NULL, false);
			}
			else
				break;
		case 2:
			if (framework.nowscene == MENU) {
				framework.curFrameTime = clock();
				framework.OnUpdate(framework.GetTick());
				framework.prevFrameTime = framework.curFrameTime;
				InvalidateRgn(hWnd, NULL, false);
			}
			else
				break;
			break;
		}

	}
	break;


	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		framework.KeyBoard(message, wParam, lParam);
	}
	break;


	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}