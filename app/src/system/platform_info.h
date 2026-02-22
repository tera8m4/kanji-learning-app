#pragma once

#include <filesystem>

namespace kanji::system
{
	class PlatformInfo
	{
	public:
		static std::filesystem::path GetDatabaseLocation();
	};
} // namespace kanji::system
