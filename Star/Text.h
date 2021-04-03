#pragma once

#include <string>
#include <iterator>
#include <vector>

namespace Star
{
	template<class CharType>
	struct is_basic_string : std::false_type
	{
	};

	template<class Ch, class Tr, class Al>
	struct is_basic_string<std::basic_string<Ch, Tr, Al>> : std::true_type
	{
	};

	#define requires_base_string(Type) \
		static_assert(is_basic_string<Type>::value, __FUNCTION__ ": The parameter '" #Type "' can only be derived from 'std::basic_string'");

	class ConvertString
	{
	public:
		template<typename ResultCharType = wchar_t, typename InputCharType = wchar_t>
		static std::basic_string<ResultCharType> ToString(const InputCharType* input)
		{
			auto str = std::basic_string<InputCharType>(input);
			if constexpr (std::is_same_v<ResultCharType, InputCharType>)
			{
				return str;
			}
			else
			{
				return std::basic_string<ResultCharType>(str.begin(), str.end());
			}
		}

		template<typename ResultStringType = WString, typename InputCharType = wchar_t>
		static ResultStringType ToStringT(const InputCharType* input)
		{
			requires_base_string(ResultStringType);
			auto str = std::basic_string<InputCharType>(input);
			if (std::is_same_v<ResultStringType.value_type, InputCharType>)
			{
				return str;
			}
			else
			{
				return ResultStringType(str.begin(), str.end());
			}
		}

		template<typename ResultCharType = wchar_t, typename InputCharType = wchar_t>
		static const ResultCharType* ToCString(const std::basic_string<InputCharType>& input)
		{
			auto arraySize = input.size() + 1;
			auto arraySizeInBytes = arraySize * sizeof(ResultCharType);
			ResultCharType* cstring = new ResultCharType[arraySize];
			if constexpr (std::is_same_v<ResultCharType, InputCharType>)
			{
				memcpy(cstring, input.c_str(), arraySizeInBytes);
			}
			else
			{
				auto str = std::basic_string<ResultCharType>(input.begin(), input.end());
				memcpy(cstring, str.c_str(), arraySizeInBytes);
			}
			return cstring;
		}

		template<typename ResultCharType = wchar_t, typename InputCharType = wchar_t>
		static const ResultCharType* ToCString(const InputCharType* input)
		{
			return ToCString<ResultCharType, ResultCharType>(
				ToString<ResultCharType, InputCharType>(input)
				);
		}
	};

	struct String
	{
		template<typename CharType = wchar_t, class InputIterator>
		static std::basic_string<CharType> Join(const CharType* separator, InputIterator first, InputIterator last)
		{
			_Adl_verify_range(first, last);
			auto begin = _Get_unwrapped(first);
			const auto end = _Get_unwrapped(last);
			std::basic_string<CharType> joinedString;
			for (auto it = begin; it != end; ++it)
			{
				if (it != begin && separator != nullptr)
				{
					joinedString += separator;
				}
				joinedString += *it;
			}
			return joinedString;
		}

		template<typename CharType = wchar_t, class Sequence>
		static std::basic_string<CharType> Join(const CharType* separator, Sequence seq)
		{
			return Join(separator, std::begin(seq), std::end(seq));
		}

		template<typename CharType = wchar_t, class Sequence>
		static std::basic_string<CharType> Join(const std::basic_string<CharType> separator, Sequence seq)
		{
			return Join(separator.c_str(), std::begin(seq), std::end(seq));
		}

		template<typename CharType = wchar_t, class Sequence>
		static std::basic_string<CharType> Join(Sequence seq)
		{
			return Join(nullptr, std::begin(seq), std::end(seq));
		}

		template<typename CharType = wchar_t, class Container>
		static std::back_insert_iterator<Container> Split(
			std::basic_string<CharType> separator,
			std::back_insert_iterator<Container> inserter,
			std::basic_string<CharType> input
		)
		{
			const size_t separatorLength = separator.length();
			size_t start = 0, end;
			std::basic_string<CharType> token;
			while ((end = input.find(separator, start)) != std::basic_string<CharType>::npos)
			{
				token = input.substr(start, end - start);
				start = end + separatorLength;
				*inserter = token;
			}
			*inserter = input.substr(start);
			return inserter;
		}

		template<typename CharType = wchar_t>
		static std::vector<std::basic_string<CharType>> Split(
			std::basic_string<CharType> separator,
			std::basic_string<CharType> input
		)
		{
			std::vector<std::basic_string<CharType>> result;
			Split(separator, std::back_inserter(result), input);
			return result;
		}

		template<typename CharType = wchar_t>
		static inline std::basic_string<CharType> WHITESPACES = ConvertString::ToString<CharType, wchar_t>(L" \n\r\t\f\v");

		template<typename CharType = wchar_t>
		static inline std::basic_string<CharType> EMPTY = ConvertString::ToString<CharType, wchar_t>(L"");

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> TrimStart(const std::basic_string<CharType>& input, const std::basic_string<CharType>& trimChars = WHITESPACES<CharType>)
		{
			size_t start = input.find_first_not_of(trimChars);
			return (start == std::basic_string<CharType>::npos) ? EMPTY<CharType> : input.substr(start);
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> TrimEnd(const std::basic_string<CharType>& input, const std::basic_string<CharType>& trimChars = WHITESPACES<CharType>)
		{
			size_t end = input.find_last_not_of(trimChars);
			return (end == std::basic_string<CharType>::npos) ? EMPTY<CharType> : input.substr(0, end + 1);
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Trim(const std::basic_string<CharType>& input, const std::basic_string<CharType>& trimChars = WHITESPACES<CharType>)
		{
			return TrimEnd<CharType>(TrimStart<CharType>(input, trimChars), trimChars);
		}
	};
}