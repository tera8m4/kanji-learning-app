#pragma once

#include "config.h"
#include <string>

namespace kanji::auth
{
	class AuthService
	{
	public:
		explicit AuthService(const kanji::config::AuthSettings& in_auth_settings);

		std::string GenerateToken(std::string_view telegram_id) const;
		std::string ValidateToken(std::string_view token) const;

	private:
		kanji::config::AuthSettings auth_settings;
	};
} // namespace kanji::auth
