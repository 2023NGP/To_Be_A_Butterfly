#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#include "stdafx.h"
#include "common.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

char* SERVERIP = (char*)"127.0.0.1";
//char* SERVERIP;
#define SERVERPORT 9000
#define BUFSIZE 1024

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

	// return value;
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect() : TCP프로토콜 수준에서 서버와 논리적 연결을 설정 (bind() 역할 수행, 능동적)
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	printf("connect 성공");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	const char* testdata[] = {
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요",
	};
	int len;

	// 서버와 데이터 통신
	for (int i = 0; i < 4; i++) {
		// 데이터 입력(시뮬레이션)
		len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);

		// 데이터 보내기(고정 길이)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// 데이터 보내기(가변 길이)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d+%d바이트를 "
			"보냈습니다.\n", (int)sizeof(int), retval);
	}


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

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

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
		if (framework.NowScene == MENU) {
			hBitmap = CreateCompatibleBitmap(mainHDC, FRAME_WIDTH, FRAME_HEIGHT);
			memdc = CreateCompatibleDC(mainHDC);
			SelectObject(memdc, hBitmap);
			framework.OnDraw(memdc);
			BitBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, memdc, 0, 0, SRCCOPY);
			DeleteDC(memdc);
		}
		else if (framework.NowScene == STAGE2) {
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, STAGE2_HEIGHT);		//gamescene일땐 memdc가 길어야 함
			game2dc = CreateCompatibleDC(mainHDC);
			SelectObject(game2dc, hBitmap);
			framework.OnDraw(game2dc);
			StretchBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, game2dc, framework.mainCamera->m_vLookAt.x, framework.mainCamera->m_vLookAt.y, FRAME_WIDTH, FRAME_HEIGHT, SRCCOPY);
			DeleteDC(game2dc);
		}
		else {		// framework.NowScene == GAME
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, MEM_HEIGHT);		//gamescene일땐 memdc가 길어야 함
			gamedc = CreateCompatibleDC(mainHDC);
			SelectObject(gamedc, hBitmap);
			framework.OnDraw(gamedc);
			StretchBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, gamedc, framework.mainCamera->m_vLookAt.x, framework.mainCamera->m_vLookAt.y, FRAME_WIDTH, FRAME_HEIGHT, SRCCOPY);
			DeleteDC(gamedc);
		}

		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
			if (framework.NowScene == GAME || framework.NowScene == STAGE2) {
				framework.curFrameTime = clock();
				framework.OnUpdate(framework.GetTick());
				framework.prevFrameTime = framework.curFrameTime;
				InvalidateRgn(hWnd, NULL, false);
			}
			else
				break;
		case 2:
			if (framework.NowScene == MENU) {
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
		FreeConsole();	// 콘솔창 닫기
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}