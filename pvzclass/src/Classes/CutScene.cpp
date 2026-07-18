#include "Classes/CutScene.hpp"

PVZ::CutScene::CutScene(int address)
{
	BaseAddress = Memory::ReadMemory<int>(address + 0x15C);
}

void PVZ::CutScene::PlaceStreetZombie(ZombieType::ZombieType type, int x, int y)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(y)
		.push(x)
		.push(type)
		.push(BaseAddress)
		.invoke(0x439140)
		.ret()
	);
}

void PVZ::CutScene::PlaceStreetZombie(ZombieType::ZombieType type, bool grid[5][5])
{
	int address = 0;
	if (!PVZ::Memory::localExecute)
	{
		address = PVZ::Memory::AllocMemory(0, 25);
		for (int i = 0; i < 5; i++) PVZ::Memory::WriteArrayUnsafe<bool>(address + i * 5, grid[i], sizeof(grid[i]));
	}
	else
	{
		address = (int)grid;
	}
	PVZ::Memory::Execute(AsmBuilder()
		.push(address)
		.mov_reg_imm(REG_ESI, type)
		.mov_reg_imm(REG_EBX, BaseAddress)
		.invoke(0x439590)
		.ret()
	);
	if (!PVZ::Memory::localExecute)
	{
		for (int i = 0; i < 5; i++) PVZ::Memory::ReadArray<bool>(address + i * 5, grid[i], sizeof(grid[i]));
		PVZ::Memory::FreeMemory(address);
	}
}

void PVZ::CutScene::CancelIntro()
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(this->GetBaseAddress())
		.invoke(0x43B3C0)
		.ret()
	);
}