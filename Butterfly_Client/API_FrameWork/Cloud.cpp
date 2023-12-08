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
	
	m_tInfo.iCX = 200;
	m_tInfo.iCY = 200;

	m_eGroup = GROUPID::CLOUD;
}

int CCloud::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	frame_time += GetTickCount();
	if (frame_time > 1.f)
	{
		frame = (int)(frame + frame_time) % 10;
		frame_time = 0.f;
	}

	return OBJ_NOEVENT;
}

void CCloud::Late_Update()
{
}

void CCloud::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	
	if (i_type == 1) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"THUNDERCLOUD");
		GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			frame * 512, 0,
			512, 500,
			RGB(255, 255, 255));
	}
	else if (i_type == 2) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"RAINCLOUD");
		GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			frame * 512, 0,
			512, 500,
			RGB(255, 255, 255));
		
	}
	else if (i_type == 3) {
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"NORMALCLOUD");
		GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			frame * 512, 0,
			512, 500,
			RGB(255, 255, 255));
	}
	std::cout << m_lIndex << '\t';
}

void CCloud::Release()
{
}

void CCloud::CallBackCollision()
{
	// 서버에 알린다.
	//g_tCloudRes.iIndex = m_lIndex;
	//g_tCloudRes.bCollision = true;

}

