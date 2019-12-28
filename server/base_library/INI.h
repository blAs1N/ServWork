#pragma once

#include <string>
#include <map>

namespace ServWork
{
	class INI final
	{
	public:
		INI(const std::string& inPath);

		INI(const INI& other) = default;
		INI(INI&& other) = default;

		INI& operator=(const INI& other) = default;
		INI& operator=(INI&& other) = default;

		~INI() = default;

		inline const std::string& Get(const std::string& key) const
		{
			return (*map.find(key)).second;
		}

		void Set(const std::string& key, const std::string& value);

	private:
		std::map<std::string, std::string> map;
		std::string path;
	};
}