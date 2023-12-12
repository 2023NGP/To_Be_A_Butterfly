#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#include "Potion.h"
#include "Coin.h"
#include "DataMgr.h"
#include "Cloud.h"

int CCollisionMgr::m_iGold_Idx = 0;

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Shield(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		if(true == Dst->Get_Arrow())  //활 불 만
		{ 
			for (auto& Src : _Src)
			{
				if (Check_Sphere(Dst, Src))
				{
					Dst->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Heart(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->Set_Hp(15);
				dynamic_cast<CPotion*>(Src)->CallBackCollision();
				Src->Set_Dead();

			}
		}
	}
}

void CCollisionMgr::Collision_Coin(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				dynamic_cast<CCoin*>(Src)->CallBackCollision();
				Src->Set_Dead();
				CDataMgr::Get_Instance()->m_tPlayerInfo.coinNum += 1;

			}
		}
	}
}

void CCollisionMgr::Collision_Cloud(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &Dst->Get_Rect(), &Src->Get_Rect()))
			{
				Dst->m_eCurState = CObj::STATE::HIT;
				Dst->m_bHit = true;
				Dst->hit_start_time = clock();
				dynamic_cast<CCloud*>(Src)->CallBackCollision();
				Dst->Set_Hp(-1);
				if (Dst->Get_Hp() <= 0) {
					CDataMgr::Get_Instance()->m_tPlayerInfo.isDead = true;
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				Src->Set_Collision(true);

				if (true == Dst->Get_Arrow()) //활 위자드불 공격 없어짐
					Dst->Set_Dead();
				//Dst->Set_Dead();
				//Src->Set_Dead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	float fRad = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCY) >> 1);

	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDis = sqrtf(fX * fX + fY * fY);

	return fRad > fDis;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCY + _Src->Get_Info().iCY) >> 1);

	if (fCX > fX && fCY > fY)
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		return true;
	}

	return false;
}
