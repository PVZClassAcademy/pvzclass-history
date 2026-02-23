#pragma once
#include "AsmBuilder.hpp"
/// @file Memory.hpp
/// @brief 包含读写 PVZ 本体内存，以及其他内存操作的若干函数和宏定义。

/// @brief 默认的一页内存的字节数
#define PAGE_SIZE 1024

namespace PVZ
{
	/// @brief 包含各种控制 PVZ 程序的函数和变量的类。所有成员均为静态。
	/// @note 在调用过 InitPVZ() 或 InitPVZNoLock() 之后才能使用此类的各种功能。
	class Memory
	{
	public:
		/*	 */
		/// @brief Init() 系函数申请的、PVZ 本体的内存的基址，供部分函数使用。
		/// @detail 000-100存放创建子弹的函数\n
		///		100 - 200存放字符串或者PlantEffect的伪造植物对象\n
		///		200 - 224存放 AsmInit() 为传送门创建设定的辅助代码\n
		///		225 - 274存放随机数调用\n
		///		300 - 400存放__autocollect_set\n
		///		500 - 600存放Execute的同步代码\n
		/// @attention 存放字符串的功能将在 3.0 完工时被废弃。
		static int Variable;
		/// @brief 临时字符数组，用于存放字符串参数
		static uint32_t StringVariable;
		/// @brief PVZ 进程句柄
		static HANDLE hProcess;
		/// @brief PVZ 进程 ID
		static DWORD processId;
		/// @brief 主线程句柄
		static HANDLE hThread;
		/// @brief 主线程 ID
		static DWORD mainThreadId;
		/// @brief 主窗口句柄
		static HWND mainwindowhandle;
		/// @brief 如果为 true，则不会等待PVZ进程，立即执行远程代码
		/// @note 在 localExecute 为 true 时无效。
		static bool immediateExecute;
		/// @brief 如果为 true，则在当前进程执行代码。若在 dll 中，请将其设置为 true
		static bool localExecute;
		/// @brief 注入的 dll 所在的内存地址
		static int DLLAddress;
		/// @brief 读取 PVZ 程序指定地址上的数据
		/// @tparam T 数据类型
		/// @param address 数据的内存地址
		/// @return 读取的数值
		template <class T>
		inline static T ReadMemory(DWORD address)
		{
			if (localExecute)
			{
				T* buffer = (T*)address;
				return *buffer;
			}
			else
			{
				T buffer = (T)NULL;
				ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
				return buffer;
			}
		};
		/// @brief 将数据写入 PVZ 程序指定地址。在 localExecute 为 true 时，忽略权限设置。
		/// @tparam T 数据类型
		/// @param address 写入数据的内存地址
		/// @param value 写入的数值
		/// @return 写入是否成功
		template <class T>
		inline static BOOL WriteMemoryUnsafe(DWORD address, T value)
		{
			if (localExecute)
			{
				T* buffer = (T*)address;
				*buffer = value;
				return true;
			}
			else
			{
				return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
			}
		};
		/// @brief 将数据写入 PVZ 程序指定地址
		/// @tparam T 数据类型
		/// @param address 写入数据的内存地址
		/// @param value 写入的数值
		/// @return 写入是否成功
		template <class T>
		inline static BOOL WriteMemory(DWORD address, T value)
		{
			if (localExecute)
			{
				AllAccess(address);
				T* buffer = (T*)address;
				*buffer = value;
				return true;
			}
			else
			{
				return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);
			}
		};
		/// @brief 读取 PVZ 程序指定地址上的一连串数据
		/// @tparam T 数据类型
		/// @param address 数据的内存地址
		/// @param result 存放结果的指针
		/// @param length 数组的长度（按字节计）
		/// @return 读取是否成功
		template <class T>
		inline static BOOL ReadArray(DWORD address, T* result, size_t length)
		{
			if (localExecute)
			{
				memcpy(result, (const void*)address, length);
				return true;
			}
			else
			{
				return ReadProcessMemory(hProcess, (LPCVOID)address, (LPVOID)result, length, NULL);
			}
		};
		/// @brief 将一连串数据写入 PVZ 程序指定地址
		/// @tparam T 数据类型
		/// @param address 数据的内存地址
		/// @param result 待写入数据的指针
		/// @param length 数组的长度（按字节计）
		/// @return 读取是否成功
		template <class T>
		inline static BOOL WriteArray(DWORD address, T* value, size_t length)
		{
			if (localExecute)
			{
				AllAccess(address);
				memcpy((void*)address, value, length);
				return true;
			}
			else
			{
				return WriteProcessMemory(hProcess, (LPVOID)address, value, length, NULL);
			}
		};
		static int ReadPointer(int baseaddress, int offset);
		static int ReadPointer(int baseaddress, int offset, int offset1);
		static int ReadPointer(int baseaddress, int offset, int offset1, int offset2);
		/// @brief 调整程序从指定位置开始，PAGE_SIZE 个字节的内存访问权限为可读写执行。
		/// @param address 被调整访问权限的首地址
		/// @return 操作是否成功
		static BOOL AllAccess(int address);
		/// @brief 申请一定大小的内存空间，访问权限为可读写执行。
		/// @param pages 内存页数，默认为 1。
		/// @param size 小于一页部分的内存字节数，默认为 0。
		/// @return 申请的内存的首地址
		static int AllocMemory(int pages = 1, int size = 0);
		/// @brief 在 PVZ 主程序创建一个线程。\n
		/// 在 dll 中调用此函数会改为直接执行指定地址的函数。
		/// @param address 线程的起始地址
		static void CreateThread(int address);
		/// @brief 释放指定位置的内存空间
		/// @param address 被释放的内存的首地址
		static void FreeMemory(int address);
		/// @brief 在 PVZ 主程序中执行指定指令序列。
		/// @param asmcode 被执行的指令序列
		/// @param length 指令序列长度，按字节数计
		/// @return 指令执行完毕时，Variable 地址上的数值
		static int Execute(byte asmcode[], int length);
		/// @brief 在 PVZ 主程序中执行指定指令序列。
		/// @tparam _SZ AsmBuilder 序列长度上限
		/// @param builder 构建指令序列的 AsmBuilder
		/// @return 指令执行完毕时，Variable 地址上的数值
		template<typename _Derived, size_t _SZ>
		static int Execute(BaseBuilder<_Derived, _SZ>& builder)
		{
			if (localExecute)
			{
				byte* code = builder.get_code();
				DWORD length = builder.get_length();
				code[0] = PUSHAD;
				code[length - 1] = POPAD;
				code[length] = RET;
				void (*func)() = (void (*)())code;
				func();
				return ReadMemory<int>(Variable);
			}
			else
			{
				return PVZ::Memory::Execute(builder.get_code() + 1, builder.get_length() - 1);
			}
		}
		/// @brief 注入指定的 lib 或 dll 文件。
		/// @param dllname 注入的链接库的路径
		/// @return 是否注入成功
		static bool InjectDll(const char* dllname);
		/// @brief 获取已注入 dll 指定名称的函数的内存地址
		/// @param procname 函数名称
		/// @return 函数的内存地址
		static int GetProcAddress(const char* procname);
		/// @brief 执行注入的 dll 中，指定名称的函数
		/// @param procname 函数名称
		/// @return 指令执行完毕时，Variable 地址上的数值
		static int InvokeDllProc(const char* procname);
		/// @brief 等待PVZ到达更新前
		/// @note 不适用通过 InitPVZNoLock() 初始化的情况。
		static void WaitPVZ();
		/// @brief 恢复PVZ
		/// @note 不适用通过 InitPVZNoLock() 初始化的情况。
		static void ResumePVZ();
	};

}

