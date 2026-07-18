#include "Classes/Mouse.hpp"

PVZ::MousePointer::MousePointer(int address) : GameObject()
{
	BaseAddress = Memory::ReadMemory<int>(address + 0x138);
}

PVZ::Animation PVZ::MousePointer::GetAnimation()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x48);
	return (((ID & 0xFFFF0000) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}
