#ifdef _WIN32
#include "platform_info.h"
#include <ShlObj.h>
#include <windows.h>

namespace
{
	std::filesystem::path GetMainFolder()
	{
		PWSTR path = nullptr;

		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path)))
		{
			std::filesystem::path result(path);
			CoTaskMemFree(path);
			return result / "KanjiReview";
		}

		return {};
	}
} // namespace

namespace kanji::system
{
	std::filesystem::path PlatformInfo::GetDatabaseLocation()
	{
		return GetMainFolder() / "kanji.db";
	}
} // namespace kanji::system
#endif
