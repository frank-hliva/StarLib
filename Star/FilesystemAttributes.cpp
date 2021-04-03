#include "FileSystemAttributes.h"
#include <windows.h>
#include "Star.h"

namespace Star::IO
{
	FileSystemAttributes::FileSystemAttributes()
	{
		fileAttributeMetas = Metadata{
			{ ReadOnly, L"ReadOnly" },
			{ Hidden, L"Hidden" },
			{ System, L"System" },
			{ Directory, L"Directory" },
			{ Archive, L"Archive" },
			{ Device, L"Device" },
			{ Normal, L"Normal" },
			{ Temporary, L"Temporary" },
			{ SparseFile, L"SparseFile" },
			{ ReparsePoint, L"ReparsePoint" },
			{ Compressed, L"Compressed" },
			{ Offline, L"Offline" },
			{ NotContentIndexed, L"NotContentIndexed" },
			{ Encrypted, L"Encrypted" },
			{ IntegrityStream, L"IntegrityStream" },
			{ NoScrubData, L"NoScrubData" },
			{ NotExists, L"NotExists" }
		};
	}

	bool FileSystemAttributes::Exists(Enum attribute) const
	{
		return fileAttributeMetas.find(attribute) != fileAttributeMetas.end();
	}

	std::optional<WString> FileSystemAttributes::ToString(Enum attribute) const
	{
		auto item = fileAttributeMetas.find(attribute);
		return (
			item == fileAttributeMetas.end()
			? std::nullopt
			: std::optional<WString>{ item->second }
		);
	}

	DWORD FileSystemAttributes::GetAllAttributesAsDWORD(const WString& path) const
	{
		return GetFileAttributesW(path.c_str());
	}

	FileSystemAttributes::FileAttributeSet FileSystemAttributes::GetAllAttributes(const WString& path) const
	{
		const auto attributes = GetAllAttributesAsDWORD(path);
		
		if (attributes == FileSystemAttributes::NotExists)
		{
			return FileAttributeSet{ FileSystemAttributes::NotExists };
		}
		else
		{
			auto attributeSet = FileAttributeSet();
			for (auto& fileAttributeMeta : fileAttributeMetas)
			{
				const auto attribute = fileAttributeMeta.first;
				if ((attributes & attribute) == attribute)
				{
					attributeSet.insert(
						static_cast<FileSystemAttributes::Enum>(attribute)
					);
				}
			}
			return attributeSet;
		}
	}

	void FileSystemAttributes::SetAllAttributes(const WString& path, const DWORD fileAttributes)
	{
		SetFileAttributesW(path.c_str(), fileAttributes);
	}

	void FileSystemAttributes::SetAllAttributes(const WString& path, const FileAttributeSet& fileAttributeSet)
	{
		auto attributes = 0UL;
		for (auto fileAttribute : fileAttributeSet)
		{
			attributes |= (DWORD)fileAttribute;
		}
		SetFileAttributesW(path.c_str(), attributes);
	}
}