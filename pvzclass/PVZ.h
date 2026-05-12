#pragma once
#include "Enums.h"
#include "include/Flags.hpp"
#include "AsmFunctions.h"
#include "AsmBuilder.hpp"
#include "Memory.hpp"
#include <ctime>
#include <vector>
#include <type_traits>
#include <optional>

using std::enable_if_t;
using std::is_base_of;

#pragma region definitions

#define STRING(str) str,sizeof(str)/sizeof(*str)
#define SETARG(asmfunction,index) *(int*)(asmfunction+index)
#define SETARGFLOAT(asmfunction,index) *(float*)(asmfunction+index)

#define LOGICALINCLUDE(c,v) (c&v)==v

#pragma endregion

#define AP_ZOMBIESPEED		1
#define AP_ZOMBIECOLOR		2
#define AP_ZOMBIESCALE		4
#define AP_PLANTCOLOR		8
#define AP_PLANTSCALE		16

#define GPD_RIGHT			0
#define GPD_LEFT			1

#define MINGAM_ENABLE		2
#define MINGAM_DISABLE		-1

#define MINGAME_STARTED		1
#define MINGAME_NOTSTARTED	-1

#define ID_INDEX(id) ((id) & 0x0000FFFF)
#define ID_RANK(id) ((id) & 0xFFFF0000)

inline constexpr auto INVALID_BASEADDRESS = 0x400000;

namespace PVZEnum
{
	/// @brief 附件效果类型
	enum class AttachEffectType : int
	{
		PARTICLE,
		TRAIL,
		REANIM,
		ATTACHMENT,
		OTHER
	};
}

/// @brief 包含大部分用于控制 PVZ 内部对象的类和方法。
/// @note Only version 1.0.0.1051 is fully supported
namespace PVZ
{
	/// @brief 初始化 PVZ 命名空间，且不在程序内附加钩子。Memory::immediateExecute 会设置为 true 。
	/// @param pid 进程 id
	void InitPVZNoLock(DWORD pid);

	/// @brief 初始化 PVZ 命名空间，且不在程序内附加钩子。Memory::localExecute 和 Memory::immediateExecute 会设置为 true 。
	/// @note 该函数供 pvzdll 初始化使用。
	void InitPVZDLL();

	/// @brief 初始化 PVZ 命名空间
	void InitPVZ(DWORD pid);
	/// @brief 消除 PVZ 命名空间初始化的影响
	void QuitPVZ();

	#pragma region structs

	struct Color
	{
		int Red;
		int Green;
		int Blue;
		int Alpha;
	};

	#pragma endregion

	#pragma region classes
	/// @brief 所有对应 PVZ 内部对象的类的基类。
	/// @attention 除非你知道你在做什么，否则你不应该直接构造 BaseClass ！
	class BaseClass
	{
	protected:
		/// @brief 对应对象的基地址
		uint32_t BaseAddress;
	public:
		BaseClass() : BaseAddress(INVALID_BASEADDRESS) {};
		BaseClass(uint32_t address) : BaseAddress(address) {};
		/// @brief 返回基址
		/// @return 基址
		uint32_t GetBaseAddress() const
		{ return(this->BaseAddress); }
		/// @brief 对应的对象是否已经失效，或者构造不良。
		/// @return 是否已经失效或构造不良。
		const bool isValid()
		{ return(this->BaseAddress != INVALID_BASEADDRESS && this->BaseAddress != 0); }
	};

	template<typename T>
	T GetByID(int id)
	{
		if (id)
		{
			T tmp = T::GetByIndex(ID_INDEX(id));
			if (tmp.Id != id)
				return T((uint32_t)nullptr);
			else
				return tmp;
		}
		else
			return T((uint32_t)nullptr);
	}

	/// @brief 对应 PVZ 内部数组的类
	/// @note 请勿将此类嵌套
	/// @tparam T 数组元素的类型
	template<typename T>
	class Array : public BaseClass
	{
	public:
		Array(uint32_t address) : BaseClass(address) {};
		class Item : public BaseClass
		{
		public:
			explicit Item(uint32_t address) : BaseClass(address) {};

			const Item& operator=(const T val)
			{
				PVZ::Memory::WriteMemoryUnsafe<T>(this->BaseAddress, val);
				return *this;
			}
			/// @brief 获取该元素的值
			T get() const { return PVZ::Memory::ReadMemory<T>(this->BaseAddress); }
		};
		/// @brief 获取指定下标的元素
		/// @param index 下标
		/// @return 元素
		Item operator[](int index)
		{
			return Item(BaseAddress + index * sizeof(T));
		}
	};

