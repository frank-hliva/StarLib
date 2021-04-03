#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <codecvt>
#include <algorithm>
#include <filesystem>
#include "Star.h"
#include "Binary.h"
#include "String.h"
#include "InputStreamIterable.h"
#include "FileSystemAttributes.h"
#include "DateTime.h"
#include "Error.h"
#include <functional>
#include <sys/utime.h>
#include "Timestamp.h"
#include "FileSystemEntry.h"

namespace Star::IO
{
	class InputStream
	{
	public:
		template<typename StreamCharType = wchar_t>
		static size_t Position(
			std::basic_ifstream<StreamCharType>& inputStream
		)
		{
			std::streampos bytesSize(inputStream.tellg());
			return static_cast<std::size_t>(bytesSize);
		}

		template<typename StreamCharType = wchar_t>
		static size_t Size(
			std::basic_ifstream<StreamCharType>& inputStream
		)
		{
			auto currentPosition = Position(inputStream);
			inputStream.seekg(0, std::ios::end);
			auto size = Position(inputStream);
			inputStream.seekg(currentPosition, std::ios::beg);
			return size;
		}
	};

#define DEFAULT_TEXT_LOCALE std::locale("en_US.UTF-8")
#define DEFAULT_BINARY_LOCALE std::locale::empty()
#define INIT_EXCEPTIONS(stream) stream.exceptions(std::ios::failbit);

	constexpr int DEFAULT_PORT = 64;

	namespace fs = std::filesystem;
	class File
	{
	public:
		static void Copy(const WString& sourceFileName, const WString& destinationFileName, bool overwrite = false)
		{
			if (overwrite)
			{
				std::filesystem::copy(sourceFileName, destinationFileName, fs::copy_options::overwrite_existing);
			}
			else
			{
				std::filesystem::copy(sourceFileName, destinationFileName);
			}
		}

		static void Copy(const WString& sourceFileName, const WString& destinationFileName, fs::copy_options options)
		{
			std::filesystem::copy(sourceFileName, destinationFileName, options);
		}

		static void Delete(const WString& sourceFileName)
		{
			std::filesystem::remove(sourceFileName);
		}

		static void Move(const WString& sourceFileName, const WString& destinationFileName, bool overwrite = false)
		{
			if (overwrite && Exists(destinationFileName))
			{
				Delete(destinationFileName);
			}
			std::filesystem::rename(sourceFileName, destinationFileName);
		}

		template<typename CharType = wchar_t>
		static std::basic_fstream<CharType> Create(
			const WString& path,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_fstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			stream.close();
			stream.open(path, std::ios_base::in | std::ios_base::out);
			return stream;
		}

		template<typename CharType = wchar_t>
		static std::basic_ofstream<CharType> CreateText(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_ofstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			stream.imbue(DEFAULT_TEXT_LOCALE);
			return stream;
		}

		template<typename CharType = wchar_t>
		static std::basic_fstream<CharType> Open(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_fstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			return stream;
		}

		template<typename CharType = wchar_t>
		static std::basic_ifstream<CharType> OpenRead(
			const WString& path,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_ifstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			return stream;
		}

		template<typename CharType = wchar_t>
		static std::basic_ifstream<CharType> OpenText(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_ifstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			stream.imbue(DEFAULT_TEXT_LOCALE);
			return stream;
		}

		template<typename CharType = wchar_t>
		static std::basic_ofstream<CharType> OpenWrite(
			const WString& path,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_ofstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream);
			return stream;
		}
		template<typename CharType = wchar_t>
		static std::basic_ofstream<CharType> AppendText(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::app,
			const int port = DEFAULT_PORT
		)
		{
			auto stream = std::basic_ofstream<CharType>(path, mode, port);
			INIT_EXCEPTIONS(stream)
			stream.imbue(DEFAULT_TEXT_LOCALE);
			return stream;
		}

		static size_t FileSize(const WString& path)
		{
			auto fileStream = OpenRead(path);
			return InputStream::Size(fileStream);
		}

		static bool Exists(const WString& path)
		{
			auto fileStream = OpenRead(path);
			return fileStream.good();
		}

		template<typename CharType = wchar_t>
		static std::basic_string<CharType> ReadAllText(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			auto inputStream = OpenText<CharType>(path, locale, mode, port);
			std::basic_stringstream<CharType> stringStream;
			stringStream << inputStream.rdbuf();
			return stringStream.str();
		}

		
		template<typename CharType = wchar_t, class Container>
		static std::back_insert_iterator<Container> ReadAllLines(
			const WString& path,
			std::back_insert_iterator<Container> inserter,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			auto inputStream = OpenText<CharType>(path, locale, mode, port);
			std::basic_string<CharType> line;
			while (std::getline(inputStream, line))
			{
				*inserter = line;
			}
			return inserter;
		}

		template<typename CharType = wchar_t>
		static std::vector<std::basic_string<CharType>> ReadAllLines(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			auto result = std::vector<std::basic_string<CharType>>{};
			ReadAllLines(path, back_inserter(result), locale, mode, port);
			return result;
		}

