#include "GameObject.hpp"

using std::max;

DWORD PVZ::Zombie::MemSize = 0x15C;

PVZ::Zombie PVZ::Zombie::GetByIndex(uint32_t index)
{
	return PVZ::Zombie(Memory::ReadMemory<uint32_t>(PVZBASEADDRESS + 0x90) + index * MemSize);
}

void PVZ::Zombie::SetMemSize(int NewSize, int NewCount)
{
	if (NewSize < 0x15C)
		return;
	MemSize = NewSize;

	Memory::WriteMemory<int>(0x407CAA, NewSize * NewCount);

	byte __asm__Mem1[] = { ADD_EAX_DWORD(NewSize) };
	byte __asm__Mem2[] = { ADD_EUX_DWORD(REG_ESI, NewSize) };
	byte __asm__Mem3[] = { ADD_EUX_DWORD(REG_EDI, NewSize) };
	byte __asm__Mem4[] = { IMUL_EUX_EVX_DWORD(REG_EAX, REG_EAX, NewSize) };
	byte __asm__Mem5[] = { IMUL_EUX_EVX_DWORD(REG_ECX, REG_ECX, NewSize) };
	byte __asm__Mem6[] = { IMUL_EUX_EVX_DWORD(REG_EDX, REG_EDX, NewSize) };
	byte __asm__Mem7[] = { IMUL_EUX_EVX_DWORD(REG_ESI, REG_ESI, NewSize) };
	byte __asm__Mem8[] = { IMUL_EUX_EVX_DWORD(REG_EDI, REG_EDI, NewSize) };
	byte __asm__Mem9[] = { PUSHDWORD(NewSize - 4) };
	byte __asm__MemA[] = { 0x8D, 0xB5, INUMBER(NewSize) };

	Memory::WriteArray<byte>(0x41BB64, STRING(__asm__Mem3));
	Memory::WriteArray<byte>(0x41BB76, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41BB8A, STRING(__asm__Mem3));
	Memory::WriteArray<byte>(0x41C802, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41C904, STRING(__asm__Mem1));
	Memory::WriteArray<byte>(0x41C90F, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0x41C928, STRING(__asm__Mem1));
	Memory::WriteArray<byte>(0x41DDBB, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41DDCD, STRING(__asm__Mem8));
	Memory::WriteArray<byte>(0x41DDD5, STRING(__asm__Mem9));
	Memory::WriteArray<byte>(0x41DE2F, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41E4DE, STRING(__asm__Mem2));
	Memory::WriteArray<byte>(0x41E4E7, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x41E502, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0x41E50A, STRING(__asm__Mem2));
	Memory::WriteArray<byte>(0x4601B8, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x4602F6, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x4679E5, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x46D8E4, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x481D52, STRING(__asm__MemA));
	Memory::WriteArray<byte>(0x481D5E, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x481D78, STRING(__asm__Mem2));
	Memory::WriteArray<byte>(0x48226C, STRING(__asm__Mem7));
	Memory::WriteArray<byte>(0x527FB6, STRING(__asm__Mem8));
	Memory::WriteArray<byte>(0x52809A, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x5289AF, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x528A76, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x52C71B, STRING(__asm__Mem6));
	Memory::WriteArray<byte>(0x52CB27, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x52EF5E, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x52EFA2, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x52FAC9, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x52FB1C, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x530345, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x53037B, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0x5303C4, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x5303EF, STRING(__asm__Mem5));
	Memory::WriteArray<byte>(0x5304DE, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x5346CF, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x5352E8, STRING(__asm__Mem4));
	Memory::WriteArray<byte>(0x535364, STRING(__asm__Mem4));
}

FLOAT PVZ::Zombie::__get_Speed()
{
	return(Memory::ReadMemory<float>(BaseAddress + 0x34));
}

void PVZ::Zombie::SetSpeed(float speed)
{
	Memory::WriteMemoryUnsafe<float>(BaseAddress + 0x34, speed);
	SETARG(__asm__Zombie__UpdateSpeed, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Zombie__UpdateSpeed));
}

