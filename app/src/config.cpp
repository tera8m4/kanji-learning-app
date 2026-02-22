#include "config.h"
#include <fstream>
#include <stdexcept>

namespace kanji::config
{
	KanjiAppConfig KanjiAppConfig::LoadFromFile(const std::filesystem::path& path)
	{
		std::ifstream file{path};
		if (!file.is_open())
		{
			throw std::runtime_error{"KanjiAppConfig: cannot open config file: " + path.string()};
		}

		nlohmann::json j;
		file >> j;
		return j.get<KanjiAppConfig>();
	}
} // namespace kanji::config
