#pragma once
#include "../../PVZ.h"
#include "Board.hpp"

namespace PVZ
{
	class Widget;
	class Board;
	/// @brief 选卡界面控件
	class SeedChooserScreen : public Widget
	{
	public:
		SeedChooserScreen(int address) : Widget(address) {};
		/// @brief 每个可选的卡牌
		class ChosenSeed : public BaseClass
		{
		public:
			ChosenSeed(int address) : BaseClass(address) {};
			/// @brief X 坐标
			INT_PROPERTY(X, __get_X, __set_X, 0);
			/// @brief Y 坐标
			INT_PROPERTY(Y, __get_Y, __set_Y, 4);
			/// @brief 卡牌类型
			T_PROPERTY(SeedType::SeedType, Type, __get_Type, __set_Type, 0x20);
			/// @brief 卡牌在选卡界面中的状态
			T_PROPERTY(SeedState::SeedState, State, __get_State, __set_State, 0x24);
			/// @brief 卡牌在卡槽中的序号
			INT_PROPERTY(IndexInSlot, __get_IndexInSlot, __set_IndexInSlot, 0x28);
			/// @brief 此卡牌是否在冷却中
			T_PROPERTY(BOOLEAN, Refreshing, __get_Refreshing, __set_Refreshing, 0x2C);
			/// @brief 冷却倒计时
			INT_PROPERTY(RefreshCounter, __get_RefreshCounter, __set_RefreshCounter, 0x30);
			/// @brief 模仿者模仿类型
			T_PROPERTY(SeedType::SeedType, ImitaterType, __get_ImitaterType, __set_ImitaterType, 0x34);
			/// @brief 是否是疯狂戴夫的选择
			T_PROPERTY(BOOLEAN, CrazyDavePick, __get_CrazyDavePick, __set_CrazyDavePick, 0x38);
		};
		/// @brief 获取选卡界面所在的 Board
		/// @return 选卡界面所在的 Board
		Board GetBoard();
		/// @brief 获取界面中指定的卡牌
		/// @param num 卡牌 ID
		/// @return 指定 ID 的卡牌
		ChosenSeed GetChosenSeed(int num);
		/// @brief 是否在查看草坪
		T_PROPERTY(BOOLEAN, IsViewingLawn, __get_IsViewingLawn, __set_IsViewingLawn, 0x0D38);
	};
	/// @brief 获取选卡界面控件
	/// @return 选卡界面对象。
	SeedChooserScreen GetSeedChooserScreen();
}