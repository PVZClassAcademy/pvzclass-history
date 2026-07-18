#include "Creators.h"
#include "Classes.hpp"
#include <iostream>

byte __asm__CreateReanimation[44]
{
	MOV_EAX(0),
	MOV_EBX(0),
	CREATEREANIMATION,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Animation Creator::CreateReanimation(AnimationType::AnimationType type, float x, float y, int layer)
{
	SETARG(__asm__CreateReanimation, 1) = PVZ_BASE;
	SETARG(__asm__CreateReanimation, 6) = type;
	SETARG(__asm__CreateReanimation, 11) = layer;
	SETARGFLOAT(__asm__CreateReanimation, 16) = y;
	SETARGFLOAT(__asm__CreateReanimation, 21) = x;
	SETARG(__asm__CreateReanimation, 39) = PVZ::Memory::Variable;
	return PVZ::Animation(PVZ::Memory::Execute(STRING(__asm__CreateReanimation)));
}

byte __asm__CreateZombie[34]
{
	MOV_ECX_PTR_ADDR(0),
	MOV_EAX(0),
	CREATEZOMBIE,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

byte __asm__CreateZombie__pieces[6]
{
	MOV_EAX_EBX,
	POP_EBX,
	RETN(8),
};

PVZ::Zombie Creator::CreateZombie(ZombieType::ZombieType type, int row, byte column)
{
	if (type == ZombieType::DrZomboss)
		row = column = 0;
	PVZ::Memory::WriteArray(0x42A209, STRING(__asm__CreateZombie__pieces));
	PVZ::Memory::WriteMemory<short>(0x42A1E4, makeshort(JMP(0x23)));
	PVZ::Memory::WriteMemory<short>(0x42A196, makeshort(JMP(0x71)));
	SETARG(__asm__CreateZombie, 2) = PVZBASEADDRESS + 0x160;
	SETARG(__asm__CreateZombie, 7) = row;
	__asm__CreateZombie[12] = column;
	__asm__CreateZombie[14] = type;
	SETARG(__asm__CreateZombie, 29) = PVZ::Memory::Variable;
	auto zombie = PVZ::Zombie(PVZ::Memory::Execute(STRING(__asm__CreateZombie)));
	if (type == ZombieType::DrZomboss) zombie.X = 0;
	return zombie;
}

byte __asm__CreatePlant[35]
{
	MOV_EAX(0),
	CREATEPLANT,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Plant Creator::CreatePlant(SeedType::SeedType type, int row, byte column, BOOLEAN imitative)
{
	if (imitative)__asm__CreatePlant[6] = type;
	__asm__CreatePlant[8] = imitative ? SeedType::Imitater : type;
	__asm__CreatePlant[10] = column;
	SETARG(__asm__CreatePlant, 1) = row;
	SETARG(__asm__CreatePlant, 12) = PVZBASEADDRESS;
	SETARG(__asm__CreatePlant, 30) = PVZ::Memory::Variable;
	return PVZ::Plant(PVZ::Memory::Execute(STRING(__asm__CreatePlant)));
}

byte __asm__CreateProjectile[47]
{
	MOV_EAX(0),
	CREATEPROJECTILE,
	MOV_PTR_EAX_ADD(0x74, 0xB),
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Projectile Creator::CreateProjectile(ProjectileType::ProjectileType type, byte row, int x)
{
	SETARG(__asm__CreateProjectile, 1) = PVZBASEADDRESS;
	__asm__CreateProjectile[6] = type;
	__asm__CreateProjectile[8] = row;
	SceneType::SceneType scene = PVZ::Memory::ReadMemory<SceneType::SceneType>(PVZBASEADDRESS + 0x554C);
	bool sixroute = (scene == SceneType::Pool) || (scene == SceneType::Fog);
	int y = sixroute ? 94 + 86 * row : 90 + 100 * row;
	SETARG(__asm__CreateProjectile, 12) = y;
	SETARG(__asm__CreateProjectile, 17) = x;
	SETARG(__asm__CreateProjectile, 42) = PVZ::Memory::Variable;
	return PVZ::Projectile(PVZ::Memory::Execute(STRING(__asm__CreateProjectile)));
}

byte __asm__CreateProjectile2[84]
{
	MOV_EAX(0),
	CREATEPROJECTILE,
	MOV_PTR_EAX_ADD(0x58, 7),
	MOV_PTR_EAX_ADD(0x74, 0xB),
	FILD_PTR(0),
	FDIV_PTR(0),
	FSTP_PTR_EAX_ADD(0x3C),
	FILD_PTR(0),
	FDIV_PTR(0),
	FSTP_PTR_EAX_ADD(0x40),
	MOV_PTR_ADDR_EAX(0),
	RET,
};

byte __asm__CreatePortalpieces1[10]
{
	MOV_ECX(4),
	JMPFAR(0),
};

byte __asm__CreatePortalpieces2[15]
{
	MOV_PTR_EAX_ADD_V_ECX(0x14),
	MOV_PTR_EAX_ADD_V_V(0x1C, 0x54B78),
	JMPFAR(0),
};

byte __asm__Random[]
{
	PUSH_ECX,
	PUSH_EDX,
	MOV_EAX(0),
	INVOKE(0x5AF400),
	POP_EUX(REG_EDX),
	POP_EUX(REG_ECX),
	RET,
	0xCC, 0xCC,
	PUSH_EAX,
	PUSH_ECX,
	PUSHDWORD(0),
	INVOKE(0x5AF410),
	POP_EUX(REG_ECX),
	POP_EUX(REG_ECX),
	POP_EUX(REG_EAX),
	RET,
};

void Creator::AsmInit()
{
	SETARG(__asm__CreateProjectile2, 50) = PVZ::Memory::Variable + 8;
	SETARG(__asm__CreateProjectile2, 56) = PVZ::Memory::Variable + 4;
	SETARG(__asm__CreateProjectile2, 65) = PVZ::Memory::Variable + 12;
	SETARG(__asm__CreateProjectile2, 71) = PVZ::Memory::Variable + 4;
	SETARG(__asm__CreateProjectile2, 79) = PVZ::Memory::Variable;
	PVZ::Memory::WriteArrayUnsafe<byte>(PVZ::Memory::Variable + 16, STRING(__asm__CreateProjectile2));
	//protal
	SETARG(__asm__CreatePortalpieces1, 6) = PVZ::Memory::Variable + 200 - 5 - 0x427071;
	PVZ::Memory::WriteArray<byte>(0x42706C, STRING(__asm__CreatePortalpieces1));
	SETARG(__asm__CreatePortalpieces2, 11) = 0x427076 - 5 - (PVZ::Memory::Variable + 210);
	PVZ::Memory::WriteArrayUnsafe<byte>(PVZ::Memory::Variable + 200, STRING(__asm__CreatePortalpieces2));
	// random
	PVZ::Memory::WriteArrayUnsafe<byte>(PVZ::Memory::Variable + 225, STRING(__asm__Random));
}

byte __asm__Asm__Reset[10]
{
	MOV_PTR_EUX_ADD_V_EVX(REG_EAX, REG_EBX, 0x14),
	MOV_PTR_EUX_ADD_V(REG_EAX, 0x1C, 0x54B78)
};

void Creator::AsmReset()
{
	PVZ::Memory::WriteArray<byte>(0x42706C, __asm__Asm__Reset, 10);
}

PVZ::Projectile Creator::CreateProjectile(ProjectileType::ProjectileType type, int x, int y, float angle, float speed)
{
	angle = angle / 180 * PI;
	int xspeed = (int)(sin(angle) * speed * 10000);
	int yspeed = (int)(-cos(angle) * speed * 10000);
	PVZ::Memory::WriteMemoryUnsafe<float>(PVZ::Memory::Variable + 4, 10000.0f);
	PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 8, xspeed);
	PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 12, yspeed);
	PVZ::Memory::WriteMemoryUnsafe<byte>(PVZ::Memory::Variable + 22, type);
	PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 28, y);
	PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 33, x);
	xytorc(&x, &y);
	PVZ::Memory::WriteMemoryUnsafe<byte>(PVZ::Memory::Variable + 24, x);
	PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 17, PVZBASEADDRESS);
	PVZ::Memory::WriteMemory<byte>(0x552014, 0xFE);

	if (PVZ::Memory::localExecute)
		PVZ::Memory::Execute((byte*)(PVZ::Memory::Variable + 16), 84);
	else
		PVZ::Memory::CreateThread(PVZ::Memory::Variable + 16);

	PVZ::Memory::WriteMemory<byte>(0x552014, 0xDB);
	return PVZ::Projectile(PVZ::Memory::ReadMemory<int>(PVZ::Memory::Variable));
}

