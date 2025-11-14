// --- FIX 1: Must be #ifndef ("if not defined") ---
#ifndef ACCOUNT_H
#define ACCOUNT_H

// This is the struct definition
typedef struct AccountNode {
    int Account_num;
    char owner_name[100];
    double balance;
    struct AccountNode *next;
} Account;

// --- Function Prototypes ---

// --- FIX 2: Removed the stray 'm' typo ---
void add_account(Account **head_ptr, int acc_num, const char* name);

void deposit(Account *account, double amount);

int withdraw(Account *account, double amount);

void list_accounts(Account *head);

// --- FIX 3: You were MISSING this prototype ---
// This is why the compiler couldn't find it.
Account* find_account(Account *head, int acc_num);

// --- FIX 4: This must take a double pointer (**) ---
// This needs to match main.c's call and account.c's definition.
void free_accounts(Account **head_ptr);

#endif