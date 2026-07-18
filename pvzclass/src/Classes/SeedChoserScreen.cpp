#include "Classes/SeedChooserScreen.hpp"

PVZ::Board PVZ::SeedChooserScreen::GetBoard()
{
	return(PVZ::Board(Memory::ReadMemory<int>(BaseAddress + 0x0D14)));
}

PVZ::SeedChooserScreen::ChosenSeed PVZ::SeedChooserScreen::GetChosenSeed(int num)
{
	return(PVZ::SeedChooserScreen::ChosenSeed(BaseAddress + 0xA4 + num * 0x3C));
}

PVZ::SeedChooserScreen PVZ::GetSeedChooserScreen()
{
	int address = Memory::ReadPointer(0x6A9EC0, 0x774);
	return(address == 0 ? INVALID_BASEADDRESS : SeedChooserScreen(address));
}