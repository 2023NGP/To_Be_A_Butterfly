#include "pch.h"
#include "Camera.h"

struct MyThread
{
	int iIndex = 0;
	SOCKET sock = 0;
};

HANDLE g_hClientEvent[CLIENT_COUNT];
int g_iWaitClientIndex[CLIENT_COUNT];
int g_iCurClientCount = 0; //������ Ŭ�� ����

CGameTimer m_GameTimer;
Camera* m_Camera;

bool SendCameraData(SOCKET sock);

// ��Ʈ ����
HpPotionInfo g_tHpPotionInfo;
float fPotionCreateTime = 0.f;
LONG iHpPotionIndex;
std::vector<POS> g_HeartPos;

// ���� ����
CoinInfo g_tCoinInfo;
float fCoinCreateTime = 0.f;
int iCoinIndex = 0;
std::vector<POS> g_CoinPos;

// ���� ����
int g_cloudCount = 0;
CLOUD g_Clouds[200];

// ���ӽ��� ����
PLAYER_INIT g_PlayerInit;
float fStartTime = 0.f;


STORE_DATA g_tStoreData;

bool isGameStart = false;
int clientEndCount = 0; // ���� ���� Ŭ���̾�Ʈ ��

// ���� ����
AttackData g_pAttackData[CLIENT_COUNT];

DWORD WINAPI ProcessClient(LPVOID arg);
DWORD WINAPI ServerMain(LPVOID arg);

//�÷��̾� ����
bool SendPlayerInit(SOCKET sock, int PlayerIndex);  // ���ӽ��ۿ��� ������
bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex);


// ��Ʈ ����
void CreateHpPotion();
bool SendRecv_HpPotionInfo(SOCKET sock);

// ���� ����
void CreateCoin();
bool SendRecv_CoinInfo(SOCKET sock, int index);

// ��Ʈ, ���� �� ������ �ҷ�����
void InitItem();

// ����
void InitCloud();
int SendCloudInfo(SOCKET sock);


void CountStart();
void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend);

CRITICAL_SECTION g_csHpPotion;
CRITICAL_SECTION g_csCoin;


bool g_isHit[CLIENT_COUNT] = { false };

// ����
bool g_bEnding = false;
void CheckEnding(int iCurIndex);

// mvp
int max = 0;
int maxIndex = 0;


void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void err_display(const char* msg)
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

//////////////


int main(int argc, char* argv[])
{
	InitializeCriticalSection(&g_csHpPotion);
	InitializeCriticalSection(&g_csCoin);

	srand(unsigned int(time(NULL)));

	// ServerMain ������
	CreateThread(NULL, 0, ServerMain, 0, 0, NULL);

	InitItem();
	InitCloud();

	m_Camera = new Camera;

	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;


	//�̺�Ʈ ����
	//Ŭ�� 3�� �������϶� 0�� 2��, 1�� 0��, 2�� 1�� �̺�Ʈ ��ٸ�
	for (int i = 0; i < CLIENT_COUNT; ++i)
	{
		g_hClientEvent[i] = CreateEvent(NULL, FALSE, (i < 2 ? FALSE : TRUE), NULL);
		g_iWaitClientIndex[i] = (i == 0) ? 2 : i - 1; // 2 0 1
	}


	MyThread tThread;
	tThread.iIndex = 0;
	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		int nagleopt = TRUE;
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nagleopt, sizeof(nagleopt));

		tThread.sock = client_sock;
		++g_iCurClientCount;
		++tThread.iIndex;

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, &tThread, 0, NULL);

		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// closesocket()
	closesocket(listen_sock);

	DeleteCriticalSection(&g_csHpPotion);
	DeleteCriticalSection(&g_csCoin);

	// ���� ����
	WSACleanup();
	return 0;
}




