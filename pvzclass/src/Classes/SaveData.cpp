#include "Classes/SaveData.hpp"

PVZ::SaveData PVZ::GetSaveData()
{
	return SaveData(Memory::ReadPointer(0x6A9EC0, 0x82C));
}

void PVZ::SaveData::GetPVZUserName(char str[])
{
	Memory::ReadArray<char>(BaseAddress + 4, str, 12);
}
int PVZ::SaveData::GetChallengeRecord(PVZLevel::PVZLevel mode)
{
	return Memory::ReadMemory<int>(BaseAddress + 0x30 + (mode - 1) * 4);
}

void PVZ::SaveData::SetChallengeRecord(PVZLevel::PVZLevel mode, int val)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x30 + (mode - 1) * 4, val);
}

int PVZ::SaveData::GetPurchase(StoreItem::StoreItem item)
{
	return Memory::ReadMemory<int>(BaseAddress + 0x1C0 + item * 4);
}

void PVZ::SaveData::SetPurchase(StoreItem::StoreItem item, int val)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x1C0 + item * 4, val);
}

BOOLEAN PVZ::SaveData::HavePurpleCard(SeedType::SeedType purplecard)
{
	if (purplecard >= SeedType::GatlingPea && purplecard <= SeedType::CobCannon)
		return Memory::ReadMemory<int>(BaseAddress + 0x1C0 + (purplecard - 40) * 4);
	return false;
}

PVZ::GardenPlant PVZ::SaveData::GetGardenPlant(int index)
{
	if (index >= 0 && index < GardenPlantCount)
		return GardenPlant(BaseAddress + 0x350 + index * 0x58);
	else
		return NULL;
}
