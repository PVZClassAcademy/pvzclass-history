#include "Classes/SeedBank.hpp"

PVZ::CardSlot::SeedCard::SeedCard(int address)
{
	BaseAddress = address;
}

void PVZ::CardSlot::SeedCard::GetCollision(CollisionBox* collbox)
{
	collbox->X = NULL;
	collbox->Y = NULL;
	collbox->Width = Memory::ReadMemory<int>(BaseAddress + 0x10);
	collbox->Height = Memory::ReadMemory<int>(BaseAddress + 0x14);
}

void PVZ::CardSlot::SeedCard::SetCollision(CollisionBox* collbox)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x10, collbox->Width);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x14, collbox->Height);
}

void PVZ::CardSlot::SeedCard::EnterCoolDown(int duration)
{
	if (duration == -1)
	{
		SeedType::SeedType type = (SeedType::SeedType)ContentCard;
		if (type == SeedType::Imitater) type = (SeedType::SeedType)ContentCardImitative;
		duration = Memory::ReadMemory<int>(0x69F2C4 + 0x24 * type);
	}
	Enable = false;
	CoolDownInterval = duration;
	CoolDown = 0;
	Active = true;
}

void PVZ::CardSlot::SeedCard::Planted()
{
	Enable = false;
	CoolDown = 0;
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x488EC0)
		.ret()
	);
}

void PVZ::CardSlot::SeedCard::MouseDown()
{
	Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, PVZ::GetBoard().GetBaseAddress())
		.invoke(0x40CD80)
		.push(BaseAddress)
		.invoke(0x488590)
		.ret()
	);
}

AsmBuilder SetPacketType_builder = AsmBuilder();
void PVZ::CardSlot::SeedCard::SetPacketType(SeedType::SeedType type, SeedType::SeedType imitater_type)
{
	SetPacketType_builder.clear()
		.mov_reg_imm(REG_EDX, imitater_type)
		.mov_reg_imm(REG_EDI, type)
		.mov_reg_imm(REG_ESI, this->GetBaseAddress())
		.invoke(0x489B50)
		.ret();

	Memory::Execute(SetPacketType_builder);
}
