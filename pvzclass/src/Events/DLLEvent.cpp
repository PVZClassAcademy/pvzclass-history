#include "Events/DLLEvent.h"

int DLLEvent::newAddress = 0;

void DLLEvent::start(BYTE* code, int newlen)
{
	if (newAddress == 0) newAddress = PVZ::Memory::AllocMemory(16, 0);
	rawCode = new BYTE[rawlen];
	PVZ::Memory::ReadArray<BYTE>(hookAddress, rawCode, rawlen);
	BYTE jmpto[] = { JMPFAR(newAddress - (hookAddress + 5)) };
	PVZ::Memory::WriteArray<BYTE>(hookAddress, jmpto, 5);
	for (int i = 5; i < rawlen; i++) PVZ::Memory::WriteMemory<BYTE>(hookAddress + i, NOP);

	BYTE jmpback[] = { JMPFAR(hookAddress - (newAddress + newlen + 7)) };
	PVZ::Memory::WriteMemoryUnsafe<BYTE>(newAddress, PUSHAD);
	PVZ::Memory::WriteArrayUnsafe<BYTE>(newAddress + 1, code, newlen);
	PVZ::Memory::WriteMemoryUnsafe<BYTE>(newAddress + newlen + 1, POPAD);
	PVZ::Memory::WriteArrayUnsafe<BYTE>(newAddress + newlen + 2, rawCode, rawlen);
	if (rawCode[0] == 0xE8 || rawCode[0] == 0xE9)
		PVZ::Memory::WriteMemoryUnsafe<uint32_t>(newAddress + newlen + 3, SETARG(rawCode, 1) + hookAddress - newAddress - newlen - 2);
	PVZ::Memory::WriteArrayUnsafe<BYTE>(newAddress + newlen + rawlen + 2, jmpback, 5);
	newAddress += newlen + rawlen + 0x10;
}

DLLEvent::~DLLEvent()
{
	if (rawCode != nullptr)
		delete[](rawCode);
}

void DLLEvent::end()
{
	PVZ::Memory::WriteArray<BYTE>(hookAddress, rawCode, rawlen);
	PVZ::Memory::FreeMemory(newAddress);
}
