#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#include "stdafx.h"
#include "common.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

char* SERVERIP = (char*)"127.0.0.1";
//char* SERVERIP;
#define SERVERPORT 9000
#define BUFSIZE 1024

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
//static WGameFramework gGameFramework;
WGameFramework framework;
// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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

		// TODO: ���⿡ �ڵ带 �Է��մϴ�.

		// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_APP_TITLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	framework.prevFrameTime = framework.curFrameTime = clock();

	// return value;
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect() : TCP�������� ���ؿ��� ������ ���� ������ ���� (bind() ���� ����, �ɵ���)
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	printf("connect ����");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�",
	};
	int len;

	// ������ ������ ���
	for (int i = 0; i < 4; i++) {
		// ������ �Է�(�ùķ��̼�)
		len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);

		// ������ ������(���� ����)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// ������ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", (int)sizeof(int), retval);
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

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();

	return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, STAGE2_HEIGHT);		//gamescene�϶� memdc�� ���� ��
			game2dc = CreateCompatibleDC(mainHDC);
			SelectObject(game2dc, hBitmap);
			framework.OnDraw(game2dc);
			StretchBlt(mainHDC, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, game2dc, framework.mainCamera->m_vLookAt.x, framework.mainCamera->m_vLookAt.y, FRAME_WIDTH, FRAME_HEIGHT, SRCCOPY);
			DeleteDC(game2dc);
		}
		else {		// framework.NowScene == GAME
			hBitmap = CreateCompatibleBitmap(mainHDC, MEM_WIDTH, MEM_HEIGHT);		//gamescene�϶� memdc�� ���� ��
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
		FreeConsole();	// �ܼ�â �ݱ�
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}