byte __asm__CreateCoin[38]
{
	MOV_ECX(0),
	CREATECOIN,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Coin Creator::CreateCoin(CoinType::CoinType type, int x, int y, CoinMotionType::CoinMotionType motion)
{
	SETARG(__asm__CreateCoin, 1) = PVZBASEADDRESS;
	__asm__CreateCoin[6] = motion;
	__asm__CreateCoin[8] = type;
	SETARG(__asm__CreateCoin, 10) = y;
	SETARG(__asm__CreateCoin, 15) = x;
	SETARG(__asm__CreateCoin, 33) = PVZ::Memory::Variable;
	return PVZ::Coin(PVZ::Memory::Execute(STRING(__asm__CreateCoin)));
}

byte __asm__ResetLawnmower[19]
{
	RESETLAWNMOWER,
	RET,
};

void Creator::ResetLawnmowers()
{
	PVZ::Memory::AllAccess(0x679BF8);
	PVZ::Memory::WriteMemory<float>(0x679BF8, -21.0f);
	PVZ::Memory::WriteMemory<short>(0x40BC98, makeshort(JMP(0x60)));
	PVZ::Memory::WriteMemory<byte>(0x40BD17, 1);
	SETARG(__asm__ResetLawnmower, 1) = PVZBASEADDRESS;
	auto lawnmowers = PVZ::GetBoard().GetAllLawnmowers();
	for (DWORD i = 0; i < lawnmowers.size(); i++)
		lawnmowers[i].Die();
	PVZ::Memory::Execute(STRING(__asm__ResetLawnmower));
	PVZ::Memory::WriteMemory<float>(0x679BF8, -160.0f);
	PVZ::Memory::WriteMemory<short>(0x40BC98, makeshort(JNZ(9)));
	PVZ::Memory::WriteMemory<byte>(0x40BD17, 0);
}

byte __asm__CreateGriditem[24]
{
	MOV_ESI(0),
	CREATEGRDITEM,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Griditem Creator::CreateGriditem()
{
	SETARG(__asm__CreateGriditem, 1) = PVZBASEADDRESS + 0x11C;
	SETARG(__asm__CreateGriditem, 19) = PVZ::Memory::Variable;
	return PVZ::Griditem(PVZ::Memory::Execute(STRING(__asm__CreateGriditem)));
}

byte __asm__CreateGrave[30]
{
	PUSH_PTR(0),
	MOV_EDI(0),
	MOV_EBX(0),
	CREATEGRAVE,
	RET,
};

void Creator::CreateGrave(int row, int column)
{
	SETARG(__asm__CreateGrave, 2) = PVZBASEADDRESS + 0x160;
	SETARG(__asm__CreateGrave, 7) = row;
	SETARG(__asm__CreateGrave, 12) = column;
	PVZ::Memory::Execute(STRING(__asm__CreateGrave));
}

byte __asm__CreateCrater[31]
{
	MOV_EAX(0),
	MOV_EDI(0),
	CREATECRATER,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Crater Creator::CreateCrater(int row, int column, int duration)
{
	SETARG(__asm__CreateCrater, 1) = PVZBASEADDRESS;
	SETARG(__asm__CreateCrater, 6) = row;
	__asm__CreateCrater[11] = column;
	SETARG(__asm__CreateCrater, 26) = PVZ::Memory::Variable;;
	auto crater = PVZ::Crater(PVZ::Memory::Execute(STRING(__asm__CreateCrater)));
	crater.DisappearCountdown = duration;
	return crater;
}

byte __asm__CreateLadder[31]
{
	MOV_EAX(0),
	MOV_EDI(0),
	CREATELADDER,
	MOV_PTR_ADDR_EAX(0),
	RET,
};

PVZ::Griditem Creator::CreateLadder(int row, byte column)
{
	SETARG(__asm__CreateLadder, 1) = PVZBASEADDRESS;
	SETARG(__asm__CreateLadder, 6) = row;
	__asm__CreateLadder[11] = column;
	SETARG(__asm__CreateLadder, 26) = PVZ::Memory::Variable;;
	return PVZ::Griditem(PVZ::Memory::Execute(STRING(__asm__CreateLadder)));
}

PVZ::Vase Creator::CreateVase(int row, int column, VaseContent::VaseContent content, VaseSkin::VaseSkin skin, ZombieType::ZombieType zombie, SeedType::SeedType plant, int sun)
{
	PVZ::Vase vase = PVZ::Vase(CreateGriditem().GetBaseAddress());
	vase.Row = row;
	vase.Column = column;
	vase.Layer = row * 0x2710 + 0x49BB0;
	vase.Type = GriditemType::Vase;
	vase.Skin = skin;
	vase.ContentZombie = zombie;
	vase.ContentPlant = plant;
	vase.Content = content;
	vase.Sun = sun;
	return vase;
}

PVZ::IZBrain Creator::CreateIZBrain(int row, int column)
{
	PVZ::IZBrain iz_brain = PVZ::IZBrain(CreateGriditem().GetBaseAddress());
	iz_brain.Row = row;
	iz_brain.Column = column;
	iz_brain.Layer = row * 0x2710 + 0x49BB0;
	iz_brain.Type = GriditemType::IZBrain;
	iz_brain.NotExist = false;
	iz_brain.Hp = 70;
	iz_brain.X = PVZ::GetBoard().GridToXPixel(row, column) - 40.0f;
	iz_brain.Y = PVZ::GetBoard().GridToYPixel(row, column) + 40.0f;
	return iz_brain;
}

byte __asm__CreatePortal2[]
{
	MOV_ESI(0),
	INVOKE(0x41E1C0), // 5+13=18
	MOV_PTR_ADDR_EAX(0),
	MOV_PTR_EAX_ADD(0x08, 0), // type
	MOV_PTR_EAX_ADD(0x10, 0), // col
	MOV_PTR_EAX_ADD(0x14, 0), // row
	MOV_PTR_EAX_ADD(0x1C, 0), // layer
	INVOKE(0x44E1B0),
	RET
};

PVZ::Portal Creator::CreatePortal(int row, int column, int isYellow)
{
	SETARG(__asm__CreatePortal2, 1) = PVZBASEADDRESS + 0x11C;
	SETARG(__asm__CreatePortal2, 19) = PVZ::Memory::Variable;
	SETARG(__asm__CreatePortal2, 26) = GriditemType::PortalBlue + isYellow;
	SETARG(__asm__CreatePortal2, 33) = column;
	SETARG(__asm__CreatePortal2, 40) = row;
	SETARG(__asm__CreatePortal2, 47) = column * 10000 + 0x4AF38;
	return PVZ::Portal(PVZ::Memory::Execute(STRING(__asm__CreatePortal2)));
}

PVZ::Vase Creator::CreateVase(VaseCreateInfo vaseinfo)
{
	return CreateVase(vaseinfo.row, vaseinfo.column, vaseinfo.content, vaseinfo.skin, vaseinfo.zombie, vaseinfo.plant, vaseinfo.sun);
}

PVZ::Rake Creator::CreateRake(byte row, byte column)
{
	PVZ::Board board = PVZ::GetBoard();

	PVZ::Rake rake = PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, PVZBASEADDRESS + 0x11C)
		.invoke(0x41E1C0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.mov_mem_reg_add_reg(REG_EAX, 8, GriditemType::Rake)
		.mov_mem_reg_add_reg(REG_EAX, 0x0C, GriditemState::RakeAttracting)
		.mov_mem_reg_add_reg(REG_EAX, 0x10, column)
		.mov_mem_reg_add_reg(REG_EAX, 0x14, row)
		.mov_mem_reg_add_reg(REG_EAX, 0x1C, row * 10000 + 301009)
		.ret()
	);

	rake.X = static_cast<float>(board.GridToXPixel(row, column));
	rake.Y = static_cast<float>(board.GridToYPixel(row, column));

	auto model = Creator::CreateReanimation(AnimationType::Rake, rake.X + 20.0f, rake.Y, 0);
	model.LoopType = PVZEnum::REANIM_PLAY_ONCE_AND_HOLD;
	model.Speed = 0;
	model.IsAttachment = true;
	rake.SetReanimationn(model);

	return rake;
}

byte __asm__CreateCaption[]
{
	PUSHDWORD(0),
	MOV_ECX(0),
	INVOKE(0x404450),
	MOV_ESI(0),
	MOV_ECX(6),
	MOV_EDX(0),
	INVOKE(0x459010),
	MOV_PTR_ESI_ADD(0x88, 0),
	MOV_PTR_ESI_ADD(0x8C, 0),
	RET,
};

void Creator::CreateCaption(const char* str, int length, CaptionStyle::CaptionStyle style, int duration)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, str, length);
	PVZ::Memory::WriteMemoryUnsafe<char>(PVZ::Memory::Variable + 100 + length, 0);
	SETARG(__asm__CreateCaption, 1) = PVZ::Memory::Variable + 100;
	SETARG(__asm__CreateCaption, 6) = PVZ::Memory::Variable + 600;
	SETARG(__asm__CreateCaption, 24) = PVZ::Memory::ReadMemory<int>(PVZBASEADDRESS + 0x140);
	SETARG(__asm__CreateCaption, 34) = PVZ::Memory::Variable + 600;
	SETARG(__asm__CreateCaption, 57) = duration;
	SETARG(__asm__CreateCaption, 67) = style;
	PVZ::Memory::Execute(STRING(__asm__CreateCaption));
}

