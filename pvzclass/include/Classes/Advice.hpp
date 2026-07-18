#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class Caption : public BaseClass
	{
	public:
		Caption(int address);
		int GetBaseAddress();
		void GetText(char str[]);//str[0x80]
		void SetText(const char str[]);//str[0x80]
		INT_PROPERTY(DisappearCountdown, __get_DisappearCountdown, __set_DisappearCountdown, 0x88);
		T_PROPERTY(CaptionStyle::CaptionStyle, Style, __get_CaptionStyle, __set_CaptionStyle, 0x8C);
	};
	using Advice = Caption;
}