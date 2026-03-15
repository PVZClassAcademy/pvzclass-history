#include "Sexy.h"

PVZ::Sexy::PButtonListener PVZ::Sexy::MakeButtonListener(ButtonListener* listener)
{
	int address = PVZ::Memory::AllocMemory(0, 32);
	PVZ::Memory::WriteMemory<int>(address, address + 4);
	PVZ::Memory::WriteMemory<ButtonListener>(address + 4, *listener);
	return address;
}

PVZ::Sexy::PEditListener PVZ::Sexy::MakeEditListener(EditListener* listener)
{
	int address = PVZ::Memory::AllocMemory(0, 20);
	PVZ::Memory::WriteMemory<int>(address, address + 4);
	PVZ::Memory::WriteMemory<EditListener>(address + 4, *listener);
	return address;
}

PVZ::Sexy::PCheckboxListener PVZ::Sexy::MakeCheckboxListener(CheckboxListener* listener)
{
	int address = PVZ::Memory::AllocMemory(0, 8);
	PVZ::Memory::WriteMemory<int>(address, address + 4);
	PVZ::Memory::WriteMemory<CheckboxListener>(address + 4, *listener);
	return address;
}

PVZ::Sexy::PListListener PVZ::Sexy::MakeListListener(ListListener* listener)
{
	int address = PVZ::Memory::AllocMemory(0, 16);
	PVZ::Memory::WriteMemory<int>(address, address + 4);
	PVZ::Memory::WriteMemory<ListListener>(address + 4, *listener);
	return address;
}

BYTE __asm__MakeButton[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x448620),
	ADD_ESP(0x0C),
	MOV_PTR_ADDR_EAX(0),
	RET
};

PVZ::Sexy::PButton PVZ::Sexy::MakeButton(PVZ::PVZString str, PButtonListener listener, int theId)
{
	SETARG(__asm__MakeButton, 1) = str.GetBaseAddress();
	SETARG(__asm__MakeButton, 6) = listener;
	SETARG(__asm__MakeButton, 11) = theId;
	SETARG(__asm__MakeButton, 32) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__MakeButton));
}

BYTE __asm__MakeImageButton[]
{
	PUSHDWORD(0),
	MOV_EBX(0),
	MOV_EDI(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x448BC0),
	ADD_ESP(0x14),
	MOV_PTR_ADDR_EAX(0),
	RET
};

PVZ::Sexy::PButton PVZ::Sexy::MakeImageButton(Draw::PImage down, Draw::PImage over, Draw::PImage normal,
	DWORD fontAddress, PVZ::PVZString str, PButtonListener listener, int theId)
{
	SETARG(__asm__MakeImageButton, 1) = down;
	SETARG(__asm__MakeImageButton, 6) = over;
	SETARG(__asm__MakeImageButton, 11) = normal;
	SETARG(__asm__MakeImageButton, 16) = fontAddress;
	SETARG(__asm__MakeImageButton, 21) = str.GetBaseAddress();
	SETARG(__asm__MakeImageButton, 26) = listener;
	SETARG(__asm__MakeImageButton, 31) = theId;
	SETARG(__asm__MakeImageButton, 52) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__MakeImageButton));
}

BYTE __asm__MakeDialog[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	MOV_ECX_PTR_ADDR(0x6A9EC0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0x64),
	CALL_EUX(2),
	MOV_PTR_ADDR_EAX(0),
	RET
};

PVZ::Sexy::PDialog PVZ::Sexy::MakeDialog(int buttonMode, PVZ::PVZString footer, PVZ::PVZString lines,
	PVZ::PVZString header, int modal, int dialogId)
{
	SETARG(__asm__MakeDialog, 1) = buttonMode;
	SETARG(__asm__MakeDialog, 6) = footer.GetBaseAddress();
	SETARG(__asm__MakeDialog, 11) = lines.GetBaseAddress();
	SETARG(__asm__MakeDialog, 16) = header.GetBaseAddress();
	SETARG(__asm__MakeDialog, 21) = modal;
	SETARG(__asm__MakeDialog, 26) = dialogId;
	SETARG(__asm__MakeDialog, 47) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__MakeDialog));
}

BYTE __asm__MakeEdit[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x4567B0),
	ADD_ESP(0x08),
	MOV_PTR_ADDR_EAX(0),
	RET
};

PVZ::Sexy::PEdit PVZ::Sexy::MakeEdit(PDialog dialog, PEditListener listener)
{
	SETARG(__asm__MakeEdit, 1) = dialog;
	SETARG(__asm__MakeEdit, 6) = listener;
	SETARG(__asm__MakeEdit, 27) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__MakeEdit));
}

PVZ::PVZString PVZ::Sexy::GetEditString(PEdit edit)
{
	return PVZ::PVZString(edit + 0x8C);
}

void PVZ::Sexy::SetEditString(PEdit edit, PVZ::PVZString pstr, bool left)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push(left)
		.push(pstr.GetBaseAddress())
		.mov_reg_imm(REG_ECX, edit)
		.mov_reg_mem_reg_add_imm(REG_EDX, REG_ECX, 0)
		.mov_reg_mem_reg_add_imm(REG_EDX, REG_EDX, 0x124)
		.call_reg(REG_EDX)
		.ret()
	);
}

