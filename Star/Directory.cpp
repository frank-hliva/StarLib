#include "Directory.h"
#ifdef _WIN32
#include <Shlwapi.h>
#endif

namespace Star::IO
{
	bool Directory::Exists(const WString& path)
	{
#ifdef _WIN32
		DWORD attributes = GetFileAttributesW(path.c_str());
		return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
		struct _stat info;
		int stat = _wstat(path.c_str(), &info); 
		return stat == 0 && info.st_mode & S_IFDIR;
#endif
	}

	fs::directory_iterator Directory::FileSystemEntries(const WString& path)
	{
		return fs::directory_iterator(path);
	}

	Star::WString Directory::Parent(const WString& path)
	{
		return fs::path(path).parent_path();
	}

	Star::WString Directory::Root(const WString& path)
	{
		return fs::path(path).root_path();
	}

	uintmax_t Directory::Delete(const WString& path)
	{
		return fs::remove_all(path);
	}

	void Directory::Move(const WString& sourcePath, const WString& destinationPath)
	{
		;
		return fs::rename(sourcePath, destinationPath);
	}

	void Directory::SetCurrent(const WString& path)
	{
		return fs::current_path(path);
	}

	Star::WString Directory::Current()
	{
		return fs::current_path();
	}

	bool Directory::Generate(const WString& path)
	{
		return fs::create_directories(path);
	}

}