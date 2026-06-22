#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	/// @brief 控件管理器类
	class WidgetManager : public WidgetContainer
	{
	public:
		WidgetManager(uint32_t baseaddress) : WidgetContainer(baseaddress) {};
		T_READONLY_PROPERTY(PVZ::PVZApp, App, __get_App, 0x5C);
		T_READONLY_PROPERTY(bool, HasFocus, __get_HasFocus, 0x84);
		/// @brief 鼠标指针是否在窗口范围内
		T_READONLY_PROPERTY(bool, IsMouseInArea, __get_IsMouseInArea, 0xDC);
		INT_PROPERTY(X, __get_X, __set_X, 0xE0);
		INT_PROPERTY(Y, __get_Y, __set_Y, 0xE4);
		/// @brief 鼠标按下状态
		/// @see MouseClickState::MouseClickState
		T_READONLY_PROPERTY(MouseClickState::MouseClickState, ClickState, __get_ClickState, 0xE8);
		/// @brief 指定按键是否已按下
		/// @see KeyCode::KeyCode
		READONLY_PROPERTY_BINDING(PVZ::Array<bool>, __get_IsKeyDown, this->BaseAddress + 0x0F4) IsKeyDown;
		/// @brief 最近一次按下的鼠标按键 ID
		INT_SIMPLE_PROPERTY(LastDownButtonId, 0x1F4);

		void MouseDown(int x, int y);
	};
    PVZ::WidgetManager GetWidgetManager();
}
