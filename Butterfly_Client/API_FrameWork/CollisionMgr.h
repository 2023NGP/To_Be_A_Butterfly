#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
//	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
//	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);

	static void Collision_Shield(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Heart(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Coin(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Cloud(list<CObj*>& _Dst, list<CObj*>& _Src);

	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Ball(list<CObj*>& _Dst, list<CObj*>& _Src);

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);
	static bool Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y);

private:
	static int m_iGold_Idx;
};

#endif // !__COLLISIONMGR_H__

