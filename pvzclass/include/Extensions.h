/// @file Extensions.h
/// @brief 包含多种修改器功能实现的文件

#pragma once
#include "../PVZ.h"
#include "Creators.h"

#define MEMMOD_BYTE(address,v,rv) PVZ::Memory::WriteMemory<byte>(address,b?v:rv)
#define MEMMOD_INT(address,v,rv) PVZ::Memory::WriteMemory<int>(address,b?v:rv)
#define MEMMOD_SHORT(address,v,rv) PVZ::Memory::WriteMemory<short>(address,b?v:rv)
#define JO 112
#define JZ 116
#define JNE 117
#define JUMP 235
#define JLE 126

using ThreeState::Enable;
using ThreeState::None;
using ThreeState::Disable;
using Memory = PVZ::Memory;

/// @brief 启用/禁用砸罐子功能。
///		包括罐子高亮、罐子可砸、小丑炸罐子、巨人砸罐子。
/// @param state 功能的启用状态。
inline void EnableVaseBreak(ThreeState::ThreeState state = Enable)
{
	switch (state)
	{
	case Enable:
		Memory::WriteMemory<short>(0x40EB36, 0x1571);
		Memory::WriteMemory<byte>(0x411AE5, 0x80);

		Memory::WriteMemory<byte>(0x422183, JO);
		Memory::WriteMemory<short>(0x424791, 0x1471);

		Memory::WriteMemory<byte>(0x526CE0, JO);

		Memory::WriteMemory<short>(0x526DBB, 0x1571);
		Memory::WriteMemory<byte>(0x527257, JO);
		break;
	case None:
		Memory::WriteMemory<short>(0x40EB36, 0x057C);
		Memory::WriteMemory<byte>(0x411AE5, 0x84);

		Memory::WriteMemory<byte>(0x422183, JZ);
		Memory::WriteMemory<short>(0x424791, 0x057C);

		Memory::WriteMemory<byte>(0x526CE0, JZ);

		Memory::WriteMemory<short>(0x526DBB, 0x057C);
		Memory::WriteMemory<byte>(0x527257, JZ);
		break;
	case Disable:
		Memory::WriteMemory<short>(0x40EB36, 0x7871);
		Memory::WriteMemory<byte>(0x411AE5, 0x81);

		Memory::WriteMemory<byte>(0x422183, JUMP);
		Memory::WriteMemory<short>(0x424791, 0x1971);

		Memory::WriteMemory<byte>(0x526CE0, JUMP);

		Memory::WriteMemory<short>(0x526DBB, 0x5971);
		Memory::WriteMemory<byte>(0x527257, JUMP);
	}
}

/// @brief 启用后台运行
/// @param b 是否开启此功能
inline void EnableBackgroundRunning(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x54EBA8, JO, JZ);
}

/// @brief 显示隐藏关卡
/// @param b 是否开启此功能
inline void ShowHiddenLevel(BOOLEAN b = true)
{
	MEMMOD_BYTE(b ? 0x42DF5D : 0x54EBA8, 56, 136);
}

/// @brief 启用或禁用传送门功能。
/// @param b 是否启用传送功能。默认为 `true`。
/// @param adjust_existed 是否调整现有的传送门。默认为 `false`。
///		- 当 `b` 为 `true` 且 `adjust_existed` 为 `false` 时，将创建默认位置的传送门。
///		- 当 `b` 为 `false` 且 `adjust_existed` 为 `true` 时，将移除现有的传送门
inline void EnablePortal(BOOLEAN b = true, BOOLEAN adjust_existed = false)
{
	if (b)
	{
		PVZ::Memory::WriteMemory<byte>(0x467665, JO);
		PVZ::Memory::WriteMemory<byte>(0x41FFB4, JO);
		PVZ::Memory::WriteMemory<byte>(0x4248CE, JO);

		if (!adjust_existed && PVZ::GetBoard().isValid())
		{
			Creator::__CreatePortal();
		}
	}
	else
	{
		PVZ::Memory::WriteMemory<byte>(0x467665, JNE);
		PVZ::Memory::WriteMemory<byte>(0x41FFB4, JNE);
		PVZ::Memory::WriteMemory<byte>(0x4248CE, JNE);

		if (adjust_existed && PVZ::GetBoard().isValid())
		{
			auto griditems = PVZ::GetBoard().GetAllGriditems();
			for (DWORD i = 0; i < griditems.size(); i++)
				if (griditems[i].Type == GriditemType::PortalBlue || griditems[i].Type == GriditemType::PortalYellow)
					griditems[i].Remove();
		}
	}
}

/// @brief 固定传送门
/// @param b 是否开启此功能
inline void FixPortal(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x4276DD, 0, 255);
}

/// @brief 设置阳光上限
/// @param sunmax 阳光上限数值
inline void SetSunMax(int sunmax)
{
	PVZ::Memory::WriteMemory<int>(0x430A1F, sunmax);
	PVZ::Memory::WriteMemory<int>(0x430A2B, sunmax);
}

