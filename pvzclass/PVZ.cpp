#include "PVZ.h"
#include "../include/Widgets/Dialog.hpp"

using std::optional;
using std::nullopt;

/*
	0x530: active 为1时阻塞游戏更新，执行完Execute之后改为0
	0x540: enable 为1时代表可以执行Execute
*/
byte __asm__Execute[]
{
	0xC6, 0x05, 0, 0, 0, 0, 0x01, // mov byte ptr [Variable+0x540], 1
	0x80, 0x3D, 0, 0, 0, 0, 0x00, // cmp byte ptr [Variable+0x530], 0
	0x75, 0xF7, // jne last line
	0xC6, 0x05, 0, 0, 0, 0, 0x00, // mov byte ptr [Variable+0x540], 0
	0x6A, 0xFF, 0x68, 0x35, 0xFA, 0x64, 0x00, // original code
	0xE9, 0, 0, 0, 0 // jmp 0x415D47
};

byte __asm__UpdateHook[]
{
	0xE9, 0, 0, 0, 0, 0x66, 0x90
};

byte __asm__Revert_UpdateHook[]
{
	PUSH(255),
	PUSHDWORD(0x64FA35)
};

namespace PVZ
{

	void InitPVZ(DWORD pid)
	{
		Memory::localExecute = false;
		Memory::processId = pid;
		Memory::hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		Memory::Variable = Memory::AllocMemory(4);
		Memory::StringVariable = Memory::AllocMemory(4);
		Memory::mainThreadId = Memory::ReadMemory<DWORD>(PVZ_BASE + 0x33C);
		Memory::hThread = OpenThread(THREAD_ALL_ACCESS, true, Memory::mainThreadId);

		Memory::immediateExecute = false;
		SETARG(__asm__Execute, 2) = Memory::Variable + 0x540;
		SETARG(__asm__Execute, 9) = Memory::Variable + 0x530;
		SETARG(__asm__Execute, 18) = Memory::Variable + 0x540;
		SETARG(__asm__Execute, 31) = 0x415D47 - Memory::Variable - 0x520 - 3;
		Memory::WriteArrayUnsafe<BYTE>(Memory::Variable + 0x500, STRING(__asm__Execute));
		SETARG(__asm__UpdateHook, 1) = Memory::Variable + 0x500 - 0x415D40 - 5;
		Memory::WriteArray<BYTE>(0x415D40, STRING(__asm__UpdateHook));
	}

	void QuitPVZ()
	{
		CloseHandle(Memory::hProcess);
		Memory::FreeMemory(Memory::Variable);
		Memory::WriteArray<BYTE>(0x415D40, STRING(__asm__Revert_UpdateHook));
	}
}

void PVZ::InitPVZDLL()
{
	PVZ::Memory::immediateExecute = true;
	PVZ::Memory::Variable = PVZ::Memory::AllocMemory(4);
	PVZ::Memory::StringVariable = PVZ::Memory::AllocMemory(4);
}

void PVZ::InitPVZNoLock(DWORD pid)
{
	Memory::localExecute = false;
	Memory::processId = pid;
	Memory::hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	Memory::Variable = Memory::AllocMemory(4);
	Memory::mainThreadId = Memory::ReadMemory<DWORD>(PVZ_BASE + 0x33C);
	Memory::hThread = OpenThread(THREAD_ALL_ACCESS, true, Memory::mainThreadId);
}

#pragma region background methods

const char* PVZ::PVZutil::__get_Version()
{
	return "3.0.260418-alpha-3";
}

PVZVersion::PVZVersion PVZ::PVZutil::__get_GameVersion()
{
	int ver = Memory::ReadMemory<int>(0x552013);
	switch (ver)
	{
	case 0xC35EDB74:
		return PVZVersion::V1_0_0_1051;
	case 0x86831977:
		return PVZVersion::V1_2_0_1065;
	case 0x3B000001:
		return PVZVersion::V1_2_0_1073;
	case 0x878B0000:
		return PVZVersion::CHINESE_ENHANCED;
	case 0xA48F:
		return PVZVersion::CHINESE_VISCOSITY;
	default:
		return PVZVersion::UnknowOrNotPVZ;
	}
}

#pragma endregion

#pragma region methods

bool PVZ::Memory::InjectDll(const char* dllname)
{
	SETARG(__asm__InjectDll, 1) = Variable + 0x600;
	SETARG(__asm__InjectDll, 19) = Variable;
	int len = strlen(dllname);
	WriteArrayUnsafe<const char>(Variable + 0x600, dllname, len);
	WriteMemoryUnsafe<char>(Variable + 0x600 + len, 0);
	DLLAddress = Execute(STRING(__asm__InjectDll));
	return DLLAddress != 0;
}

