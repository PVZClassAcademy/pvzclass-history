#include "../../PVZ.h"

void PVZ::WidgetContainer::AddWidget(Widget widget)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(widget.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x5370A0)
		.ret()
	);
}

void PVZ::WidgetContainer::RemoveWidget(Widget widget)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(widget.GetBaseAddress())
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x5371C0)
		.ret()
	);
}