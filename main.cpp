#include <iostream>
#include <vector>
#include "Account.h"
#include "TransactionManager.h"
#include "utils.h"

using namespace std;

// From UserInterface.cpp:
extern Account* register_account(vector<Account>& accounts);
extern Account* login(vector<Account>& accounts);
extern void change_password(Account* acc);
extern void update_phone_with_otp(Account* acc, vector<Account>& accounts, bool is_manager = false);

extern void manager_change_user_phone(vector<Account>& accounts);
extern void show_all_users(const vector<Account>& accounts);

//  User Menu
void user_menu(Account* acc, vector<Account>& accounts) {
    int choice;
    do {
        cout << "\n===== USER MENU =====\n";
        cout << "1. View account info\n";
        cout << "2. Change password\n";
        cout << "3. View wallet balance\n";
        cout << "4. Transfer points\n";
        cout << "5. Update phone number\n";
        cout << "6. View transaction history\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system("cls");
                cout << "\nUsername: " << acc->get_username() << endl;
                cout << "Phone Number: " << acc->get_phone() << endl;
                cout << "Role: " << Account::role_to_string(acc->get_role()) << endl;
                cout << "Balance: " << acc->get_balance() << endl;
                break;
            case 2:
                system("cls");
                change_password(acc);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 3:
                system("cls");
                cout << "Wallet balance: " << acc->get_balance() << " points\n";
                break;
            case 4:
                system("cls");
                transfer_points(accounts, acc);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 5:
                system("cls");
                update_phone_with_otp(acc, accounts);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 6:
                system("cls");
                show_transaction_history(acc->get_username());
                break;
            case 0:
                cout << "Good bye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// Manager Menu
void manager_menu(Account* manager, vector<Account>& accounts) {
    system("cls");
    int choice;
    do {
        cout << "\n===== MANAGEMENT MENU =====\n";
        cout << "1. View account list\n";
        cout << "2. Create new account\n";
        cout << "3. Change password\n";
        cout << "4. Transfer points\n";
        cout << "5. View transaction history\n";
        cout << "6. Update phone number\n";
        cout << "7. View all users\n";
        cout << "8. Update user phone number\n"; 
        cout << "0. Exit\n";
        cout << "Select: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("cls");
                cout << "\n--- List of users ---\n";
                for (const auto& acc : accounts) {
                cout << "- " << acc.get_username()
                    << " | Phone: " << acc.get_phone()
                    << " | Role: " << Account::role_to_string(acc.get_role())
                    << " | Balance: " << acc.get_balance() << "\n";
                }
                break;
            }
            case 2:
                system("cls");
                register_account(accounts);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 3:
                system("cls");
                change_password(manager);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 4:
                system("cls");
                transfer_points(accounts, manager);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 5:
                system("cls");
                cout << "\n--- View transaction history ---\n";
                cout << "1. View your transaction history\n";
                cout << "2. View all users' transaction history\n";
                cout << "Choose: ";
                int sub_choice;
                cin >> sub_choice;
                if (sub_choice == 1) {
                    show_transaction_history(manager->get_username());
                } else if (sub_choice == 2) {
                    show_all_transaction_history();
                } else {
                    cout << "Invalid choice.\n";
                }
                break;
            case 6:
                system("cls");
                update_phone_with_otp(manager, accounts, true);
                utils::save_accounts_to_file(accounts, "users.txt"); 
                break;
            case 7:
                system("cls");
                show_all_users(accounts);
                break;
            case 8:
                system("cls");
                manager_change_user_phone(accounts);
                utils::save_accounts_to_file(accounts, "users.txt");
                break;
            case 0:
                cout << "Good bye!\n";
                break;
            default:
                cout << "Choice invalid.\n";
        }
    } while (choice != 0);
}

int main() {
    vector<Account> accounts = utils::load_accounts_from_file("users.txt");

    int choice;
    do {
        cout << "\n===== REWARD POINT WALLET SYSTEM =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Account* new_account = register_account(accounts);
                if (new_account != nullptr) {
                    system("cls");
                    if (new_account->is_temp_password()) {
                        cout << "You are using a temporary password. Please change your password now!\n";
                        change_password(new_account);
                    }
                    utils::save_accounts_to_file(accounts, "users.txt");
                    if (new_account->get_role() == Role::MANAGER)
                        manager_menu(new_account, accounts);
                    else
                        user_menu(new_account, accounts);
                }
                break;
            }
            case 2: {
                system("cls");
                Account* current = login(accounts);
                if (current != nullptr) {
                    if (current->get_role() == Role::MANAGER)
                        manager_menu(current, accounts);
                    else
                        user_menu(current, accounts);
                }
                break;
            }
            case 0:
                cout << "See you agian!\n";
                break;
            default:
                cout << "Choice invalid.\n";
        }
    } while (choice != 0);

    return 0;
}