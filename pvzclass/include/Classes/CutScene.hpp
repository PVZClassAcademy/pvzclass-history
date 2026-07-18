#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class CutScene : public BaseClass
	{
	public:
		CutScene(int address);

        /// @brief 通用过场动画时长
        INT_SIMPLE_PROPERTY(mCutsceneTime, 0x08);
        /// @brief 草皮滚动展开动画时长
        INT_SIMPLE_PROPERTY(mSodTime, 0x0C);
        /// @brief 墓碑生长动画时长
        INT_SIMPLE_PROPERTY(mGraveStoneTime, 0x10);
        /// @brief ReadySetPlant字幕动画时长
        INT_SIMPLE_PROPERTY(mReadySetPlantTime, 0x14);
        /// @brief 浓雾入场动画时长
        INT_SIMPLE_PROPERTY(mFogTime, 0x18);
        /// @brief BOSS战动画时长
        INT_SIMPLE_PROPERTY(mBossTime, 0x1C);
        /// @brief 疯狂戴夫动画时长
        INT_SIMPLE_PROPERTY(mCrazyDaveTime, 0x20);
        /// @brief 割草机入场动画时长
        INT_SIMPLE_PROPERTY(mLawnMowerTime, 0x24);
        /// @brief 疯狂戴夫对话开始时间点
        INT_SIMPLE_PROPERTY(mCrazyDaveDialogStart, 0x28);

        /// @brief 是否处于选卡阶段
        T_SIMPLE_PROPERTY(bool, mSeedChoosing, 0x2C);
        /// @brief 资源是否预加载完成
        T_SIMPLE_PROPERTY(bool, mPreloaded, 0x34);
        /// @brief 是否已放置街上的僵尸
        T_SIMPLE_PROPERTY(bool, mPlacedZombies, 0x35);
        /// @brief 是否已放置草坪道具
        T_SIMPLE_PROPERTY(bool, mPlacedLawnItems, 0x36);

		/// @brief 放置街上的僵尸
		/// @note x 和 y 是街上的坐标，不是场地中的格子坐标
		/// @note x 和 y 的取值应该是 0,1,2,3,4
		/// @note 在植物僵尸 2 关卡中，创建鸭子救生圈僵尸会转化为创建豌豆射手僵尸。
		void PlaceStreetZombie(ZombieType::ZombieType type, int x, int y);
		/// @brief 寻找空位放置街上的僵尸
		/// @param type 僵尸类型
		/// @param grid 该格子是否已被占用
		/// @note grid 数组会被实时修改更新
		void PlaceStreetZombie(ZombieType::ZombieType type, bool grid[5][5]);
		/// @brief 取消过场动画
		void CancelIntro();
	};
}
