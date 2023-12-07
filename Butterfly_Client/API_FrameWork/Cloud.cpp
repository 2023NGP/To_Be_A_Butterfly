#include "stdafx.h"
#include "Cloud.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CCloud::CCloud()
{
	type = 3;
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/먹구름.bmp", L"THUNDERCLOUD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/비구름.bmp", L"RAINCLOUD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Cloud/일반구름.bmp", L"NORMALCLOUD");
	
	m_tInfo.iCX = 400;
	m_tInfo.iCY = 400;

	m_eGroup = GROUPID::OBJECT;
}

int CCloud::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CCloud::Late_Update()
{
}

void CCloud::Render(HDC _DC)
{
	//int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	//
	//if (type == 1) {
	//	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"THUNDERCLOUD");
	//	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
	//		m_tInfo.iCX, m_tInfo.iCY,
	//		hMemDC,
	//		0, 0,
	//		5120, 500,
	//		RGB(255, 255, 255));
	//}
	//else if (type == 2) {
	//	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"RAINCLOUD");
	//	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
	//		m_tInfo.iCX, m_tInfo.iCY,
	//		hMemDC,
	//		0, 0,
	//		5120, 500,
	//		RGB(255, 255, 255));
	//	
	//}
	//else if (type == 3) {
	//	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"NORMALCLOUD");
	//	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
	//		m_tInfo.iCX, m_tInfo.iCY,
	//		hMemDC,
	//		0, 0,
	//		5120, 500,
	//		RGB(255, 255, 255));
	//
	//}
	//
	//std::cout << m_lIndex << '\t';
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
