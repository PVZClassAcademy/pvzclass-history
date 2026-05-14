#pragma once
#include "../PVZ.h"
#include "../Classes.hpp"

/// @brief 包含创建多种 PVZ 内部对象的函数
namespace Creator
{

	inline short makeshort(byte b1, byte b2)
	{
		return (b2 << 8) + b1;
	}
	inline void xytorc(int* x, int* y)
	{
		int temp = *y;
		*y = (*x - 40) / 80;
		SceneType::SceneType scene = PVZ::Memory::ReadMemory<SceneType::SceneType>(PVZBASEADDRESS + 0x554C);
		bool sixroute = (scene == SceneType::Pool) || (scene == SceneType::Fog);
		*x = sixroute ? (temp - 80) / 85 : (temp - 80) / 100;
	}

#define PI 3.1415926f
#define CREATEREANIMATION INVOKE_DWORD_DWORD_DWORD(0x453C30,0,0,0)
#define CREATEZOMBIE INVOKE_BYTE_BYTE(0x42A0F0,0,0)
#define CREATEPLANT INVOKE_DWORD_BYTE_BYTE_BYTE(0x40D120,0,0,0,255)
#define CREATEPROJECTILE INVOKE_DWORD_DWORD_BYTE_BYTE_BYTE(0x40D620,0,0,0,0,0)
#define CREATECOIN INVOKE_DWORD_DWORD_BYTE_BYTE(0x40CB10,0,0,0,0)
#define RESETLAWNMOWER INVOKE_DWORD(0x40BC70,0)
#define CREATEGRDITEM INVOKE(0x41E1C0)
#define CREATEGRAVE INVOKE(0x426620)
#define CREATECRATER INVOKE_BYTE(0x408F80,0)
#define CREATELADDER INVOKE_BYTE(0x408F40,0)
#define CHARSTRTOSTRING INVOKE(0x404450)
#define CREATECAPTION INVOKE(0x459010)
#define CREATEIMAGECAPTION INVOKE(0x40CA10)
#define CREATEEXPLOTION INVOKE_DWORD_BYTE_DWORD_DWORD_DWORD_BYTE_BYTE_BYTE(0x41D8A0,0,0,0,0,0,0,0,0)
#define _CREATEEFFECT INVOKE_DWORD_DWORD_DWORD_DWORD(0x518A70,0,0,0x61A80,0)
#define CREATEUPPERSOUND INVOKE_DWORD(0x4560C0,0)
#define FREEZEALL INVOKE(0x466420)
#define STOPSOUND INVOKE(0x515290)
#define CREATEIZOMBIEFORMATION INVOKE_DWORD(0x42A890,0)
#define CREATEVASEFORMATION INVOKE(0x4286F0)
#define CREATEPORTAL INVOKE(0x426FC0)
#define CLEARZOMBIEPREVIEW INVOKE(0x40DF70)
#define CREATEZOMBIEINLEVEL INVOKE(0x4092E0)

	/// @brief 在指定位置创建一个动画模型。
	/// @param type 动画类型
	/// @param x X 坐标
	/// @param y Y 坐标
	/// @param layer 图层大小
	/// @return 动画模型
	PVZ::Animation CreateReanimation(AnimationType::AnimationType type, float x, float y, int layer = 0x310000);

	/// @brief 在指定位置生成一个指定类型的僵尸。
	/// @note 僵王博士将会被强制生成在 0 行 0 列。
	/// @param type 僵尸类型
	/// @param row 行
	/// @param column 列
	/// @return 生成的僵尸
	PVZ::Zombie CreateZombie(ZombieType::ZombieType type, int row, byte column);

	/// @brief 在指定位置生成一个指定类型的植物。
	/// @param type 植物类型
	/// @param row 行
	/// @param column 列
	/// @param imitative 是否是模仿者生成的植物。若为 true，会产生一个模仿指定植物的模仿者。
	/// @return 生成的植物
	PVZ::Plant CreatePlant(SeedType::SeedType type, int row, byte column, BOOLEAN imitative = false);

	/// @brief 在指定位置生成一个指定类型的子弹。
	/// @note 索敌类型默认为 0x0B
	/// @param type 子弹类型
	/// @param row 行
	/// @param x X 坐标
	/// @return 生成的子弹
	PVZ::Projectile CreateProjectile(ProjectileType::ProjectileType type, byte row, int x);

