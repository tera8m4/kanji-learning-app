#include "database_context.h"

namespace kanji::database
{
	DatabaseContext::DatabaseContext(std::filesystem::path in_db_path)
	    : connection{std::move(in_db_path)}
	    , kanji_repo{connection}
	    , review_repo{connection}
	{
		connection.Initialize();
	}

	KanjiRepository& DatabaseContext::GetKanjiRepository()
	{
		return kanji_repo;
	}

	ReviewStateRepository& DatabaseContext::GetReviewStateRepository()
	{
		return review_repo;
	}
} // namespace kanji::database