	class Rect
	{
	public:
		int X;
		int Y;
		int Width;
		int Height;

		Rect(int X, int Y, int Width, int Height) : X(X), Y(Y), Width(Width), Height(Height) {}
		Rect(const Rect& Rect) : X(Rect.X), Y(Rect.Y), Width(Rect.Width), Height(Rect.Height) {}
		Rect() : X(0), Y(0), Width(0), Height(0) {}

		// 判定坐标为 (X, Y)，半径为 radius 的圆与该矩阵是否有重叠部分。
		// 相切会视为有重叠部分。
		bool IsCircleOverlap(const int X, const int Y, const int radius);
		/// @brief 获取与另一矩形重叠的部分。
		/// @param rect 另一矩形
		/// @return 两矩形的重叠部分。若无交叉部分，返回空矩形。
		Rect Intersection(const Rect& rect) const;
		/// @brief 判断矩形是否非空
		/// @return 矩形非空
		inline bool IsVaild()
		{
			return this->X | this->Y | this->Width | this->Height;
		}
	};
	// 取得两个矩形横向重叠部分的长度。
	// 若横向无重叠部分，返回两矩形横向间距的相反数。
	/// @param rect 计算重叠的另一个矩形。
	/// @return 矩形横向重叠的长度，或矩形横向间距的相反数。
	int GetXOverlap(const Rect& rect1, const Rect& rect2);

	typedef Rect CollisionBox;

	class PVZutil
	{
	public:
		/// @brief 获取 PVZClass 版本
		READONLY_PROPERTY(const char*,	__get_Version)	Version;
		/// @brief 判断 PVZ 主程序是哪一个发布版本。
		/// @see PVZVersion
		READONLY_PROPERTY(PVZVersion::PVZVersion,	__get_GameVersion)	GameVersion;
	};

	/// @brief 字符串类，用于对应 PVZ 内部的 std::string
	class PVZString : public BaseClass
	{
	public:
		PVZString() = default;
		explicit PVZString(int address) : BaseClass(address) {};

		/// @brief 在 PVZ 主程序中创建字符串
		/// @note 以此法创建的字符串需要使用 Free() 销毁，否则会造成内存泄露。
		/// @return 创建的字符串
		/// @author Moon404
		static PVZString Make(const char* str);
		/// @brief 将给定的“[X]”格式字符数组转换为对应的译文字符串
		/// @note 不符合格式时，会直接转换为字符串。
		/// @param str 原字符串
		/// @return 翻译后得到的字符串
		PVZ::PVZString Translate(const char* str);
		/// @brief 释放该字符串在 PVZ 占用的内存空间
		/// @attention 只有通过 Make 创建的字符串才应该使用该函数销毁。
		void Free();
		/// @brief 将字符串转换为整数
		/// @param str 字符串
		/// @return 由 std::optional 表示的，转换为整数的结果。
		/// @retval nullopt 转换失败
		std::optional<int> ToInt(PVZString str);
		/// @brief 将字符串转换为双精度浮点数
		/// @param str 字符串
		/// @return 由 std::optional 表示的，转换为浮点数的结果。
		/// @retval nullopt 转换失败
		std::optional<double> ToDouble(PVZString str);
		/// @brief 拼接字符数组中的字符串
		/// @param src 字符串
		/// @param len 字符串长度
		void Concat(const char* src, int len);
		/// @brief 拼接字符串
		/// @param src 字符串
		void Concat(PVZ::PVZString src);
		/// @brief 赋值字符串为ptr
		/// @param ptr 字符串
		void Assign(const char* ptr, uint32_t len);
		/// @brief 赋值字符串为ptr的前count个字符
		/// @param ptr 字符串
		/// @param count 子串长度
		void Assign(const char* ptr, uint32_t len, uint32_t count);
		/// @brief 赋值字符串为ptr的第roff个字符往右count个字符
		/// @param ptr 字符串
		/// @param len 字符串长度
		/// @param 子串起始点
		/// @param count 子串长度，为-1时不限制拷贝长度
		void Assign(PVZString ptr, uint32_t len, uint32_t count, uint32_t roff);
		/// @brief 获取c风格字符串
		const char* c_str();
	};

	class LawnDialog;