int PVZ::Memory::GetProcAddress(const char* procname)
{
	SETARG(__asm__GetProcAddress, 1) = Variable + 0x600;
	SETARG(__asm__GetProcAddress, 6) = DLLAddress;
	SETARG(__asm__GetProcAddress, 24) = Variable;
	int len = strlen(procname);
	WriteArrayUnsafe<const char>(Variable + 0x600, procname, len);
	WriteMemoryUnsafe<char>(Variable + 0x600 + len, 0);
	return Execute(STRING(__asm__GetProcAddress));
}

int PVZ::Memory::InvokeDllProc(const char* procname)
{
	int address = GetProcAddress(procname);
	if (address == 0) return -1;
	byte asmcode[] = { INVOKE(address), RET };
	return PVZ::Memory::Execute(STRING(asmcode));
}

void PVZ::ExtendRenderItem(uint32_t num)
{
	PVZ::Memory::WriteMemory<uint32_t>(0x416887, num * 12);
}

uint8_t __asm__MakeString[]
{
	PUSHDWORD(0),
	MOV_ECX(0),
	INVOKE(0x404450),
	RET
};

PVZ::PVZString PVZ::PVZString::Make(const char* str) 
{
	uint32_t len = strlen(str);
	uint32_t fromAddress = PVZ::Memory::AllocMemory(0, len + 1);
	PVZ::Memory::WriteArrayUnsafe<const char>(fromAddress, str, len + 1);
	uint32_t toAddress = PVZ::Memory::AllocMemory(0, 0x1C);

	SETARG(__asm__MakeString, 1) = fromAddress;
	SETARG(__asm__MakeString, 6) = toAddress;
	PVZ::Memory::Execute(STRING(__asm__MakeString));

	PVZ::Memory::FreeMemory(fromAddress);
	return PVZ::PVZString(toAddress);
}

PVZ::PVZString PVZ::PVZString::Translate(const char* str)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, str, std::strlen(str) + 1);

	return PVZ::PVZString{ PVZ::Memory::Execute(AsmBuilder()
		.push(0)

		.mov_reg_imm(REG_ECX, PVZ::Memory::Variable + 100)
		.mov_reg_reg(REG_ESI, REG_ESP)
		.invoke(0x5195D0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)

		.add_reg_imm(REG_ESP, 4)
		.ret()
	) };
}

void PVZ::PVZString::Free()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EBX, this->BaseAddress)
		.cmp_mem_RAI32_imm32(REG_EBX, 0x18, 16)
		.jb_rel(20)

		.mov_reg_mem_reg_add_imm(REG_EAX, REG_EBX, 4)
		.push_reg(REG_EAX)
		.invoke(0x61C19A)
		.add_reg_imm(REG_ESP, 4)

		.ret()
	);
	PVZ::Memory::FreeMemory(this->BaseAddress);
}

optional<int> PVZ::PVZString::ToInt(PVZ::PVZString str)
{
	bool tmp = PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(str.GetBaseAddress())
		.mov_reg_imm(REG_ESI, PVZ::Memory::Variable + 4)
		.invoke(0x5AFD80)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	) & 0x0FF;

	if (tmp)
		return PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 4);
	else
		return nullopt;
}

optional<double> PVZ::PVZString::ToDouble(PVZ::PVZString str)
{
	bool tmp = PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(str.GetBaseAddress())
		.mov_reg_imm(REG_EDI, PVZ::Memory::Variable + 4)
		.invoke(0x5B0050)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	) & 0x0FF;
	
	if (tmp)
		return PVZ::Memory::ReadMemory<double>(PVZ::Memory::Variable + 4);
	else
		return nullopt;
}

void PVZ::PVZString::Concat(const char* src, int len)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, src, len);
	PVZ::Memory::WriteMemoryUnsafe<char>(PVZ::Memory::Variable + 100 + len, '\0');

	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(len)
		.push_imm32(PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x41E3E0)
		.ret());
}

void PVZ::PVZString::Concat(PVZ::PVZString src)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(0xFFFFFFFF)
		.push(0)
		.push_imm32(src.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x41DC70)
		.ret());
}
void PVZ::PVZString::Assign(PVZString src, uint32_t len ,uint32_t count, uint32_t roff)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(count)
		.push_imm32(roff)
		.push_imm32(src.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x403E20)
		.ret());
}

