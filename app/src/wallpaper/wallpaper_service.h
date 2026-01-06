#pragma once
#include <filesystem>

namespace kanji::database
{
	class ReviewStateRepository;
} // namespace kanji::database

namespace kanji::wallpaper
{
	struct WallpaperSettings
	{
		int width = 0;
		int height = 0;
		int xpadding = 0;
		int ypadding = 0;
		std::filesystem::path save_location;
	};

	class WallpaperService
	{
	public:
		explicit WallpaperService(kanji::database::ReviewStateRepository& in_review_repo);
		void GenerateWallpaper();

	private:
		kanji::database::ReviewStateRepository& review_repo;
	};
} // namespace kanji::wallpaper
