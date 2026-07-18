#include "../../PVZ.h"

double PVZ::PVZApp::__get_MusicVolume()
{
	return Memory::ReadMemory<double>(PVZ_BASE + 0xD0);
}

void PVZ::PVZApp::__set_MusicVolume(double value)
{
	Memory::WriteMemoryUnsafe<double>(PVZ_BASE + 0xD0, value);
	SETARG(__asm__set_MusicVolume, 1) = PVZ_BASE;
	Memory::Execute(STRING(__asm__set_MusicVolume));
}

double PVZ::PVZApp::__get_SoundFXVolume()
{
	return Memory::ReadMemory<double>(PVZ_BASE + 0xD8);
}

void PVZ::PVZApp::__set_SoundFXVolume(double value)
{
	Memory::WriteMemoryUnsafe<double>(PVZ_BASE + 0xD8, value);
	SETARG(__asm__set_MusicVolume, 1) = PVZ_BASE;
	Memory::Execute(STRING(__asm__set_MusicVolume));
}

PVZ::PVZApp PVZ::GetPVZApp()
{
	return PVZApp(PVZ::Memory::ReadMemory<DWORD>(0x6A9EC0));
}