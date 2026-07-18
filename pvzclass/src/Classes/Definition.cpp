#include "Classes/Definition.hpp"

PVZ::PlantDefinition::PlantDefinition(SeedType::SeedType type)
{
	BaseAddress = 0x69F2B0 + 0x24 * type;
}

PVZ::PlantDefinition PVZ::GetPlantDefinition(SeedType::SeedType type)
{
	return PlantDefinition(type);
}

void PVZ::PlantDef::Reposition(DWORD baseaddress, uint8_t num)
{
	if (PVZ::Memory::localExecute)
		PVZ::Memory::WriteArray<PlantDef>(baseaddress, (PlantDef*)0x69F2B0, 53 * sizeof(PlantDef));
	else
	{
		PlantDef buf[53];
		PVZ::Memory::ReadArray<PlantDef>(0x69F2B0, buf, 53 * sizeof(PlantDef));
		PVZ::Memory::WriteArray<PlantDef>(baseaddress, buf, 53 * sizeof(PlantDef));
	}

	PVZ::Memory::WriteMemory<DWORD>(0x4024C0, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x45DCBF, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x45FF73, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x463F4A, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x465AFB, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x467C29, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x467C89, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x467DD3, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x4681E6, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x46F5F2, baseaddress);

	PVZ::Memory::WriteMemory<DWORD>(0x4662F8, baseaddress + 4);

	PVZ::Memory::WriteMemory<DWORD>(0x439D63, baseaddress + 8);
	PVZ::Memory::WriteMemory<DWORD>(0x43A049, baseaddress + 8);
	PVZ::Memory::WriteMemory<DWORD>(0x45FF69, baseaddress + 8);
	PVZ::Memory::WriteMemory<DWORD>(0x466290, baseaddress + 8);

	PVZ::Memory::WriteMemory<DWORD>(0x467B99, baseaddress + 16);
	PVZ::Memory::WriteMemory<DWORD>(0x467BA4, baseaddress + 16);

	PVZ::Memory::WriteMemory<DWORD>(0x467E4E, baseaddress + 20);
	PVZ::Memory::WriteMemory<DWORD>(0x467E59, baseaddress + 20);
	PVZ::Memory::WriteMemory<DWORD>(0x486DF6, baseaddress + 20);
	PVZ::Memory::WriteMemory<DWORD>(0x486E02, baseaddress + 20);
	PVZ::Memory::WriteMemory<DWORD>(0x488F68, baseaddress + 20);
	PVZ::Memory::WriteMemory<DWORD>(0x488F79, baseaddress + 20);

	PVZ::Memory::WriteMemory<DWORD>(0x488F79, baseaddress + 32);

	PVZ::Memory::WriteMemory<DWORD>(0x439D4F, baseaddress + 44);
	PVZ::Memory::WriteMemory<DWORD>(0x43A032, baseaddress + 44);
}

PVZ::ZombieDefinition::ZombieDefinition(ZombieType::ZombieType type)
{
	BaseAddress = 0x69DA80 + 0x1C * type;
}

PVZ::ZombieDefinition PVZ::GetZombieDefinition(ZombieType::ZombieType type)
{
	return ZombieDefinition(type);
}

PVZ::ProjectileDefinition::ProjectileDefinition(ProjectileType::ProjectileType type)
{
	BaseAddress = 0x69F1C0 + 0xC * type;
}

PVZ::ProjectileDefinition PVZ::GetProjectileDefinition(ProjectileType::ProjectileType type)
{
	return ProjectileDefinition(type);
}

PVZ::ChallengeDefinition::ChallengeDefinition(PVZLevel::PVZLevel mode)
{
	BaseAddress = 0x6A2B88 + 0x18 * mode;
}

PVZ::ChallengeDefinition PVZ::GetChallengeDefinition(PVZLevel::PVZLevel mode)
{
	return ChallengeDefinition(mode);
}

