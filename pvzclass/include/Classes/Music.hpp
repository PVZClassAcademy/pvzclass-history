#pragma once
#include "../../PVZ.h"

namespace PVZ
{
	class PVZApp;

	/// @brief 控制音乐相关内容的类
	/// @todo 完善 doxygen 注释
	class Music : public BaseClass
	{
	public:
		Music(int address) : BaseClass(address) {};
		PROPERTY(MusicType::MusicType, __get_Type, __set_Type) Type;
		//MINGAM_ENABLE or MINGAM_DISABLE
		INT_PROPERTY(INGAMEable, __get_INGAMEable, __set_INGAMEable, 0x10);
		//MINGAME_STARTED or MINGAME_NOTSTARTED
		INT_PROPERTY(INGAMEStart, __get_INGAMEStart, __set_INGAMEStart, 0x18);
		INT_READONLY_PROPERTY(Tempo, __get_Tempo, 0x1C);
		INT_READONLY_PROPERTY(Ticks_Row, __get_Ticks_Row, 0x20);
		T_PROPERTY(INGAMEState::INGAMEState, State, __get_State, __set_State, 0x24);
		INT_PROPERTY(AttributeCountdown, __get_AttributeCountdown, __set_AttributeCountdown, 0x28);
		T_PROPERTY(INGAMEEffect::INGAMEEffect, INGAMEEffect, __get_INGAMEEffect, __set_INGAMEEffect, 0x2C);
		/// @brief 是否暂停
		T_PROPERTY(byte, Paused, __get_Paused, __set_Paused, 0x40);
		/// @brief 是否被禁用
		T_PROPERTY(byte, Disabled, __get_Disabled, __set_Disabled, 0x41);
		/// @brief 淡出倒计时
		INT_PROPERTY(FadeOutCounter, __get_FadeOutCounter, __set_FadeOutCounter, 0x44);
		/// @brief 淡出持续时间
		INT_PROPERTY(FadeOutDuration, __get_FadeOutDuration, __set_FadeOutDuration, 0x48);

		/// @brief 停止所有背景音乐
		void StopAllMusic();
	};

	Music GetMusic();
}
