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
//	// 명령행 인수 IP 주소로 사용
//	//if (argc > 1) SERVERIP = (char*)argv[1];
//
//	// return value;
//	int retval;
//
//	// 윈속 초기화
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//		return 1;
//
//	// 소켓 생성
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket()");
//
//	// connect() : TCP프로토콜 수준에서 서버와 논리적 연결을 설정 (bind() 역할 수행, 능동적)
//	struct sockaddr_in serveraddr;
//	memset(&serveraddr, 0, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
//	serveraddr.sin_port = htons(SERVERPORT);
//	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//	if (retval == SOCKET_ERROR) err_quit("connect()");
//
//	// 파일 이름(임의 지정)
//	char filename[BUFSIZE];
//	strncpy(filename, "client1.mp4", BUFSIZE);
//
//	// 파일 열기
//	FILE* file = fopen(filename, "rb");
//	if (!file) { printf("파일을 열 수 없습니다.\n"); return 1; }
//
//
//	// 파일명 크기 전송
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
//	// 파일명 전송
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
//	// 소켓 닫기
//	closesocket(sock);
//
//	// 윈속 종료
//	WSACleanup();
//	return 0;
//}
