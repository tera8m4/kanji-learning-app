#pragma once

#include <filesystem>

namespace kanji::system
{
	class PlatformInfo
	{
	public:
		struct ScreenInfo
		{
			int width = 0;
			int height = 0;
		};

		static ScreenInfo GetScreenInfo();
		static void SetWallpaper(const std::filesystem::path& in_path);
		static std::filesystem::path GetDatabaseLocation();
		static std::filesystem::path GetWallpaperLocation();
	};
} // namespace kanji::system
