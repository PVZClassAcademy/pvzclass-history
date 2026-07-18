#pragma once
#include "../../PVZ.h"
#include "Board.hpp"
#include "Griditem.hpp"

namespace PVZ
{
	class PVZApp;
	class Board;
	class Snail;

	/// @brief 禅境花园类，包含部分与禅境花园有关的函数。
	class ZenGarden : public BaseClass
	{
	public:
		ZenGarden(int address) : BaseClass(address) {};
		PVZApp GetLawnApp();
		PVZ::Board GetBoard();
		/// @brief 当前所在的花园场景
		T_PROPERTY(GardenScene::GardenScene, GardenType, __get_GardenType, __set_GardenType, 0x8);
		/// @brief 判断花园是否已满
		/// @param consider_items 是否考虑场上掉落的物品
		/// @return 花园是否已满
		bool IsFull(bool consider_items);
		/// @brief 获取花园中的蜗牛
		/// @return 蜗牛
		Snail GetSnail();
	};
	ZenGarden GetZenGarden();
}
