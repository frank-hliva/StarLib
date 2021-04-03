#pragma once

#include <iostream>
#include <string>
#include "Star.h"
#include "Text.h"
#include "SystemError.h"
#include "Log.h"

namespace Star
{ 
	namespace Errors
	{
		template<typename CharType = wchar_t>
		class BaseException : public Object
		{
		private:
			friend std::ostream& operator << (std::ostream& stream, const BaseException& error);
		protected:
			const CharType* message;
			const unsigned int code;
		public:
			explicit BaseException(const unsigned int code, const CharType* message) :
				code(code),
				message(message)
			{
			}
			explicit BaseException()
				: BaseException(0, ConvertString::ToCString<CharType>(""))
			{
			}
			explicit BaseException(const CharType* message)
				: BaseException(0, message)
			{
			}
			explicit BaseException(const unsigned int code)
				: BaseException(code, ConvertString::ToCString<CharType>(""))
			{
			}
			virtual ~BaseException() throw ()
			{
			}
			virtual void Log() const throw ()
			{
				return console.Error(ToString());
			}
			virtual const CharType* What() const throw ()
			{
				return message;
			}
			virtual const unsigned int Code() const throw ()
			{
				return code;
			}
			virtual const CharType* Type() const throw ()
			{
				return Star::ConvertString::ToCString<CharType>("Exception");
			}
			virtual const CharType* Message() const throw ()
			{
				return What();
			}
			WString ToString() const override
			{
				std::basic_stringstream<WString::value_type> stream;
				stream << "{" << GetType().name() << " \"" << What() << "\"}";
				return stream.str();
			}
		};

		template<typename CharType = wchar_t>
		std::ostream& operator << (std::ostream& stream, const BaseException<CharType>& error)
		{
			auto code = error.Code();
			std::basic_string<CharType> codeString = code > 0 ? (std::to_string(code) + " ") : "";
			return stream << codeString << "[" << error.Type() << "]: " << error.Message();
		}

		class Exception : public BaseException<wchar_t>
		{
		public:
			using BaseException<wchar_t>::BaseException;

			const wchar_t* Type() const throw () override
			{
				return ConvertString::ToCString<wchar_t>("Exception");
			}
		};

		class Error : public Exception
		{
		public:
			using Exception::Exception;
			const wchar_t* What() const throw () override
			{
				return ConvertString::ToCString(
					*message == 0
						?
							SystemError::ToMessage<wchar_t>(
								code == 0
									? this->Code()
									: code
							)
						:
							message
				);
			}

			virtual const unsigned int Code() const throw ()
			{
				return (
					code == 0
						? SystemError::LastErrorCode()
						: code
				);
			}

			const wchar_t* Type() const throw () override
			{
				return ConvertString::ToCString<wchar_t>("Error");
			}
		};

		class NotImplementedException : public Exception
		{
		public:
			using Exception::Exception;
			NotImplementedException() : NotImplementedException(
				L"The method or operation is not implemented."
			)
			{
			}

			const wchar_t* Type() const throw () override
			{
				return ConvertString::ToCString<wchar_t>("NotImplementedException");
			}
		};
	}
}