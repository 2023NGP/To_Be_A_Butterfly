#ifndef PCH_H
#define PCH_H

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <vector>
#include <list>
#include <map>

#include <time.h>

#include "Headers/Define.h"

#include "Headers/Include.h"
#include "Timer.h"

#endif //PCH_H
