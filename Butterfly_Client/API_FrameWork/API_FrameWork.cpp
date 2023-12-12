// API_FrameWork.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "API_FrameWork.h"

#include "MainGame.h"
#include "Potion.h"
#include "Coin.h"
#include "Cloud.h"
#include "ObjMgr.h"
#include "MyButton.h"
#include "DataMgr.h"
#include "Cloud.h"

#define MAX_LOADSTRING 100

// ���� ����:
HWND	g_hWnd;
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// ���� ���� �Լ�
void err_display(char* msg);
void err_quit(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI ServerProcess(LPVOID arg);

bool SendRecvPlayerInfo(SOCKET sock);

// ī�޶�
float g_CameraLookAt_Y;
bool RecvCameraData(SOCKET sock);

bool SendRecv_HeartInfo(SOCKET sock);
bool SendRecvCoinInfo(SOCKET sock);
bool RecvPlayerInit(SOCKET sock);

// ���� ���� ����
HANDLE hServerProcess;

// �����ο� < 3 �϶� ���
HANDLE hGameEvent;
// ��ȣ�����϶� �����ϱ� ���� �̺�Ʈ
HANDLE hSocketEvent;

char* SERVERIP = (char*)"127.0.0.1";
string IP = {};


// ��Ʈ ���� ����, �Լ�
POTIONRES g_tHeartRes;
// ���� ����
COINRES g_tCoinRes;
// ���� ����
CLOUDRES g_tCloudRes;

// ���ӽ��� ����
PLAYER_INIT_SEND g_tPlayerInit;
CLOUD g_Clouds[200];

void Add_Heart(HpPotionCreate);
void Delete_Heart(HpPotionDelete hpPotionDelete);
void Add_Coin(CoinCreate);
void Delete_Coin(CoinDelete coinDelete);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	srand(unsigned int(time(NULL)));

	IP = lpCmdLine;

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_API_FRAMEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_API_FRAMEWORK));

	MSG msg;
	msg.message = WM_NULL;

	CMainGame mainGame;
	mainGame.Initialize();

	CMyButton button;

	//hServerProcess = CreateThread(NULL, 0, ServerProcess, NULL, 0, NULL);

	DWORD	dwTime1 = GetTickCount();

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (dwTime1 + 10  < GetTickCount())
		{
			WaitForSingleObject(hGameEvent, INFINITE);

			mainGame.Update();
			mainGame.Late_Update();

			// ���� ��� ����

			mainGame.Render();

			SetEvent(hSocketEvent);


			dwTime1 = GetTickCount();
		}

	}

	CDataMgr::Get_Instance()->Destroy_Instance();

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_API_FRAMEWORK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	RECT rc = { 0, 0, WINCX, WINCY };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0
		, rc.right - rc.left, rc.bottom - rc.top
		, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_hWnd = hWnd;

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

DWORD WINAPI ServerProcess(LPVOID arg)
{
    //���� ���� ����
    hGameEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        err_quit("socket()");

    // connet()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(IP.c_str());
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
        err_quit("connect()");

    int nagleopt = TRUE;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nagleopt, sizeof(nagleopt));



	// ���� ���� �޾� ���� �� ����
	for (int i = 0; i < CLOUD_COUNT; ++i) {
		retval = recvn(sock, (char*)&g_Clouds[i], sizeof(CLOUD), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			return FALSE;
		}
		else if (retval == 0)
			return FALSE;

		CObj* pObj1 = CAbstractFactory<CCloud>::Create();
		pObj1->Set_Pos(g_Clouds[i].pos.fX, g_Clouds[i].pos.fY);
		dynamic_cast<CCloud*>(pObj1)->SetIndex(i);
		dynamic_cast<CCloud*>(pObj1)->SetType(g_Clouds[i].type);
		CObjMgr::Get_Instance()->Add_Object(OBJID::CLOUD, pObj1);
	}


    while (1)
    {
        WaitForSingleObject(hSocketEvent, INFINITE);
        
        // �̰��� ������ �Լ� �߰�! �ְ� �޴� ��
        //////////////////////////////////////////////////////////

		retval = RecvPlayerInit(sock);
		if (retval == FALSE)
			break;

        retval = SendRecvPlayerInfo(sock);
        if (retval == FALSE)
            break;

		// ī�޶�
		retval = RecvCameraData(sock);
		if (retval == FALSE) {
			break;
		}
   
        // ��Ʈ
        retval = SendRecv_HeartInfo(sock);
        if (retval == FALSE)
            break;

		// ����
		retval = SendRecvCoinInfo(sock);
		if (retval == FALSE) {
			break;
		}

        //////////////////////////////////////////////////////////

        SetEvent(hGameEvent);
    }

    SetEvent(hGameEvent);

    closesocket(sock);

    // ���� ����
    WSACleanup();

}

