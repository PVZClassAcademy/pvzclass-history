#include "../../PVZ.h"

namespace PVZ
{
	class Dialog : public PVZ::Widget
	{
	public:
		Dialog(int address) : PVZ::Widget(address) {};
		Dialog(PVZ::Widget& widget) : PVZ::Widget(widget.GetBaseAddress()) {};

		INT_PROPERTY(ID, __get_ID, __set_ID, 0x13C);
		void CalcSize(int ExtraX, int ExtraY);
	};

	class LawnDialog : public Dialog
	{
	public:
		LawnDialog(int address) : PVZ::Dialog(address) {};
		LawnDialog(PVZ::Dialog& dialog) : PVZ::Dialog(dialog.GetBaseAddress()) {};

	};	
}