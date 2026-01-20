# 目录
- [3.0](#3.0)
- [2.7.1](#2.7.1)
- [2.7](#2.7)
- [2.6](#2.6)
- [2.5.1](#2.5.1)
- [2.5](#2.5)
- [2.4](#2.4)
- [2.3](#2.3)
- [2.2.1](#2.2.1)
- [2.2](#2.2)
- [2.1.2](#2.1.2)
- [2.1.1](#2.1.1)
- [2.1](#2.1)
- [2.0.1](#2.0.1)
- [2.0](#2.0)
- [1.17.2](#1.17.2（2025/1/25）)
- [1.17.1](#1.17.1（2025/1/4）)
- [1.17](#1.17（2024/12/23）)
- [1.16.2](#1.16.2（2024/10/19）)
- [1.16.1](#1.16.1（2024/10/2）)
- [1.16](#1.16（2024/9/6）)
- [1.15.3](#1.15.3（2024/1/25）)
- [1.15.2](#1.15.2（2023/11/24）)
- [1.15.1](#1.15.1（2023/11/22）)
- [1.15.0](#1.15.0（2023/10/27）)
- [1.14.3](#1.14.3（2023/10/22）)

## 3.0

### 说明

**注意**：该版本目前处于开发阶段。

本章的内容会随着开发版本的更新而更新和完善。

### 新增内容

- `PVZ::PVZString` 及其相关的一系列函数，用于取代 `Draw::PString`。
  - `Draw::PString` 已被彻底移除。
- `PVZApp` 新增一系列加载 XML 配置文件和获取配置项的函数。

### 改动内容

- `BaseClass::BaseAddress` 现在是 `uint32_t` 类型的变量。
- 将大部分原本位于 `Draw` 命名空间的函数迁移到 `PVZ::Graphics` 中。
- 清除了 pvzdll 中的所有导出声明。

### 修复漏洞

- 修复 `Debug` 构建下 pvzdll 无法正确添加附加库目录的漏洞。
- 修复僵王冰火球导致绘图偏移的漏洞。

### 兼容性

**不兼容** 2.x 版本。

迁移至 3.0 版本时，请参照迁移指南调整代码。

## 2.7.1

### 改动内容

- 设法消除了目前所有的编译警告。
- `DamageRangeFlags` 常量现在使用 `constexpr`。

### 漏洞修复

- 修复 Zombie::GetAnimation() 和 Zombie::GetSpecialHeadAnimation() 不能返回无效对象的漏洞。

### 兼容性

兼容 2.7 版本。

## 2.7

### 构建

- 引入了 `general.props` 和 `pvzclass.props`，前者影响所有三个项目，后者只影响 `pvzclass`。
- 现在 `pvzclass` 的构建工具集可通过 `Directory.Build.props` 覆盖。
  - 默认工具集依然是 `v143` 。
- 现在 CI 构建检查使用 `Debug` 构建，以检查更多潜在问题。

### 新增内容

- `GameSelector` 及 `GameSelectorUpdateEvent`
- `PVZApp` 新增若干成员。
- `Plant` 新增若干成员。
- `Zombie` 新增若干成员。
- `PuzzlePhaseCompleteBonusEvent`
- `Board` 新增若干成员。
- `Music` 新增若干成员。
- `AsmBuilder::call_reg()`
- `Sexy::SetEditString()`
- `CardSlot::AddSeed()`
- `PVZEnum::FoleyType`，取代原本 `LowerSoundType` 的作用。
  - 默认使用 `PVZApp::PlayFoley()` 播放。
- `BossBungeeSpawnEvent`
- `PSaveGameContext` 及若干相关函数。
- `GetRandomNumberEvent`
- `SeedBank::SeedPacket` 新增若干成员函数。
- `PlantFromBankEvent`
- `DisableZombieFailHome()`
- `Griditem` 新增若干成员。
- `Griditem` 的派生类现在可以直接从 `Griditem` 对象构造了。
- `Animation` 新增若干成员函数。
- `ZombieAlwaysDive()`
- `LawnMower` 新增若干成员。
- `ZombieEatByZombieEvent`
- `CutScene`
- `FatalErrorEvent`
- `BoardUpdateStartEvent` 等更新相关事件。
- `Challenge::GetAnotherSameTypeGriditem()`

### 改动内容

- 部分 `AsmFunctions.h` 中的数组不再具有全局性。
- 撤销了 `AsmFunctions.h` 中的部分指令宏。
- 现在包含 `WIN32_LEAN_AND_MEAN` 宏、`VC_EXTRALEAN` 宏和 `NOMINMAX` 宏，以减少 `windows.h` 带来的编译开销。
  - 这意味着 `min` 和 `max` 将使用 STL 版本，而不是宏版本。
- 大部分源代码文件和头文件分别分入 `src` 和 `include` 文件夹中。
- `INVALID_BASEADDRESS` 现在是常量，而非宏。
- `Board` 的 `GetAll` 系列函数现在能正确适配 `MemSize` 以构造变量了。
- `Zombie::Height` 更名为 `Zombie::Altitude`，因为原名称与 `GameObject` 的同名成员冲突。
- `ZombieState::ToString()` 现在返回 `char*`。
- `Creator::CreateRake()` 原理调整，现在可以返回生成的钉耙对象了。
- 现在所有 `MemSize` 均不再是常量。
  - `SetMemSize()` 会自动调整它们，不再需要在派生类中单独定义静态成员。

### 漏洞修复

- 修复 `ReanimatorCache::SetMemSize()` 的崩溃漏洞。
- 修复 `ConvertSubClass2Flag()` 没有正常生效的漏洞。
- 修复 `Animation` 的 `YScale` 和 `YSlant` 位置相反的漏洞。
- 修复 `ReanimationParams::Reposition` 在存档读档时会崩溃的漏洞。
- 修复 `StarfruitFindTargetEvent` 对象获取不正确的漏洞。
- 修复 `fstp` 相关内容的数值不正确的漏洞。
- 修复 `DisableBoardDraw` 功能异常的漏洞。
- 修复 `PlantDamageZombieEvent` 对部分植物崩溃的漏洞。
- 修复 `PlantTakeDamageEvent` 有时崩溃的漏洞。

### pvzdll

- 修复不构造 `Debug` 工件的漏洞。
- 移除了所有 `make_shared()` 的使用。
- 默认不使用增强指令集。

## 2.6

### 新增内容

- 系统：
  - `LawnAppInitAfterEvent` 事件。
  - `PlantDef` 及其重定位函数。
- 常规：
  - `Plant` 添加 `DoSpecial()`。
- 绘制：
  - `Graphics`，用来配合绘制事件。
  - `DrawGriditemEvent` 事件。
  - `DrawUICoinBankEvent` 事件。
- 资源：
  - `ReanimatorCache` 及其大小重载，用于存储动画制图。

### 改动内容

- 视觉效果：
  - 现在 `TodParticleSystem` 的成员函数使用匿名 `AsmBuilder`。
  - `ReanimationParams` 的文件名现在必须是常量。
- 资源：
  - 现在 `ResourceManager` 的成员函数使用匿名 `AsmBuilder`。
  - `APA_` 系列宏、`utils.h` 和 `HZC_` 系列宏已被移至 `deprecated.hpp`。
- 枚举：
  - `ProjectileType::Cactus` 的正式名称改为 `ProjectileType::Spike`。
  - 调整了部分头文件的分类。

### 漏洞修复

- 资源：
  - 修复 `GetImage()` 不能正确载入资源的漏洞。
- 扩展：
  - 修复 `AutoCollect()` 不检查是否在关卡内的漏洞。
 
### 兼容性

部分不兼容 2.5.x 及以前的版本。

## 2.5.1

### 改动内容

- 现在 `PlantDefinition` 和 `ZombieDefinition` 的 `AnimType` 属性可写。

### 漏洞修复

- 修复 `ReanimationParams::Reposion()` 的一系列漏洞。

## 2.5

### 新增内容

略。

### 兼容性

部分内容**不兼容** 2.4 及以前的版本。

## 2.4

### 新增内容

- `Challenge::IZPlacePlants()`，使用原生方法生成 IZ 初始植物。
- `ButtonWidget`，按钮控件。
- `Plant::GetCost()`，用于获取卡牌基础阳光消耗。
- `DiversionEventTemplate` 事件模板，用于快速设置一个事件，其结算函数的返回值为 bool。根据返回值，将会跳转到不同的地址。
- `ThreeStateEventTemplate` 事件模板，用于快速设置一个事件，其结算函数的返回值为 int（或 ThreeState::ThreeState）。根据返回值的正负性，将会跳转到不同的地址。负数不会进行跳转。
- 从其他项目中搬运大量新事件。

### 改动内容

- 现在 PVZClass 项目的默认 C++ 标准为 C++17 。
- 优化 `DLLEventTemplate` 的构造，降低工件大小。
- 更新 `Zombie` 类，增加若干接口。
- 交换了 `ThreeState::None` 和 `ThreeState::Disabled` 的数值，方便事件返回特定数值。

### 漏洞修复

- 修复 `Const::GetLevelScene()` 无法获得正确数值的漏洞。
- 修复 `Creator::CreateCrater()` 不能设置消失倒计时的漏洞。
- 修复 `PlantShoveledEvent` 不正常的漏洞。
- 修复 `Plant` 类若干接口的偏移不正确的漏洞。
- 修复 `Coin` 类一个接口的偏移不正确的漏洞。
- 修复 `ZombieHypnotizeEvent` 指定的函数名称不正确的漏洞。
- 修复 `pvzdll` 的过滤器不正确的漏洞。
- 修复部分情况下 `DLLEventTemplate` 的构造会在相关 `AsmBuilder` 析构时崩溃的漏洞。

### 兼容性

兼容 2.3。

## 2.3

### 新增内容

- `Creator::RandFloat()`，使用 PVZ 主程序的随机数生成器获取一个随机非负浮点数。
- `Memory::WriteMemoryUnsafe()`，用于在 dll 中减少不必要的权限调整。 
  - `T_PROPERTY` 系列属性和 `T_ARRAY_PROPERTY` 系列属性现在默认使用使用 `Memory::WriteMemoryUnsafe()`。
  - 对于需要进行权限调整的情况，请使用对应的 `SAFE_PROPERTY`。
- `BaseAddressEventTemplate` 事件模板，用于快速设置一个事件，其结算函数的返回值为基址。若返回空指针，则按原本的方法获取基址。
- `ChallengeDef`，类似 `ChallengeDefinition`，但是每个成员变量都用内存空间存储。
- `ChallengeDef::Reposition`，用于 `ChallengeDef` 挪表。

### 改动内容

- 现在除了 DrawUITopEvent 仅将构造函数改为内联外，所有事件现在都具有 doxygen 注释和新的构造函数，且这些构造函数均内联。
- `HZC` 系列宏被废弃，它们的作用被 `PVZ::DamageRangeFlags` 取代。
- `pvzdll` 项目现在含有 Debug 构建规则。
  - 虽然它默认不会加载 `pvzclass.lib` 。
- 所有事件模板的 `_Raw_Len` 现在均为 `uint8_t` 类型。
- 更新了码风校正脚本。
- 现在 `pvzmain` 是默认的启动项目。
- 将 `SeedCardClickEvent` 并入 `SeedPacketMouseDownEvent`，并修复 \# 47。
- `DLLEvent::newAddress` 对应的空间扩展至 16KB 。
- 删除了实际上没有作用的 `.gitattributes`。
- 三个项目现在都是 UTF-8 项目。
- `ProjectileDamageZombieEvent` 对应的函数添加一个新参数，表示溅射伤害的伤害数值。
- `ZombieBurntEvent` 现在并入 `ZombieBlastEvent.h` 。

### 漏洞修复

- 修复 \# 60
- 修复 \# 77
- 修复 `Zombie::GetActualRect()` 获取的矩形不正确的漏洞。
- 修复 `ProjectileDamageZombieEvent` 参数获取不正确的漏洞。

### 兼容性

大部分兼容 2.2.x，涉及 `SeedCardClickEvent` 和 `ZombieBurntEvent` 的部分除外。

## 2.2.1

### 新增内容

- `ProjectileImageEvent`
- `FloatDLLEventTemplate`，用于快速设置一个事件，其结算函数的返回值为 float。若返回值为指定数值，则会在清栈后跳转至指定位置。
- `ProjectileImageSizeEvent`，子弹图片大小事件。
- `Plant::PlayIdleAnim`，植物播放闲置动画。
- `Creator::Rand()`，使用 PVZ 主程序的随机数生成器获取一个随机非负整数。
- `Board::SetMemSize()`，设置该类的大小。
- `BoardInitAfterEvent`，`Board` 构造完成事件。

### 改动内容

- `Injectors.h` 现在被 `pvzclass.h` 包含。
- 以下类添加 doc 注释：
  - `PVZApp`
  - `Image`
  - `Projectile`
  - `Zombie`
  - `Wave`
  - `Icetrace`
  - `Lawn`
  - `Animation`
  - `GameObject`
  - `Board`
  - `Widget`
  - `DLLEvent`
- 以下事件现在支持 int 参数构造：
  - `PlantAddProjectileEvent` 及其基础事件。
  - `PlantDamageZombieEvent`
  - `UpdateGameObjectsEvent`
  - `ZombieInitAfterEvent`
  - `PlantInitAfterEvent`
- `__asm__Plant_memset` 被清除。
- 优化了 `ProjectileDamageZombieEvent`。

### 漏洞修复

- 修复 \# 52
- 修复 \# 66
- 修复 \# 67
- 修复 \# 68
- 修复 `CreateCaption` 导致编译失败的漏洞。
- 修复 `NormalPlantAddProjectileEvent` 崩溃漏洞。

### 兼容性

兼容 2.2 。

## 2.2

### 改动内容

- 为大量文件补充了 `Doxygen` 注释。
- 纠正了部分代码格式问题。
- `Board` 添加 `__prototype_GetAll()`，是原本各种 `GetAll()` 函数的模板化版本。
  - 原本的 `GetAll()` 系列函数和 `GetChallenge()` 已经模板化，可以接收它们对应的类的派生类。
  - `GetAllGriditems()` 可以根据传入的类自动过滤不想要的 `Griditem`，但是类必须定义静态变量 `ItemType`，作为过滤判据。
- 优化了 `EnablePortal()`，现在可以控制是否生成或移除默认传送门。
- `Portal` 现在具有两个派生类 `CirclePortal` 和 `SquarePortal`，用来分辨不同类别的传送门。
- `Const.h` 现在加入 `pvzclass.h`。
- 部分涉及 `Memory` 类的宏被搬运至 `Memory.hpp` 中。
- 部分 `PVZ.h` 中的类被分离到 `Classes` 目录的文件中。
- 将部分函数标为废弃。
- `Board::PlayingTime` 现在可写。

### 事件改动

- 添加了 `PlantUpdateEvent`，用于植物更新。
- 修复了 `ChallengeUpdateEvent` 导致崩溃的漏洞。
- 添加了 `DLLEventTemplate` 类，用于简化部分事件的定义。
- 添加了 `BoolDLLEventTemplate`，`TrueDLLEventTemplate` 和 `IntDLLEventTemplate`，用于简化部分事件的定义。
- 添加了 `ProjectileDamageZombieEvent`，用于结算子弹对僵尸的伤害。
- 添加了 `GetPlantRectEvent` 和 `GetPlantAttackRectEvent`，用于修改植物的攻击和受击范围。
- 若 `DLLEvent` 钩的第一条语句是 `call` 或 `jmpfar`，则 `start()` 期间会加以调整。
- 添加了 `PlantDamageZombieEvent`，用于多种植物直接伤害僵尸的情况。
- 将 `DLLEvent` 的部分内容分到源文件中，减少重复定义的链接问题。

### 兼容性

兼容 2.1.x 。

## 2.1.1

### 改动内容

- 微调了 `AsmBuilder` 的机制，优化了 `Memory::Execute(AsmBuilder&)` 的性能。
- 修复了 `PlantAddProjectileEvent` 的前向声明问题。
- 修复了 `PlantTakeDamageEvent` 未被包含的问题和前向声明问题。

### 兼容性

兼容 2.1.x 。

## 2.1.2

### 改动内容

- `SeedChooserScreen` 现在分入独立的文件中。
- 调整了 `BaseClass::isValid()` 的判定标准。现在基址为 0 的对象也会返回 `true`。
- 为大量文件补充了 `Doxygen` 注释。
- 添加了 `Creator::CreateIZBrain()`，用于生成 IZ 关卡的脑子。

### 兼容性

兼容 2.1.x 。

## 2.1

### 新增内容

- `SeedCard` 新增 `SetPacketType()`，用于初始化卡槽卡片。
- `Doxyfile`，doxygen 配置文件，可用于生成 API 文档。
- `AStyle.bat`，用于调用 astyle 实现代码风格调整。
  - 目前已有部分文件接受调整，后续所有代码文件都应当遵从此脚本指定的码风。
- `SetGoldMagnetLimit()`，用于设置吸金磁的吸取目标数上限。
- `GameObjectType` 枚举，用于表示 `GameObject` 类型。
- `ZombieAttackType` 枚举，用于表示僵尸攻击植物的类型。
- `PVZ::InitPVZNoLock()`，用于实现非互斥的初始化。
- `PlantInitAfterEvent` 事件。
- `SeedPacketUpdateEvent` 事件。
- 复合事件 `PlantTakeDamageEvent` 和它的附属事件。
- 复合事件 `PlantAddProjectileEvent` 和它的附属事件。

### 改动内容

- 部分文件现在使用 `UTF-8 BOM` 编码。
  - 后续所有代码文件都会使用此编码，请注意调整。
- `ImitaterPlantEvent` 现在是 `DLLEvent` 的派生类。
- 移除了 `DebugEvents` 相关的所有代码文件和描述。

### 漏洞修复

- 修复 `Creator::CreateUpperSound()` 不能正常生效的漏洞。
- 修复 `Creator::CreateProjectile()` 在 dll 中结算异常的漏洞。
 
### 兼容性

兼容 2.0.x 。

## 2.0.1

### 改动内容

- 修复了 `PlantStolenEvent` 描述与函数规定不符的漏洞；
- 修复了 `ExtractResourceEvent` 描述与函数规定不符的漏洞；
- 修复了 `PlantEatenEvent` 会导致崩溃的漏洞；
- 修复了 pvzclass 编译失败的漏洞；
- 现在 pvzmain 对 `Memory::localExecute` 的设置改在 `PVZ::InitPVZ()` 中进行。

### 兼容性

兼容 2.0 。

## 2.0

### 新增内容

- `AsmBuilder` 类，用于构建注入的机器码。
  - `Memory::Execute()` 已适配 `AsmBuilder`。
- `TodParticleSystem` 类，对应 PVZ 本体的粒子效果。
- `Zombie::ShowDoorArms()` 方法，设置是否显示铁门僵尸的手臂。
- `Zombie::GetActualAttackRect()` 与 `Zombie::GetActualRect()`，分别获取僵尸的攻击和受击判定范围。
- `Animation::AssignRenderGroupToTrack()`，设置指定动画轨道在绘制时的分组。
- `ZombieDetachShieldEvent` ，僵尸因各种原因失去盾牌事件。
- `ZombieDropHeadParticleEvent`，僵尸生成掉头粒子效果事件。
- `ResourceManager` 大幅翻新，获得了导入 pak、解析 xml 资源文件、导入资源、 获取音效 ID 和图片的功能。
- 将大部分对接 PVZ 本体对象的类变为 `BaseClass` 的派生类。

### 改动

- `pvzclass `现在**只生成** `.lib` 静态链接库。生成可执行文件的项目现在是 `pvzmain`。

- `pvzdll` 现在只在 release Win32 条件下构建。

- `ZombieAccessoriesType2` 被重命名为 `ShieldType`，旧名称作为别名存在。

- 以下代码规范已应用到代码当中：
  - `BaseClass` 派生类写在 `Classes/*.hpp` 和 `Class/*.cpp`  内；
  - `hpp` 完成构造函数、`GetAll()` 和 `Create()` 的声明，取缔 `Creator`；
  - 函数的 `char*` 参数均带 const 标注。
  - 对于 `Struct / Class` 参数，`BaseClass` 派生类用值传递，其余用引用传递。
  * `Execute()` 代码用 `AsmBuilder` 实现，在执行前调用构造函数，执行时先 `clear()` 再补齐代码。
  
- `BaseClass::GetBaseAddress()` 现在保证是 `const` 方法。

- `Miscellaneous` 被重命名为 `Challenge`。

- 大部分 `Events` 现在已经完成分类。

- 现在 `BaseClass` 添加了默认构造函数和 `isValid()` 方法，虽然这个方法并没有有效适配。

- `Memory::AllocMemory()` 现在不再调用 `Memory::AllAccess()`。

### 漏洞修复

- 修复了本地模式的 `Memory::Execute()` 导致内存泄露的漏洞。
- 修复了 `GraveBuster` 长期被命名为 `CraveBuster` 的漏洞。

### 移除内容

- `AsmFunctions.h` 中的 `F_ST` 系列宏。它们已变为 `AsmBuilder` 的静态常量。
- `DamageType::DamageType`。它已经合并到 `PVZ::DamageFlags`。

### 兼容性

与 1.x 版本**大部分不兼容**。

## 1.17.2（2025/1/25）

### 新增内容

- `DisableBoardDraw()`，可以关闭关卡内大部分内容的绘制。
- `DisableNewParticle()`，阻止新生成的粒子系统产生粒子效果。
- `DisableAllSounds()`，禁用一切音效播放。
- `ResourceManager`，用于管理资源文件。
- `Coin::DisappearCounter`，硬币消失倒计时。
- `Coin::Die()`，可以直接移除硬币。

### 改动

- `CardSlot::__get_CardsCount` 和 `CardSlot::SetCardsCount` 现在均可通过 `CardsCount` 使用。
- `Coin::__get_Speed` 和 `Coin::SetSpeed` 现在均可通过 `Speed` 使用。

### 漏洞修复

- 修复 #9
- 修复 #11
- 修复了 QuitPVZ() 不会清理 UpdateHook 代码段的漏洞。

### 兼容性

兼容 1.17.x 。

## 1.17.1（2025/1/4）

### 新增内容

- `PlantMouseDownEvent`，处理植物被鼠标点击的事件。
- `BoardKeyDownEvent`，处理游戏内键盘按键的事件。
- `KeyCode` 枚举，存储按键类型。
- `PlantEatenEvent`，处理植物被啃食完毕的事件。
- `PlantAddProjectileEvent`，处理植物（不含杨桃）生成子弹的事件。
- `ZombieDropLootEvent`，处理僵尸掉落物品的事件。
- `CoinMouseDownEvent`，处理物品被鼠标点击的事件。
- `SeedPacketMouseDownEvent`，处理卡槽卡片被鼠标点击的事件。
- `VaseBreakerPopulateEvent`，处理砸罐模式生成罐子的事件。
- `PlantDieLowHealthEvent`，处理植物因低生命值被移除的事件。
- `PlantStolenEvent`，处理植物被偷走的事件。
- `LawnmowerUpdateEvent`，处理小推车更新的事件。
- `GriditemDieEvent`，处理场地物件被清除的事件。
- `ChallengeInitAfterEvent`，处理小游戏关卡初始化的事件。
- `ChallengeUpdateEvent`，处理小游戏特性更新的事件。
- `BegTwistFailMoveEvent`，处理宝石迷阵转转看中失配移动的事件。
- `PlantShoveledEvent`，处理植物被铲除的事件。
- `ZombieEatSoundEvent `，处理僵尸生成啃食音效的事件。
- `PlantProduceEvent`，处理植物产生物品的事件。
- `ZombieBurntEvent`，处理僵尸受到灰烬伤害的事件。

### 改动

- 现在`VaseOpenEvent` 是 `DLLEvent` 的派生类。
- 修复了 `ARRAY_PROPERTY ` 导致编译错误的漏洞。

### 移除内容

- 移除了 `LICENSE` 中的 Lazuplis-Mei 。

### 兼容性

- 基本兼容 1.17 。

## 1.17（2024/12/23）

### 新增内容

- `Animation` 类新增 `Type` 成员和 `SetFramesForLayer()` 成员函数。
- `ZombieUpdateActionEvent` 事件，对应僵尸行为动作的更新。
- `ZombieUpdatePlayingEvent` 事件，被冻结、黄油定身时也会触发的存活僵尸的更新。
- `PlantUpdateAbilityEvent` 事件，植物技能的更新事件。
- `Resource` 类，含有对应资源对象的静态成员变量。
  - 需要用 `InitImages()` 初始化。
- `Matrix3` 类，用于存储一个三维矩阵。
  - 含有 `ScaleRotateTransformMatrix()` 成员函数，用于将当前类变为指定变换的变换矩阵。
- `Board` 新增 `TakeSunMoney()` 成员函数，用于自动判断阳光是否足够的同时扣除阳光。

### 改动

- `Injector` 重做。现在它的代码必须在构造时就指明，不能动态更换。
- 枚举 `ZombieAccessoriesType1` 被更名为 `HelmType`。
- `TrackInstance` 现在是 `BaseClass` 的派生类。
- `Animation::AttachTo()` 调整了参数，使其现在可以容易地使用。
- `PlantSquishEvent` 现在是 `DLLEvent` 的派生类。
- `CollisionBox` 重做为 `Rect` 类，可以较方便地判定矩形覆盖。

### 移除内容

- `CT` 文件夹。
- 枚举 `CardType` 和 `PlantType`。它们现在是 `SeedType` 的别名。

### 兼容性

- 由于 Enum 方面的改动，可能出现不兼容 1.16.x 的情况。如果遇到相关问题，请在 issue 汇报。

## 1.16.2（2024/10/19）

### 新增内容

- `Image` 类，用于访问内存中的图片。
- `StoreItem` 枚举，对应商店售卖物品的编号。
- `Miscellaneous` 新增 `IZSquishBrain()`，压扁指定脑子。
- `Animation` 类新增 `SetImageOverride()`，可以令动画重载贴图。

### 改动

- `SaveData`，`Widget`，`AttachEffect`，`Mouse`，`Music` 现在是 `BaseClass` 的派生类。
- `GameObject`，`Plant`，`Miscellaneous` 新增成员。
- `SaveData` 现在可以读写指定模式的通关纪录和指定商店物品的购买量。

### 漏洞修复

- 修复 `Plant::SetMemSize()` 会导致图鉴易崩溃的漏洞。

### 兼容性

- **不考虑兼容** 1.16.x 及以前的版本。

## 1.16.1（2024/10/2）

### 新增内容

- `ReadMemory()` 等函数的地址现在使用 DWORD 表示内存地址。
- 添加了以下新事件：
  - Board 游戏更新事件 `BoardUpdateGameEvent`；
  - 小推车启动事件 `LawnmowerStartEvent`；
  - IZ 关卡完成事件 `IZLevelCompleteEvent`；
  - IZ 关卡初始化后的事件 `IZLevelStartAfterEvent`；
  - IZ 食脑事件`IZScoreBrainEvent`。
- `PuzzlePhaseCompleteEvent` 现在由 `DLLEvent` 派生，`dll` 中函数名为 `onPuzzlePhaseComplete()`。

### 兼容性

- 大部分兼容 1.16。

## 1.16（2024/9/6）

### 新增内容

- 重做了事件机制。现在使用 `DLLEvent` 实现事件处理功能。
  - 目前大部分事件已转变为 `DLLEvent` 形式。\
- 添加了 `Dialog` 组件、编辑框组件、`List` 控件和 `CheckBox` 控件的控制功能。
- 对部分代码文件进行分类。 
- 新增保存和读取存档的功能。
- 新增若干事件。

### 漏洞修复

- 修复 `Board::GetAllLawnmowers()` 无法获取任何小推车的漏洞。

### 兼容性

- **不考虑兼容** 1.15.3 及以前的版本。

## 1.15.3（2024/1/25）

### 漏洞修复

- 修复 `ChallengePage` 相关的一系列问题。
- 修复部分代码文件使用 UTF-8（无签名）的问题。

### 兼容性

- 兼容 1.15.x。
- **不兼容** 1.14.3 及以前的版本。

## 1.15.2（2023/11/24）

### 新增内容

- `ChallengeScreen` 类，用于处理选关界面。
- `VaseOpenEvent` 类，罐子被打开的事件。
- `UpdateGameObjectsEvent` 类，更新游戏内物品的事件。
- `PlantSquishEvent` 类，植物被碾压的事件。
- `ImitaterPlantEvent` 类，模仿者生成新植物的事件。
- `ChallengePage` 枚举，用于表示选关界面的页数。

### 兼容性

- 兼容 1.15.x。
- **不兼容** 1.14.3 及以前的版本。

## 1.15.1（2023/11/22）

### 新增内容

- `ThreeState` 枚举，用于三态判断。
- `Miscellaneous` 类添加了 `SetMemSize()`，用于扩展该类。
- `Extensions.h` 添加 `EnableVaseBreak()`，可以启用砸罐子模式的部分特性。
- `Const::GetLevelScene()` 和 `Const::SetLevelScene`，用于获取/设置模式的默认关卡。
- `Flags.h`，包含 `DamageRangeFlags` 和 `DamageFlags`，用于存储状态压缩存储的量。
- `Zombie` 类添加了 `Hit()` 和 `HitDirect()` 的 `DamageFlags` 适配版。
- `Zombie` 类添加了 `EffectedBy()`，用于判定僵尸是否符合某些条件。
- `PVZApp` 类添加若干属性。

### 改动

- 平台工具集从 `v142` 更新至 `v143`。
- `Events` 组件现在支持 `x64` 构建。
- `Events` 组件的部分功能分拆到 `.cpp` 文件中。
- 使用 `Creator::CreateZombie()` 生成的僵王博士将被强制生成在 0 行 0 列。
- 更新 `README.md` 的过时内容。
- `Memory` 类现在使用新的方式实现 `Execute()`，可用 `immediateExecute` 控制是否不等待 `Board` 更新就执行指令。
- 优化 `Memory::Variable` 的注释。

### 漏洞修复

- 修复 `Const::WriteCoinValue()` 不能正确修改小阳光价值的漏洞。
- 修复 `Zombie::canDecelerate()` 无法正常判定的漏洞。

### 兼容性

- 兼容 1.15.0。
- **不兼容** 1.14.3 及以前的版本。

## 1.15.0（2023/10/27）

### 新增内容

- `PVZApp` 类，用于处理 `PVZ_BASE` 偏移的属性。
- `PVZutil` 类，用于存放部分功能，目前只支持版本查询。
- `BaseClass` 类，作为大部分类的基类，提供只读基址和简单构造函数。

### 改动

- 将 `PVZ` 从类转换为命名空间。
  - 原本的属性均已转移到其他类的定义之下。
  - `PVZ.h` 中的四个结构体均移入 `PVZ` 命名空间内部。
  - 需要 `PVZ` 对象作为参数的函数均已更新。
- 根据 `PVZ` 命名空间的改动，更新 `README.md`。

### 漏洞修复

- 修复 `CALC_PTR_ADDR` 宏不能正确指定运算类型的漏洞。
- 修复某些情况下无法初始化 pvzclass 的漏洞。

### 兼容性

- **不兼容** 1.14.3 及以前的版本。

## 1.14.3（2023/10/22）
- 大重构前的最后一个稳定版本。