void PVZ::PVZString::Assign(const char* src, uint32_t len, uint32_t count)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, src, len);
	PVZ::Memory::WriteMemoryUnsafe<char>(PVZ::Memory::Variable + 100 + len, '\0');

	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(count)
		.push_imm32(PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x404330)
		.ret());
}

void PVZ::PVZString::Assign(const char* src, uint32_t len)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, src, len);
	PVZ::Memory::WriteMemoryUnsafe<char>(PVZ::Memory::Variable + 100 + len, '\0');

	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x404300)
		.ret());
}

const char* PVZ::PVZString::c_str()
{
	uint32_t base_addr = this->GetBaseAddress();
	int size = PVZ::Memory::ReadMemory<int>(base_addr + 0x18);
	DWORD address = 0;
	if (size < 16) address = base_addr + 4;
	else address = PVZ::Memory::ReadMemory<DWORD>(base_addr + 4);
	int len = PVZ::Memory::ReadMemory<int>(base_addr + 0x14);
	char* result = new char[len + 1];
	PVZ::Memory::ReadArray<char>(address, result, len);
	result[len] = 0;
	return result;
}

byte __asm_KillGameSelector[] =
{
	MOV_ESI(0),
	INVOKE(0x44F9E0),
	RET
};

void PVZ::PVZApp::KillGameSelector()
{
	SETARG(__asm_KillGameSelector, 1) = BaseAddress;
	PVZ::Memory::Execute(STRING(__asm_KillGameSelector));
}

void PVZ::PVZApp::PreNewGame(PVZLevel::PVZLevel mode, bool look_for_saved_game)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(look_for_saved_game)
		.push_imm32(mode)
		.mov_reg_imm(REG_ESI, BaseAddress)
		.invoke(0x44F560)
		.ret()
	);
}

void PVZ::PVZApp::FastLoad(PVZLevel::PVZLevel mode)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(mode)
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x452C60)
		.ret()
	);
}

void PVZ::PVZApp::RemoveNotExistEffects()
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(EffectSystem)
		.invoke(0x445680)
		.ret()
	);
}

void PVZ::PVZApp::PlayFoley(PVZEnum::FoleyType type)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, type)
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x453630)
		.ret()
	);
}

bool PVZ::PVZApp::GetBoolean(PVZ::PVZString id, bool default_val)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.mov_reg_imm(REG_ECX, id.GetBaseAddress())
		.push_imm32(default_val)
		.invoke(0x552840)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	) & 0x0FF;
}

int PVZ::PVZApp::GetInteger(PVZ::PVZString id, int default_val)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.mov_reg_imm(REG_ECX, id.GetBaseAddress())
		.push_imm32(default_val)
		.invoke(0x5528B0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

PVZ::PVZString PVZ::PVZApp::GetString(PVZ::PVZString id, PVZ::PVZString default_val)
{
	return PVZ::PVZString{ PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.mov_reg_imm(REG_ESI, PVZ::Memory::Variable)
		.mov_reg_imm(REG_ECX, id.GetBaseAddress())
		.push_imm32(default_val.GetBaseAddress())
		.invoke(0x552920)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	) };
}

bool PVZ::PVZApp::LoadProperties(PVZ::PVZString file_name, bool check_sig)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(check_sig)
		.mov_reg_imm(REG_ECX, file_name.GetBaseAddress())
		.push_imm32(this->BaseAddress)
		.invoke(0x5524C0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	) & 0x0FF;
}

PVZ::LawnDialog PVZ::PVZApp::NewDialog(
	int						theDialogId,
	bool					isModal,
	PVZ::PVZString			theDialogHeader,
	PVZ::PVZString			theDialogLines,
	PVZ::PVZString			theDialogFooter,
	int						theButtonMode
	)
{
	return PVZ::LawnDialog{ PVZ::Memory::Execute(AsmBuilder128()
		.push_imm32(theButtonMode)
		.push_imm32(theDialogFooter.GetBaseAddress())
		.push_imm32(theDialogLines.GetBaseAddress())
		.push_imm32(theDialogHeader.GetBaseAddress())
		.push_imm32(isModal)
		.push_imm32(theDialogId)
		.mov_reg_imm(REG_ECX,this->GetBaseAddress())
		.invoke(0x451580)
		.mov_reg_imm(REG_EAX,PVZ::Memory::Variable)
		.ret()
	) };
}

#pragma endregion