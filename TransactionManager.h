#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include <vector>
#include "Account.h"
#include "Transaction.h"

using namespace std;

void log_transaction(const Transaction& tx);
void transfer_points(vector<Account>& accounts, Account* sender);
void show_transaction_history(const string& username);
void manager_update_user_info(vector<Account>& accounts);
void manager_view_user_history();
void show_all_transaction_history();
void show_all_users(const vector<Account>& accounts);
#endif