#define PVZ_BASE PVZ::Memory::ReadMemory<int>(0x6A9EC0)
#define PVZBASEADDRESS PVZ::Memory::ReadMemory<int>(PVZ_BASE + 0x768)

/// @brief 用来替代成员变量声明的类型部分，将其转化为属性。\n
///		转化后对该变量的读写操作分别会转化为调用 getmethod 和 setmethod。
/// @note getmethod 和 setmethod 仅有声明，需要自己另行定义。
/// @param type 变量类型
/// @param getmethod 读成员变量的方法名
/// @param setmethod 写成员变量的方法名
#define PROPERTY(type,getmethod,setmethod) type getmethod();void setmethod(type value);__declspec(property(get=getmethod,put=setmethod)) type
/// @brief PROPERTY 宏的只读版本，不允许写操作。
/// @see PROPERTY
#define READONLY_PROPERTY(type,getmethod) type getmethod();__declspec(property(get=getmethod)) type
/// @brief PROPERTY 宏的只写版本，不允许读操作。
/// @see PROPERTY
#define WRITEONLY_PROPERTY(type,setmethod) void setmethod(type value);__declspec(property(put=setmethod)) type

/// @brief 用来替代成员变量声明的类型部分，将其转化为属性。\n
///		转化后对该变量的读写操作分别会转化为调用 getmethod 和 setmethod。
/// @note 在 PROPERTY 的基础上，该语句直接定义读写方法。
/// @param type 变量类型
/// @param getmethod 读成员变量的方法名
/// @param getter 读方法的语句
/// @param setmethod 写成员变量的方法名
/// @param getter 读方法的语句
#define PROPERTY_BINDING(type,getmethod,getter,setmethod,setter) inline type getmethod(){return getter;}; \
	inline void setmethod(type value){setter;}; \
	__declspec(property(get=getmethod,put=setmethod)) type
/// @brief PROPERTY_BINDING 宏的只读版本，不允许写操作。
/// @see PROPERTY_BINDING
#define READONLY_PROPERTY_BINDING(type,getmethod,getter) inline type getmethod(){return getter;};\
	__declspec(property(get=getmethod)) type
