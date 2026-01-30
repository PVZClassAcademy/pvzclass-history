#pragma once
#include "DLLEvent.h"

/// @brief 僵尸啃食植物事件
/// @param 触发事件的僵尸和植物
class ZombieEatEvent : public DLLEventTemplate<0x52FB40, 7, REG_ECX, REG_EDI>
{
public:
	ZombieEatEvent() : DLLEventTemplate() { Init("onZombieEat"); };
	ZombieEatEvent(const char* str) : DLLEventTemplate() { Init(str); };
	ZombieEatEvent(int address) : DLLEventTemplate() { Init(address); };
};

namespace PVZEvent
{
	/// @brief 掉头僵尸判断啃食事件
	/// @param 触发事件的僵尸
	/// @return 僵尸是否可以啃食
	class ZombieNoHeadEatEvent
	{
	private:
		class Part1 : public DiversionEventTemplate<0x52BD13, 7, 0x52BD20, 0x52BE4E, REG_EDI>
		{
		public:
			Part1(int address) : DiversionEventTemplate() { Init(address); };
		} *p1;
		class Part2 : public DiversionEventTemplate<0x52F629, 6, 0x52F635, 0x52F6BC, REG_EDI>
		{
		public:
			Part2(int address) : DiversionEventTemplate() { Init(address); };
		} *p2;
	public:
		ZombieNoHeadEatEvent() : ZombieNoHeadEatEvent("IsZombieNoHeadEat") {};
		ZombieNoHeadEatEvent(const char* name) : ZombieNoHeadEatEvent(PVZ::Memory::GetProcAddress(name)) {};
		ZombieNoHeadEatEvent(int address)
		{
			p1 = new Part1(address);
			p2 = new Part2(address);
		}
		void end()
		{
			p1->end();
			p2->end();
		}
	};

	/// @brief 僵尸啃食植物时，植物受到伤害事件
	/// @param zombie，plant, damage
	/// @note 此函数会重载原版受伤的代码
	/// @return 重载后的伤害，负数会取消伤害
	/// @todo 将修改伤害的功能使用传入引用实现，植物是否受到伤害改用返回值返回ThreeState判断
	class ZombieEatDamagePlantEvent : public DLLEventTemplate<0x52FCF0, 7, CONST_VAL(4), REG_ESI, REG_EBP>
	{
	public:
		ZombieEatDamagePlantEvent() : DLLEventTemplate() { Init("onZombieEatDamagePlant"); };
		ZombieEatDamagePlantEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieEatDamagePlantEvent(int address) : DLLEventTemplate() { Init(address); };
	protected:
		inline static constexpr byte asm_code[3] = { 0x29, 0x46, 0x40 };//sub [esi+40h] eax
		virtual void InitExtra(AsmBuilder& builder) override
		{
			builder.cmp_reg_imm(REG_EAX, 0)
				.js_rel(3)
				.add_bytes(asm_code, 3)
				.popad()
				.mov_reg_mem_reg_add_imm(REG_ECX, REG_ESI, 0x40)
				.push_imm32(0x52FCF7)
				.ret();
		}
	};
}