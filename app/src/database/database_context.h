#pragma once

#include "kanji_repository.h"
#include "review_state_repository.h"
#include "sqlite_connection.h"
#include <string>

namespace kanji::database
{
	class DatabaseContext
	{
	public:
		explicit DatabaseContext(std::filesystem::path in_db_path);

		KanjiRepository& GetKanjiRepository();
		ReviewStateRepository& GetReviewStateRepository();

	private:
		SQLiteConnection connection;
		KanjiRepository kanji_repo;
		ReviewStateRepository review_repo;
	};
} // namespace kanji::database
