#include "Item.h"

void Item::SetType(int value)
{
	type = value;
}

int Item::GetType()
{
	return type;
}

void Item::SetIsGot(bool value)
{
	isGot = value;
}

bool Item::GetIsGot()
{
	return isGot;
}
