#pragma once
#include "../../PVZ.h"

namespace PVZEnum
{
	enum ReanimFlags
	{
		REANIM_NO_ATLAS = 1,
		REANIM_FAST_DRAW_IN_SW_MODE = 2
	};
}

namespace PVZ
{
	/// @brief 动画参数类，但是每个成员变量都用内存空间存储。
	class ReanimationParams
	{
	public:
		AnimationType::AnimationType Type;
		const char* FileName;
		PVZEnum::ReanimFlags Flags;

		/// @brief 重新定位默认动画参数数组的基址，并将旧基址的所有旧内容复制到新基址上。
		/// @param baseaddress 主程序中的新基址
		/// @param num 动画定义总数。必须大于等于 143。
		static void Reposition(DWORD baseaddress, DWORD num = 143);
	};

}