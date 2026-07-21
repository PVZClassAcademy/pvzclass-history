#pragma once
#include "../../PVZ.h"

namespace PVZ
{
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
		T_SIMPLE_PROPERTY(FLOAT, AnimTime, 4);
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
		void AssignRenderGroupToPrefix(const char* TrackName, byte RenderGroup);
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
}