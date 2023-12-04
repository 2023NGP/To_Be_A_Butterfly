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
	hServerProcess = CreateEvent(NULL, FALSE, FALSE, NULL);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// ���� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
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


}

NetWork::~NetWork()
{
	closesocket(sock);
	// ���� ����
	WSACleanup();
}

bool NetWork::RecvInitData()
{
	char buf[BUFSIZE + 1];
	int retval = recv(sock, buf, BUFSIZE, 0);
	if (retval != -1) {
		return true;
	}
	return false;
}

void NetWork::SendClientPos(int px, int py)
{
	// ������ ������(���� ����)
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

	//�ڱ� ��ǥ ������
	PLAYER_INFO tPlayerInfo = m_tPlayerInfo;
	retval = send(sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	//��� ��ǥ �ޱ�
	retval = recvn(sock, (char*)&m_tStoreData, sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// ���⼭ ���� Ȯ��
	cout << "��ü ���� ���� : YPos - " << m_tStoreData.playersInfo->pos.fY << endl;

	//�浹�� ��ǥ�� ����
	//Player Render ����



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

	cout << "init ���� ���� : YPos - " << m_tPlayerInit.pos->fY << endl;
}