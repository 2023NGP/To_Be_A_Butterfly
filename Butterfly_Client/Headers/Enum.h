#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN, END };
}

namespace OBJID
{
	enum ID { PLAYER, SHIELD, ATTACK, PRISON, HEART, COIN, EFFECT, INGAME_UI, UI, MOUSE, CLOUD, END };
}

namespace GROUPID
{
	enum ID { BACKGROUND, CLOUD, OBJECT, EFFECT, UI, END };
}

namespace ATTACK
{
	enum NAME { NORMAL, FIRE, ICE, BLAST, END };
}
namespace TEAMNUM
{
	enum TEAM { NONE, TEAM1, TEAM2, TEAM3, END };
}
namespace ENDING
{
	enum END_TYPE { ING, WIN, LOSE, PASS, END };
}
#endif // !__ENUM_H__
