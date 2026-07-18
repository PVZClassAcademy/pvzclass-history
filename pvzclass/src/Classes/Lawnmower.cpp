#include "Classes/LawnMower.hpp"

DWORD PVZ::LawnMower::MemSize = 0x48;

PVZ::LawnMower PVZ::LawnMower::GetByIndex(uint32_t index)
{
    return LawnMower(Memory::ReadMemory<int>(PVZ::GetBoard().GetBaseAddress() + 0x100) + index * MemSize);
}

PVZ::Animation PVZ::LawnMower::GetAnimation()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x1C);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

byte __asm__Lawnmower_Die[]
{
	MOV_EUX(REG_EAX, 0),
	LAWNMOWER_DIE,
	RET,
};

void PVZ::LawnMower::Die()
{
	SETARG(__asm__Lawnmower_Die, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Lawnmower_Die));
	return;
}

void PVZ::LawnMower::MowZombie(PVZ::Zombie zombie)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.push_imm32(zombie.GetBaseAddress())
		.invoke(0x458540)
		.ret()
	);
}

void PVZ::LawnMower::Squish()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x458EB0)
		.ret()
	);
}

void PVZ::LawnMower::Start()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, this->GetBaseAddress())
		.invoke(0x458DA0)
		.ret()
	);
}

void PVZ::LawnMower::Update()
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(this->GetBaseAddress())
		.invoke(0x4586E0)
		.ret()
	);
}

void PVZ::LawnMower::UpdatePool()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x4581E0)
		.ret()
	);
}