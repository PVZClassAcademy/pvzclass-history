#include "Classes/GameObject.hpp"

DWORD PVZ::Coin::MemSize = 0x0D8;

PVZ::Coin PVZ::Coin::GetByIndex(uint32_t index)
{
    return PVZ::Coin(Memory::ReadMemory<uint32_t>(PVZBASEADDRESS + 0x0E4) + index * MemSize);
}

void PVZ::Coin::GetCollision(PVZ::Rect* collbox)
{
	collbox->X = NULL;
	collbox->Y = NULL;
	collbox->Width = Memory::ReadMemory<int>(BaseAddress + 0x10);
	collbox->Height = Memory::ReadMemory<int>(BaseAddress + 0x14);
}

void PVZ::Coin::SetCollision(PVZ::Rect* collbox)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x10, collbox->Width);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x14, collbox->Height);
}

PVZ::Attachment PVZ::Coin::GetAttachment()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x60);
	return (((ID & 0xFFFF0000) == 0) ? INVALID_BASEADDRESS : Attachment::GetByIndex(ID & 0x00FFFF));
}

PVZ::GardenPlant PVZ::Coin::GetGardenPlant()
{
	return PVZ::GardenPlant(this->BaseAddress + 0x68);
}

void PVZ::Coin::Collect()
{
	SETARG(__asm__Collect, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Collect));
}

byte __asm__Coin_die[] =
{
	MOV_ESI(0),
	INVOKE(0x432DD0),
	RET
};

void PVZ::Coin::Die()
{
	SETARG(__asm__Coin_die, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Coin_die));
}