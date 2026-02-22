#include "telegram_notification_service.h"
#include <curl/curl.h>
#include <format>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace kanji::notification
{
	TelegramNotificationService::TelegramNotificationService(kanji::config::TelegramSettings in_config)
	    : config{std::move(in_config)}
	{
	}

	void TelegramNotificationService::SendReviewReminder(int pending_review_count)
	{
		const std::string url =
		    std::format("https://api.telegram.org/bot{}/sendMessage", config.bot_token);

		const std::string text = std::format(
		    "🌸 Kanji time! 📖\n"
		    "You have {} card{} waiting for review~\n"
		    "Ganbatte! 💪✨",
		    pending_review_count,
		    pending_review_count == 1 ? "" : "s");

		const nlohmann::json body = {
		    {"chat_id", config.chat_id},
		    {"text", text},
		};
		const std::string payload = body.dump();

		CURL* curl = curl_easy_init();
		if (!curl)
		{
			spdlog::error("TelegramNotificationService: failed to initialize curl");
			return;
		}

		curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(payload.size()));

		// Discard response body
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](char*, size_t size, size_t nmemb, void*) { return size * nmemb; });

		const CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			spdlog::error("TelegramNotificationService: curl error: {}", curl_easy_strerror(res));
		}
		else
		{
			spdlog::info("TelegramNotificationService: reminder sent ({} pending)", pending_review_count);
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}
} // namespace kanji::notification