/// @brief PROPERTY_BINDING 宏的只写版本，不允许读操作。
/// @see PROPERTY_BINDING
#define WRITEONLY_PROPERTY_BINDING(type,setmethod,setter) inline void setmethod(type value){setter;};\
	__declspec(property(put=setmethod)) type

/// @brief 用来替代成员变量声明，声明一个 32 位有符号整数属性。\n
///		对该属性的读写操作分别会转化为读写指定地址的数值。\n
///		地址为 BaseAddress 的值与偏移的和。
/// @note 该宏包含完整的变量声明和读写方法定义。
/// @see PROPERTY_BINDING
/// @param propname 属性名称
/// @param getmethod 读方法的名称
/// @param setmethod 写方法的名称
/// @param offset 相对基址的偏移
#define INT_PROPERTY(propname,getmethod,setmethod,offset) PROPERTY_BINDING(int,getmethod,Memory::ReadMemory<int>(BaseAddress+offset),setmethod,Memory::WriteMemoryUnsafe<int>(BaseAddress+offset,value)) propname
/// @brief INT_PROPERTY 宏的只读版本，不允许写操作。
/// @see INT_PROPERTY
#define INT_READONLY_PROPERTY(propname,getmethod,offset) READONLY_PROPERTY_BINDING(int,getmethod,Memory::ReadMemory<int>(BaseAddress+offset)) propname
/// @brief 与 INT_PROPERTY 类似，不过它可以表示任何类型的属性。
/// @param type 属性类型。
/// @see INT_PROPERTY
#define T_PROPERTY(type,propname,getmethod,setmethod,offset) PROPERTY_BINDING(type,getmethod,Memory::ReadMemory<type>(BaseAddress+offset),setmethod,Memory::WriteMemoryUnsafe<type>(BaseAddress+offset,value)) propname
/// @brief T_PROPERTY 宏的只读版本，不允许写操作。
/// @see T_PROPERTY
#define T_READONLY_PROPERTY(type,propname,getmethod,offset) READONLY_PROPERTY_BINDING(type,getmethod,Memory::ReadMemory<type>(BaseAddress+offset)) propname

/// @brief 与 INT_PROPERTY 类似，只是写操作使用 WriteMemory() ，而不是 WriteMemoryUnsafe() 。
/// @see INT_PROPERTY
#define INT_SAFE_PROPERTY(propname,getmethod,setmethod,offset) PROPERTY_BINDING(int,getmethod,Memory::ReadMemory<int>(BaseAddress+offset),setmethod,Memory::WriteMemory<int>(BaseAddress+offset,value)) propname
/// @brief 与 T_PROPERTY 类似，只是写操作使用 WriteMemory() ，而不是 WriteMemoryUnsafe() 。
/// @see T_PROPERTY
#define T_SAFE_PROPERTY(type,propname,getmethod,setmethod,offset) PROPERTY_BINDING(type,getmethod,Memory::ReadMemory<type>(BaseAddress+offset),setmethod,Memory::WriteMemory<type>(BaseAddress+offset,value)) propname

/// @brief 用来简化声明读写 32 位有符号整数数组元素函数的宏。\n
/// @note 该宏实际上并不创造真的数组，也没有提供类似 T_PROPERTY 的定义，只是把读写函数绑在一起而已。
/// @param getmethod 读方法的名称
/// @param setmethod 写方法的名称
/// @param offset 数组首个元素的首地址相对基址的偏移
#define INT_ARRAY_PROPERTY(getmethod,setmethod,offset) inline int getmethod(int index) \
	{ return Memory::ReadMemory<int>(BaseAddress+offset+index*4); } \
	inline void setmethod(int index, int value) \
	{ Memory::WriteMemoryUnsafe<int>(BaseAddress+offset+index*4, value); }

/// @brief 类似 INT_ARRAY_PROPERTY，只是此宏支持其他类型。
/// @see INT_ARRAY_PROPERTY
/// @param type 返回值的类型
/// @param size 类型的大小，按字节数计算
#define T_ARRAY_PROPERTY(type,getmethod,setmethod,offset,size) inline type getmethod(int index) \
	{ return Memory::ReadMemory<type>(BaseAddress+offset+index*size); } \
	inline void setmethod(int index, type value) \
	{ Memory::WriteMemoryUnsafe<type>(BaseAddress+offset+index*size, value); }

/// @brief 与T_PROPERTY类似，但自动生成get和set方法名
/// @see T_PROPERTY
/// @param type 类型
/// @param propname 属性名称
/// @param offset 偏移
#define T_SIMPLE_PROPERTY(type, propname, offset) T_PROPERTY(type, propname, __get_##propname, __set##propname, offset)
/// @brief 与T_SIMPLE_PROPERTY类似, 但属性类型为int
#define INT_SIMPLE_PROPERTY(propname, offset) T_PROPERTY(int, propname, __get_##propname, __set##propname, offset)
/// @brief 与T_SIMPLE_PROPERTY类似, 但属性类型为float
#define FLOAT_SIMPLE_PROPERTY(propname, offset) T_PROPERTY(float, propname, __get_##propname, __set##propname, offset)
