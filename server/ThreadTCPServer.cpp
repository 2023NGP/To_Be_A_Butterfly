#include "Common.h"
#include <iostream>
#define SERVERPORT 9000
#define BUFSIZE    512

struct MyThread
{
	int iIndex = 0;
	SOCKET sock = 0;
};

HANDLE g_hClientEvent[3];
int g_iWaitClientIndex[3];
int g_iClientCount = 0; //접속한 클라 갯수

DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

//플레이어 관련
bool SendPlayerInit(SOCKET sock, int PlayerIndex);  // 게임시작여부 보내기
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);

//충돌
void CheckCollision(int iIndex);

// 게임시작
bool isGameStart = false;

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
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

/////////////

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
	//클라3개 접속중일때 0이 2번, 1이 0번, 2가 1번 이벤트 기다림
	for (int i = 0; i < 3; ++i)
	{
		g_hClientEvent[i] = CreateEvent(NULL, FALSE, (i < 2 ? FALSE : TRUE), NULL);
		g_iWaitClientIndex[i] = (i == 0) ? 2 : i - 1; // 2 0 1
	}


	MyThread tThread;
	tThread.iIndex = 0;


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		int nagleopt = TRUE;
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nagleopt, sizeof(nagleopt));

		tThread.sock = client_sock;
		++g_iClientCount;
		++tThread.iIndex;

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

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


// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	//SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];
	int len; // 고정 길이 데이터

	MyThread* pThread = (MyThread*)arg;
	SOCKET client_sock = (SOCKET)pThread->sock;
	int iCurIndex = pThread->iIndex - 1; //현재 쓰레드 인덱스, 배열 인덱스로 사용해서 -1

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// 클라이언트와 데이터 통신
	while (1) {

		if (!isGameStart)
		{
			if (g_iClientCount < 4)
				continue;
			else
				isGameStart = true;
		}

		//if (g_iClientCount >= 2) 
			//WaitForSingleObject(g_hClientEvent[g_iWaitClientIndex[iCurIndex]], INFINITE);

		// 데이터 받기(고정 길이)
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 데이터 받기(가변 길이)
		retval = recv(client_sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
	}

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}

