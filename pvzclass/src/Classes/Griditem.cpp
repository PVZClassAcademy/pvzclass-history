#include "../../PVZ.h"
#include "Classes/GameObject.hpp"
#include "Classes/Griditem.hpp"

DWORD PVZ::Griditem::MemSize = 0x0EC;

PVZ::Griditem PVZ::Griditem::GetByIndex(uint32_t index)
{
	return PVZ::Griditem(Memory::ReadMemory<uint32_t>(PVZ::GetBoard().GetBaseAddress() + 0x11C) + index * MemSize);
}

PVZ::Board PVZ::Griditem::GetBoard()
{
	return(PVZ::Board(Memory::ReadMemory<int>(BaseAddress + 4)));
}

byte __asm__Griditem__Remove[]
{
	MOV_EUX(REG_ESI, 0),
	INVOKE(0x44D000),
	RET
};

PVZ::Animation PVZ::Griditem::GetReanimation()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x34);
	return ((ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID)));
}

void PVZ::Griditem::SetReanimationn(Animation anim)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x34, anim.Id);
}

void PVZ::Griditem::Remove()
{
	SETARG(__asm__Griditem__Remove, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Griditem__Remove));
}

byte __asm__Vase_Open[]
{
	INVOKE_DWORD_DWORD(0x429AC0,0,0),
	RET
};

void PVZ::Vase::Open()
{
	SETARG(__asm__Vase_Open, 1) = BaseAddress;
	SETARG(__asm__Vase_Open, 6) = Memory::ReadPointer(BaseAddress + 4, 0x160);
	Memory::Execute(STRING(__asm__Vase_Open));
}

byte __asm__PortalClose[]
{
	MOV_ESI(0),
	INVOKE(0x44E360),
	RET
};

void PVZ::Portal::Close()
{
	SETARG(__asm__PortalClose, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__PortalClose));
}

bool PVZ::Portal::isZombieIn(PVZ::Zombie zombie)
{
	return (zombie.Row == Row) && (abs(Column * 80 - zombie.X) < 10);
}

int PVZ::Portal::getZombieOutX()
{
	return Column * 80 - 40;
}

bool PVZ::Portal::isProjectileIn(PVZ::Projectile projectile)
{
	return (projectile.Row == Row) && (abs(Column * 80 + 20 - projectile.X) < 20);
}

int PVZ::Portal::getProjectileOutX()
{
	return Column * 80 + 40;
}
