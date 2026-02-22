#pragma once

#include "notification_service.h"
#include <chrono>
#include <memory>
#include <thread>

namespace kanji::database
{
	class DatabaseContext;
}

namespace kanji::notification
{
	class ReviewNotifier
	{
	public:
		ReviewNotifier(database::DatabaseContext& in_db,
		               std::unique_ptr<INotificationService> in_notification_service,
		               std::chrono::minutes in_check_interval = std::chrono::minutes{30});

		void Start();

	private:
		void Run(std::stop_token stop_token);

		database::DatabaseContext& db;
		std::unique_ptr<INotificationService> notification_service;
		std::chrono::minutes check_interval;

		std::jthread worker;
	};
} // namespace kanji::notification