		template<typename CharType = wchar_t>
		static InputStreamIterable<CharType> ReadLines(
			const WString& path,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::in,
			const int port = DEFAULT_PORT
		)
		{
			return InputStreamIterable<CharType>(
				OpenText(path, locale, mode, port)
			);
		}

		template<typename CharType = wchar_t>
		static void WriteAllText(
			const WString& path,
			const std::basic_string<CharType> contents,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			auto outputStream = OpenWrite<CharType>(path, mode, port);
			outputStream.imbue(locale);
			outputStream << contents;
		}

		template<typename CharType = wchar_t>
		static void WriteAllText(
			const WString& path,
			const CharType* contents,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			WriteAllText(path, std::basic_string<CharType>(contents), locale, mode, port);
		}

		template<typename CharType = wchar_t>
		static void AppendAllText(
			const WString& path,
			const std::basic_string<CharType> contents,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::app,
			const int port = DEFAULT_PORT
		)
		{
			WriteAllText(path, contents, locale, mode, port);
		}

		template<typename CharType = wchar_t>
		static void AppendAllText(
			const WString& path,
			const CharType* contents,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::app,
			const int port = DEFAULT_PORT
		)
		{
			WriteAllText(path, std::basic_string<CharType>(contents), locale, mode, port);
		}

		template<typename CharType = wchar_t, class InputIterator>
		static void WriteAllLines(
			const WString& path,
			InputIterator first,
			InputIterator last,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = IO::DEFAULT_PORT
		)
		{
			_Adl_verify_range(first, last);
			auto begin = _Get_unwrapped(first);
			const auto end = _Get_unwrapped(last);
			std::basic_string<CharType> contents;
			for (auto it = begin; it != end; ++it)
			{
				contents.append(*it);
				contents.append(NewLine<CharType>);
			}
			WriteAllText<CharType>(path, contents, locale, mode, port);
		}

		template<typename CharType = wchar_t, typename Sequence>
		static void WriteAllLines(
			const WString& path,
			const Sequence lines,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::out,
			const int port = DEFAULT_PORT
		)
		{
			WriteAllLines(path, std::begin(lines), std::end(lines), locale, mode, port);
		}

		template<typename CharType = wchar_t, class InputIterator>
		static void AppendAllLines(
			const WString& path,
			InputIterator first,
			InputIterator last,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::app,
			const int port = DEFAULT_PORT
		)
		{
			WriteAllLines(path, first, last, locale, mode, port);
		}

		template<typename CharType = wchar_t, typename Sequence>
		static void AppendAllLines(
			const WString& path,
			const Sequence lines,
			const std::locale locale = DEFAULT_TEXT_LOCALE,
			const std::ios_base::openmode mode = std::ios_base::app,
			const int port = DEFAULT_PORT
		)
		{
			AppendAllLines(path, std::begin(lines), std::end(lines), locale, mode, port);
		}

		template<typename CharType = char>
		static std::vector<CharType> ReadAllBytes(
			const WString& path,
			const std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary,
			const int port = DEFAULT_PORT
		)
		{
			auto inputStream = OpenRead<CharType>(path, mode, port);
			auto size = InputStream::Size<CharType>(inputStream);
			std::vector<CharType> bytes(size);
			inputStream.read(bytes.data(), size);
			return bytes;
		}

		template<typename CharType = char>
		static void WriteAllBytes(
			const WString& path,
			std::vector<CharType> bytes,
			const std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary,
			const int port = DEFAULT_PORT
		)
		{
			auto outputStream = OpenWrite<CharType>(path, mode, port);
			outputStream.write(bytes.data(), bytes.size() * sizeof(CharType));
		}

		/** From FileSystemEntry */
		static constexpr auto GetStat = &FileSystemEntry::GetStat;
		static constexpr auto GetTimes = &FileSystemEntry::GetTimes;
		static constexpr auto SetTimes = &FileSystemEntry::SetTimes;

		static constexpr auto GetCreationTime = &FileSystemEntry::GetCreationTime;
		static constexpr auto GetLastAccessTime = &FileSystemEntry::GetLastAccessTime;
		static constexpr auto GetLastWriteTime = &FileSystemEntry::GetLastWriteTime;
		static constexpr auto UpdateTimes = &FileSystemEntry::UpdateTimes;

		static constexpr auto SetCreationTime = &FileSystemEntry::SetCreationTime;
		static constexpr auto SetLastAccessTime = &FileSystemEntry::SetLastAccessTime;
		static constexpr auto SetLastWriteTime = &FileSystemEntry::SetLastWriteTime;
		static constexpr auto GetAttributesAsDWORD = &FileSystemEntry::GetAttributesAsDWORD;
		static constexpr auto GetAttributes = &FileSystemEntry::GetAttributes;
		static constexpr auto SetWORDAsAttributes = &FileSystemEntry::SetWORDAsAttributes;
		static constexpr auto SetAttributes = &FileSystemEntry::SetAttributes;
	};

#undef DEFAULT_TEXT_LOCALE
#undef DEFAULT_BINARY_LOCALE
#undef INIT_EXCEPTIONS
}