byte __asm__CreateImageCaption[50]
{
	MOV_EDI(0),
	PUSHDWORD(0),
	LEA_ECX_ESP_ADD(0x10),
	CHARSTRTOSTRING,
	MOV_ECX(0xF),
	LEA_EDX_ESP_ADD(0xC),
	CREATEIMAGECAPTION,
	RET,
};

void Creator::CreateImageCaption(const char* str, int length)
{
	PVZ::Memory::WriteArrayUnsafe<const char>(PVZ::Memory::Variable + 100, str, length);
	SETARG(__asm__CreateImageCaption, 1) = PVZBASEADDRESS;
	SETARG(__asm__CreateImageCaption, 6) = PVZ::Memory::Variable + 100;
	PVZ::Memory::Execute(STRING(__asm__CreateImageCaption));
}

byte __asm__CreateExplosion[42]
{
	CREATEEXPLOTION,
	RET,
};

void Creator::CreateExplosion(int x, int y, int radius, BOOLEAN cinder, byte bound, BOOLEAN fromjackzombie)
{
	byte damage = fromjackzombie ? 0xFF : 0x7F;
	__asm__CreateExplosion[1] = damage;
	__asm__CreateExplosion[3] = cinder;
	__asm__CreateExplosion[5] = bound;
	SETARG(__asm__CreateExplosion, 7) = radius;
	SETARG(__asm__CreateExplosion, 12) = y;
	SETARG(__asm__CreateExplosion, 17) = x;
	xytorc(&x, &y);
	__asm__CreateExplosion[22] = x;
	SETARG(__asm__CreateExplosion, 24) = PVZBASEADDRESS;
	PVZ::Memory::Execute(STRING(__asm__CreateExplosion));
}

