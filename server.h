#pragma once
#include "stdafx.h"

SOCKET sock;

bool RecvInitData()
{
	char buf[BUFSIZE + 1];
	int retval = recv(sock, buf, BUFSIZE, 0);
	if (retval != -1) {
		return true;
	}
	return false;
}