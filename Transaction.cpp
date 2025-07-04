#include "Transaction.h"
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

string Transaction::get_current_time() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    ostringstream oss;
    oss << put_time(ltm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

string Transaction::serialize() const {
    ostringstream oss;
    oss << from_username << "," << to_username << "," << amount << "," << timestamp << "," << status;
    return oss.str();
}

Transaction Transaction::deserialize(const string& line) {
    stringstream ss(line);
    string from, to, amt, ts, st;
    getline(ss, from, ',');
    getline(ss, to, ',');
    getline(ss, amt, ',');
    getline(ss, ts, ',');
    getline(ss, st);

    return Transaction{from, to, stod(amt), ts, st};
}
