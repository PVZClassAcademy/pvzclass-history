#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class PVZApp;
	class Widget;
	/// @brief 选关界面
	class ChallengeScreen : public Widget
	{
	public:
		ChallengeScreen(int address) : Widget(address) {};
		/// @brief 获取选关界面所在的 app
		/// @return 选关界面所属的 app
		PVZApp GetPVZApp();
		/// @brief 当前页数
		PROPERTY(ChallengePage::ChallengePage, __get_Page, __set_Page) Page;
	};
	/// @brief 获取选关界面控件
	/// @return 选关界面对象。
	ChallengeScreen GetChallengeScreen();
}