#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Star.h"

namespace Star::IO
{
	class Path
	{
	private:
		static WString Combine(const WString& path1, const WString& path2);
	public:
		#ifdef _WIN32
		static WString Join(const std::vector<WString> paths);
		template <typename ... Ts>
		static WString Join(WString const& path, Ts const& ... paths)
		{
			return Join(std::vector<WString>{ { path, paths... } });
		}
		static WString DirectoryName(const WString& path);
		#endif
		static bool Exists(const WString& path);
		static WString Extension(const WString& path);
		static bool HasExtension(const WString& path);
		static WString FileName(const WString& path);
		static WString FileNameWithoutExtension(const WString& path);
		static bool HasFileName(const WString& path);
		static WString Parent(const WString& path);
		static WString ChangeFileName(const WString& path, const WString& newFileName);
		static WString ChangeExtension(const WString& path, const WString& newExtension);
		static WString RootPath(const WString& path);
		static WString RootDirectory(const WString& path);
		static WString RootName(const WString& path);
		static WString RandomFileName(const unsigned int size = 8);
		static WString RandomFileName(const WString& extension, const unsigned int size = 8);
		static WString TrimEndingDirectorySeparator(const WString& path);
		static WString RemoveFileName(const WString& path);
	};
}