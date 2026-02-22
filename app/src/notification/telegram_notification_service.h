#pragma once

#include "config.h"
#include "notification_service.h"

namespace kanji::notification
{
	class TelegramNotificationService : public INotificationService
	{
	public:
		explicit TelegramNotificationService(kanji::config::TelegramSettings config);

		void SendReviewReminder(int pending_review_count) override;

	private:
		kanji::config::TelegramSettings config;
	};
} // namespace kanji::notification
