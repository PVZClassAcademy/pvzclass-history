#pragma once
#include "DLLEvent.h"

namespace PVZEvent {
	/// @brief 子弹运动时碰撞箱击中检测事件
	/// @param 子弹
	/// @return 负数则使用原版判断，0时只检测僵尸，正数时只检测植物
	/// @note 如果需要让原本不能击中僵尸的子弹击中僵尸还需要修改子弹的DamageAbility
	class ProjectileCollisionEvent : public DLLEvent
	{
	public:
		ProjectileCollisionEvent(int address) : DLLEvent() { Init(address); }
		class ProjectileNormalCollisionEvent : public DLLEventTemplate<0x46CFC5, 10, REG_EBP>
		{
		public:
			ProjectileNormalCollisionEvent(int address) : DLLEventTemplate() { Init(address); }
		protected:
			void Init(int address)
			{
				hookAddress = 0x46CFC5;
				rawlen = 10;
				LabelBuilder builder{};
				builder.add_bytes(compiled_base_bytes.data(), calculate_total_size());

				builder.invoke(address).add_reg_imm(REG_ESP, this->regs.size() << 2)
					.cmp_reg_imm(REG_EAX, 0)
					.popad()
					.push_reg(REG_EBP)

					.jge_label("no_pvz_judge")
					.cmp_reg_imm(REG_ECX, 0xD)
					.jnz_label("hit_zombie")
					.jmp_label("hit_plant")
					.label("no_pvz_judge")

					.jz_label("hit_zombie")
					.label("hit_plant")
					.jmp_to(0x46CFCF)
					.label("hit_zombie")
					.jmp_to(0x46D058);

				start(builder.get_code() + 1, builder.get_length() - 1);
			}
		} *normal;

		class ProjectileThrowCollisionEvent : public DLLEventTemplate<0x46D63E, 10, REG_EBP>
		{
		public:
			ProjectileThrowCollisionEvent(int address) : DLLEventTemplate() { Init(address); }
		protected:
			void Init(int address)
			{
				hookAddress = 0x46D63E;
				rawlen = 10;
				LabelBuilder builder{};
				builder.add_bytes(compiled_base_bytes.data(), calculate_total_size());

				builder.invoke(address).add_reg_imm(REG_ESP, this->regs.size() << 2)
					.cmp_reg_imm(REG_EAX, 0)
					.popad()
					.jge_label("no_pvz_judge")
					.cmp_reg_imm(REG_EAX, 0x9)
					.jz_label("hit_plant")
					.cmp_reg_imm(REG_EAX, 0xD)
					.jz_label("hit_plant")
					.jmp_to(0x46D648)

					.label("no_pvz_judge")
					.jz_label("hit_zombie")
					.label("hit_plant")
					.jmp_to(0x46D656)
					.label("hit_zombie")
					.jmp_to(0x46D648);

				start(builder.get_code() + 1, builder.get_length() - 1);
			}
		} *throvv;
		// part3没看懂所以不搬
	public:
		void Init(int address)
		{
			normal = new ProjectileNormalCollisionEvent(address);
			throvv = new ProjectileThrowCollisionEvent(address);
		}
		void end()
		{
			normal->end();
			throvv->end();
		}
	};

	/// @brief 子弹击中植物时，判断使用的受伤植物顺序的事件
	/// @param 子弹地址
	/// @note 目前不支持自定义
	/// @retval ThreeState::Enable 投掷顺序
	/// @retval ThreeState::Disable 啃食顺序
	/// @retval ThreeState::None 与原版一致
	class ProjectileHitPlantPriorityEvent_ts : public ThreeStateEventTemplate<0x46CB5D, 7, 0x46CBC9, 0x46CB6C, REG_EBP>
	{
	public:
		ProjectileHitPlantPriorityEvent_ts(const char* str) : ThreeStateEventTemplate() { Init(str); };
		ProjectileHitPlantPriorityEvent_ts(int address) : ThreeStateEventTemplate() { Init(address); };
		ProjectileHitPlantPriorityEvent_ts() : ProjectileHitPlantPriorityEvent_ts("onProjectileHitPlantPriority") {};
	};
};