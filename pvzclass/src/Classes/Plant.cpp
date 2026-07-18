#include "Classes/GameObject.hpp"

DWORD PVZ::Plant::MemSize = 0x14C;

PVZ::Plant PVZ::Plant::GetByIndex(uint32_t index)
{
	return PVZ::Plant(Memory::ReadMemory<uint32_t>(PVZBASEADDRESS + 0x0AC) + index * MemSize);
}

void PVZ::Plant::SetMemSize(int NewSize = 0x14C, int NewCount = 1024)
{
	if (NewSize < 0x14C)
		return;
	MemSize = NewSize;
	Memory::WriteMemory<int>(0x401AAC, NewSize - 4);
	Memory::WriteMemory<int>(0x401C08, NewSize - 4);
	Memory::WriteMemory<int>(0x407CC2, NewSize * NewCount);
	Memory::WriteMemory<int>(0x4109F6, NewSize);
	Memory::WriteMemory<int>(0x41BAED, NewSize);
	Memory::WriteMemory<int>(0x41BAFF, NewSize);
	Memory::WriteMemory<int>(0x41BB1E, NewSize);
	Memory::WriteMemory<int>(0x41C965, NewSize);
	Memory::WriteMemory<int>(0x41C971, NewSize);
	Memory::WriteMemory<int>(0x41C989, NewSize);
	Memory::WriteMemory<int>(0x41DE9D, NewSize);
	Memory::WriteMemory<int>(0x41DEAF, NewSize);
	Memory::WriteMemory<int>(0x41DEB6, NewSize - 4);
	Memory::WriteMemory<int>(0x41DF11, NewSize);
	Memory::WriteMemory<int>(0x41E5AA, NewSize);
	Memory::WriteMemory<int>(0x41E5B3, NewSize);
	Memory::WriteMemory<int>(0x41E5CD, NewSize);
	Memory::WriteMemory<int>(0x41E5D5, NewSize);
	Memory::WriteMemory<int>(0x438ACD, NewSize);
	Memory::WriteMemory<int>(0x438F34, NewSize);
	Memory::WriteMemory<int>(0x481CFD, NewSize);
	Memory::WriteMemory<int>(0x481D09, NewSize);
	Memory::WriteMemory<int>(0x481D2E, NewSize);
	Memory::WriteMemory<int>(0x48235E, NewSize);
	Memory::WriteMemory<int>(0x524DB6, NewSize);
	Memory::WriteMemory<int>(0x52CBC0, NewSize);
	Memory::WriteMemory<int>(0x530433, NewSize);
	Memory::WriteMemory<int>(0x5304A5, NewSize);
}

PVZ::Animation PVZ::Plant::GetAnimationPart1()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x94);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationPart2()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x98);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationPart3()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x9C);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationPart4()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0xA0);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationEyeBlink()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0xA4);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationPotatoGlow()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0xA8);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

PVZ::Animation PVZ::Plant::GetAnimationSleep()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0xAC);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

void PVZ::Plant::Light(int cs)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0xB8, cs);
}

void PVZ::Plant::Flash(int cs)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0xBC, cs);
}

void PVZ::Plant::SetSleeping(bool sleeping)
{
	SETARG(__asm__Plant_SetSleeping, 1) = BaseAddress;
	__asm__Plant_SetSleeping[6] = sleeping;
	Memory::Execute(STRING(__asm__Plant_SetSleeping));
}

void PVZ::Plant::CreateEffect()
{
	this->DoSpecial();
}

void PVZ::Plant::DoSpecial()
{
	SETARG(__asm__CreateEffect, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__CreateEffect));
}

void PVZ::Plant::SetStatic()
{
	SETARG(__asm__SetStatic, 1) = PVZBASEADDRESS + 0x160;
	SETARG(__asm__SetStatic, 6) = BaseAddress;
	Memory::Execute(STRING(__asm__SetStatic));

}

void PVZ::Plant::Smash()
{
	SETARG(__asm__Smash, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Smash));
}

