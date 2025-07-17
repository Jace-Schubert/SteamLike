#include <iostream>
#include <string>
#include "sqlite3.h"

int QueryCallback(void* notUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << azColName[i] << " = " << argv[i];
		if (i + 1 != argc)
			std::cout << " | ";
	}
	std::cout << "\n";
	return 0;
}

int main()
{
	sqlite3* db;
	int rc = sqlite3_open("src/temp.db", &db);
	if (rc != SQLITE_OK)
	{
		std::cout << "Fail\n";
		sqlite3_close(db);
		return 0;
	}

	std::string query, line;

	while (true)
	{
		query.clear();
		std::cout << "Enter Query: ";
		while (true)
		{
			std::getline(std::cin, line);
			query += line;
			if (query != "" && query.back() == ';')
			{
				break;
			}

			query += " ";
			std::cout << "  -> ";
		} 

		char* errMsg;
		rc = sqlite3_exec(db, query.c_str(), QueryCallback, nullptr, &errMsg);

		if (rc != SQLITE_OK)
		{
			std::cout << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		std::cout << "\n";
	}
	sqlite3_close(db);
	return 0;
}