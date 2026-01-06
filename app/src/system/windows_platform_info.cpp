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
	PlatformInfo::ScreenInfo PlatformInfo::GetScreenInfo()
	{

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		return {width, height};
	}

	void PlatformInfo::SetWallpaper(const std::filesystem::path& in_path)
	{
		wchar_t* path = const_cast<wchar_t*>(in_path.c_str());
		SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	}

	std::filesystem::path PlatformInfo::GetDatabaseLocation()
	{
		return GetMainFolder() / "kanji.db";
	}

	std::filesystem::path PlatformInfo::GetWallpaperLocation()
	{
		auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
		std::string wallpaper_name = std::format("{:%Y-%m-%d}.png", today);
		return GetMainFolder() / "wallpapers" / wallpaper_name;
	}
} // namespace kanji::system
#endif
