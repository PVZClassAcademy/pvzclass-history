#pragma once
#include "DLLEvent.h"

namespace PVZEvent
{
	/// @brief 在一切更新开始前的事件
	/// @param Board
	class BoardUpdateStartEvent : public DLLEventTemplate<0x415D40, 7, REG_ECX>
	{
	public:
		BoardUpdateStartEvent(const char* str) : DLLEventTemplate() { Init(str); };
		BoardUpdateStartEvent(int address) : DLLEventTemplate() { Init(address); };
		BoardUpdateStartEvent() : DLLEventTemplate() { Init("onBoardUpdateStart"); };
	};

	/// @brief 因为游戏暂停而中止更新的事件
	/// @param Board
	/// @note 暂停并不会中止所有更新，比如暴风雨夜关暂停会更新到屏幕完全暗下来为止。
	class BoardPauseUpdateEvent : public DLLEventTemplate<0x415E14, 5, REG_EBP>
	{
	public:
		BoardPauseUpdateEvent(const char* str) : DLLEventTemplate() { Init(str); };
		BoardPauseUpdateEvent(int address) : DLLEventTemplate() { Init(address); };
		BoardPauseUpdateEvent() : DLLEventTemplate() { Init("pauseBoardUpdate"); };
	};

	/// @brief 游戏即将检查时停倒计时的事件。可借助此事件实现时停倒计时调节。
	/// @param Board
	class BoardCheckTimeStopEvent : public DLLEventTemplate<0x415F6B, 6, REG_EBP>
	{
	public:
		BoardCheckTimeStopEvent(const char* str) : DLLEventTemplate() { Init(str); };
		BoardCheckTimeStopEvent(int address) : DLLEventTemplate() { Init(address); };
		BoardCheckTimeStopEvent() : DLLEventTemplate() { Init("BoardCheckTimeStop"); };
	};

	/// @brief 游戏正常更新结束的事件
	/// @param Board
	class BoardAfterUpdateEvent : public DLLEventTemplate<0x41606E, 8, REG_EBP>
	{
	public:
		BoardAfterUpdateEvent(const char* str) : DLLEventTemplate() { Init(str); };
		BoardAfterUpdateEvent(int address) : DLLEventTemplate() { Init(address); };
		BoardAfterUpdateEvent() : DLLEventTemplate() { Init("afterBoardUpdate"); };
	};
}