	/// @brief 为部分函数建立基础。
	void AsmInit();

	/// @brief 消除 AsmInit() 产生的效应。
	void AsmReset();

	/// @brief 快速创建一个子弹。
	/// @attention 你需要先调用一次 AsmInit() 后才能使用这个函数。
	/// @see AsmInit()
	/// @param type 子弹类型
	/// @param x	X坐标
	PVZ::Projectile CreateProjectile(ProjectileType::ProjectileType type, int x, int y, float angle, float speed);

	/// @brief 在指定位置生成一个指定类型的物品。
	/// @param type 物品类型
	/// @param x X 坐标
	/// @param y Y 坐标
	/// @param motion 物品的位移类型
	/// @return 生成的物品
	PVZ::Coin CreateCoin(CoinType::CoinType type, int x, int y, CoinMotionType::CoinMotionType motion);

	/// @brief 移除场上已有的除草机，然后初始化本关的除草机。
	void ResetLawnmowers();
	/// @brief 生成一个空的场地物件
	/// @note 此物件的各项属性都需要自行设定。
	/// @return 生成的场地物件
	PVZ::Griditem CreateGriditem();

	/// @brief 在指定位置生成一个墓碑
	/// @note 此函数没有返回值。
	/// @todo 尝试实现一个可捕获返回值的版本。
	/// @param row 行
	/// @param column 列
	void CreateGrave(int row, int column);

	/// @brief 在指定位置生成一个弹坑
	/// @param row 行
	/// @param column 列
	/// @param duration 持续时间
	/// @return 生成的弹坑
	PVZ::Crater CreateCrater(int row, int column, int duration);

	/// @brief 在指定位置生成一个梯子
	/// @note 返回值为 PVZ::Griditem 类型，而不是梯子类型
	/// @param row 行
	/// @param column 列
	/// @return 生成的梯子
	PVZ::Griditem CreateLadder(int row, byte column);

	/// @brief 在指定位置生成一个包含指定内容物的罐子
	/// @param row 行
	/// @param column 列
	/// @param content 罐子内容物状态
	/// @param skin 罐子外观
	/// @param zombie 僵尸类型
	/// @param plant 植物卡牌类型
	/// @param sun 阳光数量
	/// @return 生成的罐子
	PVZ::Vase CreateVase(int row, int column, VaseContent::VaseContent content, VaseSkin::VaseSkin skin = VaseSkin::VaseSkinUnknow, ZombieType::ZombieType zombie = ZombieType::Zombie, SeedType::SeedType plant = SeedType::Peashooter, int sun = 0);

	/// @brief 在指定位置生成 IZ 模式的脑子
	/// @param row 行
	/// @param column 列。默认情况下非零值不会改变僵尸索敌范围。
	/// @return 生成的脑子
	PVZ::IZBrain CreateIZBrain(int row, int column = 0);

	/// @brief 在指定位置生成一扇传送门
	/// @param row 行
	/// @param column 列
	/// @param isYellow 是否为方传送门
	/// @return 生成的传送门
	PVZ::Portal CreatePortal(int row, int column, int isYellow = 0);

	/// @brief 待创建罐子的详细信息
	struct VaseCreateInfo
	{
		/// @brief 行
		int row;
		/// @brief 列
		int column;
		/// @brief 罐子内容物类型
		VaseContent::VaseContent content;
		/// @brief 罐子外观状态
		VaseSkin::VaseSkin skin;
		/// @brief 僵尸类型
		ZombieType::ZombieType zombie;
		/// @brief 植物卡牌类型
		SeedType::SeedType plant;
		/// @brief 阳光数量
		int sun;
	};

	/// @brief 在指定位置生成一个包含指定内容物的罐子
	/// @param vaseinfo 罐子详细信息
	/// @return 生成的罐子
	PVZ::Vase CreateVase(VaseCreateInfo vaseinfo);

	/// @brief 在指定位置生成一个钉耙
	/// @param row 行
	/// @param column 列
	PVZ::Rake CreateRake(byte row, byte column);

	/// @brief 创建文字字幕
	/// @param str 文字内容
	/// @param length 字符串长度
	/// @param style 字幕样式
	/// @param duration	字幕持续时间
	void CreateCaption(const char* str, int length, CaptionStyle::CaptionStyle style, int duration = 500);

	/// @brief 创建图片样式的文字字幕
	/// @param str 文字内容
	/// @param length 字符串长度
	void CreateImageCaption(const char* str, int length);

