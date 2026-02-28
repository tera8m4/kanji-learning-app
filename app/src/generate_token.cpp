#include "config.h"
#include <filesystem>
#include <format>
#include <iostream>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>

int main()
{
	const std::filesystem::path config_path{"config.json"};
	if (!std::filesystem::exists(config_path))
	{
		std::cout << "config.json not found" << std::endl;
		return 1;
	}

	const auto config = kanji::config::KanjiAppConfig::LoadFromFile(config_path);

	using namespace std::chrono;
	const auto now = system_clock::now();
	const auto exp = now + hours{config.auth.token_expiry_hours};

	auto token = jwt::create()
	                 .set_subject(std::to_string(config.notification.telegram.chat_id))
	                 .set_issued_at(now)
	                 .set_expires_at(exp)
	                 .sign(jwt::algorithm::hs256{config.auth.jwt_secret});

	std::cout << token << std::endl;
	return 0;
}
