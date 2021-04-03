#pragma once

#include <sys/utime.h>
#include "DateTime.h"
#include "Timestamp.h"
#include "Error.h"
#include "FileSystemAttributes.h"

namespace Star::IO
{
	namespace {
		static Star::IO::FileSystemAttributes fileSystemAttributes = Star::IO::FileSystemAttributes();
	}

	class FileSystemEntry
	{
	public:
		class Times : public Object
		{
		private:
			const timestamp creationTime;
			const timestamp lastAccessTime;
			const timestamp lastWriteTime;
		public:
			virtual timestamp CreationTime() const
			{
				return creationTime;
			};
			virtual Times SetCreationTime(const timestamp creationTime) const
			{
				return Times(creationTime, lastAccessTime, lastWriteTime);
			};
			virtual timestamp LastAccessTime() const
			{
				return lastAccessTime;
			};
			virtual Times SetLastAccessTime(const timestamp lastAccessTime) const
			{
				return Times(creationTime, lastAccessTime, lastWriteTime);
			};
			virtual timestamp LastWriteTime() const
			{
				return lastWriteTime;
			};
			virtual Times SetLastWriteTime(const timestamp lastWriteTime) const
			{
				return Times(creationTime, lastAccessTime, lastWriteTime);
			};

			Times() :
				creationTime(Timestamp::Empty()),
				lastAccessTime(Timestamp::Empty()),
				lastWriteTime(Timestamp::Empty())
			{
			}

			Times(
				const timestamp creationTime,
				const timestamp lastAccessTime,
				const timestamp lastWriteTime
			) :
				creationTime(creationTime),
				lastAccessTime(lastAccessTime),
				lastWriteTime(lastWriteTime)
			{
			}

			Times(struct _stati64 stat) :
				creationTime(stat.st_ctime),
				lastAccessTime(stat.st_atime),
				lastWriteTime(stat.st_mtime)
			{
			}
		};

		class Stat : public Object, public Times
		{
		private:
			const _dev_t deviceID;
			const _ino_t inodeNumber;
			const unsigned short protectionMode;
			const short numberOfHardLinks;
			const short ownerUserID;
			const short ownerGroupID;
			const _dev_t rDeviceID;
			const __int64 totalSize;
		public:
			virtual _dev_t DeviceID() const
			{
				return deviceID;
			};
			virtual _ino_t InodeNumber() const
			{
				return inodeNumber;
			};
			virtual unsigned short ProtectionMode() const
			{
				return protectionMode;
			};
			virtual short NumberOfHardLinks() const
			{
				return numberOfHardLinks;
			};
			virtual short  OwnerUserID() const
			{
				return ownerUserID;
			};
			virtual short OwnerGroupID() const
			{
				return ownerGroupID;
			};
			virtual _dev_t RDeviceID() const
			{
				return rDeviceID;
			};
			virtual __int64 TotalSize() const
			{
				return totalSize;
			};
			Stat(struct _stati64 stat) :
				Times(stat),
				deviceID(stat.st_dev),
				inodeNumber(stat.st_ino),
				protectionMode(stat.st_mode),
				numberOfHardLinks(stat.st_nlink),
				ownerUserID(stat.st_uid),
				ownerGroupID(stat.st_gid),
				rDeviceID(stat.st_rdev),
				totalSize(stat.st_size)
			{
			}
		};
	private:
#if _WIN32
		static void WinSetTimes(const WString& path, const timestamp creationTime, const timestamp lastAccessTime, const timestamp lastWriteTime)
		{
			auto file = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			auto ftCreationTime = Timestamp::ToFileTime(creationTime);
			auto ftLastAccessTime = Timestamp::ToFileTime(lastAccessTime);
			auto ftLastWriteTime = Timestamp::ToFileTime(lastWriteTime);
			const auto result = SetFileTime(
				file,
				&ftCreationTime,
				&ftLastAccessTime,
				&ftLastWriteTime
			);
			if (result == 0)
			{
				throw Errors::Error();
			}
			CloseHandle(file);
		}
#endif
	public:
		static const Stat GetStat(const WString& path)
		{
			struct _stati64 stat;
			_wstati64(path.c_str(), &stat);
			return stat;
		}

		static const Stat GetTimes(const WString& path)
		{
			return GetStat(path);
		}

		static void SetTimes(const WString& path, const Times& times)
		{
			auto stat = GetStat(path);
			struct __utimbuf64 newTimes;
#if _WIN32
			const auto creationTime = times.CreationTime();
#else

#endif
			const auto lastAccessTime = times.LastAccessTime();
			const auto lastWriteTime = times.LastWriteTime();
#if _WIN32
			WinSetTimes(
				path,
				creationTime == Timestamp::Empty() ? stat.CreationTime() : creationTime,
				lastAccessTime == Timestamp::Empty() ? stat.LastAccessTime() : lastAccessTime,
				lastWriteTime == Timestamp::Empty() ? stat.LastWriteTime() : lastWriteTime
			);
#else
			newTimes.modtime = lastWriteTime == Timestamp::Empty() ? stat.LastWriteTime() : lastWriteTime;
			newTimes.actime = lastAccessTime == Timestamp::Empty() ? stat.LastAccessTime() : lastAccessTime;
			_wutime64(path.c_str(), &newTimes);
#endif
		}

		static timestamp GetCreationTime(const WString& path)
		{
			return GetStat(path).CreationTime();
		}

		static timestamp GetLastAccessTime(const WString& path)
		{
			return GetStat(path).LastAccessTime();
		}

		static timestamp GetLastWriteTime(const WString& path)
		{
			return GetStat(path).LastWriteTime();
		}

		static void UpdateTimes(const WString& path, const std::function<Times(Times)>& updater)
		{
			SetTimes(path, updater(Times()));
		}

		static void SetCreationTime(const WString& path, const timestamp& creationTime)
		{
			UpdateTimes(path, [&](Times times)
			{
				return times.SetCreationTime(creationTime);
			});
		}

		static void SetLastAccessTime(const WString& path, const timestamp& lastAccessTime)
		{
			UpdateTimes(path, [&](Times times)
			{
				return times.SetLastAccessTime(lastAccessTime);
			});
		}

		static void SetLastWriteTime(const WString& path, const timestamp& lastWriteTime)
		{
			UpdateTimes(path, [&](Times times)
			{
				return times.SetLastWriteTime(lastWriteTime);
			});
		}

		static DWORD GetAttributesAsDWORD(const WString& path)
		{
			return fileSystemAttributes.GetAllAttributesAsDWORD(path);
		}

		static FileSystemAttributes::FileAttributeSet GetAttributes(const WString& path)
		{
			return std::move(
				fileSystemAttributes.GetAllAttributes(path)
			);
		}

		static void SetWORDAsAttributes(const WString& path, const DWORD fileAttributes)
		{
			fileSystemAttributes.SetAllAttributes(path, fileAttributes);
		}

		static void SetAttributes(const WString& path, const FileSystemAttributes::FileAttributeSet& fileAttributeSet)
		{
			fileSystemAttributes.SetAllAttributes(path, fileAttributeSet);
		}
	};
}


