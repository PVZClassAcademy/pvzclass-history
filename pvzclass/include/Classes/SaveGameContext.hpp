#pragma once
#include "../../PVZ.h"
#include "../../Classes/Board.hpp"

namespace PVZ
{
	/// @brief 游戏存档上下文，用于缓存存读档的信息
	class SaveGameContext : public BaseClass
	{
	public:
		SaveGameContext(uint32_t address) : PVZ::BaseClass(address) {};

		/// @brief 是否出现存读档同步失败
		T_SIMPLE_PROPERTY(bool, Failed, 0x20);
		/// @brief 是否处于读模式。若为 false，则处于写模式
		T_SIMPLE_PROPERTY(bool, Reading, 0x21);

		/// @brief 构建空白上下文
		/// @author Moon404
		static SaveGameContext Make();
		/// @brief 释放该上下文在 PVZ 占用的内存空间
		/// @attention 只有通过 Make 创建的上下文才应该使用该函数销毁。
		/// @author Moon404
		void Free();
		/// @brief 读取SaveGameContext
		/// @param buf 如果 localExecute 为 true，则直接赋值为内存地址，否则会拷贝到 buf
		/// @author Moon404
		int Read(char* buf);
		/// @brief 写入SaveGameContext
		/// @param buf 如果 localExecute 为 true，则直接将地址作为参数传入，否则会产生拷贝
		/// @author Moon404
		void Write(char* buf, int buflen);

		/// @brief 读模式下会从缓冲区中读取指定长度的内容，写入到基址。写模式下会将字节数和基址起的一段字节写入缓冲区中。
		/// @note 若读取失败，则会读入默认值 0 。
		/// @param baseaddress 基址
		/// @param size 读写的字节数
		void SyncBytes(uint32_t baseaddress, int size);
		/// @brief 将游戏内容与 SaveGameContext 同步，可以理解为存入内存的存/读档。
		/// @param context 需要使用 MakeSaveGameContext 构建
		/// @param read true 读取内存数据，false 将数据写入内存
		/// @author Moon404
		void SyncBoard(Board board, bool read);
	};
}