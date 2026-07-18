#pragma once
#include "../../PVZ.h"
#include "GameObject.hpp"

namespace PVZ
{
	/// @brief 卡槽对象
	class CardSlot : public GameObject
	{
	public:
		CardSlot(int address);
		/// @brief 卡槽左上角 X 坐标
		INT_PROPERTY(X, __get_X, __set_X, 8);
		/// @brief 卡槽左上角 Y 坐标
		INT_PROPERTY(Y, __get_Y, __set_Y, 0xC);
		INT_PROPERTY(CollisionLength, __get_CollisionLength, __set_CollisionLength, 0x10);
		/// @brief 卡片数量
		PROPERTY(int, __get_CardsCount, SetCardsCount) CardsCount;
		class SeedCard : public GameObject
		{
		public:
			SeedCard(int address);
			INT_PROPERTY(X, __get_X, __set_X, 0x8);
			INT_PROPERTY(Y, __get_Y, __set_Y, 0xC);
			/// @brief 获取卡牌判定范围
			/// @param collbox 存储返回值的指针
			void GetCollision(CollisionBox* collbox);
			/// @brief 设置卡牌判定范围
			/// @param collbox 设置的判定范围，不影响 X Y 坐标
			void SetCollision(CollisionBox* collbox);
			/// @brief 已冷却时间，从0开始
			INT_PROPERTY(CoolDown, __get_CoolDown, __set_CoolDown, 0x24);
			/// @brief 默认冷却时间
			INT_PROPERTY(CoolDownInterval, __get_CoolDownInterval, __set_CoolDownInterval, 0x28);
			/// @brief 卡牌编号
			INT_READONLY_PROPERTY(Index, __get_Index, 0x2C);
			/// @brief X 坐标偏移量
			INT_PROPERTY(XInConveyorBelt, __get_XInConveyorBelt, __set_XInConveyorBelt, 0x30);
			/// @brief 卡牌类型
			T_PROPERTY(SeedType::SeedType, ContentCard, __get_ContentCard, __set_ContentCard, 0x34);
			/// @brief 卡牌模仿的植物类型
			T_PROPERTY(SeedType::SeedType, ContentCardImitative, __get_ContentCardImitative, __set_ContentCardImitative, 0x38);
			/// @brief 老虎机倒计时
			INT_PROPERTY(SlotCountdown, __get_SlotCountdown, __set_SlotCountdown, 0x3C);
			/// @brief 老虎机中，下一个卡牌的类型
			T_PROPERTY(SeedType::SeedType, SlotType, __get_SlotType, __set_SlotType, 0x40);
			/// @brief 老虎机中的位置
			T_PROPERTY(FLOAT, SlotPosition, __get_SlotPosition, __set_SlotPosition, 0x44);
			/// @brief 该卡槽是否可点击
			T_PROPERTY(BOOLEAN, Enable, __get_Enable, __set_Enable, 0x48);
			/// @brief 该卡槽是否正在CD
			T_PROPERTY(BOOLEAN, Active, __get_Active, __set_Active, 0x49);
			INT_PROPERTY(UsageCount, __get_UsageCount, __set_UsageCount, 0x4C);
			/// @brief 该卡槽进入CD.
			/// @param duration 冷却时间。若为 -1，则改该卡槽的默认冷却时间
			void EnterCoolDown(int duration = -1);
			/// @brief 该卡槽被种植
			/// @note 如果是传送带则移除，不是传送带则进入CD
			void Planted();
			/// @brief 模拟点击该卡槽
			/// @note 模拟点击前会强制放下手上的东西
			void MouseDown();
			/// @brief 设置卡牌类型
			/// @param type 卡牌类型
			/// @param imitater_type 模仿者模仿的植物类型s 
			void SetPacketType(SeedType::SeedType type, SeedType::SeedType imitater_type = SeedType::None);
		};
		/// @brief 获得指定卡槽卡片
		/// @param index 卡片下标
		/// @return 指定的卡槽卡片
		PVZ::CardSlot::SeedCard GetCard(int index);
		/// @brief 给传送带添加卡片
		void AddSeed(SeedType::SeedType type);
		using SeedPacket = SeedCard;
	};
	using SeedBank = CardSlot;
}