byte __asm__CreateSampleSound[24]
{
	MOV_ECX(0),
	CREATEUPPERSOUND,
	RET,
};

void Creator::CreateUpperSound(UpperSoundType::UpperSoundType sound)
{
	SETARG(__asm__CreateSampleSound, 1) = PVZ_BASE;
	SETARG(__asm__CreateSampleSound, 6) = sound;
	PVZ::Memory::Execute(STRING(__asm__CreateSampleSound));
}

byte __asm__FrozeAll[19]
{
	MOV_EDI(0),
	FREEZEALL,
	RET,
};

void Creator::FrozeAll()
{
	SETARG(__asm__FrozeAll, 1) = PVZ::Memory::ReadMemory<int>(PVZBASEADDRESS + 0xAC);
	PVZ::Memory::Execute(STRING(__asm__FrozeAll));
}

byte __asm__StopSound[24]
{
	MOV_EDI(0),
	MOV_EAX(0),
	STOPSOUND,
	RET,
};

void Creator::StopSound(int soundid)
{
	SETARG(__asm__StopSound, 1) = PVZ::Memory::ReadPointer(0x6A9EC0, 0x784);
	SETARG(__asm__StopSound, 6) = soundid;
	PVZ::Memory::Execute(STRING(__asm__StopSound));
}

