#pragma once

#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include "StringDefs.h"
#include "Text.h"
#include "Binary.h"
#include "Reflect.h"

namespace Star
{
	template<typename Base, typename T>
	inline bool InstanceOf(const T*)
	{
		return std::is_base_of<Base, T>::value;
	}

	class ICompatibleWithString
	{
	public:
		virtual WString ToString() const = 0;
	};

	class Object : public ICompatibleWithString
	{
	public:
		const type_info& GetType() const
		{
			return typeid(*this);
		}

		virtual WString ToString() const override
		{
			auto typeName = ConvertString::ToString<wchar_t, char>(
				GetType().name()
			);
			return Reflect::Format(typeName);
		}
	};

	class Guid : public Object
	{
	private:
		std::string guid;
	protected:
		Guid(std::string guid);
	public:
		static Guid NewGuid();

		static Guid Empty();

		binaries ToBinaries() const;

		template<typename CharType>
		std::basic_string<CharType> ToString() const;

		WString ToString() const override;
	};

	template<typename CharType>
	std::basic_string<CharType>
		Guid::ToString() const
	{
		if constexpr (std::is_same_v<CharType, char>)
		{
			return guid;
		}
		else
		{
			return WString(guid.begin(), guid.end());
		}
	}
}