void PVZ::Zombie::Light(int cs)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x54, cs);
}

void PVZ::Zombie::GetCollision(CollisionBox* collbox)
{
	collbox->X = Memory::ReadMemory<int>(BaseAddress + 0x8C);
	collbox->Y = Memory::ReadMemory<int>(BaseAddress + 0x90);
	collbox->Width = Memory::ReadMemory<int>(BaseAddress + 0x94);
	collbox->Height = Memory::ReadMemory<int>(BaseAddress + 0x98);
}

void PVZ::Zombie::SetCollision(CollisionBox* collbox)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x8C, collbox->X);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x90, collbox->Y);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x94, collbox->Width);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x98, collbox->Height);
}

void PVZ::Zombie::GetAttackCollision(CollisionBox* collbox)
{
	collbox->X = Memory::ReadMemory<int>(BaseAddress + 0x9C);
	collbox->Y = Memory::ReadMemory<int>(BaseAddress + 0xA0);
	collbox->Width = Memory::ReadMemory<int>(BaseAddress + 0xA4);
	collbox->Height = Memory::ReadMemory<int>(BaseAddress + 0xA8);
}

void PVZ::Zombie::SetAttackCollision(CollisionBox* collbox)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x9C, collbox->X);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0xA0, collbox->Y);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0xA4, collbox->Width);
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0xA8, collbox->Height);
}

AsmBuilder ShowDoorArms_builder = AsmBuilder();
void PVZ::Zombie::ShowDoorArms(bool shown)
{
	ShowDoorArms_builder.clear()
		.push(shown ? 1 : 0)
		.push_imm32(this->GetBaseAddress())
		.invoke(0x524210)
		.ret();

	PVZ::Memory::Execute(ShowDoorArms_builder);
}

void PVZ::Zombie::AttachShield()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x533000)
		.ret()
	);
}

PVZ::Animation PVZ::Zombie::GetAnimation()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x118);
	return (ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : PVZ::Animation::GetByIndex(ID_INDEX(ID));
}

PVZ::Animation PVZ::Zombie::GetSpecialHeadAnimation()
{
	int ID = Memory::ReadMemory<int>(BaseAddress + 0x144);
	return (ID_RANK(ID) == 0) ? INVALID_BASEADDRESS : Animation::GetByIndex(ID_INDEX(ID));
}

void PVZ::Zombie::SetSpecialHeadAnimation(PVZ::Animation anim)
{
	Memory::WriteMemoryUnsafe<int>(BaseAddress + 0x144, anim.Id);
}

void PVZ::Zombie::Hit(int damage, DamageFlags flags)
{
	SETARG(__asm__Hit, 1) = BaseAddress;
	SETARG(__asm__Hit, 6) = flags;
	SETARG(__asm__Hit, 11) = damage;
	Memory::Execute(STRING(__asm__Hit));
}

void PVZ::Zombie::HitBody(int damage, DamageFlags flags)
{
	SETARG(__asm__HitBody, 1) = flags;
	SETARG(__asm__HitBody, 6) = damage;
	SETARG(__asm__HitBody, 11) = BaseAddress;
	Memory::Execute(STRING(__asm__HitBody));
}

void PVZ::Zombie::Blast()
{
	SETARG(__asm__Blast, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Blast));
}

byte __asm__Butter[]
{
	MOV_EAX(0),
	BUTTER,
	RET
};

void PVZ::Zombie::Butter(int countdown = 400)
{
	int temp = FixedCountdown;
	SETARG(__asm__Butter, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Butter));
	FixedCountdown = max(temp, countdown);
}

byte __asm__Decelerate[]
{
	MOV_EAX(0),
	DECELERATE,
	RET
};

void PVZ::Zombie::Decelerate(int countdown = 1000)
{
	int temp = DecelerateCountdown;
	SETARG(__asm__Decelerate, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Decelerate));
	DecelerateCountdown = max(temp, countdown);
}

