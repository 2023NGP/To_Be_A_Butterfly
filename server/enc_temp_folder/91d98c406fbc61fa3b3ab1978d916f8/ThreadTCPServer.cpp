#include "Common.h"
#include "pch.h"

// 스레드
struct MyThread
{
	int iIndex = 0;
	SOCKET sock = 0;
};

DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
void CountStart();
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);
void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend);
bool SendPlayerInit(SOCKET sock, int PlayerIndex);
void CheckEnding(int iCurIndex);
bool Check_Sphere(INFO& tMePos, INFO& tYouPos);
bool Check_Rect(INFO& tMePos, INFO& tYouPos, float* _x, float* _y);


HANDLE clientEvent[3]{};		// 클라이언트 별 이벤트
int waitClientIndex[3];			// 대기 클라이언트 관련
int clientCount = 0;			// 접속한 클라이언트 개수

CGameTimer Timer;				// 서버 타이머
float fStartTime = 0.f;			// 시작 시간

STORE_DATA storeData; // 클라이언트 통합 정보

// 게임 시작 관련
PLAYER_INIT playerInit;
bool isGameStart = false;

// 게임 종료판정 관련
bool ending = false;

// 오류 메시지를 생성하고 콘솔에 출력하는 함수
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	// WSAGetLastError()로 얻은 오류 코드에 대한 시스템 오류 메시지를 가져옴
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	// 콘솔에 오류 메시지를 출력
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	// 동적으로 할당된 메모리를 해제
	LocalFree(lpMsgBuf);
}
// 지정된 소켓에서 지정된 길이만큼 데이터를 받아오는 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		// 소켓으로부터 데이터를 받아옴
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;  // 상대방이 연결을 종료한 경우
		left -= received;
		ptr += received;
	}

	return (len - left);  // 받아온 데이터의 총 길이 반환
}

// 게임 시작 카운트 다운
void CountStart()
{
	fStartTime += Timer.GetTimeElapsed();  // 이전 프레임에서 현재 프레임까지 시간 
	if (fStartTime >= START_TIME)
	{
		playerInit.start = true;
	}
}

// 서버 프로세스 구현
DWORD WINAPI ServerMain(LPVOID arg)
{
	Timer.Reset();
	while (true)
	{
		if (clientCount == 3)   // 클라 3명이면 스타트
		{
			Timer.Tick(60.0f);
			// 매순간 해줘야하는 거 여기 둔다
			if (!playerInit.start)
				CountStart();
		}

	}
}

