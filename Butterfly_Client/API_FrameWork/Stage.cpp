#include "stdafx.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Mouse.h"
#include "UI.h"
#include "KeyMgr.h"
#include "Potion.h"

CStage::CStage()
	:m_pPlayer(nullptr)
{
	m_lStartTime = GetTickCount64();

}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CObj* pObj1 = CAbstractFactory<CMouse>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MOUSE, pObj1);

	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	m_pPlayer->Set_Target(pObj1);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, m_pPlayer);

	CObj* pObj = CAbstractFactory<CUI>::Create();
	pObj->Set_Target(m_pPlayer);
	CObjMgr::Get_Instance()->Add_Object(OBJID::INGAME_UI, pObj);
	
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/WOL_TILE_DUNGEON.bmp", L"WOL_TILE_DUNGEON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/YAMMA.bmp", L"WOL_TILE_DUNGEON");
	
}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{
	CObjMgr::Get_Instance()->Render(_DC);
}

void CStage::Release()
{
}
void CStage::Add_Potion()
{
	// 랜덤하게 
	// 
	//  생성
	if (GetTickCount64() - m_lStartTime > 10000)
	{
		m_lStartTime = GetTickCount64();

		int iX = (rand() % 1000) + 50;
		int iY = (rand() % 500) + 50;

		CObj* pObj1 = CAbstractFactory<CPotion>::Create();
		pObj1->Set_Pos(iX, iY);
		CObjMgr::Get_Instance()->Add_Object(OBJID::HEART, pObj1);
	}
}