void PVZ::Zombie::DropHelm(DamageFlags damage_flags)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(damage_flags)
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x530E30)
		.ret()
	);
}

void PVZ::Zombie::DetachShield()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x5330E0)
		.ret()
	);
}

void PVZ::Zombie::DropShield(DamageFlags damage_flags)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(damage_flags)
		.push_imm32(this->GetBaseAddress())
		.invoke(0x530A00)
		.ret()
	);
}

void PVZ::Zombie::EatPlant(PVZ::Plant plant)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX, plant.GetBaseAddress())
		.push_imm32(this->GetBaseAddress())
		.invoke(0x52FB40)
		.ret()
	);
}

void PVZ::Zombie::EatZombie(PVZ::Zombie zombie)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, zombie.GetBaseAddress())
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x52FE10)
		.ret()
	);
}

byte __asm__Froze[]
{
	MOV_EAX(0),
	FROZE,
	RET
};

void PVZ::Zombie::Froze(int countdown = 300)
{
	if (countdown == 0)
		return;

	Memory::WriteMemory<byte>(0x532493, 0);
	Memory::WriteMemory<byte>(0x5319E5, 112);//无视魅惑
	int temp = FrozenCountdown, temp2 = DecelerateCountdown;
	SETARG(__asm__Froze, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Froze));
	Memory::WriteMemory<byte>(0x532493, 20);
	Memory::WriteMemory<byte>(0x5319E5, 117);
	DecelerateCountdown = temp2;

	if (this->FrozenCountdown == 0 && temp == 0)
		return;

	FrozenCountdown = max(temp, countdown);
}

void PVZ::Zombie::Hypnotize()
{
	SETARG(__asm__Hypnotize, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Hypnotize));
}

void PVZ::Zombie::Remove()
{
	SETARG(__asm__Zombie__Remove, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Zombie__Remove));
}

void PVZ::Zombie::RemoveWithLoot()
{
	SETARG(__asm__Zombie__RemoveWithLoot, 1) = BaseAddress;
	Memory::Execute(STRING(__asm__Zombie__RemoveWithLoot));
}

void PVZ::Zombie::RemoveColdEffects()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x532B40)
		.ret()
	);
}

void PVZ::Zombie::SetAnimation(LPCSTR animName, PVZEnum::ReanimLoopType LoopType, int blend_time, float fps)
{
	int Address = PVZ::Memory::AllocMemory();
	SETARG(__asm__Zombie__setAnimation, 2) = BaseAddress;
	SETARGFLOAT(__asm__Zombie__setAnimation, 7) = fps;
	SETARG(__asm__Zombie__setAnimation, 12) = blend_time;
	__asm__Zombie__setAnimation[17] = LoopType;
	SETARG(__asm__Zombie__setAnimation, 19) = Address + 41;
	lstrcpyA((LPSTR)(__asm__Zombie__setAnimation + 41), animName);
	PVZ::Memory::WriteArrayUnsafe<byte>(Address, STRING(__asm__Zombie__setAnimation));
	PVZ::Memory::CreateThread(Address);
	PVZ::Memory::FreeMemory(Address);
}

void PVZ::Zombie::PlayZombieReanimation(DWORD animAddress, PVZEnum::ReanimLoopType loopType, BYTE blendTime, float rate)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_float(rate)
		.push(blendTime)
		.push(loopType)
		.push(animAddress)
		.mov_reg_imm(REG_EDI, BaseAddress)
		.invoke(0x528B00)
		.ret()
	);
}

void PVZ::Zombie::EquipBucket(int shield)
{
	if (this->HelmType)
		return;
	this->GetAnimation().AssignRenderGroupToPrefix("anim_bucket", 0);
	this->HelmType = HelmType::Bucket;
	this->HelmHealth = shield;
	this->HelmMaxHealth = shield;
}

