#include "review_state_repository.h"
#include "kanji.h"
#include "scheduler/scheduler.h"
#include "sqlite_connection.h"
#include <spdlog/spdlog.h>
#include <sqlite3.h>
#include <vector>

namespace kanji::database
{
	std::vector<KanjiReviewState> ReviewStateRepository::GetReviewStates(const std::vector<std::uint32_t>& ids)
	{
		std::vector<KanjiReviewState> states;

		if (ids.empty())
		{
			return states;
		}

		// Build the IN clause with placeholders
		std::string placeholders(ids.size() * 2 - 1, ',');
		for (size_t i = 0; i < ids.size(); ++i)
		{
			placeholders[i * 2] = '?';
		}

		std::string select_sql =
		    "SELECT kanji_id, level, next_review_date, created_at FROM kanji_review_state WHERE kanji_id IN (" +
		    placeholders + ");";

		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(connection, select_sql.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			spdlog::error("Failed to prepare statement: {0}", sqlite3_errmsg(connection));
			return states;
		}

		// Bind all the IDs
		for (size_t i = 0; i < ids.size(); ++i)
		{
			sqlite3_bind_int(stmt, static_cast<int>(i + 1), ids[i]);
		}

		// Fetch all matching states
		while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
		{
			KanjiReviewState state;
			state.kanji_id = sqlite3_column_int(stmt, 0);
			state.level = sqlite3_column_int(stmt, 1);
			std::int64_t next_review_timestamp = sqlite3_column_int64(stmt, 2);
			std::int64_t created_timestamp = sqlite3_column_int64(stmt, 3);
			state.next_review_date = std::chrono::system_clock::from_time_t(next_review_timestamp);
			state.created_at = std::chrono::system_clock::from_time_t(created_timestamp);

			states.push_back(state);
		}

		sqlite3_finalize(stmt);

		if (rc != SQLITE_DONE && rc != SQLITE_ROW)
		{
			spdlog::error("Failed to fetch review states: {0}", sqlite3_errmsg(connection));
		}

		return states;
	}

	void ReviewStateRepository::InitializeNewReviewStates(int count)
	{
		// Get next K kanjis that don't have review states
		const char* select_sql =
		    "SELECT id FROM kanjis "
		    "WHERE id NOT IN (SELECT kanji_id FROM kanji_review_state) "
		    "ORDER BY id LIMIT ?;";
		sqlite3_stmt* select_stmt;

		int rc = sqlite3_prepare_v2(connection, select_sql, -1, &select_stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			spdlog::error("Failed to prepare statement: {0}", sqlite3_errmsg(connection));
			return;
		}

		sqlite3_bind_int(select_stmt, 1, count);

		std::vector<std::uint32_t> kanji_ids;
		while ((rc = sqlite3_step(select_stmt)) == SQLITE_ROW)
		{
			kanji_ids.push_back(sqlite3_column_int(select_stmt, 0));
		}
		sqlite3_finalize(select_stmt);

		// Insert new review states with next_review_date = created_at = now()
		std::int64_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const char* insert_sql =
		    "INSERT INTO kanji_review_state (kanji_id, level, incorrect_streak, next_review_date, created_at) "
		    "VALUES (?, 0, 0, ?, ?);";

		for (std::uint32_t kanji_id : kanji_ids)
		{
			sqlite3_stmt* insert_stmt;
			rc = sqlite3_prepare_v2(connection, insert_sql, -1, &insert_stmt, nullptr);
			if (rc != SQLITE_OK)
			{
				spdlog::error("Failed to prepare statement: {0}", sqlite3_errmsg(connection));
				return;
			}

			sqlite3_bind_int(insert_stmt, 1, kanji_id);
			sqlite3_bind_int64(insert_stmt, 2, now);
			sqlite3_bind_int64(insert_stmt, 3, now);

			rc = sqlite3_step(insert_stmt);
			sqlite3_finalize(insert_stmt);

			if (rc != SQLITE_DONE)
			{
				spdlog::error("Failed to insert review state: {0}", sqlite3_errmsg(connection));
				return;
			}
		}
	}

	void ReviewStateRepository::CreateOrUpdateReviewState(const KanjiReviewState& state)
	{
		const char* sql =
		    "INSERT INTO kanji_review_state (kanji_id, level, next_review_date, created_at) "
		    "VALUES (?, ?, ?, unixepoch()) "
		    "ON CONFLICT(kanji_id) DO UPDATE SET "
		    "level = excluded.level, "
		    "next_review_date = excluded.next_review_date;";
		sqlite3_stmt* stmt;

		int rc = sqlite3_prepare_v2(connection, sql, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			spdlog::error("Failed to prepare statement: {0}", sqlite3_errmsg(connection));
			return;
		}

		std::int64_t timestamp = std::chrono::system_clock::to_time_t(state.next_review_date);

		sqlite3_bind_int(stmt, 1, state.kanji_id);
		sqlite3_bind_int(stmt, 2, state.level);
		sqlite3_bind_int64(stmt, 3, timestamp);

		rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		if (rc != SQLITE_DONE)
		{
			spdlog::error("Failed to insert/update review state: {0}", sqlite3_errmsg(connection));
			return;
		}

		return;
	}
} // namespace kanji::database
