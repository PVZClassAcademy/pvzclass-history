#pragma once
#include "DLLEvent.h"
#include "../Classes/GameObject.hpp"

/// @brief 植物受到啃食伤害事件。
/// @note 此事件不考虑 IZ 模式下的双倍伤害。若要手动避免，请另行结算后取消伤害，
/// @param 依次为：植物地址、僵尸地址、GameObjectType::OBJECT_TYPE_NONE、伤害数值（非引用）
/// @return 调整后的伤害数值，负数会取消伤害。
class PlantTakeEatDamageEvent : public DLLEvent
{
public:
	PlantTakeEatDamageEvent() : PlantTakeEatDamageEvent("onPlantTakeEatDamage") {};
	PlantTakeEatDamageEvent(const char* name) : PlantTakeEatDamageEvent(PVZ::Memory::GetProcAddress(name)) {};
	PlantTakeEatDamageEvent(int address)
	{
		hookAddress = 0x52FCF0;
		rawlen = 7;
		BYTE code[] =
		{
			PUSH(4),
			PUSHDWORD(GameObjectType::OBJECT_TYPE_NONE),
			PUSH_EBP,
			PUSH_ESI,
			INVOKE(address),
			ADD_ESP(16),
			TEST_EUX_EVX(REG_EAX, REG_EAX),
			JNS(7),

			POPAD,
			PUSHDWORD(0x52FD2E),
			RET,

			0x29,0x46,0x40,
			POPAD,
			0x8B,0x4E,0x40,
			PUSHDWORD(0x52FCF7),
			RET
		};
		start(STRING(code));
	}
};

/// @brief 植物受到子弹伤害事件。
/// @param 依次为：植物地址、子弹地址、子弹的 GameObjectType、伤害数值（非引用）
/// @return 调整后的伤害数值，负数会取消伤害。
class PlantTakeProjectileDamageEvent
{
private:
	class Part1 : public DLLEvent
	{
	public:
		Part1(int address)
		{
			hookAddress = 0x46CFEB;
			rawlen = 6;
			BYTE code[] =
			{
				PUSH_EAX,

				PUSH_EDX,
				PUSHDWORD(GameObjectType::OBJECT_TYPE_PROJECTILE),
				PUSH_EBP,
				PUSH_EAX,
				INVOKE(address),

				ADD_ESP(16),
				TEST_EUX_EVX(REG_EAX, REG_EAX),
				POP_EUX(REG_ECX),
				JS(9),

				0xF7, 0xD8,
				ADD_PTR_EUX_ADD_V_EVX(REG_ECX, 0x40, REG_EAX),
				POPAD,
				JMP(12),

				POPAD,
				MOV_ECX(0x46CFFE),
				JMP_REG32(REG_ECX),
			};
			start(STRING(code));
		}
	} *part1;
	class Part2 : public DLLEvent
	{
	public:
		Part2(int address)
		{
			hookAddress = 0x46D7A6;
			rawlen = 6;
			BYTE code[] =
			{
				PUSH_ECX,
				PUSHDWORD(GameObjectType::OBJECT_TYPE_PROJECTILE),
				PUSH_EBP,
				PUSH_ESI,
				INVOKE(address),

				ADD_ESP(16),
				TEST_EUX_EVX(REG_EAX, REG_EAX),
				JS(9),

				0xF7, 0xD8,
				ADD_PTR_EUX_ADD_V_EVX(REG_ESI, 0x40, REG_EAX),
				POPAD,
				JMP(12),

				POPAD,
				MOV_ECX(0x46CFFE),
				JMP_REG32(REG_ECX),
			};
			start(STRING(code));
		}
	} *part2;
public:
	PlantTakeProjectileDamageEvent() : PlantTakeProjectileDamageEvent("onPlantTakeProjectileDamage") {};
	PlantTakeProjectileDamageEvent(const char* name) : PlantTakeProjectileDamageEvent(PVZ::Memory::GetProcAddress(name)) {};
	PlantTakeProjectileDamageEvent(int address)
	{
		part1 = new Part1(address);
		part2 = new Part2(address);
	}
	void end()
	{
		part1->end();
		part2->end();
	}
};

/// @brief 钢地刺因车辆、碾压等受伤事件。
/// @param 依次为：植物地址、0、GameObjectType::None、伤害数值（非引用）
/// @return 调整后的伤害数值，负数会取消伤害。
class SpikeRockTakeDamageEvent : public DLLEvent
{
public:
	SpikeRockTakeDamageEvent() : SpikeRockTakeDamageEvent("onSpikeRockTakeDamage") {};
	SpikeRockTakeDamageEvent(const char* name) : SpikeRockTakeDamageEvent(PVZ::Memory::GetProcAddress(name)) {};
	SpikeRockTakeDamageEvent(int address)
	{
		hookAddress = 0x45EC63;
		rawlen = 7;
		BYTE code[] =
		{
			PUSH(50),
			PUSHDWORD(GameObjectType::OBJECT_TYPE_NONE),
			PUSH(0),
			PUSH_ESI,
			INVOKE(address),
			ADD_ESP(16),
			TEST_EUX_EVX(REG_EAX, REG_EAX),
			JS(8),

			0xF7, 0xD8,
			ADD_PTR_EUX_ADD_V_EVX(REG_ESI, 0x40, REG_EAX),
			POPAD,
			JMP(13),

			POPAD,
			MOV_ECX(0x45ECF2),
			JMP_REG32(REG_ECX),
		};
		start(STRING(code));
	}
};

/// @brief 植物受伤事件。
/// @note 复合事件。
/// @note 此事件不考虑 IZ 模式下的双倍伤害。若要手动避免，请另行结算后取消伤害，
/// @param 依次为：植物地址、伤害来源地址（可能为空）、伤害来源的 GameObjectType、伤害数值（非引用）
/// @return 调整后的伤害数值，负数会取消伤害。
class PlantTakeDamageEvent
{
private:
	PlantTakeEatDamageEvent* pted_event;
	PlantTakeProjectileDamageEvent* ptej_event;
	SpikeRockTakeDamageEvent* srtd_event;
public:
	PlantTakeDamageEvent() : PlantTakeDamageEvent("onPlantTakeDamage") {};
	PlantTakeDamageEvent(const char* name) : PlantTakeDamageEvent(PVZ::Memory::GetProcAddress(name)) {};
	PlantTakeDamageEvent(int address)
	{
		pted_event = new PlantTakeEatDamageEvent(address);
		ptej_event = new PlantTakeProjectileDamageEvent(address);
		srtd_event = new SpikeRockTakeDamageEvent(address);
	}
	void end()
	{
		pted_event->end();
		ptej_event->end();
		srtd_event->end();
	}
};
