#pragma once
#include <string>
#include <fstream>
#include "sqlite3.h"

//Still trying to attempt to add this admin menu to the beginning.


//allows admin to add a new user. Basically like the registration form in the first menu. 
void adminAddUser() 
{
    string adminNewUsername;
    string adminNewPassword;

    std::cout << "ADMIN USER CREATION" << endl;
    std::cout << "Enter a new username: ";
    saferCin(adminNewUsername);
    std::cout << "Enter a new password: ";
    saferCin(adminNewPassword);

    ofstream registerFile;
    registerFile.open("login.csv", ios::app); // 'ios::app' is appended mode. Which writes data into a file with already existing data.
    registerFile << adminNewUsername << "," << adminNewPassword;
    registerFile << endl;
    registerFile.close();
}

// adminChangePassword allows the admin to changed the password of a current customer. 
bool adminChangePassword() 
{
    string username;
    string oldPassword;
    string newPassword;

    cout << "ADMIN PASSWORD CHANGER" << endl;
    cout << "Enter the username: " << endl;
    saferCin(username);
    cout << "Enter the user's current password: " << endl;
    saferCin(oldPassword);
    cout << "Enter the new password: " << endl;
    saferCin(newPassword);

    sqlite3* db;
    sqlite3_stmt* stmt;

    //Opens the database.
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    //string sql allows us to use sql code in c++. This specific line will update the password. It will search for the Username and Password.
    string sql = "UPDATE customerLogin SET Password = ? WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    //
    rc = sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 3, oldPassword.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cout << "Password has been changed!" << endl;
    return true;
}

