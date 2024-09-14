#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STAFF_FILE "staff.txt"
#define CUSTOMER_FILE "customers.txt"

// Function prototypes
void displayMenu();
void createStaffID();
void login();
void deposit();
void withdraw();
void listCustomers();
void addCustomer();
void clearScreen();

int main() {
    int choice;

    while (1) {
        printf("Welcome to Bank Management System\n");
        printf("--------------------------------------------------"
           "--------------------"
           "--------------------\n");
        printf("1. Login\n");
        printf("2. Create New Staff ID\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume the newline character left by scanf

        if (choice == 1) {
            login();
        } else if (choice == 2) {
            createStaffID();
        } else {
            printf("Invalid choice. Please try again.\n");
            getchar();
        }
    }

    return 0;
}

// Function to create a new staff ID
void createStaffID() {
    FILE *file = fopen(STAFF_FILE, "a");
    char staffID[20];
    char password[20];

    if (!file) {
        perror("Error opening staff file");
        return;
    }

    printf("Enter new Staff ID: ");
    scanf("%s", staffID);
    printf("Enter new Password: ");
    scanf("%s", password);

    fprintf(file, "%s %s\n", staffID, password);
    fclose(file);

    printf("New Staff ID created.\n");
    getchar();
    clearScreen();
}

// Function to handle login
void login() {
    FILE *file = fopen(STAFF_FILE, "r");
    char staffID[20];
    char password[20];
    char id[20], pass[20];
    int valid = 0;

    if (!file) {
        perror("Error opening staff file");
        return;
    }

    printf("Enter Staff ID: ");
    scanf("%s", staffID);
    printf("Enter Password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s", id, pass) != EOF) {
        if (strcmp(id, staffID) == 0 && strcmp(pass, password) == 0) {
            valid = 1;
            break;
        }
    }
    fclose(file);

    if (valid) {
        printf("Login successful.\n");
        getchar();
        clearScreen();
        while (1) {
            displayMenu();
            int choice;
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();  // To consume the newline character left by scanf

            switch (choice) {
                case 1:
                    deposit();
                    break;
                case 2:
                    withdraw();
                    break;
                case 3:
                    listCustomers();
                    break;
                case 4:
                    addCustomer();
                    break;
                case 5:
                    exit(0);
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
    } else {
        printf("Invalid ID or Password. Please try again.\n");
        getchar();
        clearScreen();
    }
}

// Function to display the menu after login
void displayMenu() {
    printf("\t MENU \t\n");
    printf("--------------------------------------------------\n");
	printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. List Customer Data\n");
    printf("4. Add New Customer\n");
    printf("5. Exit\n");
}

// Function to handle deposits
void deposit() {
    FILE *file = fopen(CUSTOMER_FILE, "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char account[21];
    double amount;
    char acc[21];
    char name[51];
    double balance;
    int found = 0;

    if (!file) {
        perror("Error opening customer file for reading");
        return;
    }
    if (!tempFile) {
        perror("Error opening temporary file for writing");
        fclose(file);
        return;
    }

    printf("Enter Customer Account: ");
    scanf("%s", account);
    printf("Enter Amount: ");
    scanf("%lf", &amount);

    while (fscanf(file, "%50[^\n] %20s %lf\n", name, acc, &balance) != EOF) {
        if (strcmp(acc, account) == 0) {
            balance += amount;
            found = 1;
        }
        fprintf(tempFile, "%-50s %-20s %.2f\n", name, acc, balance);
    }

    if (!found) {
        printf("Invalid account number.\n");
    }

    fclose(file);
    fclose(tempFile);

    if (remove(CUSTOMER_FILE) != 0) {
        perror("Error deleting old customer file");
    }
    if (rename("temp.txt", CUSTOMER_FILE) != 0) {
        perror("Error renaming temporary file");
    }

    if (found) {
        printf("Deposit successful.\n");
        getchar();
    }
    clearScreen();
}

// Function to handle withdrawals
void withdraw() {
    FILE *file = fopen(CUSTOMER_FILE, "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char account[21];
    double amount;
    char acc[21];
    char name[51];
    double balance;
    int found = 0;

    if (!file) {
        perror("Error opening customer file for reading");
        return;
    }
    if (!tempFile) {
        perror("Error opening temporary file for writing");
        fclose(file);
        return;
    }

    printf("Enter Customer Account: ");
    scanf("%s", account);
    printf("Enter Amount: ");
    scanf("%lf", &amount);

    while (fscanf(file, "%50[^\n] %20s %lf\n", name, acc, &balance) != EOF) {
        if (strcmp(acc, account) == 0) {
            if (balance >= amount) {
                balance -= amount;
                found = 1;
            } else {
                printf("Insufficient balance. Available Balance is %f only\n",balance);
                fclose(file);
                fclose(tempFile);
                remove("temp.txt");
                getchar();
                clearScreen();
                return;
            }
        }
        fprintf(tempFile, "%-50s %-20s %.2f\n", name, acc, balance);
    }

    if (!found) {
        printf("Invalid account number.\n");
    }

    fclose(file);
    fclose(tempFile);

    if (remove(CUSTOMER_FILE) != 0) {
        perror("Error deleting old customer file");
    }
    if (rename("temp.txt", CUSTOMER_FILE) != 0) {
        perror("Error renaming temporary file");
    }

    if (found) {
        printf("Withdrawal successful.\n");
        getchar();
    }
    clearScreen();
}

// Function to list all customer data
void listCustomers() {
    FILE *file = fopen(CUSTOMER_FILE, "r");
    char name[51];
    char account[21];
    double balance;

    if (!file) {
        perror("Error opening customer file for reading");
        return;
    }

    // Print header with proper spacing
    clearScreen();
    printf("%-50s %-20s %20s\n", "Name", "Account Number", "Balance");
    printf("--------------------------------------------------"
           "--------------------"
           "--------------------\n");

    // Read and print customer data
    while (fscanf(file, "%50[^\n] %20s %lf\n", name, account, &balance) != EOF) {
        printf("%-50s %-20s %20.2f\n", name, account, balance);
    }

    fclose(file);

    clearScreen();
}

// Function to add a new customer
void addCustomer() {
    FILE *file = fopen(CUSTOMER_FILE, "a");
    char name[51];
    char account[21];
    double balance;

    if (!file) {
        perror("Error opening customer file for writing");
        return;
    }

    printf("Enter Customer Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove newline character from the input

    printf("Enter Account Number: ");
    scanf("%s", account);

    printf("Enter Initial Balance: ");
    scanf("%lf", &balance);

    fprintf(file, "%-50s %-20s %.2f\n", name, account, balance);
    fclose(file);

    printf("Customer added successfully.\n");
    getchar();
    clearScreen();
}

// Function to clear the screen after pressing Enter
void clearScreen() {
    printf("\nPress Enter to continue...\n");
    getchar();  // Wait for Enter key
    system("cls");  // Use "clear" instead of "cls" if you're on a Unix-based system
}

