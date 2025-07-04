#include "utils.h"
#include <random>
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <limits>
#include <iomanip>
#include <ctime>
#include <openssl/evp.h>

using namespace std;
using namespace chrono;

namespace utils {

    string sha256(const string& str) {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int length;

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, str.c_str(), str.size());
        EVP_DigestFinal_ex(ctx, hash, &length);
        EVP_MD_CTX_free(ctx);

        stringstream ss;
        for (unsigned int i = 0; i < length; ++i)
            ss << hex << setw(2) << setfill('0') << (int)hash[i];

        return ss.str();
    }

    string generate_random_password(int length) {
        const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string result;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, chars.size() - 1);

        for (int i = 0; i < length; ++i) {
            result += chars[distrib(gen)];
        }
        return result;
    }

    string generate_otp() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(100000, 999999);
        return to_string(distrib(gen));
    }

    // Xử lý ngoại lệ khi đọc file
    vector<Account> load_accounts_from_file(const string& filename) {
        ifstream fin(filename);
        vector<Account> result;
        string line;

        while (getline(fin, line)) {
            try {
                if (!line.empty()) {
                    result.push_back(Account::deserialize(line));
                }
            } catch (...) {
                cerr << "Error reading line: " << line << endl;
            }
        }
        return result;
    }

    void save_accounts_to_file(const vector<Account>& accounts, const string& filename) {
        // backup file when will be overwritten
        backup_file(filename);
        ofstream fout(filename);
        for (const auto& acc : accounts) {
            fout << acc.serialize() << "\n";
        }
    }

    void backup_file(const string& filename) {
        ifstream src(filename, ios::binary);
        if (!src.is_open()) return;

        string backup_name = filename + ".bak";
        ofstream dst(backup_name, ios::binary);
        dst << src.rdbuf();
    }

    OTPResult verify_otp_with_timeout(const string& generated_otp, int timeout_seconds) {
        atomic<bool> input_received(false);
        string input;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter OTP: ";
        
        thread input_thread([&]() {
            getline(cin, input);
            input_received = true;
        });

        auto start = steady_clock::now();
        int remaining = timeout_seconds;
        
        while (remaining > 0 && !input_received) {
            cout << "\rTime remaining: " << remaining - 1 << " seconds. OTP: ";
            cout.flush();
            
            this_thread::sleep_for(seconds(1));
            auto now = steady_clock::now();
            remaining = timeout_seconds - duration_cast<seconds>(now - start).count();
        }

        if (!input_received) {
            input_thread.detach();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOTP expired. Please try again.\n";
            return OTP_TIMEOUT;
        }

        input_thread.join();
    
        if (!input.empty() && input.back() == '\n') {
            input.pop_back();
        }
        
        if (input == generated_otp) {
            return OTP_SUCCESS;
        }
        return OTP_FAIL;
    }
}
