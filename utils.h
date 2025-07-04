#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "Account.h"

using namespace std;

namespace utils {
    string sha256(const string& input);
    string generate_random_password(int length = 8);
    string generate_otp();
    
    // File I/O
    vector<Account> load_accounts_from_file(const string& filename);
    void save_accounts_to_file(const vector<Account>& accounts, const string& filename);
    
    // Backup
    void backup_file(const string& filename);

    enum OTPResult {
        OTP_SUCCESS,
        OTP_FAIL,
        OTP_TIMEOUT
    };
    OTPResult verify_otp_with_timeout(const string& generated_otp, int timeout_seconds = 60);
}

#endif