byte __asm__CreateIZombieFormation[19]
{
	CREATEIZOMBIEFORMATION,
	RET,
};

void Creator::CreateIZombieFormation(PVZLevel::PVZLevel izlevel)
{
	if (izlevel >= PVZLevel::I_Zombie && izlevel <= PVZLevel::I_Zombie_Endless)
	{
		PVZLevel::PVZLevel curlevel = PVZ::Memory::ReadMemory<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8);
		PVZ::Memory::WriteMemoryUnsafe<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8, izlevel);
		SETARG(__asm__CreateIZombieFormation, 1) = PVZ::Memory::ReadMemory<int>(PVZBASEADDRESS + 0x160);
		PVZ::Memory::Execute(STRING(__asm__CreateIZombieFormation));
		PVZ::Memory::WriteMemoryUnsafe<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8, curlevel);
	}
}

byte __asm__CreateVaseFormation[19]
{
	MOV_ESI(0),
	CREATEVASEFORMATION,
	RET,
};

void Creator::CreateVaseFormation(PVZLevel::PVZLevel vblevel)
{
	if (vblevel >= PVZLevel::Vasebreaker && vblevel <= PVZLevel::Vasebreaker_Endless)
	{
		PVZLevel::PVZLevel curlevel = PVZ::Memory::ReadMemory<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8);
		PVZ::Memory::WriteMemoryUnsafe<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8, vblevel);
		SETARG(__asm__CreateVaseFormation, 1) = PVZ::Memory::ReadMemory<int>(PVZBASEADDRESS + 0x160);
		PVZ::Memory::Execute(STRING(__asm__CreateVaseFormation));
		PVZ::Memory::WriteMemoryUnsafe<PVZLevel::PVZLevel>(PVZ_BASE + 0x7F8, curlevel);
	}
}

