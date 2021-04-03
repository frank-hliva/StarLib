#pragma once

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <system_error>
#include <filesystem>
#include "Star.h"
#include "FileSystemEntry.h"
#include "Error.h"

namespace Star::IO
{
	namespace fs = std::filesystem;

	class Directory
	{
	public:
		static bool Exists(const WString& path);

		
		static WString Current();
		static void SetCurrent(const WString& path);

		static bool Generate(const WString& path);
		static void Move(const WString& sourcePath, const WString& destinationPath);
		static uintmax_t Delete(const WString& path);
		static WString Root(const WString& path);
		static WString Parent(const WString& path);

		static fs::directory_iterator FileSystemEntries(const WString& path);

		static constexpr auto Contains = &Exists;
		static constexpr auto Change = &SetCurrent;
		static constexpr auto Make = &Generate;
		static constexpr auto Remove = &Delete;

		static constexpr auto GetStat = &FileSystemEntry::GetStat;
		static constexpr auto GetTimes = &FileSystemEntry::GetTimes;
		static constexpr auto SetTimes = &FileSystemEntry::SetTimes;

		static constexpr auto GetCreationTime = &FileSystemEntry::GetCreationTime;
		static constexpr auto GetLastAccessTime = &FileSystemEntry::GetLastAccessTime;
		static constexpr auto GetLastWriteTime = &FileSystemEntry::GetLastWriteTime;
		static constexpr auto UpdateTimes = &FileSystemEntry::UpdateTimes;

		static constexpr auto SetCreationTime = &FileSystemEntry::SetCreationTime;
		static constexpr auto SetLastAccessTime = &FileSystemEntry::SetLastAccessTime;
		static constexpr auto SetLastWriteTime = &FileSystemEntry::SetLastWriteTime;
		static constexpr auto GetAttributesAsDWORD = &FileSystemEntry::GetAttributesAsDWORD;
		static constexpr auto GetAttributes = &FileSystemEntry::GetAttributes;
		static constexpr auto SetWORDAsAttributes = &FileSystemEntry::SetWORDAsAttributes;
		static constexpr auto SetAttributes = &FileSystemEntry::SetAttributes;
	};

}