void PVZ::Zombie::EquipCone(int shield)
{
	if (this->HelmType)
		return;
	this->GetAnimation().AssignRenderGroupToPrefix("anim_cone", 0);
	this->HelmType = HelmType::RoadCone;
	this->HelmHealth = shield;
	this->HelmMaxHealth = shield;
}

void PVZ::Zombie::ReanimShowPrefix(const char* TrackName, int renderGroup)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, TrackName, std::strlen(TrackName) + 1);
	SETARG(__asm__Zombie_ReanimShowPrefix, 1) = BaseAddress;
	SETARG(__asm__Zombie_ReanimShowPrefix, 6) = renderGroup;
	SETARG(__asm__Zombie_ReanimShowPrefix, 11) = PVZ::Memory::Variable + 100;
	Memory::Execute(STRING(__asm__Zombie_ReanimShowPrefix));
}

PVZ::TodParticleSystem PVZ::Zombie::AddAttachedParticle(int X, int Y, EffectType::EffectType effect)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(effect)
		.push_imm32(Y)
		.push_imm32(X)
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x5321F0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

PVZ::Animation PVZ::Zombie::AddAttachedReanim(int X, int Y, AnimationType::AnimationType reanim_type)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(Y)
		.push_imm32(X)
		.mov_reg_imm(REG_EDX, reanim_type)
		.mov_reg_imm(REG_ESI, this->GetBaseAddress())
		.invoke(0x5322C0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

bool PVZ::Zombie::canDecelerate()
{
	Memory::WriteMemory<byte>(0x5319E5, 112);//无视魅惑
	SETARG(__asm__CanDecelerate, 1) = BaseAddress;
	SETARG(__asm__CanDecelerate, 19) = Memory::Variable;
	return(Memory::Execute(STRING(__asm__CanDecelerate)) == 1);
	Memory::WriteMemory<byte>(0x5319E5, 117);
}

bool PVZ::Zombie::canFroze()
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x531A10)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

byte __asm__Zombie_EffectedBy[]
{
	MOV_ESI(0),
	ZOMBIE_EFFECTEDBY,
	AND_EUX(REG_EAX, 1),
	MOV_PTR_ADDR_EAX(0),
	RET
};

bool PVZ::Zombie::EffectedBy(DamageRangeFlags range, bool usepvzfunc)
{
	if (usepvzfunc)
	{
		SETARG(__asm__Zombie_EffectedBy, 1) = BaseAddress;
		__asm__Zombie_EffectedBy[6] = range;
		SETARG(__asm__Zombie_EffectedBy, 24) = Memory::Variable;
		return(Memory::Execute(STRING(__asm__Zombie_EffectedBy)) != 0);
	}
	else
	{
		ZombieType::ZombieType type = this->Type;
		ZombieState::ZombieState state = this->State;

		if ((range & DRF_DYING) == 0)
		{
			if (this->NotExist)
				return(false);
			if (state == ZombieState::DYING
				|| state == ZombieState::DYING_FROM_INSTANT_KILL
				|| state == ZombieState::DYING_FROM_LAWNMOWER)
				return(false);
		}
		BOOLEAN intendHypnotized = range & DRF_HYPNOTIZED;
		if (this->Hypnotized ^ intendHypnotized)
			return(false);
		if (type == ZombieType::BungeeZombie)
		{
			if (state != ZombieState::BUNGEE_IDLE_AFTER_DROP && state != ZombieState::BUNGEE_GRAB)
				return(false);
		}
		if (this->ZombieHeight == 9)
			return(false);
		if (type == ZombieType::DrZomboss)
		{
			PVZ::Animation anim = this->GetAnimation();
			if (state == ZombieState::ZOMBOSS_FALL && anim.CycleRate < 0.5)
				return(false);
			if (state == ZombieState::ZOMBOSS_RISE && anim.CycleRate > 0.5)
				return(false);
			if (state != ZombieState::ZOMBOSS_DOWN
				&& state != ZombieState::ZOMBOSS_PREPARE_RISE
				&& state != ZombieState::ZOMBOSS_BALL)
				return(false);
		}
		//if (type == ZombieType::ZombieBobsledTeam && )
		//return(false);
		if (state == ZombieState::POLE_VALUTING_JUMPPING
			|| state == ZombieState::IMP_FLYING
			|| state == ZombieState::DIGGER_DRILL
			|| state == ZombieState::DIGGER_LOST_DIG
			|| state == ZombieState::DIGGER_LANDING
			|| state == ZombieState::DOPHIN_JUMP_IN_POOL
			|| state == ZombieState::DOPHIN_JUMP
			|| state == ZombieState::SNORKEL_JUMP_IN_THE_POOL
			|| state == ZombieState::BALLOON_FALLING
			|| state == ZombieState::RISING_FROM_GROUND
			|| state == ZombieState::BOBSLED_GETOFF
			|| state == ZombieState::BACKUP_SPAWNING)
			return(range & DRF_OFF_GROUND);

		//)if (this->mZombieType == ZombieType::BOBSLED

		bool submerged = type == ZombieType::SnorkedZombie && this->InWater && !this->Eating;
		if ((range & DRF_SUBMERGED) != 0 && submerged)
			return(true);

		if ((range & DRF_UNDERGROUND) != 0
			&& state == ZombieState::DIGGER_DIG)
			return(true);

		bool flying = state == ZombieState::BALLOON_FLYING || state == ZombieState::BALLOON_FALLING;
		if ((range & DRF_FLYING) != 0 && flying)
			return(true);

		if (range & DRF_GROUND && !flying && !submerged && state != ZombieState::DIGGER_DIG)
			return(true);

		return(false);
	}
}