	/// @brief 在指定位置生成爆炸
	/// @param x 爆炸中心 X 坐标
	/// @param y 爆炸中心 Y 坐标
	/// @param radius 爆炸范围
	/// @param cinder 是否带有灰烬斩杀效果
	/// @param bound 最大行差
	/// @param fromjackzombie 爆炸是否伤害魅惑僵尸
	void CreateExplosion(int x, int y, int radius, BOOLEAN cinder = true, byte bound = 5, BOOLEAN fromjackzombie = false);

	/// @brief 播放 Sample 型音效
	/// @param sound 音效类型
	/// @see UpperSoundType::UpperSoundType
	void CreateUpperSound(UpperSoundType::UpperSoundType sound);

	/// @brief 将第一个植物视为寒冰菇，然后令其冻结全场僵尸。
	/// @attention 当前关卡必须至少存在过一个植物，此函数才有效。
	void FrozeAll();

	/// @brief 停止指定 Foley 型音效
	/// @param soundid 通过 FoleyType::FoleyType 表示的音效类型
	void StopSound(int soundid);

	/// @brief 生成指定 IZ 关卡的植物阵型
	/// @param izlevel IZ 关卡编号
	void CreateIZombieFormation(PVZLevel::PVZLevel izlevel);

	/// @brief 生成指定 VB 关卡的罐子阵型
	/// @param vblevel VB 关卡编号
	void CreateVaseFormation(PVZLevel::PVZLevel vblevel);

	/// @brief 移除场上所有传送门，然后在默认位置生成传送门
	/// @attention 你需要先调用一次 AsmInit() 后才能使用这个函数。
	/// @see AsmInit()
	void __CreatePortal();

	/// @brief 在指定位置生成传送门
	/// @attention 你需要先调用一次 AsmInit() 后才能使用这个函数。
	/// @see AsmInit()
	/// @param yellow1Row 第一扇方传送门的行
	/// @param yellow1Column 第一扇方传送门的列
	/// @param yellow2Row 第二扇方传送门的行
	/// @param yellow2Column 第二扇方传送门的列
	/// @param blue1Row 第一扇圆传送门的行
	/// @param blue1Column 第一扇圆传送门的列
	/// @param blue2Row 第二扇圆传送门的行
	/// @param blue2Column 第二扇圆传送门的列
	void CreatePortal(int yellow1Row, int yellow1Column, int yellow2Row, int yellow2Column, int blue1Row, int blue1Column, int blue2Row, int blue2Column);

	/// @brief 清除选卡界面的预览僵尸
	void __ClearZombiePreview();

	/// @brief 使用 PVZ 主程序的随机数生成器（一个 mt19937）获取一个随机非负整数。
	/// @attention 你需要先调用一次 AsmInit() 后才能使用这个函数。
	/// @attention 该函数仅供 pvzdll 使用，在应用程序直接使用会导致崩溃。
	/// @see AsmInit()
	/// @param range 随机数的上限，必须大于 0 。
	/// @return 一个小于 range 的随机非负整数
	inline int Rand(const int range)
	{
		PVZ::Memory::WriteMemoryUnsafe<int>(PVZ::Memory::Variable + 228, range);
		int (*func)() = (int (*)())(PVZ::Memory::Variable + 225);
		return func();
	}

	/// @brief 使用 PVZ 主程序的随机数生成器（一个 mt19937）获取一个随机非负浮点数。
	/// @attention 你需要先调用一次 AsmInit() 后才能使用这个函数。
	/// @attention 该函数仅供 pvzdll 使用，在应用程序直接使用会导致崩溃。
	/// @see AsmInit()
	/// @param range 随机数的上限，必须大于 0 。
	/// @return 一个小于等于 range 的随机非负整数
	inline float RandFloat(const float range)
	{
		PVZ::Memory::WriteMemoryUnsafe<float>(PVZ::Memory::Variable + 253, range);
		float (*func)() = (float (*)())(PVZ::Memory::Variable + 250);
		return func();
	}

	/// @brief 刷新出怪列表
	/// @param ztypes 僵尸类型
	/// @param length ztypes 的元素数
	/// @param wave 出怪列表波数，若为 0，表示“当前关卡的默认数值”。
	void CreateZombieInLevel(ZombieType::ZombieType* ztypes, int length, int wave = 0);

}