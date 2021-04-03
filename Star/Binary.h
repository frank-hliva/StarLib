#pragma once

#include <iostream>
#include <vector>
#include <type_traits>
#include <ctype.h>
#include "Text.h"

namespace Star
{
	typedef std::vector<char> binaries;
	typedef std::vector<wchar_t> wbinaries;

	#define requires_binaries(Type) \
	static_assert(std::is_same_v<Type, binaries> || std::is_same_v<Type, wbinaries>, __FUNCTION__ ": The parameter '" #Type "' was expected to have type 'binaries' or 'wbinaries'");

	#define requires_binaries_ct(CharType) \
	static_assert(std::is_same_v<CharType, char> || std::is_same_v<CharType, wchar_t>, __FUNCTION__ ": The parameter was expected to have type 'binaries' or 'wbinaries'");

	template<typename ResultType, typename CharType = char>
	std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& out, const std::vector<ResultType>& value)
	{
		size_t len = value.size();
		for (size_t i = 0; i < len; ++i)
		{
			out << value[i];
		}
		return out;
	}

	class Binaries
	{
	private:
		template<typename ResultCharType = char, typename InputCharType = char>
		static int CharSize()
		{
			auto charSize = sizeof(InputCharType) / sizeof(ResultCharType);
			return charSize < 1 ? 1 : charSize;
		}
	public:
		template<typename CharType = char>
		static std::vector<CharType> ToQuoted(const std::vector<CharType>& binaries)
		{
			requires_binaries_ct(CharType);
			auto newBinaries = std::vector<CharType>(binaries);
			newBinaries.insert(newBinaries.begin(), '\"');
			newBinaries.push_back('\"');
			return newBinaries;
		}

		template<typename ResultType = std::string, typename InputCharType = char>
		static ResultType ToString(const std::vector<InputCharType>& binaries)
		{
			requires_binaries_ct(InputCharType);
			requires_base_string(ResultType);
			return ResultType(binaries.begin(), binaries.end());
		}

		template<typename ResultCharType = char, typename InputType = std::string>
		static std::vector<ResultCharType> OfString(const InputType& input)
		{
			requires_binaries_ct(ResultCharType);
			if constexpr (is_basic_string<InputType>::value)
			{
				return OfString(input.c_str());
			}
			else if constexpr (
				std::is_same_v<InputType, const char*> ||
				std::is_same_v<InputType, char*> ||
				(std::is_array_v<InputType> && std::is_same_v<InputType, char[std::extent_v<InputType>]>)
			)
			{
				auto charSize = CharSize<ResultCharType, char>();
				auto result = (ResultCharType*)input;
				return std::vector<ResultCharType>(
					result,
					result + (strlen(input) * charSize)
				);
			}
			else if constexpr (
				std::is_same_v<InputType, const wchar_t*> ||
				std::is_same_v<InputType, wchar_t*> ||
				(std::is_array_v<InputType> && std::is_same_v<InputType, wchar_t[std::extent_v<InputType>]>)
			)
			{
				auto charSize = CharSize<ResultCharType, wchar_t>();
				auto result = (ResultCharType*)input;
				return std::vector<ResultCharType>(
					result,
					result + (wcslen(input) * charSize)
				);
			}
			else
			{
				static_assert(false, __FUNCTION__ ": The parameter 'InputType' can only be derived from 'std::basic_string', 'const char[]' or 'const char*'");
			}
		}
	};
}