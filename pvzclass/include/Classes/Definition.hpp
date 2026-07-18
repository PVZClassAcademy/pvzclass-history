#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class BaseClass;

	/// @brief 植物定义类，存储植物相关的若干常量。
	class PlantDefinition : public BaseClass
	{
	public:
		PlantDefinition(SeedType::SeedType type);
		/// @brief 植物类型
		T_READONLY_PROPERTY(SeedType::SeedType, Type, __get_Type, 0);
		/// @brief 植物的默认动画类型
		T_SAFE_PROPERTY(AnimationType::AnimationType, AnimType, __get_AnimType, __set_AnimType, 8);
		INT_READONLY_PROPERTY(PacketID, __get_PacketID, 0xC);
		/// @brief 基础阳光消耗
		INT_SAFE_PROPERTY(Cost, __get_Cost, __set_Cost, 0x10);
		/// @brief 基础种植冷却时间
		INT_SAFE_PROPERTY(Cooldown, __get_Cooldown, __set_Cooldown, 0x14);
		/// @brief 植物默认子类型。原版只有非射手（0）与射手（1）之分。
		INT_SAFE_PROPERTY(SubClass, __get_SubClass, __set_SubClass, 0x18);
		/// @brief 基础攻击间隔
		INT_SAFE_PROPERTY(AttackCooldown, __get_AttackCooldown, __set_AttackCooldown, 0x1C);
	};
	/// @brief 获取指定植物类型的定义
	/// @param type 植物类型
	/// @return 该类型植物对应的 PlantDefinition
	PlantDefinition GetPlantDefinition(SeedType::SeedType type);

	/// @brief 植物定义类，但是每个成员变量都用内存空间存储。
	/// @see PlantDefinition
	class PlantDef
	{
	public:
		/// @brief 植物类型
		SeedType::SeedType Type;
		PVZ::Image PlantImage = 0;
		/// @brief 植物的默认动画类型
		AnimationType::AnimationType AnimType;
		int PacketID;
		/// @brief 基础阳光消耗
		int Cost;
		/// @brief 基础种植冷却时间
		int Cooldown;
		/// @brief 植物默认子类型。原版只有非射手（0）与射手（1）之分。
		int SubClass;
		/// @brief 基础攻击间隔
		int LaunchRate;
		const char* Name;

		/// @brief 重新定位默认植物定义的基址，并将旧基址的所有旧内容复制到新基址上。
		/// @attention 调用该函数会导致 GetPlantDefinition() 失效。
		/// @param baseaddress 主程序中的新基址
		/// @param num 植物定义总数。
		static void Reposition(DWORD baseaddress, uint8_t num = 53);
	};

	/// @brief 僵尸定义类，存储僵尸相关的若干常量。
	class ZombieDefinition : public BaseClass
	{
	public:
		ZombieDefinition(ZombieType::ZombieType type);
		/// @brief 僵尸类型
		T_READONLY_PROPERTY(ZombieType::ZombieType, Type, __get_Type, 0);
		/// @brief 僵尸的默认动画类型
		T_SAFE_PROPERTY(AnimationType::AnimationType, AnimType, __get_AnimType, __set_AnimType, 4);
		/// @brief 僵尸的价值，影响出怪和掉落物概率
		INT_SAFE_PROPERTY(Value, __get_Value, __set_Value, 8);
		/// @brief 僵尸在冒险模式首次出现的关卡
		INT_SAFE_PROPERTY(StartingLevel, __get_StartingLevel, __set_StartingLevel, 0xC);
		/// @brief 僵尸在出怪时允许出场最小波数
		INT_SAFE_PROPERTY(FirstAllowedWave, __get_FirstAllowedWave, __set_FirstAllowedWave, 0x10);
		/// @brief 僵尸在随机选择出怪时的权重
		INT_SAFE_PROPERTY(PickWeight, __get_PickWeight, __set_PickWeight, 0x14);
		/// @brief 僵尸名称
		T_SAFE_PROPERTY(DWORD, NamePTR, __get_NamePTR, __set_NamePTR, 0x18);
	};
	/// @brief 获取指定僵尸类型的定义
	/// @param type 僵尸类型
	/// @return 该类型僵尸对应的 ZombieDefinition
	ZombieDefinition GetZombieDefinition(ZombieType::ZombieType type);

	/// @brief 弹射物定义类，存储子弹相关的若干常量。
	class ProjectileDefinition : public BaseClass
	{
	public:
		ProjectileDefinition(ProjectileType::ProjectileType type);
		/// @brief 子弹类型
		T_READONLY_PROPERTY(ProjectileType::ProjectileType, Type, __get_Type, 0);
		INT_READONLY_PROPERTY(ImageRow, __get_ImageRow, 4);
		/// @brief 子弹的基础伤害
		INT_SAFE_PROPERTY(Damage, __get_Damage, __set_Damage, 8);
	};
	/// @brief 获取指定子弹类型的定义
	/// @param type 子弹类型
	/// @return 该类型子弹对应的 ProjectileDefinition
	ProjectileDefinition GetProjectileDefinition(ProjectileType::ProjectileType type);

	/// @brief 挑战定义类，存储非冒险关卡相关的若干常量
	class ChallengeDefinition : public BaseClass
	{
	public:
		ChallengeDefinition(PVZLevel::PVZLevel mode);
		/// @brief 关卡编号
		T_READONLY_PROPERTY(PVZLevel::PVZLevel, Mode, __get_Mode, 0);
		/// @brief 关卡在界面中的图标编号
		INT_SAFE_PROPERTY(IconIndex, __get_IconIndex, __set_IconIndex, 4);
		/// @brief 关卡默认所在的页数
		INT_SAFE_PROPERTY(Page, __get_Page, __set_Page, 8);
		/// @brief 关卡图标的行数
		INT_SAFE_PROPERTY(Row, __get_Row, __set_Row, 0x0C);
		/// @brief 关卡图标的列数
		INT_SAFE_PROPERTY(Column, __get_Column, __set_Column, 0x10);
		/// @brief 关卡名称的地址
		INT_SAFE_PROPERTY(NamePTR, __get_NamePTR, __set_NamePTR, 0x14);
	};
	/// @brief 获取指定关卡类型的定义
	/// @param mode 关卡类型
	/// @return 该关卡对应的 ProjectileDefinition
	ChallengeDefinition GetChallengeDefinition(PVZLevel::PVZLevel mode);

	/// @brief 挑战定义类，但是每个成员变量都用内存空间存储。
	class ChallengeDef
	{
	public:
		PVZLevel::PVZLevel Mode;
		int IconIndex;
		int Page;
		int Row;
		int Column;
		const char* Name;

		/// @brief 重新定位默认挑战定义的基址，并将旧基址的所有旧内容复制到新基址上。
		/// @attention 调用该函数会导致 GetChallengeDefinition() ，以及 ChallengeScreen 的 ChallengeButton 失效。
		/// @param baseaddress 主程序中的新基址
		/// @param num 挑战定义总数。必须大于等于 72 且小于等于 100，且为 6 的倍数。
		static void Reposition(DWORD baseaddress, uint8_t num = 72);
	};
}