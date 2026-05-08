#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace kanji::config
{
	struct TelegramSettings
	{
		std::string bot_token;
		int chat_id{};
	};

	struct NotificationSettings
	{
		TelegramSettings telegram;

		int refresh_interval{30};
	};

	struct PasswordAccount
	{
		std::string username;
		std::string password_hash; // hex-encoded SHA-256 of the password
	};

	struct AuthSettings
	{
		std::string jwt_secret;
		int token_expiry_hours{24};
		PasswordAccount password_account;
	};

	struct KanjiAppConfig
	{
		NotificationSettings notification;
		AuthSettings auth;

		static KanjiAppConfig LoadFromFile(const std::filesystem::path& path);
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TelegramSettings, bot_token, chat_id)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NotificationSettings, telegram, refresh_interval)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PasswordAccount, username, password_hash)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthSettings, jwt_secret, token_expiry_hours, password_account)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(KanjiAppConfig, notification, auth)

} // namespace kanji::config
