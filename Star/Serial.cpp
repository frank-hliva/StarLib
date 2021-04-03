#include "Serial.h"

Serial::Serial(std::string port, DCB options /*= { 0 }*/)
{
	auto prefix = "\\\\.\\";
	auto portName = port.rfind(prefix, 0) == 0 ? port : (prefix + port);
	this->portHandle = CreateFileA(static_cast<LPCSTR>(portName.c_str()),
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
	isAvailable = this->portHandle != INVALID_HANDLE_VALUE;
	if (isAvailable)
	{
		if (GetCommState(portHandle, &options))
		{
			if (SetCommState(portHandle, &options))
			{
				isAvailable = true;
				PurgeComm(portHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
			}
			else
			{
				throw Errors::Error();
			}
		}
		else
		{
			isAvailable = false;
			throw Errors::Error();
		}
	}
	else
	{
		throw Errors::Error();
	}
}

DWORD Serial::Write(const char* buffer, const DWORD size)
{
	DWORD writtenBytes;
	if (WriteFile(portHandle, buffer, size, &writtenBytes, nullptr)) {
		return writtenBytes;
	}
	else
	{
		return Write(buffer, size);
	}
}

DWORD Serial::Read(char* buffer, const DWORD size) const
{
	DWORD readedBytes;
	if (ReadFile(portHandle, buffer, size, &readedBytes, nullptr)) {
		return readedBytes;
	}
	else
	{
		throw Errors::Error();
	}
}

std::optional<char> Serial::Read() const
{
	char buffer;
	DWORD readedBytes;
	if (Read(&buffer, static_cast<DWORD>(1)) == 1)
	{
		return buffer;
	}
	else
	{
		return std::nullopt;
	}
}

std::string Serial::ReadLine(const size_t bufferSize) const
{
	char* buffer = new char[bufferSize + 1];
	char* pointer = buffer;
	DWORD readedBytes, length = 0;
	auto line = std::string();
	while (true) {
		auto readedBytes = Read(pointer, static_cast<DWORD>(1));
		if (*pointer == '\r' || *pointer == '\n' || readedBytes == 0)
		{
			line.append(buffer, length);
			pointer = nullptr;
			delete[] buffer;
			return line;
		}
		else
		{
			if (length == bufferSize)
			{
				line.append(buffer, length + 1);
				length = 0;
				pointer = buffer;
			}
			else
			{
				pointer++;
				length++;
			}
		}
	}
}

std::string Serial::ReadLine() const
{
	return ReadLine(1024);
}

bool Serial::IsAvailable() const
{
	return isAvailable;
}

Serial::~Serial()
{
	Close();
}

void Serial::Close()
{
	if (this->isAvailable)
	{
		this->isAvailable = false;
		CloseHandle(this->portHandle);
	}
}

DWORD Serial::Print(const std::string string)
{
	return Write(string.c_str(), string.size());
}

DWORD Serial::PrintLine(const std::string string)
{
	return Print(string + '\n');
}
