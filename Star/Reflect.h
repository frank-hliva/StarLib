#pragma once

#include <sstream>
#include "StringDefs.h"
#include "Text.h"

namespace Star
{
	class Reflect
	{
	public:
		template<typename Type>
		static WString TypeName(Type value)
		{
			auto name = typeid(value).name();
			return ConvertString::ToString(name);
		}
		static WString Format(WString typeName)
		{
			std::basic_stringstream<WString::value_type> stream;
			stream << "{" << typeName << "}";
			return stream.str();
		}
	};
}

