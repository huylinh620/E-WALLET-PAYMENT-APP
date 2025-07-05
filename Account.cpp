#include "Account.h"
#include <sstream>
#include <algorithm>

using namespace std;


Account::Account(const string& uname, const string& phone,
                 const string& hashed_pw, Role role,
                 double balance, bool temp_pass)
    : username(uname), phone(phone), hashed_password(hashed_pw),
      role(role), balance(balance), password_is_temp(temp_pass) {}

// Getters
string Account::get_username() const { return username; }
string Account::get_phone() const { return phone; }
string Account::get_password_hash() const { return hashed_password; }
Role Account::get_role() const { return role; }
double Account::get_balance() const { return balance; }
bool Account::is_temp_password() const { return password_is_temp; }

// Setters
void Account::set_phone(const string& new_phone) { phone = new_phone; }
void Account::set_password_hash(const string& hashed, bool temp_pass) {
    hashed_password = hashed;
    password_is_temp = temp_pass;
}
void Account::set_balance(double b) {
    if (b < 0) {
        cout << "The balance cannot be negative!\n";
        return;
    }
    balance = b;
}
void Account::set_role(Role r) { role = r; }

// Convert object to line of text
string Account::serialize() const {
    ostringstream oss;
    oss << username << "," << phone << "," << hashed_password << ","
        << (password_is_temp ? "1" : "0") << ","
        << balance << "," << role_to_string(role);
    return oss.str();
}

// Create object from line of text
Account Account::deserialize(const string& line) {
    stringstream ss(line);
    string uname, phone, hashed, temp_flag, bal_str, role_str;
    
    getline(ss, uname, ',');
    getline(ss, phone, ',');
    getline(ss, hashed, ',');
    getline(ss, temp_flag, ',');
    getline(ss, bal_str, ','); 
    getline(ss, role_str); 

    return Account(uname, phone, hashed,
                   string_to_role(role_str),
                   stod(bal_str),
                   temp_flag == "1");
}

// Role helpers
string Account::role_to_string(Role role) {
    return role == Role::MANAGER ? "manager" : "user";
}
Role Account::string_to_role(const string& str) {
    return (str == "manager") ? Role::MANAGER : Role::USER;
}

// Check for valid phone number (number only, 10-11 characters)
bool Account::is_valid_phone(const string& phone) {
    if (phone.length() < 10 || phone.length() > 11) return false;
    return all_of(phone.begin(), phone.end(), ::isdigit);
}