byte __asm__CreatePortal[19]
{
	MOV_EDI(0),
	CREATEPORTAL,
	RET,
};

void Creator::__CreatePortal()
{
	auto griditems = PVZ::GetBoard().GetAllGriditems();
	for (DWORD i = 0; i < griditems.size(); i++)
		if (griditems[i].Type == GriditemType::PortalBlue || griditems[i].Type == GriditemType::PortalYellow)
			griditems[i].Remove();
	SETARG(__asm__CreatePortal, 1) = PVZ::Memory::ReadMemory<int>(PVZBASEADDRESS + 0x160);
	PVZ::Memory::Execute(STRING(__asm__CreatePortal));
}

void Creator::CreatePortal(int yellow1Row, int yellow1Column, int yellow2Row, int yellow2Column, int blue1Row, int blue1Column, int blue2Row, int blue2Column)
{
	PVZ::Memory::WriteMemory<int>(0x426FE9, yellow1Row);
	PVZ::Memory::WriteMemory<int>(0x426FE2, yellow1Column);
	PVZ::Memory::WriteMemory<int>(0x427014, yellow2Row);
	PVZ::Memory::WriteMemory<int>(0x42700D, yellow2Column);
	PVZ::Memory::WriteMemory<int>(0x427044, blue1Row);
	PVZ::Memory::WriteMemory<int>(0x42703D, blue1Column);
	PVZ::Memory::WriteMemory<int>(0x42706D, blue2Row);
	PVZ::Memory::WriteMemory<int>(0x427068, blue2Column);
	__CreatePortal();
	PVZ::Memory::WriteMemory<int>(0x426FE9, 0);
	PVZ::Memory::WriteMemory<int>(0x426FE2, 2);
	PVZ::Memory::WriteMemory<int>(0x427014, 1);
	PVZ::Memory::WriteMemory<int>(0x42700D, 9);
	PVZ::Memory::WriteMemory<int>(0x427044, 3);
	PVZ::Memory::WriteMemory<int>(0x42703D, 9);
	PVZ::Memory::WriteMemory<int>(0x42706D, 4);
	PVZ::Memory::WriteMemory<int>(0x427068, 2);
}

