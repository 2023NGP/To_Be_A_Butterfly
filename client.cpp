//#include "common.h"
//#include "client.h"
//#include "stdafx.h"
//
//char* SERVERIP = (char*)"192.168.219.104";
////char* SERVERIP;
//#define SERVERPORT 9000
//#define BUFSIZE 1024
//
//
//int main(int argc, char* argv[])
//{
//	// ����� �μ� IP �ּҷ� ���
//	//if (argc > 1) SERVERIP = (char*)argv[1];
//
//	// return value;
//	int retval;
//
//	// ���� �ʱ�ȭ
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		return 1;
//
//	// ���� ����
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket()");
//
//	// connect() : TCP�������� ���ؿ��� ������ ���� ������ ���� (bind() ���� ����, �ɵ���)
//	struct sockaddr_in serveraddr;
//	memset(&serveraddr, 0, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
//	serveraddr.sin_port = htons(SERVERPORT);
//	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//	if (retval == SOCKET_ERROR) err_quit("connect()");
//
//	// ���� �̸�(���� ����)
//	char filename[BUFSIZE];
//	strncpy(filename, "client1.mp4", BUFSIZE);
//
//	// ���� ����
//	FILE* file = fopen(filename, "rb");
//	if (!file) { printf("������ �� �� �����ϴ�.\n"); return 1; }
//
//
//	// ���ϸ� ũ�� ����
//	int filenameLen = (int)strlen(filename);
//	retval = send(sock, (char*)&filenameLen, sizeof(int), 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("send()");
//		fclose(file);
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	// ���ϸ� ����
//	retval = send(sock, filename, filenameLen, 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("send()");
//		fclose(file);
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//
//	// ���� �ݱ�
//	closesocket(sock);
//
//	// ���� ����
//	WSACleanup();
//	return 0;
//}
