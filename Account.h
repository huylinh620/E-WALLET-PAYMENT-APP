#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <iostream>

enum class Role {
    USER,
    MANAGER
};

class Account {
private:
    std::string username;
    std::string phone;
    std::string hashed_password;
    Role role;
    double balance = 0.0;
    bool password_is_temp = false;

public:
    virtual ~Account() = default;

    Account(const std::string& uname, const std::string& phone, const std::string& hashed_pw,
            Role role = Role::USER, double balance = 0.0, bool temp_pass = false);

    // Getters
    std::string get_username() const;
    std::string get_phone() const;
    std::string get_password_hash() const;
    Role get_role() const;
    double get_balance() const;
    bool is_temp_password() const;

    // Setters
    void set_phone(const std::string&);
    void set_password_hash(const std::string&, bool temp_pass = false);
    void set_balance(double);
    void set_role(Role);

    // I/O
    std::string serialize() const;
    static Account deserialize(const std::string& line);

    // Helpers
    static std::string role_to_string(Role);
    static Role string_to_role(const std::string& str);
};

#endif
