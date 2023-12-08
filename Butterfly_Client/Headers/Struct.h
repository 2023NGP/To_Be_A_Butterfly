#pragma once
#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <cassert>

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;


typedef struct taPos
{
	float	fX;
	float	fY;
}POS;

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float	fX;
	float	fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _Left, LINEPOS& _Right) : tLeftPos(_Left), tRightPos(_Right) {}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;        
}FRAME;

/// ///////////////////////////////////////////
typedef struct tagFrameInfo
{
	int		iFrameStart; // 
	int		iFrameScene; // 
	int		iFrameKey;//  (PLAYER_DOWN)			

}FRAME_INFO;

typedef struct tagPlayerInit
{
	POS tPos[4] = { 0 };
	TEAMNUM::TEAM team[4];
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT;

typedef struct tagPlayerInitSend
{
	POS tPos = { 0 };
	TEAMNUM::TEAM team[4];
	bool start;
	int iCount = 0;
	int idx;
}PLAYER_INIT_SEND;


typedef struct tagPlayerInfo
{
	POS tPos = { 0 };
	FRAME_INFO	tFrameInfo;

	int iHp;

	TEAMNUM::TEAM team;
	bool start;
	bool button;

	bool				isHit = false; //충돌 판정
	bool				isDead = false; //죽었다 살아나면 false로 변환 필요 / True일때 충돌x
	ENDING::END_TYPE	eEnding = ENDING::ING;

	//FRAME tFrame;

}PLAYER_INFO;

//typedef struct tagSkillInfo
//{
//	std::vector<POS> vecFirePos;
//	bool isHit = false;
//}SKILL_INFO;

typedef struct tagStoreData
{
	PLAYER_INFO tPlayersInfo[4] = { 0 };
	//SKILL_INFO tFiresPos[4];
	int iClientIndex = 0;

	int iHp[4] = {};
	TEAMNUM::TEAM team[4];
	bool start;
	bool isDead;
}STORE_DATA;

//////////////////////////////////// 
struct HpPotionCreate
{
	int		cnt;			// 서버에서 사용, 보낸 클라이언트의 개수 판단할 때 사용함.
	int		index;			// 하트 생성 시, 하트의 인덱스
	bool	bCreateOn;		// 하트 생성 여부
	POS		pos;			// 하트 생성 위치
};

struct HpPotionDelete
{
	int		cnt; // 
	int		index; // 
	bool	bDeleteOn;
};

struct HpPotionInfo
{
	HpPotionCreate thpPotionCreate;			//  생성 시 정보
	HpPotionDelete thpPotionDelete;			// 삭제 시 정보
};

struct CoinCreate
{
	int		cnt; // 
	int		index; // 
	bool	bCreateOn;
	POS		pos;
};

struct CoinDelete
{
	int		cnt; // 
	int		index; // 
	bool	bDeleteOn;
};


struct CoinInfo
{
	CoinCreate tCoinCreate;
	CoinDelete tCoinDelete;
};

//struct HpPotion
typedef struct tagHpPotionRes
{
	bool	bCollision;				// 충돌 여부
	int		iIndex;					// 충돌된 객체의 인덱스
}POTIONRES;

//struct coin
typedef struct tagCoinRes
{
	bool	bCollision;
	int		iIndex;
}COINRES;

//// 구름 구조체
typedef struct Cloud {
	POS pos;
	int type;
}CLOUD;

typedef struct tagCloudRes {
	bool	bCollision;
	int		iIndex;
}CLOUDRES;

typedef struct tagAttackInfo
{
	size_t			iType = 0;			 
	INFO			tInfo;			 
	int				iFrameScene = 0;	 
	int				iFrameStart = 0;	 
	bool			bCollision = 0;

}ATTACKINFO;

struct AttackData
{
	int iSize = 0;
	ATTACKINFO* pAttackInfo = nullptr;
};



// vector

struct VECTOR2
{
	float x;
	float y;
public:
	float Length() {
		return sqrt(x * x + y * y);
	}
	VECTOR2& operator=(POINT _pt) {
		x = (float)_pt.x;
		y = (float)_pt.y;
	}
	VECTOR2& Normalize() {
		float fLen = Length();

		assert(fLen != 0.f);

		x /= fLen;
		y /= fLen;

		return *this;
	}
	VECTOR2 operator +(VECTOR2 _vOther) {
		return VECTOR2(x + _vOther.x, y + _vOther.y);
	}
	void operator +=(VECTOR2 _vOther) {
		x += _vOther.x;
		y += _vOther.y;
	}
	VECTOR2 operator -(VECTOR2 _vOther) {
		return VECTOR2(x - _vOther.x, y - _vOther.y);
	}
	VECTOR2 operator /(VECTOR2 _vOther) {
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));
		return VECTOR2(x / _vOther.x, y / _vOther.y);
	}
	VECTOR2 operator /(float _f) {
		assert(!(0.f == _f));
		return VECTOR2(x / _f, y / _f);
	}
	VECTOR2 operator *(VECTOR2 _vOther) {
		return VECTOR2(x * _vOther.x, y * _vOther.y);
	}
	VECTOR2 operator *(int _i) {
		return VECTOR2(x * (float)_i, y * (float)_i);
	}
	VECTOR2 operator *(float _f) {
		return VECTOR2(x * _f, y * _f);
	}
	VECTOR2() : x(0.f), y(0.f) {}
	VECTOR2(float _x, float _y) : x(_x), y(_y) {}
	VECTOR2(int _x, int _y) : x((float)_x), y((float)_y) {}
	VECTOR2(const POINT _pt) : x((float)_pt.x), y((float)_pt.y) {}

};



#endif // !__STRUCT_H__
