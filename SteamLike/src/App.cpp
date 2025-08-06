#include <iostream>
#include <string>
#include "sqlite3.h"

void ViewGamesByCategory(sqlite3* db) {
	std::string genre;
	std::cout << "Enter genre name: ";
	std::getline(std::cin, genre);

	std::string sql = "SELECT Game.title, Genre.name, Game.price FROM Game "
		"JOIN Genre ON Game.genre_id = Genre.id "
		"WHERE Genre.name = ?;";

	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
		sqlite3_bind_text(stmt, 1, genre.c_str(), -1, SQLITE_STATIC);
		std::cout << "Games in genre '" << genre << "':\n";
		while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
			std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			int price = sqlite3_column_int(stmt, 2);
			std::cout << "- " << title << " ($" << price << ")\n";
		}
	}
	sqlite3_finalize(stmt);
}

void ViewGamesByPriceRange(sqlite3* db) {
	int min, max;
	std::cout << "Enter min price: ";
	std::cin >> min;
	std::cout << "Enter max price: ";
	std::cin >> max;
	std::cin.ignore();

	std::string sql = "SELECT title, price FROM Game WHERE price BETWEEN ? AND ?;";
	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
	{
		sqlite3_bind_int(stmt, 1, min);
		sqlite3_bind_int(stmt, 2, max);
		std::cout << "Games priced between $" << min << " and $" << max << ":\n";
		while (sqlite3_step(stmt) == SQLITE_ROW) 
		{
			std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			int price = sqlite3_column_int(stmt, 1);
			std::cout << "- " << title << " ($" << price << ")\n";
		}
	}
	sqlite3_finalize(stmt);
}

void InsertUser(sqlite3* db) 
{
    int id;
    std::string name, email, password;
    std::cout << "Enter user ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::string sql = "INSERT INTO User (id, name, email, password) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            std::cout << "User inserted successfully.\n";
        }
        else 
        {
            std::cout << "Failed to insert user.\n";
        }
    }
    sqlite3_finalize(stmt);
}

void InsertTransaction(sqlite3* db) 
{
    int tid, uid, gid, amount;
    std::string date, method;

    std::cout << "Enter transaction ID: ";
    std::cin >> tid;
    std::cout << "Enter user ID: ";
    std::cin >> uid;
    std::cout << "Enter game ID: ";
    std::cin >> gid;
    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cin.ignore();
    std::cout << "Enter transaction date (YYYY-MM-DD): ";
    std::getline(std::cin, date);
    std::cout << "Enter payment method: ";
    std::getline(std::cin, method);

    std::string sql = "INSERT INTO Transactions (transaction_id, user_id, game_id, amount, transaction_date, payment_method) "
        "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, tid);
        sqlite3_bind_int(stmt, 2, uid);
        sqlite3_bind_int(stmt, 3, gid);
        sqlite3_bind_int(stmt, 4, amount);
        sqlite3_bind_text(stmt, 5, date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, method.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            std::cout << "Transaction inserted successfully.\n";
        }
        else 
        {
            std::cout << "Failed to insert transaction.\n";
        }
    }
    sqlite3_finalize(stmt);
}

void ViewPurchasesByUser(sqlite3* db) 
{
    int uid;
    std::cout << "Enter user ID: ";
    std::cin >> uid;
    std::cin.ignore();

    std::string sql = "SELECT User.name, Game.title, Transactions.amount, Transactions.transaction_date "
        "FROM Transactions "
        "JOIN User ON Transactions.user_id = User.id "
        "JOIN Game ON Transactions.game_id = Game.id "
        "WHERE User.id = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, uid);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string game = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int amount = sqlite3_column_int(stmt, 2);
            std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            std::cout << "User: " << username << " | Game: " << game
                << " | Amount: $" << amount << " | Date: " << date << "\n";
        }
    }
    sqlite3_finalize(stmt);
}

void ShowMenu() 
{
	std::cout << "\n===== Steam Like CLI =====\n"
		<< "1. View games by genre\n"
		<< "2. View games by price range\n"
		<< "3. Insert new user\n"
		<< "4. Insert new transaction\n"
		<< "5. View purchases by user\n"
		<< "0. Exit\n"
		<< "Select an option: ";
}

int main() 
{
	sqlite3* db;
	int rc = sqlite3_open("src/temp.db", &db);
	if (rc != SQLITE_OK)
	{
		std::cout << "Database file was not opened sccessfully";
		sqlite3_close(db);
		return 1;
	}

	int choice;
	do 
	{
		ShowMenu();
		std::cin >> choice;
		std::cin.ignore(); 

		switch (choice) 
		{
			case 1: ViewGamesByCategory(db); break;
			case 2: ViewGamesByPriceRange(db); break;
			case 3: InsertUser(db); break;
			case 4: InsertTransaction(db); break;
			case 5: ViewPurchasesByUser(db); break;
			case 0: std::cout << "Exiting...\n"; break;
			default: std::cout << "Invalid option.\n"; break;
		}
	} while (choice != 0);

	sqlite3_close(db);
	return 0;
}