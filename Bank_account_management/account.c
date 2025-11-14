#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h" // Include our own header

// Function to add an account to the end of the list
void add_account(Account **head_ptr, int acc_num, const char *name) {
    // 1. Create the new node
    Account *new_acc = malloc(sizeof(Account));
    if (new_acc == NULL) {
        printf("Error: Malloc failed!\n");
        return;
    }

    // --- FIX: Use 'account_number' to match the header ---
    new_acc->account_number = acc_num;
    strcpy(new_acc->owner_name, name);
    new_acc->balance = 0.0;
    new_acc->next = NULL;

    // 2. Find the end of the list
    if (*head_ptr == NULL) {
        // List is empty, this is the first node
        *head_ptr = new_acc;
    } else {
        // List is not empty, traverse to the end
        Account *current = *head_ptr;
        while (current->next != NULL) {
            current = current->next;
        }
        // 3. Link the new node
        current->next = new_acc;
    }
    printf("Account %d for %s created.\n", acc_num, name);
}

// Function to find an account by its number
Account* find_account(Account *head, int acc_num) {
    Account *current = head;
    while (current != NULL) {
        // --- FIX: Use 'account_number' to match the header ---
        if (current->account_number == acc_num) {
            return current; // Found it!
        }
        current = current->next;
    }
    return NULL; // Not found
}

// Function to deposit money
void deposit(Account *acc, double amount) {
    if (acc == NULL) {
        printf("Error: Account not found.\n");
        return;
    }
    if (amount <= 0) {
        printf("Error: Deposit amount must be positive.\n");
        return;
    }
    acc->balance += amount;
    printf("Success. New balance: $%.2f\n", acc->balance);
}

// Function to withdraw money
int withdraw(Account *acc, double amount) {
    if (acc == NULL) {
        printf("Error: Account not found.\n");
        return 0; // Failure
    }
    if (amount <= 0) {
        printf("Error: Withdrawal amount must be positive.\n");
        return 0; // Failure
    }
    if (acc->balance < amount) {
        printf("Error: Insufficient funds. Balance is $%.2f\n", acc->balance);
        return 0; // Failure
    }
    
    acc->balance -= amount;
    printf("Success. New balance: $%.2f\n", acc->balance);
    return 1; // Success
}

// Function to print all accounts
void list_accounts(Account *head) {
    Account *current = head;
    printf("\n--- All Bank Accounts ---\n");
    if (current == NULL) {
        printf(" (No accounts in the bank)\n");
    }
    while (current != NULL) {
        // --- FIX: Use 'account_number' to match the header ---
        printf("  Acc: %-5d | Name: %-20s | Balance: $%.2f\n",
               current->account_number,
               current->owner_name,
               current->balance);
        current = current->next;
    }
    printf("---------------------------\n");
}

// Function to free all memory
void free_accounts(Account **head_ptr) {
    Account *current = *head_ptr;
    Account *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp); // Free the node
    }
    *head_ptr = NULL; // Clear the original pointer
}