	/// @brief 游戏程序主类（原 LawnApp）。
	class PVZApp : public BaseClass
	{
	public:
		PVZApp(DWORD address) : BaseClass(address) {};
		/// @brief 音乐音量大小，数值应当在 [0,1] 范围内。
		PROPERTY(double,						__get_MusicVolume,	__set_MusicVolume)			aMusicVolume;
		/// @brief 音效音量大小，数值应当在 [0,1] 范围内。
		PROPERTY(double,						__get_SoundFXVolume,__set_SoundFXVolume)		SoundFXVolume;
		/// @brief 主线程 ID
		T_PROPERTY(DWORD,						PrimaryThreadId,	__get_PrimaryThreadId,		__set_PrimaryThreadId,		0x33C);
		/// @brief 是否关闭
		T_PROPERTY(BOOLEAN,						Shutdown,			__get_Shutdown,				__set_Shutdown,				0x341);
		/// @brief 窗口句柄
		T_PROPERTY(HWND,						HWnd,				__get_HWnd,					__set_HWnd,					0x350);
		/// @brief DEBUG按键是否开启
		T_PROPERTY(bool,						mDebugKeysEnabled,  __get_mDebugKeysEnabled,	__set_mDebugKeysEnabled,	0x5AC);
		/// @brief 当前模式类型
		T_PROPERTY(PVZLevel::PVZLevel,			LevelId,			__get_LevelId,				__set_LevelId,				0x7F8);
		/// @brief 游戏状态
		/// @see PVZGameState
		T_PROPERTY(PVZGameState::PVZGameState,	GameState,			__get_GameState,			__set_GameState,			0x7FC);
		/// @brief 是否开启自由种植
		T_PROPERTY(BOOLEAN,						FreePlantingCheat,	__get_FreePlantingCheat,	__set_FreePlantingCheat,	0x814);
		/// @brief 特效系统地址
		T_PROPERTY(DWORD,                       EffectSystem,       __get_EffectSystem,         __set_EffectSystem,         0x820);
		/// @brief 动画缓存地址
		T_PROPERTY(DWORD,						ReanimatorCache,	__get_ReanimatorCache,		__set_ReanimatorCache,		0x824);
		/// @brief 是否在玩完整版。部分 PVZ 主程序反转了它的效果。
		T_PROPERTY(BOOLEAN,						FullVersion,		__get_FullVersion,			__set_FullVersion,			0x8C0);

		/// @brief 移除主菜单界面
		void KillGameSelector();
		/// @brief 准备开始关卡，处理游戏的开始阶段。
		/// @param mode 模式
		/// @param look_for_saved_game 是否尝试读档。若为 false，则会删除旧有存档。
		void PreNewGame(PVZLevel::PVZLevel mode, bool look_for_saved_game);
		/// @brief 若未退出程序，则立即移除标题界面并开始 theGameMode 关卡的新游戏。
		/// @param mode 关卡
		void FastLoad(PVZLevel::PVZLevel mode);
		/// @brief 清除所有不存在的效果对象并释放内存。
		/// @note Board::Update() 如果连续多次调用则需要使用该函数清理。
		void RemoveNotExistEffects();
		/// @brief 播放指定音效
		void PlayFoley(PVZEnum::FoleyType type);
		/// @brief 获取 bool 类型的配置属性
		/// @param id 属性 ID
		/// @param default_val 获取失败时的默认值 
		/// @return 获取的属性
		bool GetBoolean(PVZString id, bool default_val);
		/// @brief 获取 int 类型的配置属性
		/// @param id 属性 ID
		/// @param default_val 获取失败时的默认值 
		/// @return 获取的属性
		int GetInteger(PVZ::PVZString id, int default_val);
		/// @brief 获取字符串类型的配置属性
		/// @param id 属性 ID
		/// @param default_val 获取失败时的默认值 
		/// @return 获取的属性
		PVZ::PVZString GetString(PVZ::PVZString id, PVZ::PVZString default_val);
		/// @brief 加载属性文件
		/// @param file_name 文件名
		/// @param check_sig 是否检查签名文件
		/// @return 是否加载成功
		bool LoadProperties(PVZ::PVZString file_name, bool check_sig = false);
		/// @brief 创建一个LawnDialog对话, 这个函数封装了LawnApp类的实现
		/// @param theDialogId 对话的id
		/// @param isModal 是否允许用户在显示此对话期间，交互其他控件
		/// @param theDialogHeader 对话的标题内容
		/// @param theDialogLines 对话的文字内容
		/// @param theDialogFooter 对话的底部内容
		/// @param theButtonMode 对话底部按钮种类
		/// @return LawnDialog
		PVZ::LawnDialog NewDialog(
			int						theDialogId,
			bool					isModal,
			PVZ::PVZString			theDialogHeader,
			PVZ::PVZString			theDialogLines,
			PVZ::PVZString			theDialogFooter,
			int						theButtonMode
		);
	};
	/// @brief 获取游戏程序主对象
	/// @return 游戏程序主对象
	PVZApp GetPVZApp();

