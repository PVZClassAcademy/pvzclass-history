#pragma once
#include "../../PVZ.h"
#include "Draw.h"

namespace PVZ
{
	class BaseClass;
	class PVZApp;
	/// @brief 资源管理器，用于加载资源包、解析和加载资源文件。
	class ResourceManager : public BaseClass
	{
	public:
		ResourceManager(int address) : BaseClass(address) {};
		/// @brief 是否允许加载已加载过的资源。若为 false，加载重复的资源时会报错。
		T_PROPERTY(BOOLEAN, AllowAlreadyDefinedResources, __get_AllowAlreadyDefinedResources, __set_AllowAlreadyDefinedResources, 0x0B1);
		/// @brief 获取资源管理器所在的 app
		/// @return 资源管理器所属的 app
		PVZApp GetPVZApp();
		/// @brief 加载指定资源包文件。
		/// @param fileName 文件名
		void AddPAKFile(const char* fileName);
		/// @brief 尝试解析指定的资源描述文件。
		/// @param fileName 文件名
		/// @return 是否解析成功。
		bool ParseResourcesFile(const char* fileName);
		/// @brief 加载指定的资源组。
		/// @param groupName 资源组名称。
		/// @return 是否加载成功。
		bool TodLoadResources(const char* groupName);
		/// @brief 获取指定音效的编号。
		/// @param soundName 音效在资源描述文件中的名称。
		/// @return 音效编号。
		SoundID GetSoundThrow(const char* soundName);
		/// @brief 获取指定的图片。
		/// @param imageName 图片在资源描述文件中的名称。
		/// @return 对应的图片。
		Image GetImage(const char* imageName);
		/// @brief 获取指定的字体。
		/// @param fontName 字体在资源描述文件中的名称。
		/// @return 对应的字体。
		Font GetFont(const char* fontName);
	};
	/// @brief 获取资源管理器对象
	/// @return 资源管理器对象
	ResourceManager GetResourceManager();
}