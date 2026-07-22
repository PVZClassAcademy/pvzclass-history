#include "Classes/TodParticleSystem.hpp"

PVZ::TodParticleSystem PVZ::TodParticleSystem::GetByIndex(uint32_t index)
{
	return PVZ::TodParticleSystem(Memory::ReadPointer(0x6A9EC0, 0x820, 0, 0) + index * 0x2C);
}

void PVZ::TodParticleSystem::Die()
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(this->BaseAddress)
		.invoke(0x5160C0)
		.ret()
	);
}

void PVZ::TodParticleSystem::MoveTo(float X, float Y)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_float(Y)
		.push_float(X)
		.mov_reg_imm(REG_ESI, this->BaseAddress)
		.invoke(0x518440)
		.ret()
	);
}

void PVZ::TodParticleSystem::OverrideColor(const char* emitter_name, Color& color)
{
	DWORD tmp_ptr;
	if (emitter_name == nullptr)
	{
		tmp_ptr = 0;
	}
	else
	{
		tmp_ptr = PVZ::Memory::Variable + 100;
		PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, emitter_name, std::strlen(emitter_name) + 1);
	}

	PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_EBX, tmp_ptr)
		.push(color.Alpha)
		.push(color.Blue)
		.push(color.Green)
		.push(color.Red)
		.push_reg(REG_ESP)
		.push(this->BaseAddress)
		.invoke(0x518560)
		.add_reg_imm(REG_ESP, 16)
		.ret()
	);
}

void PVZ::TodParticleSystem::OverrideExtraAdditiveDraw(bool isEnable)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDX, isEnable)
		.mov_reg_imm(REG_ESI, this->BaseAddress)
		.invoke(0x5185D0)
		.ret()
	);
}

void PVZ::TodParticleSystem::OverrideImage(Image image)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, image.GetBaseAddress())
		.mov_reg_imm(REG_EDX, this->BaseAddress)
		.invoke(0x518600)
		.ret()
	);
}

void PVZ::TodParticleSystem::OverrideScale(float scale)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_float(scale)
		.mov_reg_imm(REG_EDX, this->BaseAddress)
		.invoke(0x518630)
		.ret()
	);
}

PVZ::AttachEffect PVZ::TodParticleSystem::AttachTo(PVZ::AttachmentID attachmentID, float OffsetX, float OffsetY)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, this->BaseAddress)
		.mov_reg_imm(REG_EDI, attachmentID.GetBaseAddress())
		.push_float(OffsetY)
		.push_float(OffsetX)
		.invoke(0x405600)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.add_reg_imm(REG_ESP, 8)
		.ret()
	);
}

std::vector<PVZ::TodParticleSystem> PVZ::GetAllParticleSystem()
{
	std::vector<PVZ::TodParticleSystem> particle_syss;
	DWORD holder = PVZ::Memory::ReadPointer(0x6A9EC0, 0x820, 0);
	DWORD maxnum = Memory::ReadMemory<DWORD>(holder + 4), address = Memory::ReadMemory<DWORD>(holder);

	for (unsigned int i = 0; i < maxnum; i++)
	{
		PVZ::TodParticleSystem sys = PVZ::TodParticleSystem(address + i * 0x2C);
		if (!sys.Dead)
			particle_syss.push_back(sys);
	}

	return particle_syss;
}

PVZ::TodParticleSystem PVZ::CreateParticleSystem(float X, float Y, int render_order, EffectType::EffectType type)
{
	return PVZ::TodParticleSystem(PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, type)
		.mov_reg_imm(REG_ECX, render_order)
		.push_float(Y)
		.push_float(X)
		.mov_reg_imm(REG_EDX, PVZ_BASE)
		.invoke(0x453C80)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	));
}
