#include "stdafx.h"
#include "Coin.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CCoin::CCoin()
{
}


CCoin::~CCoin()
{
	Release();
}

void CCoin::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/coin.bmp", L"COIN");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroup = GROUPID::OBJECT;
}

int CCoin::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CCoin::Late_Update()
{
}

void CCoin::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"COIN");
	GdiTransparentBlt(_DC, m_tInfo.fX, m_tInfo.fY - g_CameraLookAt_Y,
		40, 40,
		hMemDC,
		0, 0,
		64, 64,
		RGB(255, 0, 255));

	// Rectangle(_DC, m_tRect.left + 0.f, m_tRect.top + 0.f, m_tRect.right + 0.f, m_tRect.bottom + 0.f);
}

void CCoin::Release()
{
}

void CCoin::CallBackCollision()
{
	// 서버에 알린다.
	g_tCoinRes.iIndex = m_lIndex;
	g_tCoinRes.bCollision = true;
}
