#pragma once
namespace ShieldType
{

	enum ShieldType
	{
		None,
		ScreenDoor,
		Newspaper,
		Ladder,
	};

	extern const char* ToString(ShieldType zombieat2);

}

#define ZombieAccessoriesType2 ShieldType