#pragma once
#include "../../PVZ.h"
#include "Griditem.hpp"

namespace PVZ
{
	/// @brief 存储小游戏、解谜、生存等特殊模式的数据的类。
	class Challenge : public BaseClass
	{
	public:
		Challenge(int address);
		static DWORD MemSize;
		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @param MemSize 更改后的大小。
		/// @return 是否修改成功。
		static bool SetMemSize(int NewSize);

		/// @brief 是否在拖拽植物
		T_READONLY_PROPERTY(BOOLEAN, DragingPlant, __get_DragingPlant, 8);
		/// @brief 宝石迷阵类模式中，拖拽的 X 格位
		INT_READONLY_PROPERTY(DragingX, __get_DragingX, 0xC);
		/// @brief 宝石迷阵类模式中，拖拽的 Y 格位
		INT_READONLY_PROPERTY(DragingY, __get_DragingY, 0x10);
		/// @brief 检测指定位置是否有宝石迷阵模式的弹坑
		/// @param row 行
		/// @param column 列
		/// @return 指定位置是否有弹坑
		BOOLEAN HaveCrater(int row, int column);
		/// @brief 设置指定位置是否有宝石迷阵模式的弹坑
		/// @param row 行
		/// @param column 列
		/// @param b 指定位置是否将有弹坑
		void SetCrater(int row, int column, BOOLEAN b);
		/// @brief 是否升级双发
		T_PROPERTY(BOOLEAN, UpgradedRepeater, __get_UpgradedRepeater, __set_UpgradedRepeater, 0x4A);
		/// @brief 是否升级大喷菇
		T_PROPERTY(BOOLEAN, UpgradedFumeshroom, __get_UpgradedFumeshroom, __set_UpgradedFumeshroom, 0x4B);
		/// @brief 是否升级高坚果
		T_PROPERTY(BOOLEAN, UpgradedTallnut, __get_UpgradedTallnut, __set_UpgradedTallnut, 0x4C);
		/// @brief 宝石迷阵模式中，自移动植物起连续达成的消除次数。
		INT_PROPERTY(BeghouledMatchesThisMove, __get_BeghouledMatchesThisMove, __set_BeghouledMatchesThisMove, 0x50);
		/// @brief 小游戏状态
		/// @see ChallengeState
		T_PROPERTY(ChallengeState::ChallengeState, State, __get_State, __set_State, 0x54);
		/// @brief 状态倒计时
		INT_PROPERTY(AttributeCountdown, __get_AttributeCountdown, __set_AttributeCountdown, 0x58);
		/// @brief 传送带传送下一张卡的倒计时
		INT_PROPERTY(ConveyorCountdown, __get_ConveyorCountdown, __set_ConveyorCountdown, 0x5C);
		/// @brief 关卡进程
		INT_PROPERTY(LevelProcess, __get_LevelProcess, __set_LevelProcess, 0x60);
		/// @brief 是否绘制保龄球关卡的红线
		T_PROPERTY(BOOLEAN, ShowBowlingLine, __get_ShowBowlingLine, __set_ShowBowlingLine, 0x64);
		/// @brief 传送带最近一次传送的卡牌类型
		T_PROPERTY(SeedType::SeedType, ConveyorLastCard, __get_ConveyorLastCard, __set_ConveyorLastCard, 0x68);
		/// @brief 关卡轮数
		INT_PROPERTY(Round, __get_Round, __set_Round, 0x6C);
		/// @brief 老虎机使用次数
		INT_PROPERTY(SlotMachineRollCount, __get_SlotMachineRollCount, __set_SlotMachineRollCount, 0x70);
		INT_PROPERTY(ChallengeGridX, __get_ChallengeGridX, __set_ChallengeGridX, 0x0A8);
		INT_PROPERTY(ChallengeGridY, __get_ChallengeGridY, __set_ChallengeGridY, 0x0AC);
		/// @brief 初始罐子数量
		INT_READONLY_PROPERTY(VaseCount, __get_VaseCount, 0x0B0);
		/// @brief 下雨相关的倒计时
		INT_PROPERTY(RainCounter, __get_RainCounter, __set_RainCounter, 0x0B4);
		/// @brief 智慧树话语序号
		INT_READONLY_PROPERTY(TreeOfWisdomTalkIndex, __get_TreeOfWisdomTalkIndex, 0x0B8);

		/// @brief 获取另一个同类型的场地物品
		/// @param griditem 场地物品
		/// @return 另一个同类型的场地物品，可能为空。
		PVZ::Griditem GetAnotherSameTypeGriditem(PVZ::Griditem griditem);

		/// @brief 压扁指定的脑子，并计算因此产生的得分
		/// @param brain 将被压扁的脑子
		/// @see IZBrain
		void IZSquishBrain(IZBrain brain);
		/// @brief 在指定行随机种植若干个指定类型的植物，并对其进行 IZ 关卡的特殊调整。
		/// @param type 植物类型
		/// @param count 种植数量
		/// @param row 指定的行。若为 -1，则没有行限制。
		void IZPlacePlants(SeedType::SeedType type, int count, int row);
	};
	using Miscellaneous = Challenge;
}