void PVZ::ChallengeDef::Reposition(DWORD baseaddress, uint8_t num)
{
	if (PVZ::Memory::localExecute)
		PVZ::Memory::WriteArray<ChallengeDef>(baseaddress, (ChallengeDef*)0x6A2BA0, 72 * sizeof(ChallengeDef));
	else
	{
		ChallengeDef buf[72];
		PVZ::Memory::ReadArray<ChallengeDef>(0x6A2BA0, buf, 72 * sizeof(ChallengeDef));
		PVZ::Memory::WriteArray<ChallengeDef>(baseaddress, buf, 72 * sizeof(ChallengeDef));
	}

	PVZ::Memory::WriteMemory<DWORD>(0x417F10, baseaddress - 0x18);
	PVZ::Memory::WriteMemory<DWORD>(0x417F38, baseaddress - 0x18);
	PVZ::Memory::WriteMemory<DWORD>(0x41802D, baseaddress - 0x18);
	PVZ::Memory::WriteMemory<DWORD>(0x43B0ED, baseaddress - 0x18);
	PVZ::Memory::WriteMemory<DWORD>(0x43B180, baseaddress - 0x18);
	PVZ::Memory::WriteMemory<DWORD>(0x457D0E, baseaddress - 0x18);

	PVZ::Memory::WriteMemory<DWORD>(0x417F09, baseaddress - 4);
	PVZ::Memory::WriteMemory<DWORD>(0x43B0E6, baseaddress - 4);
	PVZ::Memory::WriteMemory<DWORD>(0x45602E, baseaddress - 4);
	PVZ::Memory::WriteMemory<DWORD>(0x457D07, baseaddress - 4);

	PVZ::Memory::WriteMemory<DWORD>(0x42E3E4, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42E459, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42E515, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42E5AA, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42E8B6, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42E965, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x42F834, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x455BA6, baseaddress);

	PVZ::Memory::WriteMemory<DWORD>(0x42DF7A, baseaddress + 8);

	PVZ::Memory::WriteMemory<DWORD>(0x42E720, baseaddress + 0x20);

	PVZ::Memory::WriteMemory<DWORD>(0x42E425, baseaddress + num * sizeof(ChallengeDef));
	PVZ::Memory::WriteMemory<DWORD>(0x42E541, baseaddress + num * sizeof(ChallengeDef));
	PVZ::Memory::WriteMemory<DWORD>(0x42E5D1, baseaddress + num * sizeof(ChallengeDef));
	PVZ::Memory::WriteMemory<DWORD>(0x42F963, baseaddress + num * sizeof(ChallengeDef));
	PVZ::Memory::WriteMemory<DWORD>(0x455C08, baseaddress + num * sizeof(ChallengeDef));

	PVZ::Memory::WriteMemory<DWORD>(0x42E066, baseaddress + 8 + num * sizeof(ChallengeDef));

	PVZ::Memory::WriteMemory<DWORD>(0x42E7C9, baseaddress + 0x20 + num * sizeof(ChallengeDef));

	PVZ::Memory::WriteMemory<DWORD>(0x44FC47, 0x1E4 + 4 * num);

	PVZ::Memory::WriteMemory<DWORD>(0x42DF75, 0x1E4);
	PVZ::Memory::WriteMemory<DWORD>(0x42E2EB, 0x1E4);
	PVZ::Memory::WriteMemory<uint8_t>(0x42E2F0, num);
	PVZ::Memory::WriteMemory<DWORD>(0x42E957, 0x1E4);
	PVZ::Memory::WriteMemory<DWORD>(0x42F661, 0x1E4);
	PVZ::Memory::WriteMemory<uint8_t>(0x42F666, num);
	PVZ::Memory::WriteMemory<DWORD>(0x42F6D1, 0x1E4);
	PVZ::Memory::WriteMemory<uint8_t>(0x42F6D6, num);
	PVZ::Memory::WriteMemory<DWORD>(0x42E726, 0x1E8);
	PVZ::Memory::WriteMemory<DWORD>(0x42F826, 0x1E4);

	PVZ::Memory::WriteMemory<uint8_t>(0x42F4E1, num);
	PVZ::Memory::WriteMemory<uint8_t>(0x42F79D, num - 1);
}