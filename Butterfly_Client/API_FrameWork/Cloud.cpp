#include "stdafx.h"
#include "Cloud.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"



CCloud::CCloud()
{
	i_type = 3;
}

CCloud::CCloud(int type)
{
	type = type;
}


CCloud::~CCloud()
{
	Release();
}

void CCloud::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/dark.bmp", L"THUNDERCLOUD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/rain.bmp", L"RAINCLOUD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/normal.bmp", L"NORMALCLOUD");
	
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_eGroup = GROUPID::CLOUD;
}

int CCloud::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	// 게임 누적 시간 받아옴
	clock_t time = clock();
	double timeSecond = (double)(time / CLOCKS_PER_SEC);
	
	// 게임 누적 시간과 prev_time 계산해서 다를 때 frame 업데이트
	if (prev_time != timeSecond)
	{
		frame = (frame + 1) % 10;
		prev_time = timeSecond;
	}

	return OBJ_NOEVENT;
}

void CCloud::Late_Update()
{
}

void CCloud::Render(HDC _DC)
{
	if (i_type == 1) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"THUNDERCLOUD");
		GdiTransparentBlt(_DC, m_tInfo.fX, m_tInfo.fY - g_CameraLookAt_Y,
			200, 200,
			hMemDC,
			frame * 512, 0,
			512, 512,
			RGB(255, 255, 255));
	}
	else if (i_type == 2) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"RAINCLOUD");
		GdiTransparentBlt(_DC, m_tInfo.fX, m_tInfo.fY - g_CameraLookAt_Y,
			200, 200,
			hMemDC,
			frame * 512, 0,
			512, 512,
			RGB(255, 255, 255));
		
	}
	else if (i_type == 3) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"NORMALCLOUD");
		GdiTransparentBlt(_DC, m_tInfo.fX, m_tInfo.fY - g_CameraLookAt_Y,
			200, 200,
			hMemDC,
			frame * 512, 0,
			512, 512,
			RGB(255, 255, 255));
	}
	std::cout << m_lIndex << '\t';

	// Rectangle(_DC, m_tRect.left + 0.f, m_tRect.top + 0.f, m_tRect.right + 0.f, m_tRect.bottom + 0.f);
}

void CCloud::Release()
{
}

void CCloud::CallBackCollision()
{
	// 서버에 알린다.
	g_tCloudRes.iIndex = m_lIndex;
	g_tCloudRes.bCollision = true;

}

