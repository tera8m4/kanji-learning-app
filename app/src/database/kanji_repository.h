#pragma once

#include "kanji.h"

namespace kanji::database
{
	class SQLiteConnection;

	class KanjiRepository
	{
	public:
		explicit KanjiRepository(const SQLiteConnection& in_connection);

		KanjiRepository(const KanjiRepository&) = delete;
		KanjiRepository& operator=(const KanjiRepository&) = delete;

		std::vector<KanjiData> GetKanjiForReview() const;

	private:
		const SQLiteConnection& connection;

		std::vector<KanjiWord> GetKanjiWords(std::uint32_t kanji_id) const;
	};
} // namespace kanji::database
