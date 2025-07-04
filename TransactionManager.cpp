#include <iostream>
#include <fstream>
#include <vector>
#include "utils.h"
#include "Account.h"
#include "Transaction.h"

using namespace std;


void log_transaction(const Transaction& tx) {
    ofstream fout("transaction_log.txt", ios::app);
    fout << tx.serialize() << "\n";
}

void transfer_points(vector<Account>& accounts, Account* sender) {
    cout << "\n--- Point transfer transaction ---\n";
    
    bool retry = true;
    while (retry) {
        string receiver_name;
        double amount;
        
        cout << "Receiver name: ";
        cin >> receiver_name;

        if (receiver_name == sender->get_username()) {
            cout << "Cannot transfer to self.\n";
            return;
        }

        Account* receiver = nullptr;
        for (auto& acc : accounts) {
            if (acc.get_username() == receiver_name) {
                receiver = &acc;
                break;
            }
        }

        if (!receiver) {
            cout << "Recipient does not exist.\n";
            return;
        }

        // Step 2: Get amount
        cout << "Amount of points to transfer: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid number of points.\n";
            return;
        }

        if (sender->get_balance() < amount) {
            cout << "Transaction failed: insufficient balance.\n";
            Transaction tx{sender->get_username(), receiver_name, amount,
                           Transaction::get_current_time(), "FAILED"};
            log_transaction(tx);
            return;
        }

        // OTP verification
        cout << "The system requires OTP confirmation to perform the transaction.\n";
        string otp = utils::generate_otp();
        cout << "[OTP] Your code: " << otp << "\n";
        
        utils::OTPResult result = utils::verify_otp_with_timeout(otp);
        
        switch (result) {
            case utils::OTP_SUCCESS:
                retry = false;
                break;
                
            case utils::OTP_TIMEOUT:
                cout << "\n1. Retry transaction\n";
                cout << "2. Cancel transaction\n";
                cout << "Choose: ";
                int choice;
                cin >> choice;
                
                if (choice == 1) {
                    // Tiếp tục vòng lặp
                } else {
                    retry = false;
                }
                break;
                
            case utils::OTP_FAIL:
                cout << "\nInvalid OTP. Transaction canceled.\n";
                retry = false;
                break;
        }
    }
}

void show_transaction_history(const string& username) {
    ifstream fin("transaction_log.txt");
    string line;
    cout << "\n--- Transaction history of " << username << " ---\n";

    bool found = false;
    while (getline(fin, line)) {
        Transaction tx = Transaction::deserialize(line);
        if (tx.from_username == username || tx.to_username == username) {
            cout << "- " << tx.timestamp << ": "
                 << tx.from_username << " -> " << tx.to_username
                 << " | " << tx.amount << " points"
                 << " | " << tx.status << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "(No transactions)\n";
}

void manager_update_user_info(vector<Account>& accounts) {
    cout << "\n--- Manage user information updates ---\n";
    string uname;
    cout << "Enter the account name to update: ";
    cin >> uname;

    Account* target = nullptr;
    for (auto& acc : accounts) {
        if (acc.get_username() == uname) {
            target = &acc;
            break;
        }
    }

    if (!target) {
        cout << "Account does not exist.\n";
        return;
    }

    string new_phone;
    cout << "New phone number: ";
    cin >> new_phone;

    cout << "[OTP] Send confirmation code to user (" << target->get_username() << ")\n";
    string otp = utils::generate_otp();
    cout << "[OTP] OTP code: " << otp << "\n";

    utils::OTPResult result = utils::verify_otp_with_timeout(otp);

    switch (result) {
        case utils::OTP_SUCCESS:
            target->set_phone(new_phone);
            utils::save_accounts_to_file(accounts, "users.txt");
            cout << "Account phone number updated: " << uname << "\n";
            break;
            
        case utils::OTP_TIMEOUT:
            cout << "\nOTP expired. Update canceled.\n";
            break;
            
        case utils::OTP_FAIL:
            cout << "\nInvalid OTP. Update canceled.\n";
            break;
    }

    string input;
    cout << "User enters OTP to confirm: ";
    cin >> input;

    if (input != otp) {
        cout << "Invalid OTP. Not updated.\n";
        return;
    }

    target->set_phone(new_phone);
    utils::save_accounts_to_file(accounts, "users.txt");

    cout << "Account phone number updated: " << uname << "\n";
}

void manager_view_user_history() {
    cout << "\n--- View user's transaction history ---\n";
    string uname;
    cout << "Enter username: ";
    cin >> uname;
    show_transaction_history(uname);
}
