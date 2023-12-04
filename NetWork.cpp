#include "stdafx.h"
#include "common.h"
#include "NetWork.h"

bool SendRecvPlayerInfo(SOCKET sock);
bool RecvPlayerInit(SOCKET sock);

HANDLE hServerProcess;

HANDLE hGameEvent;
HANDLE hSocketEvent;

// struct
PLAYER_INIT_SEND	g_tPlayerInit;
PLAYER_INIT			m_tPlayerInit;
PLAYER_INFO			m_tPlayerInfo;
STORE_DATA			m_tStoreData;

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

NetWork::NetWork()
{
	// 윈속 초기화
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
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
	//char buf[1024];
	//const char* testdata[] = {
	//	"안녕하세요",
	//	"반가워요",
	//	"오늘따라 할 이야기가 많을 것 같네요",
	//	"저도 그렇네요",
	//};
	//int len;
	//// 서버와 데이터 통신
	//for (int i = 0; i < 4; i++) {
	//	// 데이터 입력(시뮬레이션)
	//	len = (int)strlen(testdata[i]);
	//	strncpy(buf, testdata[i], len);
	//	// 데이터 보내기(고정 길이)
	//	retval = send(sock, (char*)&len, sizeof(int), 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//		break;
	//	}
	//	// 데이터 보내기(가변 길이)
	//	retval = send(sock, buf, len, 0);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//		break;
	//	}
	//	printf("[TCP 클라이언트] %d+%d바이트를 "
	//		"보냈습니다.\n", (int)sizeof(int), retval);
	//}

	while (1)
	{
		WaitForSingleObject(hSocketEvent, INFINITE);

		// 이곳에 각각의 송수신 함수 추가
		//////////////////////////////////////////////////////////

		retval = RecvPlayerInit(sock);
		if (retval == FALSE)
			break;

		retval = SendRecvPlayerInfo(sock);
		if (retval == FALSE)
			break;


		SetEvent(hGameEvent);
	}

	SetEvent(hGameEvent);

}

NetWork::~NetWork()
{
	closesocket(sock);
	// 윈속 종료
	WSACleanup();
}

void NetWork::SendClientPos(int px, int py)
{
	// 데이터 보내기(고정 길이)
	retval = send(sock, (char*)&px, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	cout << px << endl;
}
void NetWork::RecvOtherClientPos()
{
}

bool SendRecvPlayerInfo(SOCKET sock)
{
	int retval;

	//자기 좌표 보내기
	PLAYER_INFO tPlayerInfo = m_tPlayerInfo;
	retval = send(sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	//모든 좌표 받기
	retval = recvn(sock, (char*)&m_tStoreData, sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// 여기서 엔딩 확인
	cout << "전체 정보 받음 : YPos - " << m_tStoreData.playersInfo->pos.fY << endl;

	//충돌된 좌표로 갱신
	//Player Render 에서



	return TRUE;
}

bool RecvPlayerInit(SOCKET sock)
{
	int retval;

	retval = recvn(sock, (char*)&m_tPlayerInit, sizeof(PLAYER_INIT_SEND), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	cout << "init 정보 받음 : YPos - " << m_tPlayerInit.pos->fY << endl;
}