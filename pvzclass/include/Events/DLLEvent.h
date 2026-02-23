#pragma once
#include "../../PVZ.h"
#include <array>
#include <iostream>

/// @file DLLEvent.h
/// @brief 存放所有事件类的基类，以及它相关的各种内容

#define MEM_ESP_ADD_MASK 8
/// @brief 用于在 EventTemplate 中表示 [esp+offset] 型的间接寻址。
/// @note offset 的类型为 byte，因此它的值不应超过 256 。
#define MEM_ESP_ADD(offset) (offset)
#define CONST_VAL_MASK 0x100
/// @brief 用于在 EventTemplate 中表示立即数型参数。
#define CONST_VAL(v) (CONST_VAL_MASK + (v))

using std::cout;
using std::hex;
using std::endl;

/// @brief 所有事件类的基类。结算函数的调用约定应当为 __cdecl
class DLLEvent
{
public:
	/// @brief 取消该事件产生的效应。
	void end();

protected:
	int rawlen, hookAddress;
	/// @brief 将指定代码段注入。
	/// @param code 代码段
	/// @param len 代码段长度
	void start(BYTE* code, int len);

private:
	BYTE* rawCode;
	static int newAddress;
};

/// @brief DLLEvent 的简化版本，用于快速设置一个事件，其结算函数的返回值为 void。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD ...Params>
class DLLEventTemplate : public DLLEvent
{
protected:
	template<DWORD param>
	static constexpr size_t param_size()
	{
		if constexpr (param < MEM_ESP_ADD_MASK)
			return 1;
		else if constexpr (param < CONST_VAL_MASK)
			return 4;
		else
			return 5;
	}
	static constexpr size_t calculate_total_size()
	{
		size_t total = 0;
		((total += param_size<Params>()), ...);
		return total;
	}
	static constexpr auto build_base_bytes()
	{
		constexpr size_t total_size = calculate_total_size();
		std::array<uint8_t, total_size> bytes{};
		size_t offset = 0;

		// 使用 lambda 处理每个参数
		auto process = [&](auto param)
		{
			if (param < MEM_ESP_ADD_MASK)
				bytes[offset++] = 0x50 | (param & 0x7);
			else if (param < CONST_VAL_MASK)
			{
				bytes[offset++] = 0xFF;
				bytes[offset++] = 0x74;
				bytes[offset++] = 0x24;
				bytes[offset++] = static_cast<uint8_t>(param);
			}
			else
			{
				DWORD value = param - CONST_VAL_MASK;
				bytes[offset++] = 0x68;
				bytes[offset++] = static_cast<uint8_t>(value & 0xFF);
				bytes[offset++] = static_cast<uint8_t>((value >> 8) & 0xFF);
				bytes[offset++] = static_cast<uint8_t>((value >> 16) & 0xFF);
				bytes[offset++] = static_cast<uint8_t>((value >> 24) & 0xFF);
			}
		};

		// 展开参数包
		(process(Params), ...);

		return bytes;
	}
	static constexpr auto compiled_base_bytes = build_base_bytes();
protected:
	void Init(int address)
	{
		hookAddress = _Hook_Address;
		rawlen = _Raw_Len;
		AsmBuilder builder{};
		builder.add_bytes(compiled_base_bytes.data(), calculate_total_size());

		builder.invoke(address).add_reg_imm(REG_ESP, this->regs.size() << 2);
		this->InitExtra(builder);

		start(builder.get_code() + 1, builder.get_length() - 1);
	}
	void Init(const char* str)
	{
		Init(PVZ::Memory::GetProcAddress(str));
	}
	virtual void InitExtra(AsmBuilder& builder)
	{
		return;
	}
public:
	static constexpr std::array<DWORD, sizeof...(Params)> regs = { Params... };
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为 bool。若返回值为 false，则会在清栈后跳转至指定位置。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Cancel_Addr 若返回值为 false，则会在清栈后向此地址跳转
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Cancel_Addr, DWORD ...Params>
class BoolDLLEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	static constexpr std::array<uint8_t, 11> compiled_special_bytes = { TEST_AL_AL, JNZ(7), POPAD, PUSHDWORD(_Cancel_Addr), RET };
	virtual void InitExtra(AsmBuilder& builder)
	{
		builder.add_bytes(compiled_special_bytes.data(), 11);
	}
};

/// @brief 与 BoolDLLEventTemplate 类似，但会在返回值为 true 时跳转。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Cancel_Addr 若返回值为 true，则会在清栈后向此地址跳转
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Cancel_Addr, DWORD ...Params>
class TrueDLLEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	static constexpr std::array<uint8_t, 11> compiled_special_bytes = { TEST_AL_AL, JE(7), POPAD, PUSHDWORD(_Cancel_Addr), RET };
	virtual void InitExtra(AsmBuilder& builder)
	{
		builder.add_bytes(compiled_special_bytes.data(), 11);
	}
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为 int。若返回值为指定数值，则会在清栈后跳转至指定位置。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Cancel_Addr 若返回值为指定数值，则会在清栈后向此地址跳转。为 0 则不会进行相关检测。
/// @tparam _Cancel_val 触发跳转的返回值
/// @tparam _Out_Param 若没有跳转，存储返回值的位置（立即数取值会变为立即数寻址）。
/// @tparam _Lower_Bound 接受的返回值的下界
/// @tparam _Exit 存储返回值后，是否直接 ret
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Cancel_Addr, int _Cancel_val,
	int _Lower_Bound, DWORD _Out_Param, bool _Exit, DWORD ...Params>
class IntDLLEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	virtual void InitExtra(AsmBuilder& builder)
	{
		if (_Cancel_Addr)
			builder.cmp_reg_imm(REG_EAX, _Cancel_val).jne_rel(7).popad().push_imm32(_Cancel_Addr).ret();

		if (_Out_Param < MEM_ESP_ADD_MASK)
		{
			if (_Lower_Bound > INT32_MIN)
				builder.cmp_reg_imm(REG_EAX, _Lower_Bound).jl_rel(5 + (_Exit ? 1 : 2));
			builder.mov_mem_esp_add_imm8_reg(0x1C - ((_Out_Param & 7) << 2), REG_EAX).popad();
		}
		else if (_Out_Param < CONST_VAL_MASK)
		{
			if (_Lower_Bound > INT32_MIN)
				builder.cmp_reg_imm(REG_EAX, _Lower_Bound).jl_rel(5 + (_Exit ? 1 : 2));
			builder.mov_mem_esp_add_imm8_reg(_Out_Param, REG_EAX).popad();
		}
		else
		{
			if (_Lower_Bound > INT32_MIN)
				builder.cmp_reg_imm(REG_EAX, _Lower_Bound).jl_rel(7 + (_Exit ? 1 : 2));
			builder.mov_mem_reg(_Out_Param - CONST_VAL_MASK, REG_EAX).popad();
		}

		if (_Exit)
			builder.ret();
		else
			builder.jmp_rel8(_Raw_Len + 1);
	}
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为 float。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Out_Param 存储返回值的位置（立即数取值会变为立即数寻址）。0~7的数值会视为 ST 寄存器，而不是常规 32 位寄存器。
/// @tparam _Exit 存储返回值后，是否直接 ret
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Out_Param, bool _Exit, DWORD ...Params>
class FloatDLLEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	virtual void InitExtra(AsmBuilder& builder)
	{
		if (_Out_Param < MEM_ESP_ADD_MASK)
			builder.fstp_ST(_Out_Param);
		else if (_Out_Param < CONST_VAL_MASK)
			builder.fstp_m32_esp_imm8(_Out_Param);
		else
			builder.fstp(_Out_Param - CONST_VAL_MASK);

		if (_Exit)
			builder.ret();
	}
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为基址。若返回空指针，则按原本的方法获取基址。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Out_Param 若没有跳转，存储获取的值的位置（立即数取值会变为立即数寻址）
/// @tparam _Out_Offset 若没有跳转，则为取值时的偏移大小
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Out_Param, DWORD _Out_Offset, DWORD ...Params>
class BaseAddressEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	virtual void InitExtra(AsmBuilder& builder)
	{
		builder.test_reg_reg(REG_EAX, REG_EAX);

		if (_Out_Param < MEM_ESP_ADD_MASK)
		{
			builder.jl_rel(9);
			builder.mov_reg_mem_reg_add_imm32(_Out_Param, REG_EAX, _Out_Offset);
			builder.popad().jmp_rel8(_Raw_Len + 1);
		}
		else if (_Out_Param < CONST_VAL_MASK)
		{
			builder.jl_rel(13);
			builder.mov_reg_mem_reg_add_imm32(REG_EAX, REG_EAX, _Out_Offset)
				.mov_mem_esp_add_imm8_reg(_Out_Param, REG_EAX);
			builder.popad().jmp_rel8(_Raw_Len + 1);
		}
		else
		{
			builder.jl_rel(15);
			builder.mov_reg_mem_reg_add_imm32(REG_EAX, REG_EAX, _Out_Offset)
				.mov_mem_reg(_Out_Param, REG_EAX);
			builder.popad().jmp_rel8(_Raw_Len + 1);
		}
	}
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为 bool。根据返回值的类型，将会跳转到不同的地址。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _True_Addr 返回值为 true 时跳转的地址
/// @tparam _False_Addr 返回值为 false 时跳转的地址
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _True_Addr, DWORD _False_Addr, DWORD ...Params>
class DiversionEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	static constexpr std::array<uint8_t, 17> compiled_special_bytes =
	{
		TEST_AL_AL,
		POPAD,
		JE(6),

		PUSHDWORD(_True_Addr),
		RET,

		PUSHDWORD(_False_Addr),
		RET
	};
	virtual void InitExtra(AsmBuilder& builder)
	{
		builder.add_bytes(compiled_special_bytes.data(), 17);
	}
};

/// @brief DLLEvent 的扩展，用于快速设置一个事件，其结算函数的返回值为 int。根据返回值的正负性，将会跳转到不同的地址。负数不会进行跳转。
/// @tparam _Hook_Address 原始代码的首地址
/// @tparam _Raw_Len 替代的原始代码长度
/// @tparam _Posi_Addr 返回值为正数时跳转的地址
/// @tparam _Zero_Addr 返回值为 0 时跳转的地址
/// @tparam ...Params 结算函数的参数来源。顺序为 push 顺序（即参数列表反序）
template<DWORD _Hook_Address, uint8_t _Raw_Len, DWORD _Posi_Addr, DWORD _Zero_Addr, DWORD ...Params>
class ThreeStateEventTemplate : public DLLEventTemplate<_Hook_Address, _Raw_Len, Params...>
{
protected:
	static constexpr std::array<uint8_t, 19> compiled_special_bytes =
	{
		TEST_AL_AL,
		JS(15),
		POPAD,
		JE(6),

		PUSHDWORD(_Posi_Addr),
		RET,

		PUSHDWORD(_Zero_Addr),
		RET
	};
	virtual void InitExtra(AsmBuilder& builder)
	{
		builder.add_bytes(compiled_special_bytes.data(), 19);
	}
};