void RecvClientPos(SOCKET client_sock);

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	MyThread* pThread = (MyThread*)arg;

	int curIndex = pThread->iIndex; // 현재 스레드 인덱스,	[ 0 ~ 2 ]


	int retval;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(pThread->sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (1)
	{
		// 3명이 시작하면 게임 시작
		if (!isGameStart)
		{
			if (clientCount < 3)
				continue;
			else
				isGameStart = true;
		}

		// 좀 그렇긴해 ----------------------
		if (pThread->iIndex >= 4) {
			break;
		}

		if (clientCount <= 2)
			WaitForSingleObject(clientEvent[waitClientIndex[curIndex]], INFINITE);

		// 타이머, 플레이어 배치 (수신 실패 시 루프 종료)
		if (!SendPlayerInit(pThread->sock, curIndex))
		{
			SetEvent(clientEvent[curIndex]);
			break;
		}

		//플레이어 데이터 받기 (송신 실패 시 루프 종료)
		if (!SendRecv_PlayerInfo(pThread->sock, curIndex))
		{
			SetEvent(clientEvent[curIndex]);
			break;
		}



		SetEvent(clientEvent[curIndex]);
	}

	if (--clientCount >= 2)
	{
		for (int i = 0; i < 3; ++i)
		{
			// 자신을 참조하던 클라이언트를 찾음
			if (waitClientIndex[i] == curIndex)
			{
				// 자신이 참조하고 있던 인덱스로 바꿔줌
				waitClientIndex[i] = waitClientIndex[curIndex];
				// 자신이 참조하고 있던 인덱스를 초기화
				waitClientIndex[curIndex] = -1;
				// 더 이상 검색할 필요가 없으므로 반복문을 종료
				break;
			}
		}
	}


	// 이벤트 제거
	CloseHandle(clientEvent[curIndex]);

	// closesocket()
	closesocket(pThread->sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

// main
int main(int argc, char* argv[])
{
	// ServerMain 스레드
	CreateThread(NULL, 0, ServerMain, 0, 0, NULL);

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	int len; // 고정 길이 데이터
	char buf[BUFSIZE + 1]; // 가변 길이 데이터

	//이벤트 생성
	//클라이언트 3개 접속중일 때 0이 2번, 1이 0번, 2가 1번, 이벤트 기다림
	for (int i = 0; i < 3; ++i)
	{
		clientEvent[i] = CreateEvent(NULL, FALSE, (i < 2 ? FALSE : TRUE), NULL);
		waitClientIndex[i] = (i == 0) ? 2 : i - 1; // 2 0 1
	}

	MyThread tThread;
	tThread.iIndex = 0;
	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		tThread.sock = client_sock;
		++clientCount;						// 접속한 클라이언트 수, [ 1 ~ 3 ]
		tThread.iIndex = clientCount - 1;	// 스레드 아이디,		 [ 0 ~ 2 ]

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, &tThread, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// 소켓 닫기
	
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 가현
//void RecvClientPos(SOCKET client_sock) {
//	int px = 0;
//	int retval = recv(client_sock, (char*)&px, sizeof(int), MSG_WAITALL);
//	if (retval == SOCKET_ERROR) {
//		err_display("recv()");
//	}
//	printf("%d\n", px);

// 플레이어 정보 보내고 받는 함수
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex)
{
	int retval, curIndex = iIndex;

	PLAYER_INFO tPlayerInfo;
	retval = recvn(client_sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;


	// 받은 데이터 출력 확인
	//printf("[%d] (%f, %f)\n", curIndex, tPlayerInfo.tPos.fX, tPlayerInfo.pos.fY);

	if (clientCount == 3)    // 클라이언트 3명이면 스타트
	{
		tPlayerInfo.start = true;
	}

	// 엔딩 변수 저장
	ENDING::END_TYPE eEnding = ENDING::ING;

	if (ending)
	{
		eEnding = storeData.playersInfo[curIndex].eEnding;
	}

	storeData.playersInfo[curIndex] = tPlayerInfo;
	storeData.id = curIndex;

	// 엔딩 변수 설정
	storeData.playersInfo[curIndex].eEnding = eEnding;

	storeData.hp[curIndex] = tPlayerInfo.hp;
	storeData.start = tPlayerInfo.start;

	// 엔딩 판정
	CheckEnding(curIndex);

	// 데이터 보내기
	retval = send(client_sock, (char*)&storeData, sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return FALSE;
	}

	return TRUE;
}

// 플레이어 인덱스별 위치 설정함수
void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend)
{
	switch (idx)
	{
	case 0:			// 1번 플레이어 위치
		tPlayerInitSend.pos = { 100.f, 600.f };
		break;
	case 1:			// 2번 플레이어 위치
		tPlayerInitSend.pos = { 400.f, 600.f };
		break;
	case 2:			// 3번 플레이어 위치
		tPlayerInitSend.pos = { 700.f, 600.f };
		break;
	default:
		break;
	}
}

// 플레이어 초기 전송하는 함수
bool SendPlayerInit(SOCKET sock, int PlayerIndex)
{
	int retval;

	playerInit.iCount = (int)fStartTime;
	PLAYER_INIT_SEND tPlayerInitSend;
	tPlayerInitSend.start = playerInit.start;
	tPlayerInitSend.idx = PlayerIndex;
	tPlayerInitSend.iCount = playerInit.iCount;

	if (playerInit.start)
	{
		Get_InitPos(PlayerIndex, tPlayerInitSend);
	}
	retval = send(sock, (char*)&tPlayerInitSend, sizeof(PLAYER_INIT_SEND), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return FALSE;
	}
	return TRUE;
}


// 엔딩 판정
void CheckEnding(int iCurIndex)
{
	if (ending)
		return;

	if (storeData.playersInfo[iCurIndex].isDead)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == iCurIndex)
				continue;

			if (storeData.playersInfo[i].isDead)
			{
				ending = true;
				storeData.playersInfo[iCurIndex].eEnding = ENDING::LOSE;
				storeData.playersInfo[i].eEnding = ENDING::LOSE;
				for (int j = 0; j < 3; j++)
				{
					if (j == i || j == iCurIndex)
						continue;
					storeData.playersInfo[j].eEnding = ENDING::WIN;

				}
				break;
			}
		}

	}
}


///////////////////////충돌 체크 (-ing)/////////////////////////////
bool Check_Sphere(INFO& tMePos, INFO& tYouPos)
{
	float fRadius = (float)((tMePos.iCX + tYouPos.iCY) >> 1);
	//float fRadius = 30;

	float fX = tMePos.fX - tYouPos.fX;
	float fY = tMePos.fY - tYouPos.fY;
	float fDis = sqrtf(fX * fX + fY * fY);

	return fRadius > fDis;
}

bool Check_Rect(INFO& tMePos, INFO& tYouPos, float* _x, float* _y)
{
	float fX = abs(tMePos.fX - tYouPos.fX);
	float fY = abs(tMePos.fY - tYouPos.fY);

	//플레이어 너비, 높이
	float fCX = 30.f;
	float fCY = 80.f;

	if (fCX > fX && fCY > fY)
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		return true;
	}

	return false;
}

// 여기서 충돌 체크
void CheckCollision(int index)
{
	for (int i = 0; i < 3; ++i)
	{
		// 자기 자신 충돌x
		if (index == i)
			continue;

		// 플레이어 죽은 상태면 충돌x
		if (storeData.playersInfo[i].isDead)
			continue;

	}
}