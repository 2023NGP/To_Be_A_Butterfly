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
	// set_dead() �Լ��� ���� m_bDead ������ true�� �ǰ� OBJ_DEAD �̺�Ʈ �߻�
	// obj_mgr���� �̺�Ʈ �޾Ƽ� ������.
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect_UI();

	return OBJ_NOEVENT;
}

void CPotion::Late_Update()
{
}

void CPotion::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"POTION");
	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hMemDC,
		0, 0,
		64, 64,
		RGB(255, 0, 255));
}

void CPotion::Release()
{
}

void CPotion::CallBackCollision()
{
	// ������ �˸���.
	g_tHpPotionRes.iIndex = m_lIndex;				// ���� �浹�� ��Ʈ�� �ε���
	g_tHpPotionRes.bCollision = true;				// �浹 ���� true�� ��ȯ
}
