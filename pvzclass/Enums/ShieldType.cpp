#include "ShieldType.h"

const char* ShieldType::ToString(ShieldType zombieat2)
{
	switch (zombieat2)
	{
	case ShieldType::None:
		return "ZombieAccessoriesType2None";
	case ShieldType::ScreenDoor:
		return "ScreenDoor";
	case ShieldType::Newspaper:
		return "Newspaper";
	case ShieldType::Ladder:
		return "Ladder";
	default:
		return "InValid_Value_Of_ZombieAccessoriesType2";
	}
}
