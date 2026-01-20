#pragma once
#include "Enums.h"
#include "include/Flags.hpp"
#include "AsmFunctions.h"
#include "AsmBuilder.hpp"
#include "Memory.hpp"
#include <ctime>
#include <memory>
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

#define SPT std::shared_ptr
#define UPT std::unique_ptr
#define MKU std::make_unique
#define MKS std::make_shared

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

constexpr auto INVALID_BASEADDRESS = 0x400000;

/// @brief 包含大部分用于控制 PVZ 内部对象的类和方法。
/// @note Only version 1.0.0.1051 is fully supported
namespace PVZ
{
	/// @brief 初始化 PVZ 命名空间，且不在程序内附加钩子。Memory::immediateExecute 会设置为 true 。
	/// @param pid 进程 id
	void InitPVZNoLock(DWORD pid);

	void InitPVZ(DWORD pid);
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
	/// @attention 你不应该直接构造 BaseClass！
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
		/// @brief 将给定的字符串转换为对应的 PVZ 字符串
		/// @param str 字符串
		/// @return 转换后的字符串
		static PVZString ToString(const char* str);
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
		/// @param len 长度
		void Concat(const char* src, int len);
		/// @brief 拼接字符串
		/// @param src 字符串
		void Concat(PVZ::PVZString src);
	};

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
		WidgetContainer(int address) : BaseClass(address) {};
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

		/// @brief 根据指定的平移坐标、旋转弧度和拉伸比例，设定矩阵每个项的数值。
		void ScaleRotateTransformMatrix(float x, float y, float rad, float ScaleX, float ScaleY);
	};
	class TrackInstance;
	class AttachEffect : public BaseClass
	{
	public:
		AttachEffect(int address) : BaseClass(address) {};
		Matrix3 GetOffset();
	};
	/// @brief 动画部件。相当于其他游戏的模型。
	class Animation : public BaseClass
	{
	public:
		Animation(int idoraddress);
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
		void AssignRenderGroupToPrefix(byte RenderGroup, const char* TrackName);
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
	};
	class Attachment : public BaseClass
	{
	public:
		Attachment(int idoraddress);
		// TODO: check whether this function works properly.
		PVZ::Animation GetAnimation();
		INT_READONLY_PROPERTY(Id, __get_Id, 0x308);
	};
	class AttachmentID : public BaseClass
	{
	public:
		AttachmentID(DWORD address) : BaseClass(address) {};
	};
	class TrackInstance : public BaseClass
	{
	public:
		TrackInstance(int idoraddress);
		AttachmentID GetAttachmentID();
		Attachment GetAttachment();
	};
	// 鼠标对象(控制层面的鼠标)
	class Mouse : public BaseClass
	{
	public:
		Mouse(int baseaddress) : BaseClass(baseaddress) {};
		T_READONLY_PROPERTY(BOOLEAN, InGameArea, __get_InGameArea, 0xDC);
		INT_PROPERTY(X, __get_X, __set_X, 0xE0);
		INT_PROPERTY(Y, __get_Y, __set_Y, 0xE4);
		T_READONLY_PROPERTY(MouseClickState::MouseClickState, ClickState, __get_ClickState, 0xE8);
		void WMClick(short x, short y);
		void GameClick(int x, int y);
		void MoveTo(int x, int y);
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

	void InitImages();
	Mouse GetMouse();

	#pragma endregion

	#pragma region Images

	class Resource
	{
	public:
		static Image* IMAGE_BLANK;
	};

	#pragma endregion
};