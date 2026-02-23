#pragma once

#include <cstdint>
#include <string>

namespace kanji::auth
{
	struct TelegramAuthData
	{
		std::string id;
		std::string first_name;
		std::string username;
		std::int64_t auth_date;
		std::string hash;

		std::string GetDataCheckString() const;
	};

	bool VerifyTelegramAuth(const TelegramAuthData& data, const std::string& bot_token);

} // namespace kanji::auth
