#include "stdafx.h"
#include "common.h"
#include "NetWork.h"

NetWork::NetWork()
{
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

	// ������ ��ſ� ����� ����
	char buf[1024];
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
		retval = send(sock, (char*)&len, sizeof(int), 0);
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
}

NetWork::~NetWork()
{
	closesocket(sock);
	// ���� ����
	WSACleanup();
}

void NetWork::SendClientPos()
{
	cout << "position ����" << endl;
}
void NetWork::RecvOtherClientPos()
{
}
