#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strcspn
#include "account.h" // Our custom header

// Helper to clear keyboard buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Account *account_list_head = NULL; // Start with an empty list
    int choice = 0;

    while (choice != 6) {
        printf("\n=== Simple Bank System ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Funds\n");
        printf("3. Withdraw Funds\n");
        printf("4. Check Account Balance\n");
        printf("5. List All Accounts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            choice = 0; // Force default case
        }
        clear_input_buffer(); // Consume the '\n'

        switch (choice) {
            case 1: { // Create Account
                int acc_num;
                char name[100];
                printf("Enter new account number: ");
                scanf("%d", &acc_num);
                clear_input_buffer();
                printf("Enter owner's name: ");
                fgets(name, 100, stdin);
                name[strcspn(name, "\n")] = 0; // Remove newline
                
                // Check if account already exists
                if (find_account(account_list_head, acc_num) != NULL) {
                    printf("Error: Account number %d already exists.\n", acc_num);
                } else {
                    add_account(&account_list_head, acc_num, name);
                }
                break;
            }
            case 2: // Deposit
            case 3: // Withdraw
            case 4: { // Check Balance
                int acc_num;
                printf("Enter account number: ");
                scanf("%d", &acc_num);
                clear_input_buffer();
                
                Account *acc = find_account(account_list_head, acc_num);
                if (acc == NULL) {
                    printf("Error: Account %d not found.\n", acc_num);
                    break;
                }
                
                if (choice == 2) { // Deposit
                    double amount;
                    printf("Enter deposit amount: ");
                    scanf("%lf", &amount);
                    clear_input_buffer();
                    deposit(acc, amount);
                } else if (choice == 3) { // Withdraw
                    double amount;
                    printf("Enter withdrawal amount: ");
                    scanf("%lf", &amount);
                    clear_input_buffer();
                    withdraw(acc, amount);
                } else { // Check Balance
                    printf("Account %d | Name: %s | Balance: $%.2f\n",
                           acc->account_number, acc->owner_name, acc->balance);
                }
                break;
            }
            case 5: // List All
                list_accounts(account_list_head);
                break;
            case 6: // Exit
                printf("Freeing all memory and exiting. Goodbye!\n");
                free_accounts(&account_list_head);
                break;
            default:
                printf("Invalid choice. Please enter a number from 1 to 6.\n");
        }
    }
    return 0;
}