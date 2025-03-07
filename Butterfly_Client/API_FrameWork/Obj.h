#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, DIRECTION_END };
	enum STATE { IDLE, WALK, DASH, ATTACK, ATTACK2, HIT, DEAD, END };
public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;
	
public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_Dead() { m_bDead = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFrameKey = _pFrameKey; }
	void Set_Dir(DIRECTION dir) { m_eDir = dir; }
	void Set_FrameX(int framex) { m_iFrameX = framex; }
	void Set_FrameY(int framey) { m_iFrameY = framey; }
	void Set_Collision(bool col) { m_bCollision = col; }
	void Set_Mana(int mana) { m_iMana += mana; }
	void Set_Gold(int gold) { m_iGold += gold; }
	void Set_MaxHp() { m_iHp = m_iMaxHp; }
	void Set_Hp(int hp);

	void Set_Ultimate(bool ult) { m_bUltimate = ult; }
public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const FRAME& Get_Frame() const { return m_tFrame; }

	GROUPID::ID Get_GroupID() const { return m_eGroup; }

	const int& Get_Option() { return m_iOption; }
	const int& Get_Hp() { return m_iHp; }
	const int& Get_Mana() { return m_iMana; }
	const int& Get_Gold() { return m_iGold; }

	const bool Get_Arrow() { return m_bArrow; }
	const bool Get_Fire() { return m_bFire; }
	const bool Get_Ball() { return m_bBall; }
	const bool Get_Attack() { return m_bAttack; }

	STATE	m_ePreState;
	STATE	m_eCurState;

protected:
	void Update_Rect_UI();
	void Update_Rect();
	void Frame_Move();

protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	FRAME	m_tFrame;

	float	m_fSpeed;
	
	bool	m_bDead;
	float	m_fAngle;
	float	m_fDis;

	CObj*	m_pTarget;

	TCHAR*	m_pFrameKey;

	DIRECTION		m_eDir;

	int		m_iFrameX;
	int		m_iFrameY;

	DWORD	Attack_Time;
	bool	m_bAttack;

	int			m_iImageCX; //이미지 사이즈
	int			m_iImageCY;
	LONG		Image_Dif_X; 
	LONG		Image_Dif_Y;

	bool		m_bCollision;
	DWORD		Hit_Time;

	GROUPID::ID	m_eGroup;

	bool		m_bHit_GetTick;
	bool		m_bOne_Hit;

	bool		m_bSummon;
	DWORD		Summon_Time;

	int		m_iOption;//타일

	int		m_iHp;
	int		m_iMaxHp;
	int		m_iMana;
	int		m_iGold;

	bool	m_bArrow;//아쳐 활,위자드 불
	bool    m_bFire; //플레이어 불

	bool    m_bUltimate;

	bool    m_bBall;

	int		m_iHit_Idx;

	DWORD	m_dDead_Time;

public:
	size_t	m_iAttackName;
	bool m_bHit = false;
	float hit_start_time = 0.f;
};


#endif // !__OBJ_H__
