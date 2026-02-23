#include "telegram_auth.h"
#include "utils/crypto.h"
#include <format>
namespace kanji::auth
{
	std::string TelegramAuthData::GetDataCheckString() const
	{
		return std::format("auth_date={0}\nfirst_name={1}\nid={2}\nusername={3}", auth_date, first_name, id, username);
	}

	bool VerifyTelegramAuth(const TelegramAuthData& data, const std::string& bot_token)
	{
		// Derive secret key: SHA256(bot_token)
		namespace crypto = kanji::utils::crypto;
		auto secret_key = crypto::SHA256(bot_token);

		return crypto::HMAC_SHA256(data.GetDataCheckString(), secret_key).ToLowerCase() == data.hash;
	}
} // namespace kanji::auth
