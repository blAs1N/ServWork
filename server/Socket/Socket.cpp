#include "Socket.h"
#include <chrono>
#include <mutex>
#include <thread>
#include "Config.h"

namespace ServWork
{
	Socket::Socket()
		: s(INVALID_SOCKET) {}

	Socket::Socket(Socket&& other) noexcept
		: s(std::exchange(other.s, INVALID_SOCKET)) {}

	Socket& Socket::operator=(Socket&& other) noexcept
	{
		s = std::exchange(other.s, INVALID_SOCKET);
		return *this;
	}

	Socket::~Socket()
	{
		if (s != INVALID_SOCKET)
			Close();
	}

	void Socket::Close() noexcept
	{
		closesocket(s);
		s = INVALID_SOCKET;
	}

	void Socket::Send(byte id, const Buffer& buf) const
	{
		if (s == INVALID_SOCKET)
			throw MakeWarning("socket_not_open");
		
		static std::mutex sendMutex;
		std::lock_guard<std::mutex> lock{ sendMutex };

		auto size = static_cast<uint32>(buf.GetCurSize());
		const Header header{ Config::checkKey, id, EndianTranslator::Translate(size) };

		auto buffer = buf;
		size += HEADER_SIZE;
		buffer >>= HEADER_SIZE;
		buffer.Set(0, header);

		if (send(s, buffer, size, 0) != size)
			throw MakeWarning("send_failed");
	}

	void Socket::Recv(Buffer& buf, size_t size) const
	{
		using namespace std::chrono_literals;
		
		if (s == INVALID_SOCKET)
			throw MakeWarning("socket_not_open");

		static std::mutex recvMutex;
		std::lock_guard<std::mutex> lock{ recvMutex };

		auto tmp = new char[size];
		size_t totalSize = 0;
		uint8 retry = 0;

		while (totalSize < size)
		{
			size_t readSize = recv(s, tmp, static_cast<int>(size - totalSize), 0);
			buf.Set(totalSize, tmp, readSize);

			if (readSize == SOCKET_ERROR)
			{
				if (++retry > 10)
					throw MakeWarning("recv_failed");
				std::this_thread::sleep_for(50ms);
			}
			else
			{
				retry = 0;
				totalSize += readSize;
				if (totalSize < size)
					std::this_thread::sleep_for(5ms);
			}
		}
	}	
}