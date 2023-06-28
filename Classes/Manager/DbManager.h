#include <sqlite3.h>

#include <Model/Painting.h>


namespace lhs::manager
{
	/**
	* The Database manager
	* 
	* It is connected to sqlite3 database.
	* Singleton pattern used.
	* Read-only
	*/
	class DbManager
	{
	public:
		~DbManager();

		static DbManager& Instance() noexcept;

		// Get query failed message or sqlite error message
		std::string_view GetErrorMessage() const noexcept;

		// Get painting data from the database.
		std::vector<model::Painting> GetPaintings();

		// Get painter data from the database.
		std::vector<std::string> GetPainters();

	private:
		DbManager();

		// Execute the given sql.
		bool Execute(std::string_view sql, int(*callback)(void*, int, char**, char**), void* data);

	private:
		sqlite3* db;
		char errorMessage[BUFSIZ];
	};
}