BYTE __asm__MakeCheckbox[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x456860),
	ADD_ESP(0x0C),
	MOV_PTR_ADDR_EAX(0),
	RET
};

PVZ::Sexy::PCheckbox PVZ::Sexy::MakeCheckbox(int checked, PCheckboxListener listener, int theId)
{
	SETARG(__asm__MakeCheckbox, 1) = checked;
	SETARG(__asm__MakeCheckbox, 6) = listener;
	SETARG(__asm__MakeCheckbox, 11) = theId;
	SETARG(__asm__MakeCheckbox, 32) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__MakeCheckbox));
}

bool PVZ::Sexy::IsCheckboxChecked(PCheckbox checkbox)
{
	return PVZ::Memory::ReadMemory<bool>(checkbox + 0x90);
}

BYTE __asm__setCheckboxChecked[]
{
	PUSH(0),
	PUSH(0),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0x118),
	CALL_EUX(2),
	RET
};

void PVZ::Sexy::setCheckboxChecked(PCheckbox checkbox, bool checked, bool tellListener)
{
	__asm__setCheckboxChecked[1] = tellListener;
	__asm__setCheckboxChecked[3] = checked;
	SETARG(__asm__setCheckboxChecked, 5) = checkbox;
	PVZ::Memory::Execute(STRING(__asm__setCheckboxChecked));
}

BYTE __asm__MakeList[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x53AC70),
	RET
};

PVZ::Sexy::PList PVZ::Sexy::MakeList(PListListener listener)
{
	int address = PVZ::Memory::AllocMemory(0, 0xF8);
	SETARG(__asm__MakeList, 1) = listener;
	SETARG(__asm__MakeList, 6) = address;
	PVZ::Memory::Execute(STRING(__asm__MakeList));
	return address;
}

BYTE __asm__AddListLine[]
{
	PUSH(0),
	PUSHDWORD(0),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0x124),
	CALL_EUX(2),
	MOV_PTR_ADDR_EAX(0),
	RET
};

int PVZ::Sexy::AddListLine(PList list, PVZ::PVZString line, bool alphabetical)
{
	__asm__AddListLine[1] = alphabetical;
	SETARG(__asm__AddListLine, 3) = line.GetBaseAddress();
	SETARG(__asm__AddListLine, 8) = list;
	SETARG(__asm__AddListLine, 23) = PVZ::Memory::Variable;
	return PVZ::Memory::Execute(STRING(__asm__AddListLine));
}

void PVZ::Sexy::SetListLineHeight(PList list, int height)
{
	PVZ::Memory::WriteMemory<int>(list + 0xEC, height);
}

void PVZ::Sexy::SetListJustify(PList list, int justify)
{
	PVZ::Memory::WriteMemory<int>(list + 0x98, justify);
}

void PVZ::Sexy::SetListSelected(PList list, int id)
{
	PVZ::Memory::WriteMemory<int>(list + 0xD4, id);
}

int PVZ::Sexy::GetListSelected(PList list)
{
	return PVZ::Memory::ReadMemory<int>(list + 0xD4);
}

BYTE __asm__FreeWidget[]
{
	PUSH(1),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0),
	CALL_EUX(2),
	RET
};

void PVZ::Sexy::FreeWidget(PWidget widget)
{
	SETARG(__asm__FreeWidget, 3) = widget;
	PVZ::Memory::Execute(STRING(__asm__FreeWidget));
}

BYTE __asm__ResizeButton[]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0xA0),
	CALL_EUX(2),
	RET
};

void PVZ::Sexy::ResizeWidget(PWidget widget, int x, int y, int width, int height)
{
	SETARG(__asm__ResizeButton, 1) = height;
	SETARG(__asm__ResizeButton, 6) = width;
	SETARG(__asm__ResizeButton, 11) = y;
	SETARG(__asm__ResizeButton, 16) = x;
	SETARG(__asm__ResizeButton, 21) = widget;
	PVZ::Memory::Execute(STRING(__asm__ResizeButton));
}

BYTE __asm__AddToWidget[]
{
	PUSHDWORD(0),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0x0C),
	CALL_EUX(2),
	RET
};

void PVZ::Sexy::AddToWidget(PWidget widget, PWidget father)
{
	SETARG(__asm__AddToWidget, 1) = widget;
	SETARG(__asm__AddToWidget, 6) = father;
	PVZ::Memory::Execute(STRING(__asm__AddToWidget));
}

BYTE __asm__RemoveFromWidget[]
{
	PUSHDWORD(0),
	MOV_ECX(0),
	0x8B, 0x11, // mov edx,[ecx]
	MOV_EUX_PTR_EVX_ADD(2, 2, 0x10),
	CALL_EUX(2),
	RET
};

void PVZ::Sexy::RemoveFromWidget(PWidget widget, PWidget father)
{
	SETARG(__asm__RemoveFromWidget, 1) = widget;
	SETARG(__asm__RemoveFromWidget, 6) = father;
	PVZ::Memory::Execute(STRING(__asm__RemoveFromWidget));
}