/// @brief 移除阳光上限，优先级高于 SetSunMax()
/// @param b 是否开启此功能
inline void RemoveSunUpperLimit(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x430A23, JUMP, JLE);
	MEMMOD_BYTE(0x430A78, JUMP, JLE);
	MEMMOD_BYTE(0x48CAB0, JUMP, JLE);
}

/// @brief 允许叠种
/// @param b 是否开启此功能
inline void OverlapPlanting(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x40FE30, 129, 132);
	MEMMOD_BYTE(0x42A2D9, 141, 132);
	MEMMOD_BYTE(0x438E40, JUMP, JZ);
}

inline void IgnoreResources(BOOLEAN b = true)
{
	MEMMOD_INT(0x41BA72, -214234000, -215282561);
	MEMMOD_BYTE(0x41BAC0, 145, 158);
	MEMMOD_BYTE(0x42487F, JUMP, JZ);
	MEMMOD_BYTE(0x427A92, 128, 143);
	MEMMOD_BYTE(0x427DFD, 128, 143);
	MEMMOD_BYTE(0x48CAA5, 57, 41);
	PVZ::Memory::WriteMemory<LONGLONG>(0x48C7A0, b ? 174109865281658857 : 173951535625964815);
}

/// @brief 取消卡牌的冷却时间
/// @param b 是否开启此功能
inline void CancelCardCooldown(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x487296, JO, JLE);
	MEMMOD_BYTE(0x488250, JUMP, JNE);
	MEMMOD_BYTE(0x488E76, 1, 0);
}

/// @brief 取消传送带的运输冷却时间
/// @param b 是否开启此功能
inline void ConveyorBeltNoDelay(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x422D20, 128, 143);
	MEMMOD_BYTE(0x489CA1, 51, 133);
}

/// @brief 将雾的范围改为全场
/// @param b 是否开启此功能
inline void FullScreenFog(BOOLEAN b = true)
{
	MEMMOD_SHORT(0x41A476, 16363, 1397);
	MEMMOD_INT(0x41C1C0, 12828723, 9208203);
	MEMMOD_BYTE(0x41A4BA, 0, 4);
	if (b)
		for (int i = 0; i < 64; i++)
			PVZ::Memory::WriteMemory<int>(PVZBASEADDRESS + 0xC8 + 4 * i, 0);
}

/// @brief 暂停自然刷僵尸的进程
/// @param b 是否开启此功能
inline void BlockZombie(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x4265DC, JUMP, JZ);
}

/// @brief 开启罐子透视
/// @param b 是否开启此功能
inline void VasePerspect(BOOLEAN b = true)
{
	byte __vaseperspect_set[10] { 0xC7, 0x47, 0x4C, 0x64, 0, 0, 0, 0x5E, 0x59, 0xC3 };
	byte __vaseperspect_reset[10] { 0x85, 0xC0, 0x7E, 6, 0x83, 0xC0, 0xFF, 0x89, 0x47, 0x4C };
	if (b)PVZ::Memory::WriteArray<byte>(0x44E5CC, STRING(__vaseperspect_set));
	else PVZ::Memory::WriteArray<byte>(0x44E5CC, STRING(__vaseperspect_reset));
}

/// @brief 令手中的物品强制变为铲子
/// @param b 是否开启此功能
inline void LockShovel(PVZ::MousePointer* mousepointer, BOOLEAN b = true)
{
	if (b)mousepointer->Type = MouseType::Shovel;
	PVZ::Memory::WriteMemory<LONGLONG>(0x41233D, b ? -8029759805927192901 : 586669480753);
}

/// @brief 自动拾取物品
/// @param b 是否开启此功能
inline void AutoCollect(BOOLEAN b = true)
{
	byte __autocollect_set[26]
	{
		PUSHAD,
		0x8B, 0xC8,
		0x83, 0x79, 0x58, 0x10,
		0x74, 5,
		INVOKE(0x432060),
		POPAD,
		RETN(0x10),
	};

	byte __autocollect_reset[3] { 0xC2, 0x10, 0 };
	if (b)
	{
		auto board = PVZ::GetBoard();
		if (board.isValid())
		{
			auto coins = PVZ::GetBoard().GetAllCoins();
			for (auto coin : coins)
				coin.Collect();
		}
		PVZ::Memory::WriteMemory<byte>(0x40CCDA, 0xE9);
		PVZ::Memory::WriteMemory<int>(0x40CCDB, PVZ::Memory::Variable + 300 - 4 - 0x40CCDB);
		PVZ::Memory::WriteArray<byte>(PVZ::Memory::Variable + 300, STRING(__autocollect_set));
	}
	else
		PVZ::Memory::WriteArray<byte>(0x40CCDA, STRING(__autocollect_reset));
}

/// @brief 禁用初始小推车
/// @param b 是否开启此功能
inline void DisableInitialLawnmower(BOOLEAN b = true)
{
	MEMMOD_INT(0x40BC8C, 34793, 1965488771);
	MEMMOD_BYTE(0x40BC90, 0, 5);
}

