#pragma once

#include <memory>
#include <string>
#include <optional>
#include "Error.h"
#include "Log.h"

namespace Star
{

	class Serial
	{
	private:
		DCB commState;
		HANDLE portHandle;
		bool isAvailable;
	public:
		Serial(std::string port, DCB options);
		Serial(std::string port, DWORD baudRate);
		Serial(std::string port);
		~Serial();
		DCB CommState() const;
		bool IsAvailable() const;
		DWORD Write(const char* buffer, const DWORD size);
		DWORD Read(char* buffer, const DWORD size) const;
		DWORD Print(const std::string string);
		DWORD PrintLine(const std::string string);
		std::optional<char> Read() const;
		std::string ReadLine(const size_t bufferSize) const;
		std::string ReadLine() const;
		void Close();
	};

}