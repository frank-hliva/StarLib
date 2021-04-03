#pragma once

#include <map>
#include <string>
#include <optional>
#include <set>
#include "Star.h"

namespace Star::IO
{
	using DWORD = unsigned long;

	class FileSystemAttributes : public Object
	{
	public:
		const enum Enum
		{
			ReadOnly = 0x1UL,
			Hidden = 0x2UL,
			System = 0x4UL,
			Directory = 0x10UL,
			Archive = 0x20UL,
			Device = 0x40UL,
			Normal = 0x80UL,
			Temporary = 0x100UL,
			SparseFile = 0x200UL,
			ReparsePoint = 0x400UL,
			Compressed = 0x800UL,
			Offline = 0x1000UL,
			NotContentIndexed = 0x2000UL,
			Encrypted = 0x4000UL,
			IntegrityStream = 0x8000UL,
			NoScrubData = 0x20000UL,
			NotExists = ULONG_MAX
		};
		using Metadata = std::map<Enum, WString>;
		using FileAttributeSet = std::set<FileSystemAttributes::Enum>;
		Metadata fileAttributeMetas;
		FileSystemAttributes();
		bool Exists(const Enum attribute) const;
		std::optional<WString> ToString(Enum attribute) const;
		DWORD GetAllAttributesAsDWORD(const WString& path) const;
		FileAttributeSet GetAllAttributes(const WString& path) const;
		void SetAllAttributes(const WString& path, const DWORD fileAttributes);
		void SetAllAttributes(const WString& path, const FileAttributeSet& fileAttributeSet);
	};
}