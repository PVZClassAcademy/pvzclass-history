#include "Classes/Reanimator.hpp"

void PVZ::ReanimationParams::Reposition(DWORD baseaddress, DWORD num)
{
	if (PVZ::Memory::localExecute)
		PVZ::Memory::WriteArray<ReanimationParams>(baseaddress, (ReanimationParams*)0x6A1340, 143 * sizeof(ReanimationParams));
	else
	{
		ReanimationParams buf[143];
		PVZ::Memory::ReadArray<ReanimationParams>(0x6A1340, buf, 143 * sizeof(ReanimationParams));
		PVZ::Memory::WriteArray<ReanimationParams>(baseaddress, buf, 143 * sizeof(ReanimationParams));
	}

	PVZ::Memory::WriteMemory<int>(0x47377E, num);
	PVZ::Memory::WriteMemory<DWORD>(0x4737A5, baseaddress);
	PVZ::Memory::WriteMemory<DWORD>(0x4737F5, baseaddress + 4);

	PVZ::Memory::WriteMemory<int>(0x4814F3, num);
	PVZ::Memory::WriteMemory<int>(0x48153C, num);
}
