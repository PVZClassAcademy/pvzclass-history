#include "../../PVZ.h"
#include "Classes/ChallengeScreen.hpp"

namespace PVZ
{
	ChallengeScreen GetChallengeScreen()
	{
		return ChallengeScreen(Memory::ReadPointer(0x6A9EC0, 0x780));
	}
	PVZApp ChallengeScreen::GetPVZApp()
	{
		return PVZApp(Memory::ReadMemory<DWORD>(BaseAddress + 0x1C0));
	}
	ChallengePage::ChallengePage ChallengeScreen::__get_Page()
	{
		return(Memory::ReadMemory<ChallengePage::ChallengePage>(BaseAddress + 0x1C8));
	}
	void ChallengeScreen::__set_Page(ChallengePage::ChallengePage value)
	{
		Memory::WriteMemoryUnsafe<ChallengePage::ChallengePage>(BaseAddress + 0x1C8, value);
		byte __asm__ChallScr_UpdateButtons[]
		{
			CHALLENGESCREEN_UPDATEBUTTONS(BaseAddress),
			RET
		};
		Memory::Execute(STRING(__asm__ChallScr_UpdateButtons));
	}
}