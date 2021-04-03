#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <windows.h>
#include "Text.h"
#include "DateTime.h"
#include "Timestamp.h"

namespace Star
{
	enum class LogTypes
	{
		NORMAL,
		PRIMARY,
		SUCCESS,
		INFO,
		WARN,
		DANGER
	};

	class Console : public Object
	{
	public:
		struct Options
		{
			std::shared_ptr<std::wostream> OStream = std::make_shared<std::wostream>(std::wcout.rdbuf());
			bool Parentheses = false;
			bool PrintDateTime = false;
			WString DatetimeFormat = L"%Y-%m-%d %H:%M:%S";
			bool PrintType = false;
			unsigned char EOLCount = 1;
		};
	private:
		CONSOLE_SCREEN_BUFFER_INFOEX* consoleScreenInfo = new CONSOLE_SCREEN_BUFFER_INFOEX();
		Options options;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	protected:
		unsigned int ToColor(LogTypes& logType);

		void Colorize(LogTypes& logType, char margin = 0);

		void RollbackColors();

		WString ToLogTypeString(LogTypes logType);

		template<typename ValueType>
		void logValue(LogTypes logType, ValueType value, WString leftWrapper, WString rightWrapper)
		{
			auto& ostream = *options.OStream;
			if (options.Parentheses)
			{
				ostream << L"(";
			}
			if (leftWrapper != L"")
			{
				ostream << leftWrapper;
			}
			Colorize(logType, 8);
			ostream << value;
			Colorize(logType);
			if (rightWrapper != L"")
			{
				ostream << rightWrapper;
			}
			if (options.Parentheses)
			{
				ostream << L")";
			}
		}

		template<typename ValueType>
		void logValue(LogTypes logType, ValueType value, WString wrapper)
		{
			logValue<ValueType>(logType, value, wrapper, wrapper);
		}

		template<typename ValueType>
		void logValue(LogTypes logType, ValueType value)
		{
			logValue<ValueType>(logType, value, L"");
		}

		template<typename NameType, typename ValueType>
		void log(LogTypes logType, NameType name, ValueType value)
		{
			Colorize(logType);
			auto& ostream = *options.OStream;
			if (options.PrintDateTime)
			{
				ostream << Timestamp::Format(
					Timestamp::Now(),
					options.DatetimeFormat
				) << L" | ";
			}
			if (options.PrintType)
			{
				ostream << ToLogTypeString(logType) << std::endl;
			}
			if (name != nullptr)
			{
				ostream << name << L": ";
			}
			if constexpr (std::is_base_of<ICompatibleWithString, ValueType>::value)
			{
				logValue(logType, value.ToString());
			}
			else if constexpr (is_basic_string<ValueType>::value)
			{
				if constexpr (std::is_same_v<ValueType, WString>)
				{
					logValue<WString>(logType, value, L"L\"", L"\"s");
				}
				else
				{
					logValue<WString>(logType, WString(value.begin(), value.end()), L"\"", L"\"s");
				}
			}
			else if constexpr (std::is_same_v<ValueType, bool>)
			{
				logValue(logType, value ? L"true" : L"false");
			}
			else if constexpr (std::is_fundamental<ValueType>::value)
			{
				logValue(logType, value);
			}
			else
			{
				logValue(logType, Reflect::Format(Reflect::TypeName(value)));
			}
			for (unsigned char i = 0; i < options.EOLCount; ++i)
			{
				ostream << std::endl;
			}
			ostream.flush();
			RollbackColors();
		}

	public:
		Console(Options options);

		Console();

		~Console();

		template<typename NameType, typename ValueType>
		void Log(NameType name, ValueType value)
		{
			log(LogTypes::NORMAL, name, value);
		}

		template<typename ValueType>
		void Log(ValueType value)
		{
			log(LogTypes::NORMAL, nullptr, value);
		}

		template<typename NameType, typename ValueType>
		void Primary(NameType name, ValueType value)
		{
			log(LogTypes::PRIMARY, name, value);
		}

		template<typename ValueType>
		void Primary(ValueType value)
		{
			log(LogTypes::PRIMARY, nullptr, value);
		}

		template<typename NameType, typename ValueType>
		void Success(NameType name, ValueType value)
		{
			log(LogTypes::SUCCESS, name, value);
		}

		template<typename ValueType>
		void Success(ValueType value)
		{
			log(LogTypes::SUCCESS, nullptr, value);
		}

		template<typename NameType, typename ValueType>
		void Info(NameType name, ValueType value)
		{
			log(LogTypes::INFO, name, value);
		}

		template<typename ValueType>
		void Info(ValueType value)
		{
			log(LogTypes::INFO, nullptr, value);
		}

		template<typename NameType, typename ValueType>
		void Warn(NameType name, ValueType value)
		{
			log(LogTypes::WARN, name, value);
		}

		template<typename ValueType>
		void Warn(ValueType value)
		{
			log(LogTypes::WARN, nullptr, value);
		}

		template<typename NameType, typename ValueType>
		void Error(NameType name, ValueType value)
		{
			log(LogTypes::DANGER, name, value);
		}

		template<typename ValueType>
		void Error(ValueType value)
		{
			log(LogTypes::DANGER, nullptr, value);
		}
	};

	static Console console = Console();

	#define log(n, value) console.Log(n, value)
	#define logv(value) console.Log(#value, value)
	#define log_primary(n, value) console.Primary(n, value)
	#define logv_primary(value) console.Primary(#value, value)
	#define log_success(n, value) console.Success(n, value)
	#define logv_success(value) console.Success(#value, value)
	#define log_info(n, value) console.Info(n, value)
	#define logv_info(value) console.Info(#value, value)
	#define log_warn(n, value) console.Warn(n, value)
	#define logv_warn(value) console.Warn(#value, value)
	#define log_error(n, value) console.Error(n, value)
	#define logv_error(value) console.Error(#value, value)
}