bool SendRecvPlayerInfo(SOCKET sock)
{
    int retval;

    // ���� Ŭ���̾�Ʈ ���� ������
    PLAYER_INFO tPlayerInfo = CDataMgr::Get_Instance()->m_tPlayerInfo;
    retval = send(sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    // �ٸ� Ŭ���̾�Ʈ ���� �ޱ�
    retval = recvn(sock, (char*)&(CDataMgr::Get_Instance()->m_tStoreData), sizeof(STORE_DATA), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("recv()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;
    
	return TRUE;
}

bool RecvCameraData(SOCKET sock)
{
	int retval = recvn(sock, (char*)&g_CameraLookAt_Y, sizeof(float), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;
	return TRUE;
}

bool SendRecv_HeartInfo(SOCKET sock)
{
	int retval;

	// ��Ʈ ���� �ޱ� ���� & ����
	HpPotionInfo tHeartInfo;
	retval = recvn(sock, (char*)&tHeartInfo, sizeof(HpPotionInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// ��Ʈ ����
	if (tHeartInfo.thpPotionCreate.bCreateOn)
	{
		Add_Heart(tHeartInfo.thpPotionCreate);
	}

	// ��Ʈ ����
	if (tHeartInfo.thpPotionDelete.bDeleteOn)
	{
		Delete_Heart(tHeartInfo.thpPotionDelete);
	}

	// ��Ʈ �浹 ���� ������
	retval = send(sock, (char*)&g_tHeartRes, sizeof(POTIONRES), 0); // ���̰� ������ ���� �ƴ� ���������� len
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	ZeroMemory(&g_tHeartRes, sizeof(POTIONRES));

	return TRUE;
}

bool SendRecvCoinInfo(SOCKET sock)
{
	int retval;

	// ���� �����ޱ� ����&����
	CoinInfo tCoinInfo;
	retval = recvn(sock, (char*)&tCoinInfo, sizeof(CoinInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// ���� ����
	if (tCoinInfo.tCoinCreate.bCreateOn)
	{
		Add_Coin(tCoinInfo.tCoinCreate);
	}

	// ���� ����
	if (tCoinInfo.tCoinDelete.bDeleteOn)
	{
		Delete_Coin(tCoinInfo.tCoinDelete);
	}

	// ���� �浹 ���� ������
	retval = send(sock, (char*)&g_tCoinRes, sizeof(COINRES), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	ZeroMemory(&g_tCoinRes, sizeof(COINRES));

	return TRUE;
}

void Add_Heart(HpPotionCreate hpPotionCreate)
{
    CObj* pObj1 = CAbstractFactory<CPotion>::Create();
    pObj1->Set_Pos(hpPotionCreate.pos.fX, hpPotionCreate.pos.fY);
    dynamic_cast<CPotion*>(pObj1)->SetIndex(hpPotionCreate.index);
    CObjMgr::Get_Instance()->Add_Object(OBJID::HEART, pObj1);
}

void Delete_Heart(HpPotionDelete hpPotionDelete)
{
	// index ��ġ�ϴ� ��Ʈ ã�Ƽ� �����ϱ�
	CObjMgr::Get_Instance()->Delete_Potion(hpPotionDelete.index);
}

void Add_Coin(CoinCreate coinCreate)
{
    CObj* pObj1 = CAbstractFactory<CCoin>::Create();
    pObj1->Set_Pos(coinCreate.pos.fX, coinCreate.pos.fY);
    dynamic_cast<CCoin*>(pObj1)->SetIndex(coinCreate.index);
    CObjMgr::Get_Instance()->Add_Object(OBJID::COIN, pObj1);
}

void Delete_Coin(CoinDelete coinDelete)
{
	// index ��ġ�ϴ� ��Ʈ ã�Ƽ� �����ϱ�
	CObjMgr::Get_Instance()->Delete_Coin(coinDelete.index);
}

bool RecvPlayerInit(SOCKET sock)
{
	int retval;

	retval = recvn(sock, (char*)&g_tPlayerInit, sizeof(PLAYER_INIT_SEND), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	return TRUE;
}