#pragma once

#include <memory>
#include <string>
#include <optional>
#include "Error.h"
#include "Log.h"

using namespace Star;

class Serial
{
private:
	HANDLE portHandle;
	bool isAvailable;
public:
	Serial(std::string port, DCB options = { 0 });
	DWORD Write(const char* buffer, const DWORD size);
	DWORD Read(char* buffer, const DWORD size) const;
	DWORD Print(const std::string string);
	DWORD PrintLine(const std::string string);
	std::optional<char> Read() const;
	std::string ReadLine(const size_t bufferSize) const;
	std::string ReadLine() const;
	bool IsAvailable() const;
	~Serial();
	void Close();
};