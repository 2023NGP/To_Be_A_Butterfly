#pragma once
class NetWork
{
private:
	char* SERVERIP = (char*)"127.0.0.1";
	int SERVERPORT = 9000;
	int BUFSIZE = 1024;

public:
	SOCKET sock;
	int retval;
	NetWork();
	~NetWork();

	void SendClientPos();
	void RecvOtherClientPos();

};

