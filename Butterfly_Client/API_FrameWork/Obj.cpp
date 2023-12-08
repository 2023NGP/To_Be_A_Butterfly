#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	: m_bDead(false), m_fSpeed(0.f), m_fAngle(0.f), m_fDis(0.f), m_pTarget(nullptr)
	, m_pFrameKey(L""), m_eDir(DIRECTION_END), m_iFrameX(0), m_iFrameY(0), m_bCollision(false)
	, m_eGroup(GROUPID::END), m_bHit_GetTick(false), m_bOne_Hit(false), m_bSummon(false), Summon_Time(NULL)
	, m_bAttack(false), m_iOption(0), m_iHp(0), m_iMaxHp(0), m_iMana(0), m_iGold(0), m_bArrow(false), m_bFire(false)
	, m_bUltimate(false), m_bBall(false), m_iHit_Idx(0)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}


CObj::~CObj()
{
}

void CObj::Set_Hp(int hp)
{
	if (m_iMaxHp >= m_iHp + hp)
		m_iHp += hp;
	else
		m_iHp = m_iMaxHp;

	if (m_iHp <= 0) {
		Set_Dead();
	}
}

void CObj::Update_Rect_UI()
{
	//m_tInfo.fY -= g_CameraLookAt_Y;

	m_tRect.left = (LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top =  (LONG)(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.right = (LONG)(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCY >> 1));
}
void CObj::Update_Rect()
{
	//m_tInfo.fY -= g_CameraLookAt_Y;

	if (Get_GroupID() == GROUPID::CLOUD)
	{
		m_tRect.left = (LONG)(m_tInfo.fX + 50.f - (m_tInfo.iCX >> 1));
		m_tRect.top = (LONG)(m_tInfo.fY + 100.f - (m_tInfo.iCY >> 1));
		m_tRect.right = (LONG)(m_tInfo.fX + 150.f + (m_tInfo.iCX >> 1));
		m_tRect.bottom = (LONG)(m_tInfo.fY + 100.f + (m_tInfo.iCY >> 1));
	}
	else {
		m_tRect.left = (LONG)(m_tInfo.fX + 15.f - (m_tInfo.iCX >> 1));
		m_tRect.top = (LONG)(m_tInfo.fY + 15.f - (m_tInfo.iCY >> 1));
		m_tRect.right = (LONG)(m_tInfo.fX + 15.f + (m_tInfo.iCX >> 1));
		m_tRect.bottom = (LONG)(m_tInfo.fY + 15.f + (m_tInfo.iCY >> 1));
	}


	m_tRect.top -= g_CameraLookAt_Y;
	m_tRect.bottom -= g_CameraLookAt_Y;
}
void CObj::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			//죽은 상태 마지막 프레임 고정
			if (m_eCurState == DEAD)
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
			else
				m_tFrame.iFrameStart = 0;
		}
	}
}
