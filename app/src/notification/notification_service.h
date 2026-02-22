#pragma once

#include <string>

namespace kanji::notification
{
	class INotificationService
	{
	public:
		virtual void SendReviewReminder(int pending_review_count) = 0;
		virtual ~INotificationService() = default;
	};
} // namespace kanji::notification
