#pragma once
#include "../PVZ.h"
#include "GameObject.hpp"

namespace PVZ
{
	class PVZApp;
	class Board;
	class Zombie;
	/// @brief 除草机
	class LawnMower : public BaseClass
	{
	public:
		/// @brief 除草机的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static DWORD MemSize;
		/// @attention 从 3.0 起，该函数将不再具有按编号构造的功能。
		LawnMower(uint32_t address) : BaseClass(address) {};
		/// @brief 获取指定编号的对象
		/// @param index 编号
		/// @note 不保证以此法获得的对象未被移除
		static LawnMower GetByIndex(uint32_t index);
		/// @brief 所在的 PVZApp
		T_PROPERTY(PVZApp, App, __get_App, __set_App, 0);
		/// @brief 所在的 Board
		T_PROPERTY(Board, mBoard, __get_Board, __set_Board, 4);
		/// @brief X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 8);
		/// @brief Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0xC);
		/// @brief 绘制图层编号
		INT_PROPERTY(Layer, __get_Layer, __set_Layer, 0x10);
		/// @brief 行
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x14);
		INT_PROPERTY(AnimTicksPerFrame, __get_AnimTicksPerFrame, __set_AnimTicksPerFrame, 0x18);
		/// @brief 获取除草机的动画
		/// @return 除草机的动画
		PVZ::Animation GetAnimation();
		/// @brief 碾压僵尸后的倒计时
		INT_PROPERTY(ChompCounter, __get_ChompCounter, __set_ChompCounter, 0x20);
		/// @brief 入场倒计时
		INT_PROPERTY(RollingInCounter, __get_RollingInCounter, __set_RollingInCounter, 0x24);
		/// @brief 被碾压倒计时
		INT_PROPERTY(SquishedCounter, __get_SquishedCounter, __set_SquishedCounter, 0x28);
		/// @brief 除草机状态
		T_PROPERTY(LawnMowerState::LawnMowerState, State, __get_State, __set_State, 0x2C);
		/// @brief 是否已消失
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x30);
		/// @brief 是否可见
		T_PROPERTY(BOOLEAN, Visible, __get_Visible, __set_Visible, 0x31);
		/// @brief 除草机类型
		T_PROPERTY(LawnMowerType::LawnMowerType, Type, __get_Type, __set_Type, 0x34);
		/// @brief Y 坐标偏移量
		T_PROPERTY(FLOAT, YOffset, __get_YOffset, __set_YOffset, 0x38);
		/// @brief 高度状态
		/// @todo 枚举化
		INT_PROPERTY(MowerHeight, __get_MowerHeight, __set_MowerHeight, 0x3C);
		/// @brief 最后一次被传送门传送时的 X 坐标
		INT_PROPERTY(LastPortalX, __get_LastPortalX, __set_LastPortalX, 0x40);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x44);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 移除该除草机
		void Die();
		/// @brief 碾压指定僵尸
		/// @param zombie 僵尸
		void MowZombie(Zombie zombie);
		/// @brief 压扁除草机
		void Squish();
		/// @brief 启动除草机
		void Start();
		/// @brief 更新该除草机
		void Update();
		/// @brief 更新除草机出入水时的高度偏移和状态
		void UpdatePool();
	};
	using Lawnmower = LawnMower;
}