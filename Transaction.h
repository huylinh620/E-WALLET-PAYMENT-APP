#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

using namespace std;

struct Transaction {
    string from_username;
    string to_username;
    double amount;
    string timestamp;
    string status;

    string serialize() const;
    static Transaction deserialize(const string& line);
    static string get_current_time();
};

#endif