void PVZ::Zombie::UpdateAnimSpeed()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, this->GetBaseAddress())
		.invoke(0x52F050)
		.ret());
}

void PVZ::Zombie::PickRandomSpeed()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x524A70)
		.ret());
}

void PVZ::Zombie::PoolSplash(bool into_pool)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(into_pool)
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x52F6D0)
		.ret()
	);
}

void PVZ::Zombie::RiseFromGrave(int row, int col)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(row)
		.mov_reg_imm(REG_EAX, col)
		.mov_reg_imm(REG_EBX, BaseAddress)
		.invoke(0x531C90)
		.ret()
	);
}
void PVZ::Zombie::LandFlyer(PVZ::DamageFlags flag)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(flag)
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x525B60)
		.ret()
	);
}
void PVZ::Zombie::GetTrackPosition(const char* trackName, float& thePosX, float& thePosY)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, trackName, strlen(trackName));
	PVZ::Memory::WriteMemoryUnsafe<char>(PVZ::Memory::Variable + 100 + strlen(trackName), '\0');
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI,PVZ::Memory::Variable)
		.push_imm32(PVZ::Memory::Variable + 4)
		.push_imm32(PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_ESI,this->GetBaseAddress())
		.invoke(0x5345F0)
		.ret()
	);
	thePosY = PVZ::Memory::ReadMemory<float>(PVZ::Memory::Variable);
	thePosX = PVZ::Memory::ReadMemory<float>(PVZ::Memory::Variable + 4);
}

void PVZ::Zombie::OverrideParticleColor(PVZ::TodParticleSystem particle)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX,particle.GetBaseAddress())
		.mov_reg_imm(REG_EAX,this->GetBaseAddress())
		.invoke(0x529810)
		.ret()
	);
}

