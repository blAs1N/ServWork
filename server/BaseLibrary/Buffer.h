#pragma once

#include "Type.h"
#include <cstring>
#include <vector>
#include "StringTranslator.h"

namespace ServWork
{
	class Buffer final
	{
	public:
		Buffer() = default;
		Buffer(std::nullptr_t) : Buffer() {}

		Buffer(size_t size) : vec(size) {};

		Buffer(const Buffer& other) = default;
		Buffer(Buffer&& other) noexcept = default;

		Buffer& operator=(const Buffer& other) = default;
		Buffer& operator=(Buffer&& other) noexcept = default;

		~Buffer() = default;

		Buffer& operator=(const char_t* content);
		Buffer& operator=(const byte* content);

		template <size_t N>
		Buffer& operator=(const byte(&content)[N])
		{
			Init();
			Set(0, content);
			return *this;
		}

		template <class T>
		Buffer& operator=(const T& content)
		{
			Init();
			Set(0, content);
			return *this;
		}

		inline byte operator[](size_t index) const
		{
			return vec[index];
		}

		Buffer& operator+=(const char_t* content);

		template <size_t N>
		inline Buffer& operator+=(const byte(&content)[N])
		{
			vec.insert(vec.cend(), content, content + N);
			return *this;
		}

		template <class T>
		inline Buffer& operator+=(const T& value)
		{
			return operator+=<sizeof(T)>(reinterpret_cast<const byte*>(&value));
		}

		inline Buffer& operator<<=(size_t index) noexcept
		{
			std::move(
				std::make_move_iterator(vec.begin() + index),
				std::make_move_iterator(vec.end()),
				vec.begin()
			);
			vec.shrink_to_fit();
			return *this;
		}

		inline Buffer& operator>>=(size_t index) noexcept
		{
			vec.resize(vec.size() + index);
			std::move(
				std::make_move_iterator(vec.begin()),
				std::make_move_iterator(vec.end() - index),
				vec.begin() + index
			);
			return *this;
		}

		inline void Reserve(size_t size)
		{
			vec.reserve(size);
		}

		inline void Init() noexcept
		{
			vec.clear();
		}

		inline byte* Get(size_t index = 0) noexcept
		{
			return vec.data() + index;
		}

		inline const byte* Get(size_t index = 0) const noexcept
		{
			return vec.data() + index;
		}

		inline String ToString(size_t index = 0) const noexcept
		{
			return reinterpret_cast<const char_t*>(Get());
		}

		void Set(size_t index, const byte* content, size_t size);

		template <size_t N>
		inline void Set(size_t index, const byte(&content)[N])
		{
			Set(index, content, N);
		}

		inline void Set(size_t index, const char_t* content)
		{
			Set(index, reinterpret_cast<const byte*>(content),
				strnlen(content, GetMaxSize() - index));
		}

		inline void Set(size_t index, const char_t* content, size_t size)
		{
			Set(index, reinterpret_cast<const byte*>(content), size);
		}

		template <class T>
		inline void Set(size_t index, const T& value)
		{
			Set(index, reinterpret_cast<const byte*>(&value), sizeof(T));
		}

		inline size_t GetCurSize() const noexcept
		{
			return vec.size();
		}

		inline size_t GetMaxSize() const noexcept
		{
			return vec.capacity();
		}

		inline operator byte*() noexcept
		{
			return Get();
		}

		inline operator const byte*() const noexcept
		{
			return Get();
		}

		inline operator char_t*() noexcept
		{
			return reinterpret_cast<char_t*>(Get());
		}

		inline operator const char_t*() const noexcept
		{
			return reinterpret_cast<const char_t*>(Get());
		}

		inline operator void*() noexcept
		{
			return reinterpret_cast<void*>(Get());
		}

		inline operator const void*() const noexcept
		{
			return reinterpret_cast<const void*>(Get());
		}

		inline operator bool() const noexcept
		{
			return Get();
		}

	private:
		std::vector<byte> vec;
	};

	inline Buffer operator+(const Buffer& buffer, const char_t* content)
	{
		return Buffer{ buffer } += content;
	}

	template <size_t N>
	inline Buffer operator+(const Buffer& buffer, const byte(&content)[N])
	{
		return Buffer{ buffer } += content;
	}

	template <class T>
	inline Buffer operator+(const Buffer& buffer, const T& content)
	{
		return Buffer{ buffer } += content;
	}

	inline Buffer operator<<(const Buffer& buffer, size_t index) noexcept
	{
		return Buffer{ buffer } <<= index;
	}

	inline Buffer operator>>(const Buffer& buffer, size_t index) noexcept
	{
		return Buffer{ buffer } >>= index;
	}
}