	/// @brief 图片类
	class Image : public BaseClass
	{
	public:
		Image(int address) : BaseClass(address) {};
	};

	class Attachment;
	class AttachmentID;
	class LawnMower;
	class Griditem;
	class MousePointer;
	class GameButton;
	class Caption;
	class CardSlot;
	class Challenge;
	class CutScene;
	class Lawn;
	class Icetrace;
	class Wave;
	class Widget;

	class WidgetContainer : public BaseClass
	{
	public:
		WidgetContainer(uint32_t address) : BaseClass(address) {};
		INT_PROPERTY(ViewX,			__get_ViewX,		__set_ViewX,		0x30);
		INT_PROPERTY(ViewY,			__get_ViewY,		__set_ViewY,		0x34);
		INT_PROPERTY(ViewLength,	__get_ViewLength,	__set_ViewLength,	0x38);
		INT_PROPERTY(ViewHeight,	__get_ViewHeight,	__set_ViewHeight,	0x3C);

		/// @brief 将指定控件设定为容器的子控件。
		/// @param widget 子控件
		void AddWidget(Widget widget);
		/// @brief 将指定控件移出容器的子控件列表。
		/// @param widget 子控件
		void RemoveWidget(Widget widget);
	};
	/// @brief 控件
	class Widget : public WidgetContainer
	{
	public:
		Widget(int address) : WidgetContainer(address) {};
	};
	/// @brief 三行三列矩阵
	class Matrix3 : public BaseClass
	{
	public:
		Matrix3(DWORD address) : BaseClass(address) {};

		FLOAT_SIMPLE_PROPERTY(XScale,	0x00);
		FLOAT_SIMPLE_PROPERTY(XSlant,	0x04);
		FLOAT_SIMPLE_PROPERTY(XOffset,	0x08);
		FLOAT_SIMPLE_PROPERTY(YSlant,	0x0C);
		FLOAT_SIMPLE_PROPERTY(YScale,	0x10);
		FLOAT_SIMPLE_PROPERTY(YOffset,	0x14);

		/// @brief 根据指定的平移坐标、旋转弧度和拉伸比例，设定矩阵每个项的数值。
		void ScaleRotateTransformMatrix(float x, float y, float rad, float ScaleX, float ScaleY);
	};
	class TrackInstance;

