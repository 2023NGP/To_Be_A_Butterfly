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
int g_iClientCount = 0; //������ Ŭ�� ����

DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

//�÷��̾� ����
bool SendPlayerInit(SOCKET sock, int PlayerIndex);  // ���ӽ��ۿ��� ������
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);

//�浹
void CheckCollision(int iIndex);

// ���ӽ���
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

	// ServerMain ������
	CreateThread(NULL, 0, ServerMain, 0, 0, NULL);

	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	int len; // ���� ���� ������
	char buf[BUFSIZE + 1]; // ���� ���� ������


	//�̺�Ʈ ����
	//Ŭ��3�� �������϶� 0�� 2��, 1�� 0��, 2�� 1�� �̺�Ʈ ��ٸ�
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

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, &tThread, 0, NULL);

		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}


// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	//SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];
	int len; // ���� ���� ������

	MyThread* pThread = (MyThread*)arg;
	SOCKET client_sock = (SOCKET)pThread->sock;
	int iCurIndex = pThread->iIndex - 1; //���� ������ �ε���, �迭 �ε����� ����ؼ� -1

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
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

		// ������ �ޱ�(���� ����)
		retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ������ �ޱ�(���� ����)
		retval = recv(client_sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
	}

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}

