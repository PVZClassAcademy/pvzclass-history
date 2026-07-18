#include "Classes/Music.hpp"

PVZ::Music PVZ::GetMusic()
{
	return Music(Memory::ReadPointer(0x6A9EC0, 0x83C));
}

MusicType::MusicType PVZ::Music::__get_Type()
{
	return Memory::ReadMemory<MusicType::MusicType>(BaseAddress + 8);
}

void PVZ::Music::__set_Type(MusicType::MusicType value)
{
	__asm__Music__set_Type[1] = value;
	SETARG(__asm__Music__set_Type, 6) = BaseAddress;
	Memory::Execute(STRING(__asm__Music__set_Type));
}

void PVZ::Music::StopAllMusic()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, BaseAddress)
		.invoke(0x45ABB0)
		.ret()
	);
}
