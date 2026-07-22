#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class BaseClass;
	/// @brief 粒子系统。
	class TodParticleSystem : public BaseClass
	{
	public:
		/// @attention 从 3.0 起，该函数将不再具有按编号构造的功能。
		TodParticleSystem(uint32_t address) : BaseClass(address) {};
		/// @brief 获取指定编号的对象
		/// @param index 编号
		/// @note 不保证以此法获得的对象未被移除
		static TodParticleSystem GetByIndex(uint32_t index);
		/// @brief 是否已被移除。
		T_PROPERTY(BOOLEAN, Dead, __get_Dead, __set_Dead, 0x1C);
		T_PROPERTY(DWORD, Id, __get_Id, __set_Id, 0x28);
		/// @brief 移除此粒子系统。
		void Die();
		/// @brief 将粒子系统移至指定位置。
		/// @param X 新位置 X 坐标
		/// @param Y 新位置 Y 坐标
		void MoveTo(float X, float Y);
		/// @brief 重载粒子系统的颜色。
		/// @param emitter_name 重载的粒子系统的发射器名称。若为空指针，则重载所有发射器。
		/// @param color 重载的颜色
		void OverrideColor(const char* emitter_name, Color& color);
		/// @brief 更改粒子系统内所有发射器中粒子是否高亮。
		/// @param isEnable 是否启用高亮
		void OverrideExtraAdditiveDraw(bool isEnable);
		/// @brief 重载粒子系统内所有发射器中粒子的图像。
		/// @param image 重载的图像
		void OverrideImage(Image image);
		/// @brief 重载粒子系统内所有发射器中粒子的大小。
		/// @param scale 重载的大小倍数
		void OverrideScale(float scale);
		/// @brief 将粒子系统挂载到指定 AttachmentID，使其跟随目标对象移动。
		/// @param attachmentID 目标附件 ID
		/// @param OffsetX X 偏移
		/// @param OffsetY Y 偏移
		/// @return AttachEffect
		AttachEffect AttachTo(AttachmentID attachmentID, float OffsetX, float OffsetY);
	};
	/// @brief 获取所有粒子系统。
	/// @return 包含所有粒子系统的 vector。
	std::vector<TodParticleSystem> GetAllParticleSystem();
	/// @brief 创建一个粒子系统。
	/// @param X 粒子系统的 X 坐标
	/// @param Y 粒子系统的 Y 坐标
	/// @param render_order 粒子系统的绘制顺序，数值大的会覆盖数值小的
	/// @param type 粒子系统类型
	/// @return 创建的粒子系统
	PVZ::TodParticleSystem CreateParticleSystem(float X, float Y, int render_order, EffectType::EffectType type);
}