bool SendCameraData(SOCKET sock)
{
	m_Camera->Update(m_GameTimer.GetTimeElapsed(), g_PlayerInit.start);

	float cy = m_Camera->getLookAt().y;
	int retval = send(sock, (char*)&cy, sizeof(float), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return false;
	}
	return true;
}

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	MyThread* pThread = (MyThread*)arg;
	SOCKET client_sock = (SOCKET)pThread->sock;
	int iCurIndex = pThread->iIndex - 1; //���� ������ �ε���, �迭 �ε����� ����ؼ� -1

	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	retval = SendCloudInfo(client_sock);
	if (retval == SOCKET_ERROR)
	{
		err_display("sendcloudinfo()");
		return FALSE;
	}

	while (1)
	{
		if (!isGameStart)
		{
			if (g_iCurClientCount < CLIENT_COUNT)
				continue;
			else {
				isGameStart = true;
			}
		}

		if (g_iCurClientCount >= 2)
			WaitForSingleObject(g_hClientEvent[g_iWaitClientIndex[iCurIndex]], INFINITE);

		// Ÿ�̸�, �÷��̾� ��ġ, �� ������
		if (!SendPlayerInit(client_sock, iCurIndex))
		{
			SetEvent(g_hClientEvent[iCurIndex]);
			break;
		}

		//�÷��̾� ������ �ޱ�
		//////////////////////////////////////////////////////
		if (!SendRecv_PlayerInfo(client_sock, iCurIndex))
		{
			SetEvent(g_hClientEvent[iCurIndex]);
			break;
		}
		//////////////////////////////////////////////////////

		// ī�޶�

		if (!SendCameraData(client_sock)) {

			SetEvent(g_hClientEvent[iCurIndex]);
			break;
		}


		// ��Ʈ
		if (!SendRecv_HpPotionInfo(client_sock))
		{
			SetEvent(g_hClientEvent[iCurIndex]);
			break;
		}

		// ����
		if (!SendRecv_CoinInfo(client_sock, iCurIndex))
		{
			SetEvent(g_hClientEvent[iCurIndex]);
			break;
		}

		SetEvent(g_hClientEvent[iCurIndex]);
	}


	if (--g_iCurClientCount >= 2)
	{
		for (int i = 0; i < CLIENT_COUNT; ++i)
		{
			//�ڽ��� �����ϴ� Ŭ�� ã��
			if (g_iWaitClientIndex[i] == iCurIndex)
			{
				g_iWaitClientIndex[i] = g_iWaitClientIndex[iCurIndex]; //�ڽ��� �����ϰ��ִ� �ε����� �ٲ���
				g_iWaitClientIndex[iCurIndex] = -1;
				break;
			}
		}
	}


	CloseHandle(g_hClientEvent[iCurIndex]);

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;

}

// ���� ���μ��� ����
DWORD WINAPI ServerMain(LPVOID arg)
{
	m_GameTimer.Reset();

	while (true)
	{
		// 1. ��Ʈ/���� �ð��缭 ������
		if (g_iCurClientCount == CLIENT_COUNT)   // Ŭ�� 3���̸� ��ŸƮ
		{
			m_GameTimer.Tick(60.0f);
			if (!g_PlayerInit.start)
				CountStart();
			CreateHpPotion();
			CreateCoin();
		}

	}
}

