#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
// Precompiled Header
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <map>
#include <time.h>

#include "Headers/Define.h"
#include "Headers/Struct.h"

#include "Timer.h"

#endif //PCH_H
