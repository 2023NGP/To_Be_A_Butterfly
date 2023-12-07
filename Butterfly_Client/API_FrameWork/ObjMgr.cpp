#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Potion.h"
#include "Coin.h"
#include "CollisionMgr.h"
#include "DataMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		for (; iter != m_listObj[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}

	// collision
	CCollisionMgr::Collision_Heart(m_listObj[OBJID::PLAYER], m_listObj[OBJID::HEART]);
	CCollisionMgr::Collision_Coin(m_listObj[OBJID::PLAYER], m_listObj[OBJID::COIN]);

}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update();
			if (m_listObj[i].empty())
				break;
			GROUPID::ID eID = pObj->Get_GroupID();
			m_RenderGroup[eID].emplace_back(pObj);
		}
	}
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < GROUPID::END; ++i)
	{
		if (GROUPID::OBJECT == i)
			m_RenderGroup[i].sort(CompareY<CObj*>);

		for (auto& pObj : m_RenderGroup[i])
			pObj->Render(_DC);

		if (GROUPID::EFFECT == i)
		{
			// 다른 클라의 공격 렌더
			CDataMgr::Get_Instance()->RenderOthersAttack(_DC);
		}

		m_RenderGroup[i].clear();
	}

}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj* _pObj)
{
	m_listObj[_eID].emplace_back(_pObj);
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for (auto& pObj : m_listObj[_eID])
		Safe_Delete(pObj);
	m_listObj[_eID].clear();
}

CObj* CObjMgr::Get_Target(CObj* _pDst, OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;
	float fDis = 0.f;

	for (auto& _Src : m_listObj[_eID])
	{
		float fX = abs(_pDst->Get_Info().fX - _Src->Get_Info().fX);
		float fY = abs(_pDst->Get_Info().fY - _Src->Get_Info().fY);
		float fDis2 = sqrtf(fX * fX + fY * fY);

		if (fDis > fDis2 || !pTarget)
		{
			pTarget = _Src;
			fDis = fDis2;
		}
	}

	return pTarget;
}

bool CObjMgr::Get_AllDead(OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return true;
	else
		return false;
}

void CObjMgr::Delete_Potion(LONG index)
{
	for (auto& hpPotion : m_listObj[OBJID::HEART])
	{
		if (dynamic_cast<CPotion*>(hpPotion)->GetIndex() == index)
		{
			hpPotion->Set_Dead();
			break;
		}
	}
}

void CObjMgr::Delete_Coin(LONG index)
{
	for (auto& coin : m_listObj[OBJID::COIN])
	{
		if (dynamic_cast<CCoin*>(coin)->GetIndex() == index)
		{
			coin->Set_Dead();
			break;
		}
	}
}
