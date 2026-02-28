#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace kanji::auth
{
	struct TelegramAuthData
	{
		int id;
		std::string first_name;
		std::string last_name; // optional
		std::string username;
		std::string photo_url; // optional
		std::int64_t auth_date;
		std::string hash;

		std::string GetDataCheckString() const;
	};

	bool VerifyTelegramAuth(const TelegramAuthData& data, const std::string& bot_token);

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TelegramAuthData, id, first_name, last_name, username, photo_url, auth_date, hash)
} // namespace kanji::auth