bool SendRecv_PlayerInfo(SOCKET client_sock, int iIndex)
{
	int retval, iCurIndex = iIndex;

	PLAYER_INFO tPlayerInfo;
	retval = recvn(client_sock, (char*)&tPlayerInfo, sizeof(PLAYER_INFO), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	if (g_iCurClientCount == CLIENT_COUNT)    // Ŭ�� 3���̸� ��ŸƮ
	{
		tPlayerInfo.start = true;
	}
	

	g_tStoreData.tPlayersInfo[iCurIndex] = tPlayerInfo;
	g_tStoreData.iClientIndex = iCurIndex;


	g_tStoreData.iHp[iCurIndex] = tPlayerInfo.iHp;
	g_tStoreData.start = tPlayerInfo.start;
	g_tStoreData.team[iCurIndex] = TEAMNUM::TEAM1;

	g_tStoreData.tPlayersInfo[iCurIndex].isHit = g_isHit[iCurIndex];
	if (g_isHit[iCurIndex])
		g_isHit[iCurIndex] = false;

	// ���� ����
	if (!g_bEnding) {
		CheckEnding(iCurIndex);
	}

	// ������ ������
	retval = send(client_sock, (char*)&g_tStoreData, sizeof(STORE_DATA), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return FALSE;
	}

	return TRUE;
}

void CheckEnding(int iCurIndex)
{
	//DIE, LOSE, PASS, WIN, MVP

		// �׾��� �� DIE
	if (g_tStoreData.tPlayersInfo[iCurIndex].isDead)
	{
		g_tStoreData.tPlayersInfo[iCurIndex].eEnding = ENDING::LOSE;

		for (int i = 0; i < CLIENT_COUNT; i++)
		{
			if (!g_tStoreData.tPlayersInfo[i].isDead) {
				return;
			}
		}
		//g_bEnding = true;
		return;
	}
	if (g_tStoreData.tPlayersInfo[iCurIndex].eEnding == ENDING::PASS) { // PASS
		// Pass �� ������ ���
		for (int i = 0; i < CLIENT_COUNT; i++)
		{
			if (g_tStoreData.tPlayersInfo[i].eEnding == ENDING::ING) {

				return;
			}
		}

		//g_bEnding = true;

		for (int i = 0; i < CLIENT_COUNT; i++) {
			if (g_tStoreData.tPlayersInfo[i].coinNum >= max) {
				max = g_tStoreData.tPlayersInfo[i].coinNum;
				maxIndex = i;
			}
			g_tStoreData.tPlayersInfo[maxIndex].eEnding = ENDING::MVP;

			g_bEnding = true;
		}


	}


	g_bEnding = false;
}
void CreateHpPotion()
{
	fPotionCreateTime += m_GameTimer.GetTimeElapsed();

	if (fPotionCreateTime >= POTION_TIME && iHpPotionIndex < g_HeartPos.size())
	{
		EnterCriticalSection(&g_csHpPotion);

		fPotionCreateTime = 0.f;
		g_tHpPotionInfo.thpPotionCreate.cnt = 0;
		g_tHpPotionInfo.thpPotionCreate.bCreateOn = true;

		g_tHpPotionInfo.thpPotionCreate.pos.fX = g_HeartPos[iHpPotionIndex].fX;
		g_tHpPotionInfo.thpPotionCreate.pos.fY = g_HeartPos[iHpPotionIndex].fY;
		g_tHpPotionInfo.thpPotionCreate.index = iHpPotionIndex++;
		LeaveCriticalSection(&g_csHpPotion);

	}

}

bool SendRecv_HpPotionInfo(SOCKET sock)
{
	int retval;

	// ����ȭ ����
	// ���⼭ g_tHpPotionInfo�� �����ڿ�
	// ���� �ٸ� �����忡�� ���ÿ� �����ϹǷ� ��ü�� ����
	// Main�����嵵 ����ȭ�� �ؾ���

	EnterCriticalSection(&g_csHpPotion);

	// ��Ʈ ���� ������
	retval = send(sock, (char*)&g_tHpPotionInfo, sizeof(HpPotionInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		LeaveCriticalSection(&g_csHpPotion);

		return FALSE;
	}

	// [��Ʈ����] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
	if (g_tHpPotionInfo.thpPotionCreate.bCreateOn)
	{
		g_tHpPotionInfo.thpPotionCreate.cnt++;

		// ������ Ŭ�� ������ŭ ��Ʈ ���� �������� �ٽ� 0���� ����
		if (g_tHpPotionInfo.thpPotionCreate.cnt == g_iCurClientCount)
		{
			ZeroMemory(&g_tHpPotionInfo.thpPotionCreate, sizeof(HpPotionCreate));
		}
	}

	// [��Ʈ����] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
	if (g_tHpPotionInfo.thpPotionDelete.bDeleteOn)
	{
		g_tHpPotionInfo.thpPotionDelete.cnt++;

		// ������ Ŭ�� ������ŭ ��Ʈ ���� �������� �ٽ� 0���� ����
		if (g_tHpPotionInfo.thpPotionDelete.cnt == g_iCurClientCount)
		{
			ZeroMemory(&g_tHpPotionInfo.thpPotionDelete, sizeof(HpPotionDelete));
		}
	}
	LeaveCriticalSection(&g_csHpPotion);


	// ��Ʈ �浹 ���� �ޱ�
	POTIONRES tHpPotionRes;

	retval = recvn(sock, (char*)&tHpPotionRes, sizeof(POTIONRES), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// �浹�� ��� ó�� - �ʿ��� ���� �� �ٸ� Ŭ�� �˸���
	if (tHpPotionRes.bCollision)
	{
		// ���� Ŭ�� 1���� ���
		if (g_iCurClientCount == 1)
			return TRUE;

		g_tHpPotionInfo.thpPotionDelete.bDeleteOn = true;
		g_tHpPotionInfo.thpPotionDelete.cnt = 1;
		g_tHpPotionInfo.thpPotionDelete.index = tHpPotionRes.iIndex;
	}

	return TRUE;
}

void CreateCoin()
{
	fCoinCreateTime += m_GameTimer.GetTimeElapsed();

	if (fCoinCreateTime >= COIN_TIME && iCoinIndex < g_CoinPos.size())
	{
		EnterCriticalSection(&g_csCoin);

		fCoinCreateTime = 0.f;
		g_tCoinInfo.tCoinCreate.cnt = 0;
		g_tCoinInfo.tCoinCreate.bCreateOn = true;

		g_tCoinInfo.tCoinCreate.pos.fX = g_CoinPos[iCoinIndex].fX;
		g_tCoinInfo.tCoinCreate.pos.fY = g_CoinPos[iCoinIndex].fY;
		g_tCoinInfo.tCoinCreate.index = iCoinIndex++;
		//std::cout << iCoinIndex << '\t' << g_CoinPos.size() << '\n';
		//printf("���λ���\n");
		LeaveCriticalSection(&g_csCoin);
	}

}

bool SendRecv_CoinInfo(SOCKET sock, int index)
{
	int retval;

	// ����ȭ ����
	// ���⼭ g_tCoinInfo�� �����ڿ�
	// ���� �ٸ� �����忡�� ���ÿ� �����ϹǷ� ��ü�� ����
	// Main�����嵵 ����ȭ�� �ؾ���

	EnterCriticalSection(&g_csCoin);

	// ���� ���� ���� ������
	retval = send(sock, (char*)&g_tCoinInfo, sizeof(CoinInfo), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		LeaveCriticalSection(&g_csCoin);

		return FALSE;
	}

	// [���λ���] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
	if (g_tCoinInfo.tCoinCreate.bCreateOn)
	{
		g_tCoinInfo.tCoinCreate.cnt++;

		// ������ Ŭ�� ������ŭ ���� ���� �������� �ٽ� 0���� ����
		if (g_tCoinInfo.tCoinCreate.cnt == g_iCurClientCount)
		{
			ZeroMemory(&g_tCoinInfo.tCoinCreate, sizeof(CoinCreate));
		}
	}

	// [���λ���] �������ӵ� ��� Ŭ�� �������� ���� �ʱ�ȭ
	if (g_tCoinInfo.tCoinDelete.bDeleteOn)
	{
		g_tCoinInfo.tCoinDelete.cnt++;

		// ������ Ŭ�� ������ŭ ���� ���� �������� �ٽ� 0���� ����
		if (g_tCoinInfo.tCoinDelete.cnt == g_iCurClientCount)
		{
			ZeroMemory(&g_tCoinInfo.tCoinDelete, sizeof(CoinDelete));
		}
	}
	LeaveCriticalSection(&g_csCoin);


	// ���� �浹 ���� �ޱ�
	COINRES tCoinRes;

	retval = recvn(sock, (char*)&tCoinRes, sizeof(COINRES), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv()");
		return FALSE;
	}
	else if (retval == 0)
		return FALSE;

	// �浹�� ��� ó�� - �ʿ��� ���� �� �ٸ� Ŭ�� �˸���
	if (tCoinRes.bCollision)
	{
		// ���� Ŭ�� 1���� ���
		if (g_iCurClientCount == 1)
			return TRUE;

		g_tCoinInfo.tCoinDelete.bDeleteOn = true;
		g_tCoinInfo.tCoinDelete.cnt = 1;
		g_tCoinInfo.tCoinDelete.index = tCoinRes.iIndex;

		std::cout << index << "'s coin: " << g_tStoreData.tPlayersInfo[index].coinNum << std::endl;

	}

	return TRUE;
}

void InitItem()
{
	// ��Ʈ, ���� �� ������ �ҷ�����

	FILE* fp;
	fopen_s(&fp, "../Butterfly_Client/Image/mapdata/map2(heart).txt", "r");

	std::random_device rd;
	std::uniform_int_distribution <int> dis(0, 49);

	int i = 0;
	if (fp == NULL)//���� ������ ��
	{
		perror("fopen ����");//���� �޽��� ���
		return;
	}

	while (!feof(fp)) {
		int x, y, type;
		fscanf_s(fp, "%d %d %d", &x, &y, &type);

		if (type == 1) {            // ��Ʈ
			g_HeartPos.push_back({ });
			g_HeartPos.back().fX = x;
			g_HeartPos.back().fY = y;
		}
		else if (type == 2) {       // ����
			g_CoinPos.push_back({ });
			g_CoinPos.back().fX = x;
			g_CoinPos.back().fY = y;
		}
	}

	fclose(fp);
}

void InitCloud()
{
	FILE* fp;
	fopen_s(&fp, "../Butterfly_Client/Image/mapdata/map2.txt", "r");
	std::random_device rd;
	std::uniform_int_distribution <int> dis(0, 49);
	int i = 0;
	if (fp == NULL)
	{
		perror("fopen ����");
		return;
	}
	while (!feof(fp)) {
		int x, y, type;
		fscanf_s(fp, "%d %d %d", &x, &y, &type);

		g_Clouds[i].pos = { (float)x, (float)y };
		g_Clouds[i].type = type;
		++i;
	}
	g_cloudCount = i;
	fclose(fp);
}

int SendCloudInfo(SOCKET sock)
{
	int retval = 0;

	for (int i = 0; i < CLOUD_COUNT; ++i) {
		retval = send(sock, (char*)&g_Clouds[i], sizeof(CLOUD), 0);
		if (retval == SOCKET_ERROR) {
			break;
		}
		else if (retval == 0) {
			break;
		}
	}
	return retval;
}

bool SendPlayerInit(SOCKET sock, int PlayerIndex)
{
	int retval;

	g_PlayerInit.iCount = (int)fStartTime;
	PLAYER_INIT_SEND tPlayerInitSend;
	tPlayerInitSend.start = g_PlayerInit.start;
	tPlayerInitSend.idx = PlayerIndex;
	tPlayerInitSend.iCount = g_PlayerInit.iCount;

	if (g_PlayerInit.start)
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

void CountStart()
{
	fStartTime += m_GameTimer.GetTimeElapsed();  // ���� �����ӿ��� ���� �����ӱ��� �ð� 
	//std::cout << 6 - (int)fStartTime << std::endl;
	if (fStartTime >= START_TIME)
	{
		g_PlayerInit.start = true;
	}
}

void Get_InitPos(int idx, PLAYER_INIT_SEND& tPlayerInitSend)
{
	switch (idx)
	{
	case 0:
		tPlayerInitSend.tPos = { WINCX / 3, 600.f };
		break;
	case 1:
		tPlayerInitSend.tPos = { WINCX / 2, 600.f };
		break;
	case 2:
		tPlayerInitSend.tPos = { WINCX * 2 / 3, 600.f };
		break;
	default:
		break;
	}

	for (int i = 0; i < CLIENT_COUNT; ++i)
	{
		if (i == 0) {
			tPlayerInitSend.team[i] = TEAMNUM::TEAM1;
		}
		else if (i == 1) {
			tPlayerInitSend.team[i] = TEAMNUM::TEAM2;
		}
		else {
			tPlayerInitSend.team[i] = TEAMNUM::TEAM3;
		}
	}
}