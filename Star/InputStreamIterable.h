#pragma once

#include <istream>
#include <string>
#include "Star.h"

namespace Star
{
	template <typename CharType>
	class line : public Object
	{
	private:
		std::basic_string<CharType> string;
	public:
		operator const std::basic_string<CharType>& () const&
		{
			return string;
		}
		operator std::basic_string<CharType>& ()&
		{
			return string;
		}
		operator std::basic_string<CharType>()&&
		{
			return std::move(string);
		}
	};

	template <typename CharType>
	std::basic_istream<CharType>& operator >>(std::basic_istream<CharType>& stream,
		line<CharType>& str)
	{
		std::getline(stream, static_cast<std::basic_string<CharType>&>(str));
		return stream;
	}

	template<typename CharType>
	class InputStreamIterable : public Object
	{
	private:
		typedef std::istream_iterator<line<CharType>, CharType> iterator;
		std::basic_ifstream<CharType> inputStream;
	public:
		InputStreamIterable(std::basic_ifstream<CharType>&& stream)
			: inputStream(std::move(stream))
		{
		}
		auto begin()
		{
			return iterator{ inputStream };
		}
		auto end()
		{
			return iterator{};
		}
		WString ToString() const override
		{
			return L"InputStreamIterable";
		}
	};
}