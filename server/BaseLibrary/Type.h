#pragma once

#pragma warning(disable:4996)

#include <cstddef>
#include <string>

using namespace std::string_literals;

#define _STR(x) x
#define STR(x) _STR(x)

namespace ServWork
{
	using byte = unsigned char;

	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using int8 = signed char;
	using int16 = signed short;
	using int32 = signed int;
	using int64 = signed long long;
	
	using char_t = char;
	using String = std::string;

#pragma pack(push, 1)
	extern "C" struct Header
	{
		uint8 check;
		uint8 id;
		uint32 size;
	};
#pragma pack(pop)

	constexpr auto HEADER_SIZE = sizeof(Header);
	constexpr char_t ROOT_PATH[] = ROOT_DIR;
	constexpr char_t CUR_PATH[] = CUR_DIR;

	template <typename T, size_t N>
	constexpr size_t ArrSize(T(&)[N]) noexcept { return N; }
}