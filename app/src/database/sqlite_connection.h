#pragma once

#include <filesystem>

struct sqlite3;

namespace kanji::database
{
	class SQLiteConnection
	{
	public:
		explicit SQLiteConnection(std::filesystem::path in_db_path);
		SQLiteConnection(const SQLiteConnection&) = delete;
		SQLiteConnection& operator=(const SQLiteConnection&) = delete;
		~SQLiteConnection();

		bool Initialize();
		sqlite3* GetDB() const;
		operator sqlite3*() const;

	private:
		sqlite3* db;
		std::filesystem::path db_path;
	};

} // namespace kanji::database
