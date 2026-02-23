#include "ResourceManager.hpp"

namespace PVZ
{
	ResourceManager GetResourceManager()
	{
		return ResourceManager(Memory::ReadPointer(0x6A9EC0, 0x634));
	}
	PVZApp ResourceManager::GetPVZApp()
	{
		return PVZApp(Memory::ReadMemory<DWORD>(BaseAddress + 0x58));
	}
}

void PVZ::ResourceManager::AddPAKFile(const char* fileName)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, fileName, std::strlen(fileName) + 1);

	PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.push_imm32(PVZ::Memory::Variable + 100)
		.invoke(0x404450)
		.mov_reg_reg(REG_ECX, REG_EAX)
		.invoke(0x5D7D90)
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.invoke(0x404420)
		.ret()
	);
}

bool PVZ::ResourceManager::ParseResourcesFile(const char* fileName)
{
	this->AllowAlreadyDefinedResources = true;
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, fileName, std::strlen(fileName) + 1);

	return PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.push_imm32(PVZ::Memory::Variable + 100)
		.invoke(0x404450)

		.mov_reg_reg(REG_ECX, REG_EAX)
		.push_imm32(this->GetBaseAddress())
		.invoke(0x5B6A20)
		.and_reg_imm(REG_EAX, 0x0F)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)

		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.invoke(0x404420)
		.ret()
	);
}

bool PVZ::ResourceManager::TodLoadResources(const char* groupName)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, groupName, std::strlen(groupName) + 1);

	return PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.push_imm32(PVZ::Memory::Variable + 100)
		.invoke(0x404450)

		.push_reg(REG_EAX)
		.invoke(0x513120)
		.add_reg_imm(REG_ESP, 4)
		.and_reg_imm(REG_EAX, 0x0F)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)

		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.invoke(0x404420)
		.ret()
	);
}

PVZ::SoundID PVZ::ResourceManager::GetSoundThrow(const char* soundName)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, soundName, std::strlen(soundName) + 1);

	return (PVZ::SoundID)PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.push_imm32(PVZ::Memory::Variable + 100)
		.invoke(0x404450)

		.push_reg(REG_EAX)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x5B81F0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)

		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.invoke(0x404420)
		.ret()
	);
}

PVZ::Image PVZ::ResourceManager::GetImage(const char* imageName)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, imageName, std::strlen(imageName) + 1);

	return (PVZ::Image)PVZ::Memory::Execute(AsmBuilder128()
		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.push_imm32(PVZ::Memory::Variable + 100)
		.invoke(0x404450)

		.push_reg(REG_EAX)
		.push_imm32(PVZ::Memory::Variable + 4)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x5B8000)

		.mov_reg_reg(REG_ECX, REG_EAX)
		.invoke(0x59A980)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)

		.mov_reg_imm(REG_ESI, PVZ::Memory::Variable + 4)
		.invoke(0x59A8C0)

		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 600)
		.invoke(0x404420)
		.ret()
	);
}
