#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace kanji
{
	struct KanjiReviewState;
	struct KanjiAnswer;
} // namespace kanji

namespace kanji::database
{
	class SQLiteConnection;

	class ReviewStateRepository
	{
	public:
		explicit ReviewStateRepository(const SQLiteConnection& in_connection)
		    : connection{in_connection}
		{}

		std::vector<KanjiReviewState> GetReviewStates(const std::vector<std::uint32_t>& ids);
		std::unordered_map<char32_t, int> GetAllReviewLevels();
		void InitializeNewReviewStates(int count);
		void CreateOrUpdateReviewState(const KanjiReviewState& state);

	private:
		const SQLiteConnection& connection;
	};
} // namespace kanji::database
