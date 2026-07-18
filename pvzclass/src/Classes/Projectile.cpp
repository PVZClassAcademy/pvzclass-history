#include "Classes/GameObject.hpp"

DWORD PVZ::Projectile::MemSize = 0x94;

PVZ::Projectile PVZ::Projectile::GetByIndex(uint32_t index)
{
	return PVZ::Projectile(Memory::ReadMemory<int>(PVZBASEADDRESS + 0x0C8) + index * MemSize);
}

void PVZ::Projectile::SetMemSize(int NewSize, int NewCount)
{
	if (NewSize < 0x94)
		return;
	MemSize = NewSize;

	Memory::WriteMemory<int>(0x407D05, NewSize * NewCount);

	byte __asm__Mem1[] = { ADD_EAX_DWORD(NewSize) };
	byte __asm__Mem2[] = { ADD_EUX_DWORD(REG_ESI, NewSize) };
	byte __asm__Mem3[] = { ADD_EUX_DWORD(REG_EDI, NewSize) };
	byte __asm__Mem4[] = { IMUL_EUX_EVX_DWORD(REG_EAX, REG_EAX, NewSize) };
	byte __asm__Mem5[] = { IMUL_EUX_EVX_DWORD(REG_ECX, REG_ECX, NewSize) };
	byte __asm__Mem6[] = { IMUL_EUX_EVX_DWORD(REG_ESI, REG_ESI, NewSize) };
	byte __asm__Mem7[] = { PUSHDWORD(NewSize - 4) };

	Memory::WriteArray<byte>(0x41BC28, STRING(__asm__Mem3));
	Memory::WriteArray<byte>(0x41BC3A, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41BC4E, STRING(__asm__Mem3));

	Memory::WriteArray<byte>(0x41C9C4, STRING(__asm__Mem1));
	Memory::WriteArray<byte>(0x41C9CF, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0x41C9E8, STRING(__asm__Mem1));

	Memory::WriteArray<byte>(0X41DF7C, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0X41DF8E, STRING(__asm__Mem6));
	Memory::WriteArray<byte>(0X41DF96, STRING(__asm__Mem7));

	Memory::WriteArray<byte>(0X41E60E, STRING(__asm__Mem2));
	Memory::WriteArray<byte>(0X41E617, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0X41E632, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0X41E63A, STRING(__asm__Mem2));
	Memory::WriteArray<byte>(0X481DA3, STRING(__asm__Mem1));

	Memory::WriteArray<byte>(0X481DAE, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0X481DC8, STRING(__asm__Mem1));
	Memory::WriteArray<byte>(0X48244C, STRING(__asm__Mem6));
}

byte __asm__Projectile_CheckForCollision[]
{
	PUSHDWORD(0),
	INVOKE(0x46CE80),
	RET,
};
void PVZ::Projectile::CheckForCollision()
{
	SETARG(__asm__Projectile_CheckForCollision, 1) = this->GetBaseAddress();
	Memory::Execute(__asm__Projectile_CheckForCollision, 19);
}

void PVZ::Projectile::ConvertToPea(int column)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EBX, column)
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.invoke(0x46EE00)
		.ret()
	);
}

void PVZ::Projectile::DoImpact(PVZ::Zombie zombie)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, zombie.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->BaseAddress)
		.invoke(0x46E000)
		.ret()
	);
}

void PVZ::Projectile::DoSplashDamage(PVZ::Zombie zombie)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(zombie.GetBaseAddress())
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.invoke(0x46D390)
		.ret()
	);
}

byte __asm__Projectile_GetProjectileRect[]
{
	MOV_ECX(0),
	MOV_ESI(0),
	INVOKE(0x46EBC0),
	RET,
};

PVZ::Rect PVZ::Projectile::GetProjectileRect()
{
	SETARG(__asm__Projectile_GetProjectileRect, 1) = PVZ::Memory::Variable;
	SETARG(__asm__Projectile_GetProjectileRect, 6) = this->GetBaseAddress();
	Memory::Execute(__asm__Projectile_GetProjectileRect, 24);
	return Rect
	{
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 4),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 8),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 12)
	};
}
byte __asm__OnFire[]
{
	MOV_ECX(0),
	ONFIRE,
	RET,
};

void PVZ::Projectile::OnFire()
{
	SETARG(__asm__OnFire, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__OnFire));
}

byte __asm__Projectile__Remove[]
{
	MOV_EAX(0),
	PROJECTILE_REMOVE,
	RET
};

void PVZ::Projectile::Remove()
{
	SETARG(__asm__Projectile__Remove, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Projectile__Remove));
	return;
}
