#include "../../PVZ.h"

namespace PVZ
{
	class ButtonWidget : public Widget
	{
	public:
		ButtonWidget(int address) : PVZ::Widget(address) {};
		ButtonWidget(PVZ::Widget& widget) : PVZ::Widget(widget.GetBaseAddress()) {};

		T_PROPERTY(BOOL, Disabled, __get_Disabled, __set_Disabled, 0x56);
		T_PROPERTY(BOOL, Inverted, __get_Inverted, __set_Inverted, 0x100);
	};
}