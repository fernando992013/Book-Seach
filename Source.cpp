#include <vector>
#include <string>

#include "Login.h"
#include "Books.h"
#include "MenuSystem.h"
#include "adminMenu.h"
#include "sqlite3.h"


#define ISDEV 0 // Skips login prompt if set to 1 -> 0 for production

// A Menu option is a class that stores the name of a menu option and the function that should be called when the option is selected
void searchBookHandler()
{
    system("cls");

    Menu searchMenu("How would you like to search: \n");
    searchMenu.addOption("Title", 0);
    searchMenu.addOption("Author", 1);
    searchMenu.addOption("ISBN", 2);
    searchMenu.addOption("Back to main menu", 3);

    string query;
    int searchChoice;

    while (true)
    {
        system("cls");

        searchMenu.display();
        searchMenu.getChoice(searchChoice, "Enter your search query: ");

        switch (searchChoice)
        {
        case 0:
            std::cout << "Enter the title of the book: ";
            saferCin(query);
            break;
        case 1:
            std::cout << "Enter the author of the book: ";
            saferCin(query);
            break;
        case 2:
            std::cout << "Enter the ISBN of the book: ";
            saferCin(query);
            break;

        case 3:
            return;

        default:
            break;
        }

        searchBooks(query, searchChoice);
    }
}

void listHandler()
{
    system("cls");

    Menu listMenu("What would you like to do: \n");
    listMenu.addOption("Add a book to the list", 0);
    listMenu.addOption("Remove a book from the list", 1);
    listMenu.addOption("Display the list", 2);
    listMenu.addOption("Back to main menu", 3);

    while (true)
    {
        system("cls");

        listMenu.display();
        int listChoice;
        listMenu.getChoice(listChoice);

        switch (listChoice)
        {
        case 0:
            addToList();
            break;

        case 1:
            removeFromList();
            break;

        case 2:
            displayList();
            break;

        case 3:
            return;

        default:
            break;
        }
    }
}

void addBookHandler()
{
    system("cls");

    std::string bookTitle, bookAuthor, bookISBN, bookPublisher, bookDescription;
    int bookYear;

    std::cout << "Enter the title of the book: ";
    saferCin(bookTitle);

    std::cout << "Enter the author of the book: ";
    saferCin(bookAuthor);

    std::cout << "Enter the publisher of the book: ";
    saferCin(bookPublisher);

    std::cout << "Enter the year the book was published: ";
    saferCin(bookYear);

    int loopAmount = 0;
    while (bookISBN.length() != 13)
    {
        if (loopAmount > 0)
        {
            std::cout << "ISBN was not !-13-! characters.\nPlease try again: ";
        }
        else
        {
            std::cout << "Enter the !-13-! char ISBN of the book: ";
        }

        saferCin(bookISBN);
    }

    addBook(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher);
}

int authUserHandler()
{
    system("cls");

    string username;
    string password;
    string tryAgain;

    std::cout << "Enter your username: ";
    saferCin(username);

    std::cout << "Enter your password: ";
    saferCin(password);

    while (true)
    {
        if (userLogin(username, password))
        {
            break;
        }
        else
        {
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            saferCin(tryAgain);

            if (tryAgain == "y")
            {
                std::cout << "Enter your username: " << std::endl;
                saferCin(username);

                std::cout << "Enter your password: " << std::endl;
                saferCin(password);
            }
            else
            {
                exit(0);
                return 0;
            }
        }
    }

    // Has to succeed, so we can always return 1
    return 1;
}

void registrationHandle() {
    string username;
    string password;

    std::cout << "Enter a new username: ";
    saferCin(username);
    std::cout << "Enter a new password: ";
    saferCin(password);

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the INSERT statement
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO customerLogin (Username, Password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the parameters to the statement
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    // Execute the statement
    rc = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    cout << "Registration complete!\n";
}
int adminLoginHandler() 
{
    system("cls");
    
    string adminUsername;
    string adminPassword;
    string admintryAgain;

    std::cout << "Enter admin username: ";
    saferCin(adminUsername);
    std::cout << "Enter admin password: ";
    saferCin(adminPassword);

    while (true)
    {
        if (adminLogin(adminUsername, adminPassword))
        {
            break;
        }
        else
        {
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            saferCin(admintryAgain);

            if (admintryAgain == "y")
            {
                std::cout << "Enter your admin username: ";
                saferCin(adminUsername);

                std::cout << "Enter your admin password: ";
                saferCin(adminPassword);
            }
            else
            {
                exit(0);
                return 0;
            }
        }
    }
    return 1;
}
int main()
{
    Menu mainMenu("What would you like to do: \n");
    mainMenu.addOption("Search for a book", 0);
    mainMenu.addOption("Add a book to the database", 1);
    mainMenu.addOption("Create a list", 2);
    mainMenu.addOption("Exit", 3);

    int userChoice = 0;

    // Welcome message
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // ISDEV is defined at the top
    if (!ISDEV)
    {
        int loginChoice;

        cout << "1. Register\n";
        cout << "2. Customer Login\n";
        cout << "3. Admin Login\n"; //Admin login not working yet. 
        cin >> loginChoice;

        switch (loginChoice)
        {
        case 1:
            registrationHandle();
             userChoice = 1;
            break;
        case 2:
            authUserHandler();
             userChoice = 2;
            break;
        case 3:
            adminLoginHandler();
             userChoice = 3;
            break;
        }

        // Can only get here if the user is authenticated
        // So everything after is auth safe
        std::cout << "Reading books from file. Please wait... \n"
            << std::endl;
        readBooks();

        // Main Loop, switch statement is switching on return value of the menu options
        if (userChoice == 1 | userChoice == 2) {
            while (true)
            {
                system("cls");

                int choice;
                mainMenu.display();
                mainMenu.getChoice(choice);

                switch (choice)
                {
                case 0:
                    searchBookHandler();
                    break;
                case 1:
                    addBookHandler();
                    break;
                case 2:
                    listHandler();
                    break;
                case 3:
                    exit(0);
                    break;
                }
            }
        }
        else if (userChoice == 3){
            int adminChoice;
            cout << "Welcome to the Admin Menu!" << endl;
            cout << "Enter your choice" << endl;
            cout << "1. Add a new user." << endl;
            cout << "2. Change a user's password" << endl;
            cin >> adminChoice;
            
            switch (adminChoice)
            {
            case 1:
                adminAddUser();
                break;
            case 2:
                adminChangePassword();
                break;
            }

        }
    }
}
