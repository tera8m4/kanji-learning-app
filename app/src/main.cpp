#include "app.h"
#include "config.h"
#include <filesystem>
#include <spdlog/spdlog.h>

int main()
{
	const std::filesystem::path config_path{"config.json"};
	if (!std::filesystem::exists(config_path))
	{
		spdlog::warn("No config.json found — notifications disabled");
		return 1;
	}

	const std::filesystem::path fonts_dir{"assets/fonts"};
	if (!std::filesystem::exists(fonts_dir) || std::filesystem::is_empty(fonts_dir))
	{
		spdlog::warn("No fonts found in {} — UI will fall back to system fonts", fonts_dir.string());
	}

	const auto config = kanji::config::KanjiAppConfig::LoadFromFile(config_path);
	kanji::KanjiApp{config}.Run();

	return 0;
}
