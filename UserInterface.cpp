#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.h"
#include "Account.h"

using namespace std;



void send_otp_to_user(const string& username, string& generated_otp) {
    generated_otp = utils::generate_otp();
    cout << "[OTP] Your OTP code is: " << generated_otp << " (one-time use, valid for 60 seconds)\n";
}


Account* find_account(vector<Account>& accounts, const string& username) {
    for (auto& acc : accounts) {
        if (acc.get_username() == username)
            return &acc;
    }
    return nullptr;
}

Account* register_account(vector<Account>& accounts) {
    system("cls");
    string uname, phone, password, confirm_pw;
    cout << "\n--- Register account ---\n";
    cout << "Username: ";
    cin >> uname;

    if (find_account(accounts, uname)) {
        cout << "Username already exists!\n";
        return nullptr;
    }

    cout << "Phone number: ";
    cin >> phone;

    cout << "Password (leave blank for automatic generation): ";
    cin.ignore();
    getline(cin, password);

    bool is_temp = false;
    if (password.empty()) {
        password = utils::generate_random_password();
        is_temp = true;
        cout << "Your temporary password is: " << password << "\n";
    } else {
        cout << "Confirm password: ";
        getline(cin, confirm_pw);
            
        if (password != confirm_pw) {
            cout << "Passwords do not match. Please try again.\n";
        }
    }

    string hashed = utils::sha256(password);

    Role role = Role::USER;
    // char is_manager;
    // cout << "Are you the manager (Y/N)? ";
    // cin >> is_manager;
    // if (tolower(is_manager) == 'y') role = Role::MANAGER;

    accounts.emplace_back(uname, phone, hashed, role, 0.0, is_temp);
    cout << "Account created successfully.\n";

    utils::save_accounts_to_file(accounts, "users.txt");
    return find_account(accounts, uname);
}

utils::OTPResult confirm_otp(const string& generated_otp) {
    return utils::verify_otp_with_timeout(generated_otp);
}

Account* login(vector<Account>& accounts) {
    string uname, password;
    cout << "\n--- Login ---\n";
    cout << "Username: ";
    cin >> uname;
    cout << "Password: ";
    cin >> password;

    Account* acc = find_account(accounts, uname);
    if (!acc) {
        cout << "Account not found.\n";
        return nullptr;
    }

    if (utils::sha256(password) != acc->get_password_hash()) {
        cout << "Wrong password.\n";
        return nullptr;
    }

    if (acc->is_temp_password()) {
        cout << "\nYou are using a temporary password. Please change your password!\n";
        string otp;
        bool retry = true;
        
        while (retry) {
            send_otp_to_user(uname, otp);
            utils::OTPResult result = confirm_otp(otp);
            
            switch (result) {
                case utils::OTP_SUCCESS: {
                    string newpw, confirm_pw;
                    bool passwords_match = false;
                    
                    do {
                        cout << "Enter new password: ";
                        cin >> newpw;
                        cout << "Confirm new password: ";
                        cin >> confirm_pw;
                        
                        if (newpw != confirm_pw) {
                            cout << "Passwords do not match. Please try again.\n";
                        } else {
                            passwords_match = true;
                        }
                    } while (!passwords_match);
                    
                    acc->set_password_hash(utils::sha256(newpw), false);
                    utils::save_accounts_to_file(accounts, "users.txt");
                    cout << "Password changed successfully.\n";
                    retry = false;
                    break;
                }
                    
                case utils::OTP_TIMEOUT:
                    cout << "\n1. Resend OTP\n";
                    cout << "2. Cancel\n";
                    cout << "Choose: ";
                    int choice;
                    cin >> choice;
                    
                    if (choice != 1) {
                        cout << "OTP verification failed. Login failed.\n";
                        return nullptr;
                    }
                    break;
                    
                case utils::OTP_FAIL:
                    cout << "Invalid OTP. Login failed.\n";
                    return nullptr;
            }
        }
    }

    cout << "Login successful.\n";
    return acc;
}


// Change password functionality for users
void change_password(Account* acc) {
    cout << "\n--- Change password ---\n";
    string otp;
    bool retry = true;
    
    while (retry) {
        send_otp_to_user(acc->get_username(), otp);
        utils::OTPResult result = confirm_otp(otp);
        
        switch (result) {
            case utils::OTP_SUCCESS: {
                string newpw, confirm_pw;
                bool passwords_match = false;
                
                do {
                    cout << "Enter new password: ";
                    cin >> newpw;
                    cout << "Confirm new password: ";
                    cin >> confirm_pw;
                    
                    if (newpw != confirm_pw) {
                        cout << "Passwords do not match. Please try again.\n";
                    } else {
                        passwords_match = true;
                    }
                } while (!passwords_match);
                
                acc->set_password_hash(utils::sha256(newpw), false);
                cout << "Password changed successfully.\n";
                retry = false;
                break;
            }
                
            case utils::OTP_TIMEOUT:
                cout << "1. Resend OTP\n";
                cout << "2. Cancel\n";
                cout << "Choose: ";
                int choice;
                cin >> choice;
                
                if (choice != 1) {
                    return;
                }
                break;
                
            case utils::OTP_FAIL:
                cout << "Invalid OTP. Operation canceled.\n";
                return;
        }
    }
}

void update_phone_with_otp(Account* acc, vector<Account>& accounts, bool is_manager) {
    string title = is_manager ? 
        "--- Update User Phone Number ---" : 
        "--- Update Your Phone Number ---";
    
    cout << "\n" << title << "\n";
    
    if (is_manager) {
        cout << "User: " << acc->get_username() << "\n";
        cout << "Current phone: " << acc->get_phone() << "\n";
    }

    string new_phone;
    cout << "New phone number: ";
    cin >> new_phone;

    bool retry = true;
    while (retry) {
        string message = is_manager ? 
            "Sending OTP to user's current phone..." : 
            "Sending OTP to your current phone...";
        
        cout << message << "\n";
        string otp = utils::generate_otp();
        cout << "[OTP] Verification code: " << otp << "\n";

        utils::OTPResult result = utils::verify_otp_with_timeout(otp);
        
        switch (result) {
            case utils::OTP_SUCCESS:
                acc->set_phone(new_phone);
                utils::save_accounts_to_file(accounts, "users.txt");
                cout << "Phone number updated successfully!\n";
                retry = false;
                break;
                
            case utils::OTP_TIMEOUT:
                cout << "1. Resend OTP\n";
                cout << "2. Cancel\n";
                cout << "Choose: ";
                int choice;
                cin >> choice;
                
                if (choice != 1) {
                    cout << "Update canceled.\n";
                    return;
                }
                break;
                
            case utils::OTP_FAIL:
                cout << "Invalid OTP. Update canceled.\n";
                return;
        }
    }
}
