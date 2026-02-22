#ifdef __linux__
#include "platform_info.h"

namespace kanji::system
{
	std::filesystem::path PlatformInfo::GetDatabaseLocation()
	{
		return std::filesystem::current_path() / "kanji.db";
	}
} // namespace kanji::system
#endif
