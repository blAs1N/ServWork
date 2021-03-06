#include "Engine.h"
#include <iostream>
#include "Config.h"
#include "EventManager.h"
#include "ServerSocket.h"
#include "ThreadPool.h"

namespace ServWork
{
	EngineBase::EngineBase()
		: sock{ new ServerSocket }
	{
		std::locale::global(std::locale{ "ko_KR.UTF-8" });

		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa))
		{
			std::cout << Name{ STR("cannot_start_up_wsa") }.Get() << std::endl;
			exit(3);
		}

		try
		{
			Config::Init();
			sock->Open();
		}
		catch (Exception& e)
		{
			std::cout << Name{ STR("cannot_start") }.Get() << e.What() << std::endl;
			exit(2);
		}

		std::cout << Name{ STR("start") }.Get() << '\n' << std::endl;
	}

	EngineBase::~EngineBase()
	{
		sock->Close();
		WSACleanup();
		delete sock;

		std::cout << Name{ STR("end") }.Get() << '\n' << std::endl;
	}

	int EngineBase::Run()
	{
		ThreadPool threadPool;

		while (true)
		{
			const auto [index, event] = EventManager::Get().GetNetworkEvent();
			
			try
			{
				switch (event)
				{
				case FD_ACCEPT:
				{
					sock->Connect();
					break;
				}
				case FD_CLOSE:
				{
					const auto id = EventManager::Get().GetId(index);
					sock->Disconnect(sock->FindClient(id));
					break;
				}
				case FD_READ:
				{
					const auto id = EventManager::Get().GetId(index);
					sock->FindClient(id).OnReceive();
					break;
				}
				}
			}
			catch (Warning& e)
			{
				std::cout << e.What() << std::endl;
				continue;
			}
			catch (Error& e)
			{
				std::cout << e.What() << std::endl;
				return 1;
			}
		}

		return 0;
	}
}