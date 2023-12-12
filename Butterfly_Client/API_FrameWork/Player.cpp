#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Player.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

#include "DataMgr.h"

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_HPBAR.bmp", L"PLAYER_HPBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player.bmp", L"Player_DOWN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player.bmp", L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player.bmp", L"Player_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/player.bmp", L"Player_UP");

	/// <summary>
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Normal_ATTACK.bmp", L"Normal_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/SKILL_FIREDRAGON.bmp", L"SKILL_FIREDRAGON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/FIRE_PARTICLE.bmp", L"FIRE_PARTICLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Ice_ATTACK.bmp", L"Ice_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/ICE_BLAST.bmp", L"ICE_BLAST");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/ReviveTime.bmp", L"ReviveTime");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/GameStartTime.bmp", L"GameStartTime");

	// 클리어 + 오버
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/stageclear.bmp", L"WIN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/gameover.bmp", L"LOSE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/mvp.bmp", L"MVP");

	
	m_eCurState = IDLE;
	m_ePreState = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Player_DOWN";

	m_tInfo.fX = 600.f;
	m_tInfo.fY = 600.f;
	//m_tInfo.fX = 3100.f;
	//m_tInfo.fY = 600.f;

	m_tInfo.iCX = 30;  //렌더에서 바꿔줌
	m_tInfo.iCY = 80;

	m_iImageCX = 100;
	m_iImageCY = 100;
	Image_Dif_X = (m_tInfo.iCX >> 1) - (m_iImageCX >> 1);
	Image_Dif_Y = (m_tInfo.iCY >> 1) - (m_iImageCY >> 1);


	m_fSpeed = 5.f;

	m_fAngle = 0.f;
	m_fPoSize = 50.f;

	m_bJump = false;
	m_fJumpPower = 20.f;
	m_fJumpAccel = 0.f;

	m_bDash = false;
	m_eDir = DOWN;
	m_fDashSpeed = 10.f;

	m_bAttack = false;
	Attack_Scene = 4; //첨 공격시 3으로들감

	m_eGroup = GROUPID::OBJECT;

	m_bHit_GetTick = false;

	m_bBlast_GetMouse = false;
	m_Blast_TargetPalyer = false;

	m_bFireLeft = false;

	m_iHp = 150;
	m_iMaxHp = 150;
	m_iMana = 0;
	m_iGold = 0;

	m_bRight_On = true;
	m_bQ_On = true;
	m_bE_On = true;
	m_bR_On = true;

	m_bQ_Cool = false;
	m_bE_Cool = false;
	m_bR_Cool = false;

	m_iNormal_Idx = 0;
	m_iFire_Idx = 0;
	m_iBlast_Idx = 0;

	m_iRight_Remain = 8; 

	m_dDead_Time = 0.f;
}

