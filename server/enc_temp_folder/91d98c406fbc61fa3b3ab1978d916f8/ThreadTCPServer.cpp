#include "Common.h"
#include "pch.h"

// ������
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


HANDLE clientEvent[3]{};		// Ŭ���̾�Ʈ �� �̺�Ʈ
int waitClientIndex[3];			// ��� Ŭ���̾�Ʈ ����
int clientCount = 0;			// ������ Ŭ���̾�Ʈ ����

CGameTimer Timer;				// ���� Ÿ�̸�
float fStartTime = 0.f;			// ���� �ð�

STORE_DATA storeData; // Ŭ���̾�Ʈ ���� ����

// ���� ���� ����
PLAYER_INIT playerInit;
bool isGameStart = false;

// ���� �������� ����
bool ending = false;

// ���� �޽����� �����ϰ� �ֿܼ� ����ϴ� �Լ�
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	// WSAGetLastError()�� ���� ���� �ڵ忡 ���� �ý��� ���� �޽����� ������
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	// �ֿܼ� ���� �޽����� ���
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	// �������� �Ҵ�� �޸𸮸� ����
	LocalFree(lpMsgBuf);
}
// ������ ���Ͽ��� ������ ���̸�ŭ �����͸� �޾ƿ��� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		// �������κ��� �����͸� �޾ƿ�
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;  // ������ ������ ������ ���
		left -= received;
		ptr += received;
	}

	return (len - left);  // �޾ƿ� �������� �� ���� ��ȯ
}

// ���� ���� ī��Ʈ �ٿ�
void CountStart()
{
	fStartTime += Timer.GetTimeElapsed();  // ���� �����ӿ��� ���� �����ӱ��� �ð� 
	if (fStartTime >= START_TIME)
	{
		playerInit.start = true;
	}
}

// ���� ���μ��� ����
DWORD WINAPI ServerMain(LPVOID arg)
{
	Timer.Reset();
	while (true)
	{
		if (clientCount == 3)   // Ŭ�� 3���̸� ��ŸƮ
		{
			Timer.Tick(60.0f);
			// �ż��� ������ϴ� �� ���� �д�
			if (!playerInit.start)
				CountStart();
		}

	}
}

void RecvClientPos(SOCKET client_sock);

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	MyThread* pThread = (MyThread*)arg;

	int curIndex = pThread->iIndex; // ���� ������ �ε���,	[ 0 ~ 2 ]


	int retval;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(pThread->sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (1)
	{
		// 3���� �����ϸ� ���� ����
		if (!isGameStart)
		{
			if (clientCount < 3)
				continue;
			else
				isGameStart = true;
		}

		// �� �׷����� ----------------------
		if (pThread->iIndex >= 4) {
			break;
		}

		if (clientCount <= 2)
			WaitForSingleObject(clientEvent[waitClientIndex[curIndex]], INFINITE);

		// Ÿ�̸�, �÷��̾� ��ġ (���� ���� �� ���� ����)
		if (!SendPlayerInit(pThread->sock, curIndex))
		{
			SetEvent(clientEvent[curIndex]);
			break;
		}

		//�÷��̾� ������ �ޱ� (�۽� ���� �� ���� ����)
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
			// �ڽ��� �����ϴ� Ŭ���̾�Ʈ�� ã��
			if (waitClientIndex[i] == curIndex)
			{
				// �ڽ��� �����ϰ� �ִ� �ε����� �ٲ���
				waitClientIndex[i] = waitClientIndex[curIndex];
				// �ڽ��� �����ϰ� �ִ� �ε����� �ʱ�ȭ
				waitClientIndex[curIndex] = -1;
				// �� �̻� �˻��� �ʿ䰡 �����Ƿ� �ݺ����� ����
				break;
			}
		}
	}


	// �̺�Ʈ ����
	CloseHandle(clientEvent[curIndex]);

	// closesocket()
	closesocket(pThread->sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

// main
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
	//Ŭ���̾�Ʈ 3�� �������� �� 0�� 2��, 1�� 0��, 2�� 1��, �̺�Ʈ ��ٸ�
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
		++clientCount;						// ������ Ŭ���̾�Ʈ ��, [ 1 ~ 3 ]
		tThread.iIndex = clientCount - 1;	// ������ ���̵�,		 [ 0 ~ 2 ]

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

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

// ����
//void RecvClientPos(SOCKET client_sock) {
//	int px = 0;
//	int retval = recv(client_sock, (char*)&px, sizeof(int), MSG_WAITALL);
//	if (retval == SOCKET_ERROR) {
//		err_display("recv()");
//	}
//	printf("%d\n", px);

// �÷��̾� ���� ������ �޴� �Լ�
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


	// ���� ������ ��� Ȯ��
	//printf("[%d] (%f, %f)\n", curIndex, tPlayerInfo.tPos.fX, tPlayerInfo.pos.fY);

	if (clientCount == 3)    // Ŭ���̾�Ʈ 3���̸� ��ŸƮ
	{
		tPlayerInfo.start = true;
	}

	// ���� ���� ����
	ENDING::END_TYPE eEnding = ENDING::ING;

	if (ending)
	{
		eEnding = storeData.playersInfo[curIndex].eEnding;
	}

	storeData.playersInfo[curIndex] = tPlayerInfo;
	storeData.id = curIndex;

	// ���� ���� ����
	storeData.playersInfo[curIndex].eEnding = eEnding;

	storeData.hp[curIndex] = tPlayerInfo.hp;
	storeData.start = tPlayerInfo.start;

	// ���� ����
	CheckEnding(curIndex);

	// ������ ������
	retval = send(client_sock, (char*)&storeData, sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return FALSE;
	}

	return TRUE;
}

// �÷��̾� �ε����� ��ġ �����Լ�
void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend)
{
	switch (idx)
	{
	case 0:			// 1�� �÷��̾� ��ġ
		tPlayerInitSend.pos = { 100.f, 600.f };
		break;
	case 1:			// 2�� �÷��̾� ��ġ
		tPlayerInitSend.pos = { 400.f, 600.f };
		break;
	case 2:			// 3�� �÷��̾� ��ġ
		tPlayerInitSend.pos = { 700.f, 600.f };
		break;
	default:
		break;
	}
}

// �÷��̾� �ʱ� �����ϴ� �Լ�
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


// ���� ����
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


///////////////////////�浹 üũ (-ing)/////////////////////////////
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

	//�÷��̾� �ʺ�, ����
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

// ���⼭ �浹 üũ
void CheckCollision(int index)
{
	for (int i = 0; i < 3; ++i)
	{
		// �ڱ� �ڽ� �浹x
		if (index == i)
			continue;

		// �÷��̾� ���� ���¸� �浹x
		if (storeData.playersInfo[i].isDead)
			continue;

	}
}