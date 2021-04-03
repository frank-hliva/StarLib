#include "Path.h"
#ifdef _WIN32
#include <Shlwapi.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <filesystem>
#include "Error.h"
#include "StringSequence.h"

namespace Star::IO
{
	namespace fs = std::filesystem;
	using namespace Star::Errors;

	#ifdef _WIN32
	WString Path::DirectoryName(const WString& path)
	{
		auto pathCString = (wchar_t*)path.c_str();
		PathRemoveFileSpecW(pathCString);
		return WString(pathCString);
	}

	WString Path::Combine(const WString& path1, const WString& path2)
	{
		wchar_t result[MAX_PATH] = L"";
		PathCombineW(result, path1.c_str(), path2.c_str());
		return WString(result);
	}

	WString Path::Join(const std::vector<WString> paths)
	{
		WString result = L"";
		for (auto& path : paths)
		{
			if (result == L"")
			{
				result = path;
			}
			else
			{
				result = Combine(result, path);
			}
		}
		return result;
	}
	#endif

	bool Path::Exists(const WString& path)
	{
		#ifdef _WIN32
		return GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES;
		#else
		struct _stat info;
		return _wstat(path.c_str(), &info) == 0;
		#endif
	}

	WString Path::Extension(const WString& path)
	{
		return fs::path(path).extension();
	}

	bool Path::HasExtension(const WString& path)
	{
		return fs::path(path).has_extension();
	}

	WString Path::FileName(const WString& path)
	{
		return fs::path(path).filename();
	}

	WString Path::FileNameWithoutExtension(const WString& path)
	{
		return fs::path(path).stem();
	}

	bool Path::HasFileName(const WString& path)
	{
		return fs::path(path).has_filename();
	}

	WString Path::Parent(const WString& path)
	{
		return fs::path(path).parent_path();
	}

	WString Path::ChangeExtension(const WString& path, const WString& newExtension)
	{
		return fs::path(path).replace_extension(newExtension);
	}

	WString Path::ChangeFileName(const WString& path, const WString& newFileName)
	{
		return fs::path(path).replace_filename(newFileName);
	}

	WString Path::RootPath(const WString& path)
	{
		return fs::path(path).root_path();
	}

	WString Path::RootDirectory(const WString& path)
	{
		return fs::path(path).root_directory();
	}

	WString Path::RootName(const WString& path)
	{
		return fs::path(path).root_name();
	}

	WString Path::RandomFileName(const unsigned int size)
	{
		return StringSequence::RandomHex(size) + L"." + StringSequence::RandomHex(3);
	}

	WString Path::RandomFileName(const WString& extension, const unsigned int size)
	{
		return ChangeExtension(StringSequence::RandomHex(size), extension);
	}

	WString Path::TrimEndingDirectorySeparator(const WString& path)
	{
		return (
			path.ends_with(L"/") || path.ends_with(L"\\")
				? path.substr(0, path.size() - 1)
				: path
		);
	}

	WString Path::RemoveFileName(const WString& path)
	{
		return fs::path(path).remove_filename();
	}
}