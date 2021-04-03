#pragma once

#include <string>
#include <windows.h>
#include "Star.h"
#include "Text.h"

namespace Star
{
	namespace Errors
	{
		class SystemError
		{
		public:
			template<class CharType = wchar_t>
			static std::basic_string<CharType> ToMessage(DWORD errorCode)
			{
				if (errorCode == 0)
				{
					return ConvertString::ToString<CharType>("Undefined error");
				}
				else
				{
					wchar_t* messageBuffer = nullptr;
					size_t size = FormatMessageW(
						FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						nullptr,
						errorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(wchar_t*)(&messageBuffer),
						0,
						nullptr
					);
					WString message(messageBuffer, size);
					LocalFree(messageBuffer);
					if constexpr (std::is_same_v<CharType, wchar_t>)
					{
						return String::Trim<CharType>(message);
					}
					else
					{
						std::basic_string<CharType> result(message.begin(), message.end());
						return String::Trim<CharType>(result);
					}
				}
			}

			static DWORD LastErrorCode()
			{
				return ::GetLastError();
			}

			template<typename CharType = wchar_t>
			static std::basic_string<CharType> LastError()
			{
				return ToMessage<CharType>(::GetLastError());
			}
		};

	}
};
