#include "../../PVZ.h"

PVZ::Attachment PVZ::Attachment::GetByIndex(uint32_t index)
{
	return PVZ::Attachment(Memory::ReadPointer(0x6A9EC0, 0x820, 0xC, 0) + index * 0x30C);
}

PVZ::AttachEffect PVZ::Attachment::GetEffect(int index)
{
	return AttachEffect(this->GetBaseAddress() + index * 0x30);
}

void PVZ::Attachment::OverrideColor(PVZ::Color& color)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(color.Alpha)
		.push(color.Blue)
		.push(color.Green)
		.push(color.Red)
		.push_reg(REG_ESP)
		.mov_reg_imm(REG_ECX, this->BaseAddress)
		.invoke(0x404780)
		.add_reg_imm(REG_ESP, 16)
		.ret()
	);
}