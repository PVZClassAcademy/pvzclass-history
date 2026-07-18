#include "Classes/ZenGarden.hpp"

PVZ::ZenGarden PVZ::GetZenGarden()
{
	return ZenGarden(Memory::ReadPointer(0x6A9EC0, 0x81C));
}

PVZ::PVZApp PVZ::ZenGarden::GetLawnApp()
{
	return PVZ::PVZApp(Memory::ReadMemory<DWORD>(BaseAddress));
}

PVZ::Board PVZ::ZenGarden::GetBoard()
{
	return(PVZ::Board(Memory::ReadMemory<int>(BaseAddress + 4)));
}

byte __asm__ZenGarder_IsFull[]
{
	MOV_EUX(REG_ESI, 0),
	INVOKE_BYTE(0x51D740,0),
	MOV_PTR_ADDR_EAX(0),
	RET
};

bool PVZ::ZenGarden::IsFull(bool consider_items)
{
	SETARG(__asm__ZenGarder_IsFull, 1) = BaseAddress;
	__asm__ZenGarder_IsFull[3] = consider_items ? 1 : 0;
	SETARG(__asm__ZenGarder_IsFull, 21) = PVZ::Memory::Variable;
	return(Memory::Execute(STRING(__asm__ZenGarder_IsFull)) == 1);
}

byte __asm__ZenGarder_GetSnail[]
{
	MOV_EUX(REG_EBX, 0),
	INVOKE(0x520CF0),
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Snail PVZ::ZenGarden::GetSnail()
{
	if (PVZBASEADDRESS == 0)
		return(INVALID_BASEADDRESS);
	SETARG(__asm__ZenGarder_GetSnail, 1) = BaseAddress;
	SETARG(__asm__ZenGarder_GetSnail, 19) = PVZ::Memory::Variable;
	return(PVZ::Snail(Memory::Execute(STRING(__asm__ZenGarder_GetSnail))));
}
