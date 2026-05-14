#pragma once
#include "../PVZ.h"

namespace PVZ
{
	class PVZApp;

	/// @brief 控制存档相关内容的类
	class SaveData : public BaseClass
	{
	public:
		SaveData(int baseaddress) : BaseClass(baseaddress) {};
		/// @brief 获取当前用户名称
		/// @param str 保存返回值的位置（char* 形式）
		void GetPVZUserName(char str[]);//str[12]
		INT_READONLY_PROPERTY(UserSwitchCount, __get_UserSwitchCount, 0x1C);
		/// @brief 用户 ID
		INT_READONLY_PROPERTY(UserIndex, __get_UserIndex, 0x20);
		/// @brief 冒险模式进度
		INT_PROPERTY(AdventureLevel, __get_AdventureLevel, __set_AdventureLevel, 0x24);
		/// @brief 金钱总数
		INT_PROPERTY(Money, __get_Money, __set_Money, 0x28);
		/// @brief 冒险模式全通次数
		INT_PROPERTY(AdventureFinishCount, __get_AdventureFinishCount, __set_AdventureFinishCount, 0x2C);
		/// @brief 获取当前用户是否购买了指定物品，或者指定物品的剩余数量。
		/// @param item 物品类型
		/// @return 物品数量。
		/// @retval 0 对于部分物品，这个值表示“未购买过”，而不仅仅是没有。\n
		///		对于这些物品，若购买过，则它们的实际数量是（返回值 - 1000）。
		int GetPurchase(StoreItem::StoreItem item);
		/// @brief 设定当前用户是否购买了指定物品，或者指定物品的剩余数量。
		/// @param item 物品类型
		/// @param val 被设定的物品数量
		/// @see GetPurchase
		void SetPurchase(StoreItem::StoreItem item, int val);
		/// @brief 获取当前用户通过某一关卡的次数，或者最高波数的通关记录。
		/// @param mode 关卡模式
		int GetChallengeRecord(PVZLevel::PVZLevel mode);
		/// @brief 设定当前用户通过某一关卡的次数，或者最高波数的通关记录。
		/// @param mode 关卡模式
		/// @param val 更改后的数值
		void SetChallengeRecord(PVZLevel::PVZLevel mode, int val);
		/// @brief 智慧树高度
		INT_PROPERTY(TreeHight, __get_TreeHight, __set_TreeHight, 0xF4);
		/// @brief 判定是否购买紫卡植物。如果不是原版的紫卡植物，也会返回 false 。
		/// @param purplecard 植物类型
		/// @return 是否为已购买的紫卡
		BOOLEAN HavePurpleCard(SeedType::SeedType purplecard);
		T_PROPERTY(BOOLEAN, HaveImitater, __get_HaveImitater, __set_HaveImitater, 0x1E0);
		T_PROPERTY(BOOLEAN, HaveGoldenWatering, __get_HaveGoldenWatering, __set_HaveGoldenWatering, 0x1F4);
		INT_PROPERTY(Fertilizer, __get_Fertilizer, __set_Fertilizer, 0x1F8);//-1000
		INT_PROPERTY(BugSpray, __get_BugSpray, __set_BugSpray, 0x1FC);//-1000
		T_PROPERTY(BOOLEAN, HaveMusicBox, __get_HaveMusicBox, __set_HaveMusicBox, 0x200);
		T_PROPERTY(BOOLEAN, HaveGardeningGlove, __get_HaveGardeningGlove, __set_HaveGardeningGlove, 0x204);
		T_PROPERTY(BOOLEAN, HaveMushroomGarden, __get_HaveMushroomGarden, __set_HaveMushroomGarden, 0x208);
		T_PROPERTY(BOOLEAN, HaveWheelBarrow, __get_HaveWheelBarrow, __set_HaveWheelBarrow, 0x20C);
		T_PROPERTY(BOOLEAN, HaveSnail, __get_HaveSnail, __set_HaveSnail, 0x210);
		/// @brief 卡槽数量
		INT_PROPERTY(CardSlotNum, __get_CardSlotNum, __set_CardSlotNum, 0x214);
		T_PROPERTY(BOOLEAN, HavePoolCleaner, __get_HavePoolCleaner, __set_HavePoolCleaner, 0x218);
		T_PROPERTY(BOOLEAN, HaveRoofCleaner, __get_HaveRoofCleaner, __set_HaveRoofCleaner, 0x21C);
		/// @brief 剩余钉耙数量
		INT_PROPERTY(LeftRakeCount, __get_LeftRakeCount, __set_LeftRakeCount, 0x220);
		T_PROPERTY(BOOLEAN, HaveAquarium, __get_HaveAquarium, __set_HaveAquarium, 0x224);
		INT_PROPERTY(Chocolate, __get_Chocolate, __set_Chocolate, 0x228);//-1000
		T_PROPERTY(BOOLEAN, HaveTreeOfWisdom, __get_HaveTreeOfWisdom, __set_HaveTreeOfWisdom, 0x22C);
		INT_PROPERTY(TreeFood, __get_TreeFood, __set_TreeFood, 0x230);//-1000
		T_PROPERTY(BOOLEAN, HaveWallnutFirstAid, __get_HaveWallnutFirstAid, __set_HaveWallnutFirstAid, 0x234);
		/// @brief 花园盆栽数量
		INT_READONLY_PROPERTY(GardenPlantCount, __get_GardenPlantCount, 0x350);
		/// @brief 获取花园盆栽
		/// @param index 下标
		/// @return 花园盆栽对象
		GardenPlant GetGardenPlant(int index);
	};
	using PlayerInfo = SaveData;

	SaveData GetSaveData();
}