	/// @brief 附件效果，为 Attachment 的主要成员
	class AttachEffect : public BaseClass
	{
	public:
		AttachEffect(uint32_t address) : BaseClass(address) {};
		/// @brief 附件效果识别 ID
		INT_SIMPLE_PROPERTY(EffectID, 0);
		/// @brief 附件效果类型
		T_SIMPLE_PROPERTY(PVZEnum::AttachEffectType, Type, 4);
		/// @brief 获取附件效果的变换矩阵
		/// @return 变换矩阵
		Matrix3 GetOffset();
		T_SIMPLE_PROPERTY(uint8_t, DontDrawIfParentHidden, 0x2C);
		T_SIMPLE_PROPERTY(uint8_t, DontPropogateColor, 0x2D);
	};
	/// @brief 动画部件。相当于其他游戏的模型。
	class Animation : public BaseClass
	{
	public:
		/// @attention 从 3.0 起，该函数将不再具有按编号构造的功能。
		Animation(uint32_t address) : BaseClass(address) {};
		/// @brief 获取指定编号的对象
		/// @param index 编号
		/// @note 不保证以此法获得的对象未被移除
		static Animation GetByIndex(uint32_t index);
		//support muiti-animprop(AP_XXXXXX)
		static void UnLock(int animprop);
		static void Lock();
		int GetBaseAddress();
		/// @brief 动画类型
		T_READONLY_PROPERTY(AnimationType::AnimationType, Type, __get_Type, 0);
		/// @brief 动画时长
		T_PROPERTY(FLOAT, CycleRate, __get_CycleRate, __set_CycleRate, 4);
		/// @brief 播放速率
		T_PROPERTY(FLOAT, Speed, __get_Speed, __set_Speed, 8);
		/// @brief 动画循环类型
		T_PROPERTY(PVZEnum::ReanimLoopType, LoopType, __get_LoopType, __set_LoopType, 0x10);
		/// @brief 是否已移除
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x14);
		/// @brief 起始帧
		INT_PROPERTY(StartFrame, __get_StartFrame, __set_StartFrame, 0x18);
		/// @brief 帧数
		INT_PROPERTY(FrameCount, __get_FrameCount, __set_FrameCount, 0x1C);
		INT_PROPERTY(FrameBasePose, __get_FrameBasePose, __set_FrameBasePose, 0x20);
		T_PROPERTY(FLOAT, XScale, __get_XScale, __set_XScale, 0x24);
		T_PROPERTY(FLOAT, XSlant, __get_XSlant, __set_XSlant, 0x28);
		T_PROPERTY(FLOAT, XOffset, __get_XOffset, __set_XOffset, 0x2C);
		T_PROPERTY(FLOAT, YSlant, __get_YSlant, __set_YSlant, 0x30);
		T_PROPERTY(FLOAT, YScale, __get_YScale, __set_YScale, 0x34);
		T_PROPERTY(FLOAT, YOffset, __get_YOffset, __set_YOffset, 0x38);
		/// @brief 获取该动画的颜色重载。
		/// @return 颜色
		Color GetColor(); // 0x48-0x54，基础颜色
		/// @brief 设置该动画的颜色重载
		/// @param color 设置后的颜色
		void SetColor(Color color);
		/// @brief 获取指定名称的轨道
		/// @param trackName 轨道名称
		/// @return 轨道实例
		TrackInstance GetTrackInstance(const char* trackName);
		/// @brief 循环播放次数
		INT_PROPERTY(CycleCount, __get_CycleCount, __set_CycleCount, 0x5C);
		T_PROPERTY(BOOLEAN, IsAttachment, __get_IsAttachment, __set_IsAttachment, 0x64);
		/// @brief 获取该动画的加色
		/// @return 加色
		Color GetAdditiveColor();
		/// @brief 设置该动画的加色
		/// @param color 设置后动画的加色
		void SetAdditiveColor(Color color);
		/// @brief 是否启用加色模式
		T_PROPERTY(BOOLEAN, DrawAdditiveColor, __get_DrawAdditiveColor, __set_DrawAdditiveColor, 0x7C);
		/// @brief 获取该动画的覆盖色
		/// @return 覆盖色
		Color GetOverlayColor();
		/// @brief 设置该动画的覆盖色
		/// @param color 设置后动画的覆盖色
		void SetOverlayColor(Color color);
		/// @brief 是否启用覆盖颜色模式
		T_PROPERTY(BOOLEAN, DrawOverlayColor, __get_DrawOverlayColor, __set_DrawOverlayColor, 0x90);
		/// @brief 设置染色模式，主要实现模仿者的染色
		T_PROPERTY(PaintState::PaintState, Paint, __get_Paint, __set_Paint, 0x98);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x9C);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 将该模型附加在指定识别 ID 的附件上。
		/// @param attachmentID 附件的识别 ID
		/// @param OffsetX X 坐标偏移
		/// @param OffsetY Y 坐标偏移
		/// @return 附加后的效果，可以用于设置变换矩阵
		AttachEffect AttachTo(AttachmentID attachmentID, float OffsetX, float OffsetY);
		/// @brief 移除该动画部件
		void Die();
		/// @brief 播放指定动画
		/// @param TrackName 动画轨道名称
		/// @param blendType 混合类型
		/// @param loopType 循环类型
		/// @param rate 播放帧频
		void Play(const char* TrackName, int blendType, PVZEnum::ReanimLoopType loopType, float rate);
		/// @brief 判断是否在播放指定动画
		/// @param TrackName 动画轨道名称
		bool IsAnimPlaying(const char* TrackName);
		/// @brief 判断动画在 theEventTime 时刻的事件是否应该触发
		/// @param theEventTime 事件触发的时刻, 取值范围为 [0, 1]
		bool ShouldTriggerTimedEvent(float theEventTime);
		/// @brief 设置指定前缀的所有轨道在绘制时的分组。通常情况下，分组为 -1 时表示隐藏该轨道。
		/// @param trackName 动作轨道的名称前缀
		/// @param renderGroup 分组大小
		void AssignRenderGroupToPrefix(const char* TrackName,byte RenderGroup);
		/// @brief 设置指定动画轨道在绘制时的分组。通常情况下，分组为 -1 时表示隐藏该轨道。
		/// @param trackName 执行的动作轨道名称
		/// @param renderGroup 分组大小
		void AssignRenderGroupToTrack(const char* trackName, byte renderGroup);
		int FindTrackIndex(const char* trackName);

		/// @brief 令动画部件执行 trackName 动作。
		/// @param trackName 执行的动作轨道名称。
		void SetFramesForLayer(const char* theTrackName);
		/// @brief 设置指定轨道的重载图片
		/// @param theTrackName 轨道名称
		/// @param theImage 图片
		void SetImageOverride(const char* theTrackName, Image theImage);
		/// @brief 用于补间动画开始混合时，设置每条轨道的初始补间动画数据。
		/// @param blendTime 需要设置的补间市场
		void StartBlend(int blendTime);
	};
	class Attachment : public BaseClass
	{
	public:
		/// @attention 从 3.0 起，该函数将不再具有按编号构造的功能。
		Attachment(uint32_t address) : BaseClass(address) {};
		/// @brief 获取指定编号的对象
		/// @param index 编号
		/// @note 不保证以此法获得的对象未被移除
		static Attachment GetByIndex(uint32_t index);
		/// @brief 获取附件效果
		/// @param index 编号
		/// @return 附件效果
		AttachEffect GetEffect(int index);
		/// @brief 附件效果总数
		INT_READONLY_PROPERTY(NumEffect, __get_NumEffects, 0x300);
		/// @brief 是否被移除
		T_READONLY_PROPERTY(BYTE, IsDead, __get_IsDead, 0x304);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x308);
		/// @brief 重载附件中所有内容的颜色。
		/// @param color 重载的颜色
		void OverrideColor(Color& color);
	};
	class AttachmentID : public BaseClass
	{
	public:
		AttachmentID(DWORD address) : BaseClass(address) {};
	};
	class TrackInstance : public BaseClass
	{
	public:
		/// @attention 从 3.0 起，该函数将不再具有按编号构造的功能。
		TrackInstance(uint32_t address) : BaseClass(address) {};
		/// @brief 获取指定编号的对象
		/// @param index 编号
		/// @note 不保证以此法获得的对象未被移除
		static TrackInstance GetByIndex(uint32_t index);
		AttachmentID GetAttachmentID();
		Attachment GetAttachment();
	};
	class GardenPlant : public BaseClass
	{
	public:
		GardenPlant(int address);
		T_PROPERTY(SeedType::SeedType, Type, __get_Type, __set_Type, 8);
		T_PROPERTY(GardenScene::GardenScene, Location, __get_Location, __set_Location, 0xC);
		INT_PROPERTY(Column, __get_Column, __set_Column, 0x10);
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x14);
		//GPD_LEFT or GPD_RIGTHT
		INT_PROPERTY(Direction, __get_Direction, __set_Direction, 0x18);
		T_PROPERTY(std::time_t, LastWateredTime, __get_LastWateredTime, __set_LastWateredTime, 0x20);
		INT_PROPERTY(Colour, __get_Colour, __set_Colour, 0x28);
		T_PROPERTY(GardenPlantState::GardenPlantState, State, __get_State, __set_State, 0x2C);
		INT_PROPERTY(WateredCount, __get_WateredCount, __set_WateredCount, 0x30);
		INT_PROPERTY(LevelUpWaterNeed, __get_LevelUpWaterNeed, __set_LevelUpWaterNeed, 0x34);
		T_PROPERTY(GardenPlantNeed::GardenPlantNeed, LevelUpNeed, __get_LevelUpNeed, __set_LevelUpNeed, 0x38);
		T_PROPERTY(std::time_t, LastGoldTime, __get_LastGoldTime, __set_LastGoldTime, 0x40);
		T_PROPERTY(std::time_t, LastGrowthTime, __get_LastGrowthTime, __set_LastGrowthTime, 0x48);
	};

	#pragma endregion

	#pragma region methods
	/// @brief 扩展绘制物件数组
	/// @param num 调整后的绘制物件数组元素数目上限
	/// @warning num 应当不低于 2049 。
	void ExtendRenderItem(uint32_t num);

	#pragma endregion
};