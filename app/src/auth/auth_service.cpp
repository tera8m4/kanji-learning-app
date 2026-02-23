#include "auth_service.h"
#include <jwt-cpp/traits/nlohmann-json/defaults.h>

namespace kanji::auth
{

	AuthService::AuthService(const kanji::config::AuthSettings& in_auth_settings)
	    : auth_settings{in_auth_settings}
	{
	}

	std::string AuthService::GenerateToken(std::string_view telegram_id) const
	{
		using namespace std::chrono;
		const auto now = system_clock::now();
		const auto exp = now + hours{auth_settings.token_expiry_hours};

		return jwt::create()
		    .set_subject(std::string{telegram_id})
		    .set_issued_at(now)
		    .set_expires_at(exp)
		    .sign(jwt::algorithm::hs256{auth_settings.jwt_secret});
	}

	std::string AuthService::ValidateToken(std::string_view token) const
	{
		const auto verifier = jwt::verify()
		                          .allow_algorithm(jwt::algorithm::hs256{auth_settings.jwt_secret})
		                          .with_claim("sub", jwt::claim(std::string{}));

		auto decoded = jwt::decode(std::string{token});
		verifier.verify(decoded);
		return decoded.get_subject();
	}

} // namespace kanji::auth
