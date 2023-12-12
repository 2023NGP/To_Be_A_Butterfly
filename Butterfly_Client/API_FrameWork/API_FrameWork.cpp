// API_FrameWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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

// 전역 변수:
HWND	g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// 서버 관련 함수
void err_display(char* msg);
void err_quit(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI ServerProcess(LPVOID arg);

bool SendRecvPlayerInfo(SOCKET sock);

// 카메라
float g_CameraLookAt_Y;
bool RecvCameraData(SOCKET sock);

bool SendRecv_HeartInfo(SOCKET sock);
bool SendRecvCoinInfo(SOCKET sock);
bool RecvPlayerInit(SOCKET sock);

// 서버 관련 변수
HANDLE hServerProcess;

// 접속인원 < 3 일때 대기
HANDLE hGameEvent;
// 신호상태일때 갱신하기 위한 이벤트
HANDLE hSocketEvent;

char* SERVERIP = (char*)"127.0.0.1";
string IP = {};


// 하트 관련 변수, 함수
POTIONRES g_tHeartRes;
// 코인 관련
COINRES g_tCoinRes;
// 구름 관련
CLOUDRES g_tCloudRes;

// 게임시작 관련
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

	// TODO: 여기에 코드를 입력합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_API_FRAMEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
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

			// 서버 통신 진행

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
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
    //게임 먼저 실행
    hGameEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    int retval;

    // 윈속 초기화
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



	// 구름 정보 받아 오기 및 생성
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
        
        // 이곳에 각각의 함수 추가! 주고 받는 것
        //////////////////////////////////////////////////////////

		retval = RecvPlayerInit(sock);
		if (retval == FALSE)
			break;

        retval = SendRecvPlayerInfo(sock);
        if (retval == FALSE)
            break;

		// 카메라
		retval = RecvCameraData(sock);
		if (retval == FALSE) {
			break;
		}
   
        // 하트
        retval = SendRecv_HeartInfo(sock);
        if (retval == FALSE)
            break;

		// 코인
		retval = SendRecvCoinInfo(sock);
		if (retval == FALSE) {
			break;
		}

        //////////////////////////////////////////////////////////

        SetEvent(hGameEvent);
    }

    SetEvent(hGameEvent);

    closesocket(sock);

    // 윈속 종료
    WSACleanup();

}

bool SendRecvPlayerInfo(SOCKET sock)
{
    int retval;

    // 현재 클라이언트 정보 보내기
    PLAYER_INFO tPlayerInfo = CDataMgr::Get_Instance()->m_tPlayerInfo;
    retval = send(sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return FALSE;
    }
    else if (retval == 0)
        return FALSE;

    // 다른 클라이언트 정보 받기
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

	// 하트 정보 받기 생성 & 삭제
	HpPotionInfo tHeartInfo;
	retval = recvn(sock, (char*)&tHeartInfo, sizeof(HpPotionInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// 하트 생성
	if (tHeartInfo.thpPotionCreate.bCreateOn)
	{
		Add_Heart(tHeartInfo.thpPotionCreate);
	}

	// 하트 삭제
	if (tHeartInfo.thpPotionDelete.bDeleteOn)
	{
		Delete_Heart(tHeartInfo.thpPotionDelete);
	}

	// 하트 충돌 정보 보내기
	retval = send(sock, (char*)&g_tHeartRes, sizeof(POTIONRES), 0); // 길이가 고정된 값이 아닌 가변인자인 len
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

	// 코인 정보받기 생성&삭제
	CoinInfo tCoinInfo;
	retval = recvn(sock, (char*)&tCoinInfo, sizeof(CoinInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// 코인 생성
	if (tCoinInfo.tCoinCreate.bCreateOn)
	{
		Add_Coin(tCoinInfo.tCoinCreate);
	}

	// 코인 삭제
	if (tCoinInfo.tCoinDelete.bDeleteOn)
	{
		Delete_Coin(tCoinInfo.tCoinDelete);
	}

	// 코인 충돌 정보 보내기
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
	// index 일치하는 하트 찾아서 삭제하기
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
	// index 일치하는 하트 찾아서 삭제하기
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