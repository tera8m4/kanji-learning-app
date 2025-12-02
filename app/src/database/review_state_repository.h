#pragma once

struct kanji::KanjiReviewState;

namespace kanji::database
{
	class SQLiteConnection;

	class ReviewStateRepository
	{
	public:
		void InitializeNewReviewStates(int count);
		void CreateOrUpdateReviewState(const KanjiReviewState& state);

	private:
		const SQLiteConnection& connection;
	};
} // namespace kanji::database
