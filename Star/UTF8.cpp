#include "UTF8.h"

namespace Star::Encoding
{
	WString UTF8::ToWString(const binaries& binaries)
	{
		auto buffer = binaries.data();
		return UTF8ToWStringConvert().from_bytes(buffer, buffer + binaries.size());
	}

	WString UTF8::ToWString(const std::u8string& utf8string)
	{
		auto cstr = (char*)utf8string.c_str();
		return UTF8ToWStringConvert().from_bytes(cstr, cstr + utf8string.size());
	}

	WString UTF8::ToWString(const std::string& utf8string)
	{
		return UTF8ToWStringConvert().from_bytes(utf8string);
	}

	WString UTF8::ToWString(const char* first, const char* last)
	{
		return UTF8ToWStringConvert().from_bytes(first, last);
	}

	UTF8ToWStringConverter UTF8::UTF8ToWStringConvert()
	{
		return UTF8ToWStringConverter{};
	}
}
