#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class Zombie;
	class Plant;
	class Projectile;
	class Coin;

	/// @brief 包含大部分关卡内部数据和对象的类	
	class Board : public Widget
	{
	protected:
		/// @brief 所有 GetAll() 形式函数的原型，获取 DataArray\<T\> 中的全体成员。
		/// @tparam T 成员类型
		/// @note T 必须具有 MemSize 静态常量，且类型为整数
		/// @tparam _Base_offset 基址的偏移量
		/// @tparam _Max_offset 最大数量的偏移量
		/// @tparam _T_Dead_offset 在 T 中，表示该成员已被移除变量的偏移量。此变量视为用 byte 存储。
		/// @return 装有全体 T 成员对象的 std::vector
		template<typename T, size_t _Base_offset, size_t _Max_offset, size_t _T_Dead_offset>
		std::vector<T> __prototype_GetAll()
		{
			std::vector<T> container;
			int maxnum = Memory::ReadMemory<int>(BaseAddress + _Max_offset);
			DWORD base_addr = Memory::ReadMemory<DWORD>(BaseAddress + _Base_offset);
			for (int i = 0; i < maxnum; i++)
			{
				if (!Memory::ReadMemory<byte>(base_addr + _T_Dead_offset + T::MemSize * i))
					container.push_back(T(base_addr + T::MemSize * i));
			}
			return container;
		}
	public:
		Board(int address) : Widget(address) {};

		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @note 额外的空间未经初始化，使用前请设法初始化。
		/// @param MemSize 更改后的大小。
		/// @oaran sync 扩展的空间是否在存读档时按默认方式处理
		static void SetMemSize(int NewSize, bool sync = true);

		PVZApp GetPVZApp();
		/// @brief 场上僵尸数量
		INT_READONLY_PROPERTY(ZombiesCount, __get_ZombiesCount, 0xA0);
		/// @brief 场上植物数量
		INT_READONLY_PROPERTY(PlantsCount, __get_PlantsCount, 0xBC);
		/// @brief 场上子弹数量
		INT_READONLY_PROPERTY(ProjectilesCount, __get_ProjectilesCount, 0xD8);
		/// @brief 场上掉落物数量
		INT_READONLY_PROPERTY(CoinsCount, __get_CoinsCount, 0xF4);
		/// @brief 场上小推车数量
		INT_READONLY_PROPERTY(LawnmowersCount, __get_LawnmowersCount, 0x110);
		/// @brief 场地物品总数
		INT_READONLY_PROPERTY(GriditemsCount, __get_GriditemsCount, 0x12C);
		T_SIMPLE_PROPERTY(bool, IgnoreMouseUp, 0x150);
		/// @brief 游戏是否已暂停
		T_PROPERTY(BOOLEAN, GamePaused, __get_GamePaused, __set_GamePaused, 0x164);

#pragma region fog_and_grave

		/// @brief 获得指定格的雾的浓度
		/// @param row 行
		/// @param column 列
		/// @return 雾的浓度
		int GetGridFog(int row, int column);
		/// @brief 是否启用墓碑
		/// @note 大部分生成墓碑的函数会自动调整此变量
		T_SIMPLE_PROPERTY(bool, EnableGraveStones, 0x5C4);
		INT_SIMPLE_PROPERTY(SpecialGraveStoneX, 0x5C8);
		INT_SIMPLE_PROPERTY(SpecialGraveStoneY, 0x5CC);
		/// @brief 雾的偏移
		T_PROPERTY(FLOAT, FogOffset, __get_FogOffset, __set_FogOffset, 0x5D0);
		/// @brief 雾吹飞效果倒计时
		INT_PROPERTY(FogBlownCountDown, __get_FogBlownCountDown, __set_FogBlownCountDown, 0x5D4);

#pragma endregion

		/// @brief 天降阳光倒计时
		INT_PROPERTY(SunDropCountdown, __get_SunDropCountdown, __set_SunDropCountdown, 0x5538);
		/// @brief 天降阳光数
		INT_PROPERTY(SunDropCount, __get_SunDropCount, __set_SunDropCount, 0x553C);
		/// @brief 场景类型
		PROPERTY(SceneType::SceneType, __get_LevelScene, __set_LevelScene) LevelScene;
		/// @brief 冒险模式关卡
		INT_PROPERTY(AdventureLevel, __get_AdventureLevel, __set_AdventureLevel, 0x5550);
		/// @brief 冒险模式草皮铺展位置
		INT_SIMPLE_PROPERTY(SodPosition, 0x5554);
		INT_SIMPLE_PROPERTY(PrevMouseX, 0x5558);
		INT_SIMPLE_PROPERTY(PrevMouseY, 0x555C);
		/// @brief 当前阳光数
		INT_PROPERTY(Sun, __get_Sun, __set_Sun, 0x5560);
		/// @brief 总波数
		PROPERTY(int, __get_WaveCount, __set_WaveCount) WaveCount;
		/// @brief 关卡主更新次数
		INT_PROPERTY(PlayingTime, __get_PlayingTime, __set_PlayingTime, 0x5568);
		/*include preparing time*/
		INT_READONLY_PROPERTY(PlayingTime2, __get_PlayingTime2, 0x556C);
		/*lose focus and recount*/
		INT_READONLY_PROPERTY(PlayingTime3, __get_PlayingTime3, 0x5570);
		/// @brief 当前波数
		INT_READONLY_PROPERTY(CurrentWave, __get_CurrentWave, 0x557C);
		/// @brief 已刷新波数
		INT_READONLY_PROPERTY(RefreshedWave, __get_RefreshedWave, 0x5580);
		/// @brief 教程状态
		INT_PROPERTY(FlashTip, __get_FlashTip, __set_FlashTip, 0x5584);
		/*Flash tips for novice tutorials*/
		INT_PROPERTY(RefreshHp, __get_RefreshHp, __set_RefreshHp, 0x5594);
		INT_READONLY_PROPERTY(CurrentWaveHp, __get_CurrentWaveHp, 0x5598);
		/// @brief 下一波倒计时
		INT_PROPERTY(NextWaveCountdown, __get_NextWaveCountdown, __set_NextWaveCountdown, 0x559C);
		/// @brief 下一波倒计时的初值
		INT_READONLY_PROPERTY(NextWaveCountdownInitialValue, __get_NextWaveCountdownInitialValue, 0x55A0);
		/// @brief 一大波僵尸的倒计时
		INT_PROPERTY(HugeWaveCountdown, __get_HugeWaveCountdown, __set_HugeWaveCountdown, 0x55A4);
		/// @brief 是否显示铲子
		T_PROPERTY(BOOLEAN, HaveShovel, __get_HaveShovel, __set_HaveShovel, 0x55F1);
		/// @brief 金钱数显示消失倒计时
		INT_PROPERTY(ShowMoneyCountdown, __get_ShowMoneyCountdown, __set_ShowMoneyCountdown, 0x55F4);
		/// @brief 调试模式
		T_PROPERTY(DebugModeType::DebugModeType, DebugMode, __get_DebugMode, __set_DebugMode, 0x55F8);
		/// @brief 关卡是否已完成
		T_PROPERTY(byte, LevelComplete, __get_LevelComplete, __set_LevelComplete, 0x55FC);
		/// @brief 进入下一阶段的倒计时
		/// @note 解谜无尽实际上也使用这个变量
		INT_PROPERTY(NextSurvivalStageCounter, __get_NextSurvivalStageCounter, __set_NextSurvivalStageCounter, 0x5604);
		/// @brief 过关奖励是否已生成
		T_PROPERTY(byte, LevelAwardSpawned, __get_LevelAwardSpawned, __set_LevelAwardSpawned, 0x560C);
		/// @brief 关卡进度条
		INT_PROPERTY(LevelProcessBar, __get_LevelProcessBar, __set_LevelProcessBar, 0x5610);
		/// @brief 时停效果倒计时
		/// @note 若使用，你需要自己手动进行倒计时
		INT_SIMPLE_PROPERTY(TimeStopCounter, 0x5748);
		/// @brief 是否激活 Mustatche
		T_PROPERTY(BOOLEAN, Mustache, __get_Mustache, __set_Mustache, 0x5761);
		/// @brief 是否激活 Trickedout
		T_PROPERTY(BOOLEAN, Trickedout, __get_Trickedout, __set_Trickedout, 0x5762);
		/// @brief 是否激活 Future
		T_PROPERTY(BOOLEAN, Future, __get_Future, __set_Future, 0x5763);
		/// @brief 是否激活 Pinata
		T_PROPERTY(BOOLEAN, Pinata, __get_Pinata, __set_Pinata, 0x5764);
		/// @brief 是否激活 Dance
		T_PROPERTY(BOOLEAN, Dance, __get_Dance, __set_Dance, 0x5765);
		/// @brief 是否激活 Daisies
		T_PROPERTY(BOOLEAN, Daisies, __get_Daisies, __set_Daisies, 0x5766);
		/// @brief 是否激活 Sukhbir
		T_PROPERTY(BOOLEAN, Sukhbir, __get_Sukhbir, __set_Sukhbir, 0x5767);
		/// @brief 被吃掉的植物总数
		INT_READONLY_PROPERTY(EatenPlants, __get_EatenPlants, 0x5798);
		/// @brief 被铲除的植物总数
		INT_READONLY_PROPERTY(ShoveledPlants, __get_ShoveledPlants, 0x579C);

		/// @brief 获取出怪类型
		/// @param ztypes 返回值存放位置
		void GetZombieAllowed(ZombieType::ZombieType* ztypes);

		/// @brief 判断当前场景是否为六行场地
		READONLY_PROPERTY_BINDING(
			BOOLEAN,
			__get_SixRoute,
			(LevelScene == SceneType::Pool) || (LevelScene == SceneType::Fog)) SixRoute;

#pragma region methods

		/// @brief 将列数转换为 X 坐标
		/// @param row 行
		/// @param column 列
		/// @return 对应 X 坐标
		int GridToXPixel(int row, int column);
		/// @brief 将行数转换为 Y 坐标
		/// @param row 行
		/// @param column 列
		/// @return 对应 Y 坐标
		int GridToYPixel(int row, int column);
		/// @brief 将坐标转换为行数
		/// @todo 支持禅境花园
		int PixelToRow(int x, int y);
		/// @brief 将坐标转换为行数，一定在场地范围内
		/// @todo 支持禅境花园
		/// @return 坐标所在行数
		int PixelToRowKeepOnBoard(int x, int y);
		/// @brief 将坐标转换为列数
		/// @todo 支持禅境花园
		int PixelToCol(int x, int y);
		/// @brief 将坐标转换为列数，一定在场地范围内
		/// @todo 支持禅境花园
		/// @return 坐标所在列数
		int PixelToColKeepOnBoard(int x, int y);
		/// @brief 设置突袭（墓碑刷怪、水下僵尸、蹦极空降）僵尸的倒计时
		/// @param countdown 设置的倒计时，默认为 0.01 秒。
		void Assault(int countdown = 1);
		/// @brief 设置最后一波音效倒计时
		/// @param countdown 设置的倒计时，默认为 0.01 秒。
		void Bell(int countdown = 1);
		/// @brief 震动画面面板
		/// @param horizontalAmplitude X 轴方向的震动大小
		/// @param verticalAmplitude 
		/// @param duration 震动持续时间
		void Earthquake(int horizontalAmplitude = 2, int verticalAmplitude = 4, int duration = 20);
		/// @brief 立刻失败。
		void Lose();
		/// @brief 若当前可以承担 amount 点阳光的支出，则消耗 theAmount 阳光，
		//	否则触发阳光数量不足的的音效和闪红特效。
		/// @param amount 阳光消耗数值。
		/// @return 是否可以承担支出。
		bool TakeSunMoney(int amount);
		/// @brief 立刻获胜。
		void Win();
		/// @brief 保存游戏存档。
		/// @param path 存档路径
		/// @param pathlen path 的长度
		/// @return 是否保存成功。
		bool Save(const char* path, int pathlen);
		/// @brief 读取游戏存档。
		/// @param path 存档路径
		/// @param pathlen path 的长度
		/// @return 是否载入成功
		bool Load(const char* path, int pathlen);
		/// @brief 计算空的植物，原版用于计算空花盆或者睡莲的数量。
		/// @return 空的植物的数量
		int CountEmptyPlants(SeedType::SeedType type);
		/// @brief 执行一次额外更新，可以用于加速游戏进行。
		/// @note 只更新玩法相关，是僵尸快跑的实现方式，传送带、字幕等不会更新。
		void UpdateGame();
		/// @brief 执行一次额外更新，可以用于加速游戏进行。
		/// @note 是总更新接口，更新关卡内的各种东西。
		/// @note 注意，部分更新和清理是在 app 的更新中进行的。
		/// @see RemoveNotExistGameObjects RemoveNotExistEffects
		void Update();
		/// @brief 清除所有不存在的游戏对象并释放内存。
		/// @note Board::Update() 如果连续多次调用则需要使用该函数清理。
		/// @note 注意不要清理掉了后续还要被使用的对象。
		void RemoveNotExistGameObjects();

		/// @brief 更新游戏对象（僵尸、植物、子弹等）
		void UpdateGameObjects();
		/// @brief 更新阳光生成
		void UpdateSunSpawning();
		/// @brief 更新僵尸生成
		void UpdateZombieSpawning();
		/// @brief 更新冰道效果
		void UpdateIce();
		/// @brief 更新场地物品
		void UpdateGridItems();
		/// @brief 更新迷雾效果
		void UpdateFog();

		/// @brief 加载读取游戏存档。通过 theFilePath 指定需要写入的存档文件的完整路径及文件名。
		/// @return 是否载入成功
		bool LawnLoadGame(PVZ::PVZString theFilePath);
		/// @brief 保存游戏存档。通过 theFilePath 指定需要写入的存档文件的完整路径及文件名。
		/// @return 是否保存成功
		bool LawnSaveGame(PVZ::PVZString theFilePath);
#pragma endregion

#pragma region getmethod
		/// @brief 获取 DataArray\<Zombie\> 中的全体对象。
		/// @tparam T 成员的类型，必须为 Zombie 或它的派生类。
		/// @return 装有全体 Zombie （或者其派生类）对象的 std::vector
		template<typename T = Zombie, typename = enable_if_t<is_base_of<Zombie, T>::value>>
		std::vector<T> GetAllZombies()
		{
			return __prototype_GetAll<T, 0x90, 0x94, 0x0EC>();
		}
		/// @brief 获取 DataArray\<Plant\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Plant 或它的派生类。
		/// @return 装有全体 Plant （或者其派生类）对象的 std::vector
		template<typename T = Plant, typename = enable_if_t<is_base_of<Plant, T>::value>>
		std::vector<T> GetAllPlants()
		{
			return __prototype_GetAll<T, 0x0AC, 0x0B0, 0x141>();
		}
		/// @brief 获取 DataArray\<Projectile\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Projectile 或它的派生类。
		/// @return 装有全体 Projectile （或者其派生类）对象的 std::vector
		template<typename T = Projectile, typename = enable_if_t<is_base_of<Projectile, T>::value>>
		std::vector<T> GetAllProjectiles()
		{
			return __prototype_GetAll<T, 0x0C8, 0x0CC, 0x50>();
		}
		/// @brief 获取 DataArray\<Coin\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Coin 或它的派生类。
		/// @return 装有全体 Coin （或者其派生类）对象的 std::vector
		template<typename T = Coin, typename = enable_if_t<is_base_of<Coin, T>::value>>
		std::vector<T> GetAllCoins()
		{
			return __prototype_GetAll<T, 0x0E4, 0x0E8, 0x38>();
		}
		/// @brief 获取 DataArray\<LawnMower\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 LawnMower 或它的派生类。
		/// @return 装有全体 LawnMower （或者其派生类）对象的 std::vector
		template<typename T = LawnMower, typename = enable_if_t<is_base_of<LawnMower, T>::value>>
		std::vector<T> GetAllLawnmowers()
		{
			return __prototype_GetAll<T, 0x100, 0x104, 0x30>();
		}
		/// @brief 获取 DataArray\<Griditem\> 中的全体对象。
		/// @note 与其他 GetAll() 不同，此函数不依赖于 __prototype_GetAll() 。
		/// @note 类型不为 T 的 Griditem 会被滤去。
		/// @tparam T 成员值的类型，必须为 Griditem 或它的派生类。
		/// @return 装有全体 Griditem （或者其派生类）对象的 std::vector
		template<typename T = Griditem, typename = enable_if_t<is_base_of<Griditem, T>::value>>
		std::vector<T> GetAllGriditems()
		{
			std::vector<T> griditems;
			int maxnum = Memory::ReadMemory<int>(BaseAddress + 0x120);
			DWORD base_addr = Memory::ReadMemory<DWORD>(BaseAddress + 0x11C);
			for (int i = 0; i < maxnum; i++)
			{
				if (!Memory::ReadMemory<byte>(base_addr + 0x20 + T::MemSize * i)
					&& (T::ItemType == 0 || Memory::ReadMemory<byte>(base_addr + 8 + T::MemSize * i) == T::ItemType))
					griditems.push_back(T(base_addr + T::MemSize * i));
			}
			return griditems;
		}
		Lawn GetLawn();
		Icetrace GetIcetrace();
		Wave GetWave(int index);
		MousePointer GetMousePointer();
		GameButton GetMenuButton();
		Caption GetCaption();
		CardSlot GetCardSlot();
		CutScene GetCutScene();

		/// @brief 获取 Challenge 类型的成员。
		/// @tparam T 返回值的类型，必须为 Challenge 或它的派生类。
		/// @return Challenge （或者其派生类）成员对象 
		template<typename T = Challenge, typename = enable_if_t<is_base_of<Challenge, T>::value>>
		T GetChallenge()
		{
			return T(BaseAddress);
		}
		/// @brief 获取 Challenge 类型的成员。
		/// @attention 与 GetChallenge() 不同，此方法只能获得 Challenge 类型的对象。
		/// @return Challenge 成员对象 
		Challenge GetMiscellaneous();
		/// @brief 取得指定坐标处存在的僵尸。
		/// @note 关卡引入阶段和选卡阶段内的植物僵尸会被此函数忽略。
		/// @param mouse_x X 坐标
		/// @param mouse_y Y 坐标
		/// @return 指定坐标处的僵尸。可能为空。
		Zombie ZombieHitTest(int mouse_x, int mouse_y);
#pragma endregion
	};
	/// @brief 控制行地形类型和每个格位地形类型的类
	class Lawn : public BaseClass
	{
	public:
		Lawn(int baseaddress);
		/// @brief 获取指定格位的地形类型
		/// @param row 行
		/// @param column 列
		/// @return 地形类型
		LawnType::LawnType GetGridType(int row, int column);
		/// @brief 设置指定格位的地形类型
		/// @param row 行
		/// @param column 列
		/// @param type 设置后的地形类型
		void SetGridType(int row, int column, LawnType::LawnType type);
		/// @brief 获取指定行的地形类型
		/// @param route 行
		/// @return 地形类型
		RouteType::RouteType GetRouteType(int route);
		/// @brief 设置指定行的地形类型
		/// @note 该函数不会影响该行任何格位的地形类型，需要用 SetGridType() 另行改动。
		/// @param route 行
		/// @param type 设置后的地形类型
		void SetRouteType(int route, RouteType::RouteType type);
		/// @brief 判断指定类型卡牌是否可以在指定位置上使用。
		/// @param row 行
		/// @param column 列
		/// @param type 卡牌类型
		/// @return 是否可以使用
		bool Plantable(int row, int column, SeedType::SeedType type);
	};
	/// @brief 冰道
	class Icetrace : public BaseClass
	{
	public:
		Icetrace(int baseaddress);
		/// @brief 获取指定行的冰道最左侧 X 坐标
		/// @param route 行
		/// @return 冰道最左侧 X 坐标
		int GetX(int route);
		/// @brief 设置指定行的冰道最左侧 X 坐标
		/// @param route 行
		/// @param x 设置后的 X 坐标
		void SetX(int route, int x);
		/// @brief 获取指定行的冰道消失倒计时
		/// @param route 行
		/// @return 消失倒计时
		int GetDisappearCountdown(int route);
		/// @brief 设置指定行的冰道消失倒计时
		/// @param route 行
		/// @param cs 设置的消失倒计时，单位为厘秒。
		void SetDisappearCountdown(int route, int cs);
	};
	/// @brief 一波僵尸的出怪列表
	class Wave : public BaseClass
	{
	public:
		Wave(int baseaddress);
		/// @brief 此波的总僵尸数
		READONLY_PROPERTY(int, __get_Count) Count;
		/// @brief 获取此波所有僵尸
		/// @param ztypes 存储返回值的数组
		void GetAll(ZombieType::ZombieType* ztypes);
		/// @brief 设置此波僵尸。若不足 50 个，应当用 ZombieType::None 结尾。
		/// @param ztypes 僵尸列表
		/// @param length 僵尸列表长度
		void SetAll(ZombieType::ZombieType* ztypes, size_t length);
		/// @brief 获取出怪列表指定编号的僵尸
		/// @param index 僵尸在出怪列表中的编号
		/// @return 僵尸类型
		ZombieType::ZombieType Get(int index);
		/// @brief 设置指定编号上的僵尸。
		/// @note 不能以此法增加僵尸数量。若要增加，请使用 Add()
		/// @param index
		/// @param ztype
		void Set(int index, ZombieType::ZombieType ztype);
		/// @brief 删除指定编号上的僵尸
		/// @param index 编号
		void Del(int index);
		/// @brief 将指定类型的僵尸添加到列表末尾。
		/// @param ztype 僵尸类型
		void Add(ZombieType::ZombieType ztype);
		/// @brief 将指定数组的全部僵尸加到本波出怪列表中。溢出的部分会被忽略。
		/// @param ztypes 僵尸类型数组
		/// @param length 数组长度
		void AddAll(ZombieType::ZombieType* ztypes, int length);
	};

	Board GetBoard();
}