int CPlayer::Update()
{
	Key_Check();
	OffSet();
	Scene_Change();
	Frame_Move();
	Hit();

	/// ////////////////////////////////////////////////
	CDataMgr::Get_Instance()->m_tPlayerInfo.tPos.fX = m_tInfo.fX;
	CDataMgr::Get_Instance()->m_tPlayerInfo.tPos.fY = m_tInfo.fY;
	CDataMgr::Get_Instance()->m_tPlayerInfo.tFrameInfo.iFrameStart = m_tFrame.iFrameStart;
	CDataMgr::Get_Instance()->m_tPlayerInfo.tFrameInfo.iFrameScene= m_tFrame.iFrameScene;
	CDataMgr::Get_Instance()->m_tPlayerInfo.tFrameInfo.iFrameKey = CDataMgr::Get_Instance()->SetFrameKey(m_pFrameKey);
	CDataMgr::Get_Instance()->m_tPlayerInfo.iHp = m_iHp;
	//CDataMgr::Get_Instance()->m_tPlayerInfo.tFrame = m_tFrame;

	/// ////////////////////////////////////////////////

	if (g_tPlayerInit.start && !m_bStart)
	{
		Set_Pos(g_tPlayerInit.tPos.fX, g_tPlayerInit.tPos.fY);
		m_bStart = true;
	}

	Update_Rect_UI();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	float   fX = 0.f, fY = 0.f, fDis = 0.f, fRad = 0.f;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	fX = (m_pTarget->Get_Info().fX - iScrollX) - m_tInfo.fX;
	fY = (m_pTarget->Get_Info().fY - iScrollY) - m_tInfo.fY;
	fDis = sqrtf(fX * fX + fY * fY);
	fRad = acosf(fX / fDis);
	int index = CDataMgr::Get_Instance()->m_tStoreData.iClientIndex;

	m_fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < (m_pTarget->Get_Info().fY - iScrollY))
		m_fAngle *= -1.f;

	m_tPosin.x = (LONG)(m_tInfo.fX + cosf(m_fAngle * PI / 180.f) * m_fPoSize);
	m_tPosin.y = (LONG)(m_tInfo.fY - sinf(m_fAngle * PI / 180.f) * m_fPoSize);

	m_tPoLeft.x = (LONG)(m_tInfo.fX + cosf((m_fAngle + 25) * PI / 180.f) * m_fPoSize);
	m_tPoLeft.y = (LONG)(m_tInfo.fY - sinf((m_fAngle + 25) * PI / 180.f) * m_fPoSize);

	m_tPoRight.x = (LONG)(m_tInfo.fX + cosf((m_fAngle - 25) * PI / 180.f) * m_fPoSize);
	m_tPoRight.y = (LONG)(m_tInfo.fY - sinf((m_fAngle - 25) * PI / 180.f) * m_fPoSize);

	// 맵 안에서만 움직이도록
	if (m_tInfo.fX < MAP_EDGE)
		m_tInfo.fX = MAP_EDGE;
	if (m_tInfo.fX > WINCX - MAP_EDGE)
		m_tInfo.fX = WINCX - MAP_EDGE;
	//if (m_tInfo.fY < MAP_EDGE)
	//   m_tInfo.fY = MAP_EDGE;
	// 화면 밖을 벗어나면 죽음
	if (m_tInfo.fY > WINCY + MAP_EDGE)
		CDataMgr::Get_Instance()->m_tPlayerInfo.isDead = true;
	if (m_tInfo.fY <= MAP_EDGE)
		CDataMgr::Get_Instance()->m_tPlayerInfo.eEnding = ENDING::PASS;
	// 여기서 화면 끝에 닿았을 때 클리어 조건 걸어주면 될 것 같다
}

void CPlayer::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Player_DOWN");

	// Rectangle(_DC, m_tRect.left + 0.f, m_tRect.top + 0.f, m_tRect.right + 0.f, m_tRect.bottom + 0.f);

	GdiTransparentBlt(_DC
		, m_tRect.left + Image_Dif_X, m_tRect.top + Image_Dif_Y
		, CHAR_CX, CHAR_CY
		, hMemDC
		, m_tFrame.iFrameStart * 192, 0		//시작좌표
		, 192, 182							//길이
		, RGB(255, 255, 255));

	for (int i = 0; i < CLIENT_COUNT; ++i)
	{
		STORE_DATA tStoreData = CDataMgr::Get_Instance()->m_tStoreData;

		if (i != tStoreData.iClientIndex)
		{
			PLAYER_INFO tPlayerInfo = tStoreData.tPlayersInfo[i];
			RECT	tRect;
			tRect.left = (LONG)(tPlayerInfo.tPos.fX - (m_tInfo.iCX >> 1));
			tRect.top = (LONG)(tPlayerInfo.tPos.fY - (m_tInfo.iCY >> 1));

			hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(CDataMgr::Get_Instance()->GetFrameKey(tPlayerInfo.tFrameInfo.iFrameKey));

			GdiTransparentBlt(_DC
				, tRect.left + Image_Dif_X, tRect.top + Image_Dif_Y
				, CHAR_CX, CHAR_CY
				, hMemDC
				, tPlayerInfo.tFrameInfo.iFrameStart * 192, 0
				, 192, 182													
				, RGB(255, 255, 255));
		}
	}
	int iClientIndex = CDataMgr::Get_Instance()->m_tStoreData.iClientIndex;
	if (CDataMgr::Get_Instance()->m_tStoreData.tPlayersInfo[iClientIndex].eEnding == ENDING::PASS)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"WIN");

		GdiTransparentBlt(_DC
			, 0, 0
			, WINCX, WINCY
			, hMemDC
			, 0, 0
			, WINCX, WINCY
			, RGB(255, 255, 255));

		return; // 부활UI 출력 안하도록
	}
	else if (CDataMgr::Get_Instance()->m_tStoreData.tPlayersInfo[iClientIndex].eEnding == ENDING::LOSE)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"LOSE");

		GdiTransparentBlt(_DC
			, 0, 0
			, WINCX, WINCY
			, hMemDC
			, 0, 0
			, WINCX, WINCY
			, RGB(255, 255, 255));

		return; // 부활UI 출력 안하도록
	}
	else if (CDataMgr::Get_Instance()->m_tStoreData.tPlayersInfo[iClientIndex].eEnding == ENDING::MVP)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"MVP");

		GdiTransparentBlt(_DC
			, 0, 0
			, WINCX, WINCY
			, hMemDC
			, 0, 0
			, WINCX, WINCY
			, RGB(255, 255, 255));

		return; // 부활UI 출력 안하도록
	}
	//if (CDataMgr::Get_Instance()->m_tPlayerInfo.isDead)
	//{
	//	int leftTime = (m_dwDaedTime + REVIVE_TIME - GetTickCount())/1000;
	//	hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ReviveTime");

	//	GdiTransparentBlt(_DC
	//		, WINCX/2 - 200, WINCY / 2 - 300
	//		, 400, 600
	//		, hMemDC
	//		, leftTime * 200, 0
	//		, 200, 300
	//		, RGB(255, 255, 255));
	//}
}


