#include "Classes/SeedBank.hpp"

PVZ::CardSlot::CardSlot(int address)
{
	BaseAddress = Memory::ReadMemory<int>(address + 0x144);
}

int PVZ::CardSlot::__get_CardsCount()
{
	return Memory::ReadMemory<int>(BaseAddress + 0x24);
}

void PVZ::CardSlot::SetCardsCount(int num)
{
	Memory::WriteMemory<byte>(0x0041BEE0, 0xC3);
	SETARG(__asm__CardSlot__set_CardsCount, 1) = num;
	SETARG(__asm__CardSlot__set_CardsCount, 6) = this->GetBaseAddress();
	Memory::Execute(STRING(__asm__CardSlot__set_CardsCount));
	Memory::WriteMemory<byte>(0x0041BEE0, 0x56);
}

PVZ::CardSlot::SeedCard PVZ::CardSlot::GetCard(int index)
{
	if (index >= 0 && index < 10)
		return SeedCard(BaseAddress + 0x28 + index * 0x50);
	else
		return NULL;
}

void PVZ::CardSlot::AddSeed(SeedType::SeedType type)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(type)
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x489A50)
		.ret()
	);
}
