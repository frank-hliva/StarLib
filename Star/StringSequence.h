#pragma once

#include <iostream>
#include <string>
#include <random>

namespace Star
{
	struct StringSequence
	{
		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Range(const CharType from, const CharType to)
		{
			auto size = (to + 1) - from;
			auto string = std::basic_string<CharType>(size, '0');
			for (auto i = from; i <= to; i++)
			{
				string[i - from] = i;
			}
			return string;
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> UpperCaseLetters()
		{
			return Range<CharType>('A', 'Z');
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> LowerCaseLetters()
		{
			return Range<CharType>('a', 'z');
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Letters()
		{
			return UpperCaseLetters<CharType>() + LowerCaseLetters<CharType>();
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Digits()
		{
			return Range<CharType>('0', '9');
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> LettersAndDigits()
		{
			return Letters<CharType>() + Digits<CharType>();
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Random(const unsigned int size, const CharType* chars)
		{
			const auto len = std::basic_string_view<CharType>(chars).size();
			std::random_device randomDevice;
			std::srand(randomDevice());
			auto string = std::basic_string<CharType>(size, '0');
			for (auto i = 0; i < size; i++)
			{
				string[i] = chars[rand() % len];
			}
			return string;
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> Random(const unsigned int size, const std::basic_string<CharType>& chars)
		{
			return Random(size, chars.c_str());
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> RandomHex(const unsigned int size)
		{
			return Random<CharType>(size, L"0123456789ABCDEF");
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> RandomDigits(const unsigned int size)
		{
			return Random<CharType>(size, Digits<CharType>());
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> RandomLettersAndDigits(const unsigned int size)
		{
			return Random<CharType>(size, LettersAndDigits<CharType>());
		}
	};
}

