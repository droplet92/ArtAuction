#include "DbManager.h"
using namespace lhs::model;

#include <format>
#include <functional>
#include <string_view>

#include <Utility.h>


namespace lhs::manager
{
#ifdef _DEBUG
	constexpr auto dbPath = R"(../gamedata.db)";
#else
	constexpr auto dbPath = R"(./gamedata.db)";
#endif

	// Callback for read painting data from the db
	static int GetPaintingsCallback(void* data, int, char** value, char** column)
	{
		CCASSERT(!std::strcmp(column[0], "Title"), "column 0 must be 'Title'");
		CCASSERT(!std::strcmp(column[1], "Name"), "column 1 must be 'Name'");

		auto result = reinterpret_cast<std::vector<Painting>*>(data);

		Painting painting{};
		painting.title = value[0];
		painting.painter = value[1];

#ifdef _DEBUG
		painting.path = std::format("../Resources/paintings/{}/{}.jpg", painting.painter, painting.title);
#else
		painting.path = std::format("./Resources/paintings/{}/{}.jpg", painting.painter, painting.title);
#endif
		result->push_back(painting);
		return 0;
	}

	// Callback for read painter data from the db
	static int GetPaintersCallback(void* data, int, char** value, char** column)
	{
		CCASSERT(!std::strcmp(column[0], "Name"), "column 0 must be 'Name'");

		auto result = static_cast<std::vector<std::string>*>(data);
		result->push_back(value[0]);
		return 0;
	}

	DbManager::DbManager()
		: db(nullptr)
		, errorMessage{}
	{
		if (sqlite3_open(dbPath, &db) != SQLITE_OK)
			Utility::Abort("sqlite3_open failed.");
	}

	DbManager::~DbManager()
	{
		if (sqlite3_close(db) != SQLITE_OK)
			Utility::Abort("sqlite3_close failed.");
	}

	DbManager& DbManager::Instance() noexcept
	{
		static DbManager instance{};
		return instance;
	}

	std::string_view DbManager::GetErrorMessage() const noexcept
	{
		return (strlen(errorMessage) > 0)
			? errorMessage			// If sqlite execution failed, then return errorMessage
			: sqlite3_errmsg(db);	// else, return sqlite database error message
	}

	std::vector<Painting> DbManager::GetPaintings()
	{
		constexpr auto sql = R"(SELECT a.Title, b.Name FROM Painting a INNER JOIN Painter b ON a.PainterId = b.Id)";
		std::vector<Painting> result{};

		if (!Execute(sql, GetPaintingsCallback, &result))
			result.clear();

		return result;
	}

	std::vector<std::string> DbManager::GetPainters()
	{
		constexpr auto sql = R"(SELECT Name FROM Painter)";
		std::vector<std::string> result{};

		if (!Execute(sql, GetPaintersCallback, &result))
			result.clear();

		return result;
	}

	bool DbManager::Execute(std::string_view sql, int(*callback)(void*, int, char**, char**), void* data)
	{
		std::memset(errorMessage, 0, sizeof errorMessage);	// zero-fill the errorMessage
		return (sqlite3_exec(db, sql.data(), callback, data, reinterpret_cast<char**>(&errorMessage)) == SQLITE_OK);
	}
}