byte __asm__ClearZombiePreview[19]
{
	MOV_EBX(0),
	CLEARZOMBIEPREVIEW,
	RET,
};

void Creator::__ClearZombiePreview()
{
	SETARG(__asm__ClearZombiePreview, 1) = PVZBASEADDRESS;
	PVZ::Memory::Execute(STRING(__asm__ClearZombiePreview));
}

byte __asm__CreateZombieInLevel[19]
{
	MOV_EDI(0),
	CREATEZOMBIEINLEVEL,
	RET,
};

void Creator::CreateZombieInLevel(ZombieType::ZombieType* ztypes, int length, int wave)
{
	if (wave)
	{
		PVZ::Memory::WriteMemory<int>(0x4092FD, wave);
		PVZ::Memory::WriteMemory<int>(0x4093F2, wave);
		PVZ::Memory::WriteMemory<int>(0x409466, wave);
		PVZ::Memory::WriteMemory<int>(0x409472, wave);
		PVZ::Memory::WriteMemory<int>(0x40947C, wave);
		PVZ::Memory::WriteMemory<int>(0x40948A, wave);
		PVZ::Memory::WriteMemory<int>(0x409499, wave);
	}
	__ClearZombiePreview();
	SETARG(__asm__CreateZombieInLevel, 1) = PVZBASEADDRESS;
	for (int i = 0; i < 33; i++)
		PVZ::Memory::WriteMemoryUnsafe<byte>(PVZBASEADDRESS + 0x54D4 + i, 0);
	for (int i = 0; i < length; i++)
		PVZ::Memory::WriteMemoryUnsafe<byte>(PVZBASEADDRESS + 0x54D4 + ztypes[i], 1);
	PVZ::Memory::Execute(STRING(__asm__CreateZombieInLevel));
	PVZ::Memory::WriteMemory<int>(0x4092FD, 20);
	PVZ::Memory::WriteMemory<int>(0x4093F2, 12);
	PVZ::Memory::WriteMemory<int>(0x409466, 40);
	PVZ::Memory::WriteMemory<int>(0x409472, 30);
	PVZ::Memory::WriteMemory<int>(0x40947C, 0);
	PVZ::Memory::WriteMemory<int>(0x40948A, 10);
	PVZ::Memory::WriteMemory<int>(0x409499, 10);
}