int PVZ::Plant::CalcLayer()
{
	SETARG(__asm__Plant_CalcLayer, 1) = BaseAddress;
	SETARG(__asm__Plant_CalcLayer, 19) = Memory::Variable;
	return(Memory::Execute(STRING(__asm__Plant_CalcLayer)));
}

void PVZ::Plant::MoveTo(int row, int column)
{
	Board tmp_board = this->GetBoard();
	this->Row = row;
	this->Column = column;
	this->ImageX = tmp_board.GridToXPixel(row, column);
	this->ImageY = tmp_board.GridToYPixel(row, column);
	this->Layer = this->CalcLayer();
}

void PVZ::Plant::Remove()
{
	SETARG(__asm__Plant__Remove, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Plant__Remove));
	return;
}
void PVZ::Plant::Update()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x463E40)
		.ret()
	);
}

PVZ::Projectile PVZ::Plant::Shoot(MotionType::MotionType motiontype, int targetid, bool special)
{
	if (special)
		__asm__Shoot[1] = 1;
	__asm__Shoot[3] = Row;
	if (targetid != -1)
	{
		Zombie tmp = PVZ::GetByID<Zombie>(targetid);
		if (!tmp.isValid())
			tmp = PVZ::Zombie((int)nullptr);
		SETARG(__asm__Shoot, 5) = tmp.GetBaseAddress();
	}
	SETARG(__asm__Shoot, 10) = BaseAddress;
	SETARG(__asm__Shoot, 29) = Memory::Variable;
	if (Type == SeedType::Starfruit)
	{
		Memory::Execute(STRING(__asm__Shoot));
		return NULL;
	};
	Projectile re = Projectile(Memory::Execute(STRING(__asm__Shoot)));
	if (motiontype != MotionType::None)
	{
		re.Motion = motiontype;
		if (motiontype == MotionType::Track && targetid != -1)
			re.TracktargetId = targetid;
	}
	return re;
}

void PVZ::Plant::PlayBodyReanim(const char* track_name, PVZEnum::ReanimLoopType loop_type, int blend_time, float anim_rate)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::StringVariable, track_name, std::strlen(track_name) + 1);

	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.mov_reg_imm(REG_ECX, blend_time)
		.push_float(anim_rate)
		.push_imm32(loop_type)
		.push_imm32(PVZ::Memory::StringVariable)
		.invoke(0x45FD90)
		.ret()
	);
}

AsmBuilder PlayIdleAnim_builder = AsmBuilder();
void PVZ::Plant::PlayIdleAnim(float speed)
{
	PlayIdleAnim_builder.clear()
		.push_float(speed)
		.mov_reg_imm(REG_EDI, GetBaseAddress())
		.invoke(0x468280)
		.ret();

	PVZ::Memory::Execute(PlayIdleAnim_builder);
}
void PVZ::Plant::UpdateBowling()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, GetBaseAddress())
		.invoke(0x462CE0)
		.ret()
	);
}

void PVZ::Plant::AnimateNuts()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, GetBaseAddress())
		.invoke(0x464480)
		.ret()
	);
}

PVZ::Rect PVZ::Plant::GetPlantRect()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, PVZ::Memory::Variable)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x467EF0)
		.ret()
	);
	auto rect = Rect{};
	rect.X = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 0);
	rect.Y = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 4);
	rect.Width = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 8);
	rect.Height = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 12);
	return rect;
}

PVZ::Plant::MagnetItem::MagnetItem(int address)
{
	BaseAddress = address;
};

PVZ::Plant::MagnetItem PVZ::Plant::GetMagnetItem(int num)
{
	return(PVZ::Plant::MagnetItem(BaseAddress + 0xC8 + num * 0x14));
}

int PVZ::Plant::GetCost(SeedType::SeedType type, SeedType::SeedType imitater_type)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDX, imitater_type)
		.mov_reg_imm(REG_EAX, type)
		.invoke(0x467B00)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}