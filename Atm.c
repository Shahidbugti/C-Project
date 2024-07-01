#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_USERS 3

typedef struct {
    char name[20];
    char pin_file[20];
    char balance_file[20];
} User;

#define PIN_FILE_NAME "atm_pin.txt"
#define BALANCE_FILE_NAME "atm_balance.txt"

// Function declarations
void generatePIN(const char* pin_file);
int attemptPIN(const char* pin_file, int attempts);
int showBalance(const char* balance_file);
int deposit(const char* balance_file, int amount);
int withdraw(const char* balance_file, int amount);
void loadBalance(const char* balance_file);
void saveBalance(const char* balance_file, int balance);

int main(void) {
    User users[MAX_USERS] = {
        {"Shahid", "shahid_pin.txt", "shahid_balance.txt"},
        {"Taha", "taha_pin.txt", "taha_balance.txt"},
        {"Wali", "wali_pin.txt", "wali_balance.txt"}
    };

    int option, balance = 0;
    int userIndex = -1;
    char userName[20];

    // Select user
    printf("Enter your name (Shahid, Taha, Wali): ");
    scanf("%s", userName);
    int i;
    for (i = 0; i < MAX_USERS; i++) {
        if (strcmp(userName, users[i].name) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("User not found!\n");
        return 1;
    }

    while (1) {
        // Display main menu options
        printf("1: Generate PIN\n");
        printf("2: Use ATM\n");
        printf("3: Exit\n");
        scanf("%d", &option);

        // Handle menu options
        switch (option) {
            case 1:
                printf("ATM PIN Generator\n");
                printf("=================\n");
                generatePIN(users[userIndex].pin_file);
                break;

            case 2:
                if (attemptPIN(users[userIndex].pin_file, 3)) {
                    int atmOptions;
                    printf("1: Withdraw cash\n2: Deposit cash\n3: Show balance\n");
                    scanf("%d", &atmOptions);

                    // Handle ATM options
                    switch (atmOptions) {
                        case 1:
                            printf("Enter amount you want to withdraw: ");
                            int withdrawAmount;
                            scanf("%d", &withdrawAmount);
                            withdraw(users[userIndex].balance_file, withdrawAmount);
                            break;
                        case 2:
                            printf("Enter amount you want to deposit: ");
                            int depositAmount;
                            scanf("%d", &depositAmount);
                            deposit(users[userIndex].balance_file, depositAmount);
                            break;
                        case 3:
                            showBalance(users[userIndex].balance_file);
                            break;
                        default:
                            printf("Invalid option. Please select from the given options.\n");
                    }
                } else {
                    printf("Failed to enter the correct PIN after 3 attempts.\n");
                }
                break;

            case 3:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid option. Please select 1, 2, or 3.\n");
        }
    }

    return 0;
}

// PIN Generation
void generatePIN(const char* pin_file) {
    FILE *pinFile;
    srand(time(NULL));  // Seed the random number generator with the current time
    int generatedPIN = 1000 + rand() % 9000;  // Generate a random 4-digit number between 1000 and 9999

    pinFile = fopen(pin_file, "w");
    if (pinFile == NULL) {
        printf("Error opening PIN file for writing.\n");
        return;
    }

    fprintf(pinFile, "%d\n", generatedPIN);  // Write PIN to file
    fclose(pinFile);  // Close the file

    printf("Your generated PIN is: %d\n", generatedPIN);
}

// Attempt PIN Verification
int attemptPIN(const char* pin_file, int attempts) {
    if (attempts <= 0) {
        return 0;  // Fail after 3 attempts
    }

    int storedPin, enteredPin;
    FILE *pinFile = fopen(pin_file, "r");
    if (pinFile == NULL) {
        printf("Error opening PIN file for reading.\n");
        return 0;  // Assume incorrect if we can't read the file
    }

    fscanf(pinFile, "%d", &storedPin);  // Read the stored PIN from file
    fclose(pinFile);  // Close the file

    printf("Enter the generated PIN: ");
    scanf("%d", &enteredPin);

    if (enteredPin == storedPin) {
        return 1;  // Success
    } else {
        printf("Incorrect PIN. You have %d attempts remaining.\n", attempts - 1);
        return attemptPIN(pin_file, attempts - 1);  // Recursive call for remaining attempts
    }
}

// Show Balance
int showBalance(const char* balance_file) {
    int balance;
    FILE *balanceFile = fopen(balance_file, "r");
    if (balanceFile == NULL) {
        printf("Error opening balance file for reading.\n");
        return 0;
    }

    fscanf(balanceFile, "%d", &balance);
    fclose(balanceFile);

    printf("Your current balance is %d\n", balance);
    return balance;
}

// Deposit Amount
int deposit(const char* balance_file, int amount) {
    if (amount <= 0) {
        printf("Enter a valid amount to be deposited\n");
        return 0;
    }

    int balance;
    FILE *balanceFile = fopen(balance_file, "r+");
    if (balanceFile == NULL) {
        printf("Error opening balance file for reading/writing.\n");
        return 0;
    }

    fscanf(balanceFile, "%d", &balance);
    balance += amount;
    fseek(balanceFile, 0, SEEK_SET);
    fprintf(balanceFile, "%d\n", balance);
    fclose(balanceFile);

    printf("Successfully deposited the amount %d. Your new balance is %d\n", amount, balance);
    return balance;
}

// Withdraw Amount
int withdraw(const char* balance_file, int amount) {
    if (amount <= 0) {
        printf("Enter a valid amount to be withdrawn\n");
        return 0;
    }

    int balance;
    FILE *balanceFile = fopen(balance_file, "r+");
    if (balanceFile == NULL) {
        printf("Error opening balance file for reading/writing.\n");
        return 0;
    }

    fscanf(balanceFile, "%d", &balance);
    if (amount > balance) {
        printf("Insufficient balance to withdraw the requested amount\n");
        fclose(balanceFile);
        return 0;
    }

    balance -= amount;
    fseek(balanceFile, 0, SEEK_SET);
    fprintf(balanceFile, "%d\n", balance);
    fclose(balanceFile);

    printf("Successfully withdrew the amount %d. Your new balance is %d\n", amount, balance);
    return balance;
}