void CPlayer::Release()
{
}

void CPlayer::Key_Check()
{
	//죽었을때 키입력 안받음
	if (CDataMgr::Get_Instance()->m_tPlayerInfo.isDead)
		return;
	

	if (g_tPlayerInit.start == true)
	{
		Move();
	}

	Update_Rect_UI();
}

void CPlayer::OffSet()
{
	//int iOffset = WINCX >> 1;
	//float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	//if (iOffset > m_tInfo.fX + fScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffset - (m_tInfo.fX + fScrollX));
	//if (iOffset < m_tInfo.fX + fScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffset - (m_tInfo.fX + fScrollX));



	//iOffset = WINCY >> 1;
	//float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	//if (iOffset > m_tInfo.fY + fScrollY)
	//	CScrollMgr::Get_Instance()->Set_ScrollY(iOffset - (m_tInfo.fY + fScrollY));
	//if (iOffset < m_tInfo.fY + fScrollY)
	//	CScrollMgr::Get_Instance()->Set_ScrollY(iOffset - (m_tInfo.fY + fScrollY));

}

void CPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::HIT:
			m_tFrame.iFrameStart = 10;
			m_tFrame.iFrameEnd = 13;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Hit()
{
	if (m_bHit) {
		hit_progress_time += clock();
		if ((double)(hit_progress_time / CLOCKS_PER_SEC) - (double)(hit_start_time / CLOCKS_PER_SEC) > 1) {
			m_bHit = false;
			hit_progress_time = 0.f;
			hit_start_time = 0.f;
		}
	}
}

void CPlayer::Move()
{
	////////////////////////////////이동
	if (false == m_bDead && false == m_bHit && false == m_bAttack)
	{
		if (GetAsyncKeyState('A'))
		{
			if (GetAsyncKeyState('W'))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				m_eDir = LEFT_UP;
				m_pFrameKey = L"Player_UP";

			}
			else if (GetAsyncKeyState('S'))
			{
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				m_eDir = LEFT_DOWN;
				m_pFrameKey = L"Player_DOWN";
			}
			else
			{
				m_tInfo.fX -= m_fSpeed;
				m_eDir = LEFT;
				m_pFrameKey = L"Player_LEFT";
			}
			m_eCurState = WALK;
		}
		else if (GetAsyncKeyState('D'))
		{
			if (GetAsyncKeyState('W'))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				m_eDir = RIGHT_UP;
				m_pFrameKey = L"Player_UP";
			}
			else if (GetAsyncKeyState('S'))
			{
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				m_eDir = RIGHT_DOWN;
				m_pFrameKey = L"Player_DOWN";
			}
			else
			{
				m_tInfo.fX += m_fSpeed;
				m_eDir = RIGHT;
				m_pFrameKey = L"Player_RIGHT";
			}
			m_eCurState = WALK;
		}
		else if (GetAsyncKeyState('W'))
		{
			m_tInfo.fY -= m_fSpeed;
			m_pFrameKey = L"Player_UP";
			m_eCurState = WALK;
			m_eDir = UP;
		}
		else if (GetAsyncKeyState('S'))
		{
			m_tInfo.fY += m_fSpeed;
			m_pFrameKey = L"Player_DOWN";
			m_eCurState = WALK;
			m_eDir = DOWN;
		}
		else
			m_eCurState = IDLE;
	}
}
