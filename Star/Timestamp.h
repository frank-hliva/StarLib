#pragma once

#include <chrono>
#include <time.h> 
#include <iomanip> 
#include <chrono>
#include <utility>
#include "Text.h"
#include <wtypesbase.h>
#include <sstream>
#include "DateTime.h"

namespace Star
{
	struct Timestamp
	{
		static timestamp Now()
		{
			return std::time(nullptr);
		}


		static constexpr timestamp Empty()
		{
			return (timestamp)(-1);
		}

		template<typename ResulCharType = wchar_t>
		static std::basic_string<ResulCharType> Format(timestamp time, WString format)
		{
			return Format(time, format.c_str());
		}

		template<typename ResulCharType = wchar_t>
		static std::basic_string<ResulCharType> Format(timestamp time, const wchar_t* format = L"%Y-%m-%d %H:%M:%S")
		{
			auto dateTime = DateTime(
				*std::localtime(&time),
				DateTimeKind::Unspecified
			);
			return dateTime.ToString(format);
		}

		template<typename ResulCharType = wchar_t>
		static std::basic_string<ResulCharType> FormatAsGMT(timestamp time, WString format)
		{
			return FormatAsGMT(time, format.c_str());
		}

		template<typename ResulCharType = wchar_t>
		static std::basic_string<ResulCharType> FormatAsGMT(timestamp time, const wchar_t* format = L"%Y-%m-%d %H:%M:%S")
		{
			auto dateTime = DateTime(
				*std::gmtime(&time),
				DateTimeKind::Unspecified
			);
			return dateTime.ToString(format);
		}

		template<typename InputCharType = wchar_t, typename FormatCharType = wchar_t>
		static timestamp Parse(
			const InputCharType* input,
			const FormatCharType* format = L"%Y-%m-%d %H:%M:%S",
			const DateTimeKind kind = DateTimeKind::Unspecified,
			const DaylightSavingMode daylightSaving = DaylightSavingMode::Unknown
		)
		{
			auto time = tm{ .tm_isdst = static_cast<int>(daylightSaving) };
			auto stringStream = std::basic_istringstream<InputCharType>(input);
			auto formatCString = ConvertString::ToCString<InputCharType, FormatCharType>(format);
			stringStream >> std::get_time<InputCharType>(&time, formatCString);

			return DateTime(
				time,
				Star::milliseconds{ 0 },
				kind
			).ToTimestamp();
		}

		template<typename InputCharType = wchar_t, typename FormatCharType = wchar_t>
		static timestamp Parse(
			const std::basic_string<InputCharType>& input,
			const FormatCharType* format = L"%Y-%m-%d %H:%M:%S",
			const DateTimeKind kind = DateTimeKind::Unspecified,
			const DaylightSavingMode daylightSaving = DaylightSavingMode::Unknown
		)
		{
			return Parse<InputCharType, FormatCharType>(
				input.c_str(),
				format,
				kind,
				daylightSaving
			);
		}

		static timestamp FromFileTime(const FILETIME& fileTime)
		{
			ULARGE_INTEGER uLargeInt;
			uLargeInt.LowPart = fileTime.dwLowDateTime;
			uLargeInt.HighPart = fileTime.dwHighDateTime;
			return uLargeInt.QuadPart / 10000000ULL - 11644473600ULL;
		}

		static FILETIME ToFileTime(const timestamp& time)
		{
			FILETIME fileTime;
			LONGLONG longLong = Int32x32To64(time, 10000000) + 116444736000000000;
			fileTime.dwLowDateTime = (DWORD)longLong;
			fileTime.dwHighDateTime = longLong >> 32;
			return fileTime;
		}
	};

}