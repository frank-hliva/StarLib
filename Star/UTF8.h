#pragma once

#include <codecvt>
#include <locale>
#include "Star.h"
#include "Binary.h"

namespace Star::Encoding
{
	typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> UTF8ToWStringConverter;

	class UTF8
	{
	private:
		static UTF8ToWStringConverter UTF8ToWStringConvert();
	public:
		static WString ToWString(const char* first, const char* last);
		static WString ToWString(const std::string& utf8string);
		static WString ToWString(const std::u8string& utf8string);
		static WString ToWString(const binaries& binaries);
		template<typename ResultType = std::u8string>
		static ResultType FromWString(const WString& wstring)
		{
			auto string = UTF8ToWStringConvert().to_bytes(wstring);
			if constexpr (std::is_same_v<ResultType, std::u8string>)
			{
				return std::u8string(string.begin(), string.end());
			}
			else if constexpr (std::is_same_v<ResultType, std::string>)
			{
				return string;
			}
			else if constexpr (std::is_same_v<ResultType, Star::binaries>)
			{
				return Star::binaries(string.begin(), string.end());
			}
			else
			{
				static_assert(false, __FUNCTION__ ": Unsupported type");
			}
		}
	};
}
