#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "User.h"
#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <nanodbc/nanodbc.h>
#include <odbcss.h>
//#pragma comment (lib, "nanodbc.lib")
#include "User.h"
using namespace std;


class DAL {
private:
    nanodbc::connection conn;

    void disconnect() {
        if (conn.connected()) {
            conn.disconnect();
            conn.deallocate();
        }
    }
public:
    DAL(const string connectionString) : conn(connectionString) {}
    ~DAL() {}
    bool isTableEmpty() {
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
        SELECT COUNT(*) FROM users;
    )");

        nanodbc::result result = stmt.execute();
        int count = result.get<int>(0);

        return count == 0;
    }
    bool verifyCredentials(const string& username, const string& password) {
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            SELECT COUNT(*) FROM users WHERE username = ? AND password = ?;
        )");

        stmt.bind(0, username.c_str());
        stmt.bind(1, password.c_str());

        nanodbc::result result = stmt.execute();
        int count = result.get<int>(0);

        return count > 0;
    }
    int getLastUserId() {
        try {
            nanodbc::statement stmt(conn);
            nanodbc::prepare(stmt, R"(SELECT MAX(id) FROM users;)");
            nanodbc::result result = stmt.execute();
            if (result.next()) {
                return result.get<int>(0);
            }
        }
        catch (...) {
            return 0;
        }

    }
    bool createUser(const User& user) {
        try {
            // Prepare the SQL statement to insert a new user record
            nanodbc::statement stmt(conn);
            nanodbc::prepare(stmt, R"(
                INSERT INTO users (username, password) VALUES (?, ?);
            )");
            //nanodbc::result res
            // Bind the parameters to the statement
            stmt.bind(0, user.username.c_str());
            stmt.bind(1, user.password.c_str());
            // Execute the statement and check if any rows were affected
            stmt.execute();
            return true;
        }
        catch (const std::exception& e) {
            // Handle any errors that occur during the insert
            std::cerr << "Error inserting user: " << e.what() << std::endl;
            return false;
        }
    }
    User getUserById(int id) {
        User user;
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
        SELECT id, username, password FROM users WHERE id = ?;
    )");
        stmt.bind(0, &id);

        nanodbc::result result = stmt.execute();

        const short columns = result.columns();


        if (result.next()) {
            user.id = result.get<int>(0);
            user.username = result.get<std::string>(1);
            user.password = result.get<std::string>(2);
        }

        return user;
    }
    bool updateUser(const User& user) {
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            UPDATE users SET username = ?, password = ? WHERE id = ?;
        )");

        stmt.bind(0, user.username.c_str());
        stmt.bind(1, user.password.c_str());
        stmt.bind(2, &user.id);
        nanodbc::result rez = stmt.execute();

        return rez.has_affected_rows();
    }
    bool deleteUser(int id) {
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            DELETE FROM users WHERE id = ?;
        )");

        stmt.bind(0, &id);

        nanodbc::result rez = stmt.execute();

        return rez.has_affected_rows();
    }
    vector<User> getUsersByPassword(const std::string& password) {
        std::vector<User> users;
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            SELECT id, username, password FROM users WHERE password LIKE ?;
        )");

        std::string email_pattern = "%" + password + "%";
        stmt.bind(0, email_pattern.c_str());

        nanodbc::result result = stmt.execute();

        while (result.next()) {
            User user;
            user.id = result.get<int>(0);
            user.username = result.get<std::string>(1);
            user.password = result.get<std::string>(2);
            users.push_back(user);
        }

        return users;
    }
    bool getUserByUsername(const string& username, const string& password) {
        vector<User> user;
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            SELECT id, username, password FROM users WHERE username LIKE ?;
        )");

        std::string username_pattern = "%" + username + "%";
        stmt.bind(0, username_pattern.c_str());

        nanodbc::result result = stmt.execute();
        if (result.next() && result.get<string>(2) == password) {
            User usr;
            usr.id = result.get<int>(0);
            usr.username = result.get<std::string>(1);
            usr.password = result.get<std::string>(2);
            user.push_back(usr);
            return true;
        }
        return false;
    }

};