void PVZ::Zombie::BossSummonZombie(ZombieType::ZombieType type, int row)
{
	PVZ::Memory::WriteMemory<DWORD>(0x534DC4, 0); // 跳过一个判定
	PVZ::Memory::WriteMemory<DWORD>(0x534DD0, INT_MAX); // 必定进入该分支
	PVZ::Memory::WriteMemory<DWORD>(0x534DD7, type); // 决定僵尸种类
	RemoveColdEffects();
	State = ZombieState::ZOMBOSS_SUMMON;
	TargetRow = row;
	DWORD animAddress = 0x66F454 + row * 0x10;
	PlayZombieReanimation(animAddress, PVZEnum::ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
	GetPVZApp().PlayFoley(PVZEnum::FOLEY_HYDRAULIC_SHORT);
}

void PVZ::Zombie::BossRVAttack(int row, int col)
{
	if (row < 0 || row > 3) return;
	if (col < 0 || col > 2) return;
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x534AC0)
		.ret()
	);
	TargetRow = row;
	TargetCol = col;
}

void PVZ::Zombie::BossBungeeAttack()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x5350C0)
		.ret()
	);
}

void PVZ::Zombie::BossStompAttack(int row)
{
	BYTE nop6[] = { NOP, NOP, NOP, NOP, NOP, NOP };
	Memory::WriteArray<BYTE>(0x534E9B, STRING(nop6));
	BYTE code[] = { MOV_EAX(row) };
	Memory::WriteArray<BYTE>(0x534EA3, STRING(code));
	BYTE nop4[] = { NOP, NOP, NOP, NOP };
	Memory::WriteArray<BYTE>(0x534EA8, STRING(nop4));
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x534E30)
		.ret()
	);
}

void PVZ::Zombie::BossHeadSpit(int row, BYTE isFireBall)
{
	Memory::WriteMemory<DWORD>(0x535485, row);
	BYTE nop5[] = { NOP, NOP, NOP, NOP, NOP };
	Memory::WriteArray<BYTE>(0x535490, STRING(nop5));
	BYTE code[] = { 0xB0, isFireBall, NOP, NOP, NOP };
	Memory::WriteArray<BYTE>(0x5354A5, STRING(code));
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, BaseAddress)
		.invoke(0x535440)
		.ret()
	);
}

PVZ::Rect PVZ::Zombie::GetActualAttackRect()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, PVZ::Memory::Variable)
		.mov_reg_imm(REG_EBX, this->GetBaseAddress())
		.invoke(0x532140)
		.ret()
	);

	Rect tmp = Rect();
	tmp.X = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable);
	tmp.Y = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 4);
	tmp.Width = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 8);
	tmp.Height = PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 0x0C);

	return tmp;
}

byte __asm__Zombie_GetRect[]
{
	MOV_EDI(0),
	MOV_EBX(0),
	INVOKE(0x5320B0),
	RET
};
PVZ::Rect PVZ::Zombie::GetZombieRect()
{
	SETARG(__asm__Zombie_GetRect, 1) = PVZ::Memory::Variable;
	SETARG(__asm__Zombie_GetRect, 6) = this->GetBaseAddress();
	Memory::Execute(__asm__Zombie_GetRect, 24);

	return Rect
	{
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 4),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 8),
		PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable + 12)
	};
}

float PVZ::Zombie::ZombieTargetLeadX(float time)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.push_float(time)
		.invoke(0x52A580)
		.fstp(PVZ::Memory::Variable)
		.ret()
	);
	float tmp = PVZ::Memory::ReadMemory<float>(PVZ::Memory::Variable);
	return tmp;
}


PVZ::Plant PVZ::Zombie::FindCatapultTarget()
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(this->GetBaseAddress())
		.invoke(0x525890)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	return PVZ::Plant(PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable));
}

void PVZ::Zombie::ZombieCatapultFire(PVZ::Plant plant)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, plant.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x525730)
		.ret()
	);
}

int PVZ::Zombie::GetBobsledPosition()
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x5346A0)
		.ret()
	);
}

void PVZ::Zombie::BungeeDropZombie(Zombie target, int col, int row)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EBX, this->GetBaseAddress())
		.push_imm32(target.GetBaseAddress())
		.mov_reg_imm(REG_EAX, row)
		.mov_reg_imm(REG_ECX, col)
		.invoke(0x524970)
		.ret()
	);
}