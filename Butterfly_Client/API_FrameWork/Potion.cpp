#include "stdafx.h"
#include "Potion.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"


CPotion::CPotion()
{
}


CPotion::~CPotion()
{
	Release();
}

void CPotion::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/potion.bmp", L"POTION");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroup = GROUPID::OBJECT;
}

int CPotion::Update()
{
	// set_dead() 함수를 통해 m_bDead 변수가 true가 되고 OBJ_DEAD 이벤트 발생
	// obj_mgr에서 이벤트 받아서 삭제함.
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CPotion::Late_Update()
{
}

void CPotion::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"POTION");
	GdiTransparentBlt(_DC, m_tInfo.fX, m_tInfo.fY - g_CameraLookAt_Y,
		40, 40,
		hMemDC,
		0, 0,
		64, 64,
		RGB(255, 0, 255));
	// Rectangle(_DC, m_tRect.left + 0.f, m_tRect.top + 0.f, m_tRect.right + 0.f, m_tRect.bottom + 0.f);
}

void CPotion::Release()
{
}

void CPotion::CallBackCollision()
{
	// 서버에 알린다.
	g_tHeartRes.iIndex = m_lIndex;				// 현재 충돌한 하트의 인덱스
	g_tHeartRes.bCollision = true;				// 충돌 여부 true로 변환
}