/// @brief 禁用冰冻关卡延迟一段时间后播放的失败音效
/// @param b 是否开启此功能
inline void DisableIceLevelFailSound(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x42492D, JUMP, JNE);
}

/// @brief 关闭关卡内大部分内容的绘制。\n
///		建议仅在调试和测试环境下调用此函数。
/// @param b 是否开启此功能
inline void DisableBoardDraw(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x41AD23, 129, 133);
}

/// @brief 阻止新生成的粒子系统产生粒子效果。\n
///		建议仅在调试和测试环境下调用此函数。
/// @note 已生成的粒子系统不受影响。
/// @param b 是否开启此功能
inline void DisableNewParticle(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x515EBB, 129, 142);
}

/// @brief 禁用一切音效播放。\n
///		建议仅在调试和测试环境下调用此函数。
/// @note 不影响背景音乐。
/// @param b 是否开启此功能
inline void DisableAllSounds(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x554C27, JUMP, JZ);
	MEMMOD_BYTE(0x554C57, JUMP, JZ);
	MEMMOD_SHORT(0x515055, 0x4D71, 0x0A74);
}

/// @brief 设置吸金磁吸取物品数的上限。最小为 1，最大为 5 。\n
///		会影响绘制过程。
inline void SetGoldMagnetLimit(int num)
{
	if (num > 5)
		num = 5;
	else if (num < 1)
		num = 1;

	PVZ::Memory::WriteMemory<byte>(0x461D01, num);
	PVZ::Memory::WriteMemory<byte>(0x4615ED, num);
	PVZ::Memory::WriteMemory<int>(0x46549C, num);
	PVZ::Memory::WriteMemory<int>(0x4626BF, num);
}

/// @brief 禁用 IZ 下僵尸在一定条件下对防御植物造成额外啃咬伤害的效果
/// @param b 是否开启此功能
inline void DisableIZExtraEatDamage(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x52FD0B, JUMP, JZ);
}

/// @brief 禁用巨大坚果的影子偏移、眨眼动作、大小缩放的特殊设定。
/// @param b 是否开启此功能
inline void DisableGiantWallNutScale(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x465916, JUMP, JNE);
	MEMMOD_BYTE(0x487D4C, JUMP, JNE);
	MEMMOD_BYTE(0x466211, JUMP, JNE);
	MEMMOD_BYTE(0x4639E8, JUMP, JNE);
	MEMMOD_BYTE(0x463F7F, 0x80, 0x84);
}

/// @brief 将植物的子类型改为状态压缩形式。
/// @param b 是否开启此功能
inline void ConvertSubClass2Flag(bool b = true)
{
	MEMMOD_SHORT(0x4633EE, 0x47F6, 0x7F83);	
	MEMMOD_BYTE(0x4633F2, 0x74, 0x75);
}

/// @brief 禁用音乐接口更新的默认调用。\n
///		建议仅在调试和测试环境下调用此函数。
/// @note 仍可手动调用更新函数。
/// @param b 是否开启此功能
inline void DisableMusicInterfaceUpdate(BOOLEAN b = true)
{
	MEMMOD_SHORT(0x54B9B5, 0x9090, 0xD2FF);
}

/// @brief 禁用 PVZ::Music 更新的默认调用。\n
///		建议仅在调试和测试环境下调用此函数。
/// @note 仍可手动调用更新函数。
/// @param b 是否开启此功能
inline void DisableMusicUpdate(BOOLEAN b = true)
{
	MEMMOD_INT(0x452702, 0x90909090, 0x8F69E8);
	MEMMOD_BYTE(0x452706, NOP, 0);
}

/// @brief 禁止僵尸进家时触发玩家失败。
/// @param b 是否开启此功能
inline void DisableZombieFailHome(BOOLEAN b = true)
{
	MEMMOD_BYTE(0x52B308, JO, JZ);
}

/// @brief 允许潜水僵尸和海豚骑士僵尸在图内的任意位置下水。
/// @param b 是否开启此功能
inline void ZombieAlwaysDive(BOOLEAN b = true)
{
	MEMMOD_INT(0x526742, 40, 700);
	MEMMOD_INT(0x526212, 40, 700);
}

/// @brief 添加防止附件的元素数溢出的保护代码
/// @param b 是否开启此功能
inline void AttachmentOverflowGuard(BOOLEAN b = true)
{
	if (b)
	{
		BYTE code[] = { JMP(0x39), NOP };
		PVZ::Memory::WriteArray<BYTE>(0x405587, STRING(code));

		BYTE code2[] =
		{
			CMP_EUX(REG_EAX, 16),
			JL(2),
			POP_EUX(REG_ESI),
			RET,
			0x8D, 0x0C, 0x40,
			JMP(0xBC)
		};
		PVZ::Memory::WriteArray<BYTE>(0x4055C2, STRING(code2));
	}
	else
	{
		BYTE code[] = { 0x8D, 0x0C, 0x40 };
		PVZ::Memory::WriteArray<BYTE>(0x405587, STRING(code));
	}
}