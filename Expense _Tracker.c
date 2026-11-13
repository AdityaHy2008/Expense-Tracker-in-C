#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPENSES 100
#define FILENAME "expenses.txt"

// Structure to hold individual expense details
typedef struct {
    int id;
    char category[50];
    float amount;
    char date[15];
} Expense;

// Function prototypes
void addExpense(Expense expenses[], int *count);
void viewExpenses(Expense expenses[], int count);
void calculateTotal(Expense expenses[], int count);
void filterByCategory(Expense expenses[], int count);
void searchExpense(Expense expenses[], int count);
void saveToFile(Expense expenses[], int count);
void loadFromFile(Expense expenses[], int *count);

// Helper function for case-insensitive string comparison
int caseInsensitiveMatch(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

int main() {
    Expense expenses[MAX_EXPENSES];
    int count = 0;
    int choice;

    // Load existing expenses from file when the program starts
    loadFromFile(expenses, &count);

    do {
        printf("\n=====================================\n");
        printf("       EXPENSE TRACKER MENU          \n");
        printf("=====================================\n");
        printf("1. Add New Expense\n");
        printf("2. View All Expenses\n");
        printf("3. View Total Expenses\n");
        printf("4. Filter Expenses by Category\n");
        printf("5. Search Expense (by ID or Keyword)\n");
        printf("6. Save and Exit\n");
        printf("Enter your choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                addExpense(expenses, &count);
                break;
            case 2:
                viewExpenses(expenses, count);
                break;
            case 3:
                calculateTotal(expenses, count);
                break;
            case 4:
                filterByCategory(expenses, count);
                break;
            case 5:
                searchExpense(expenses, count);
                break;
            case 6:
                saveToFile(expenses, count);
                printf("\nData saved successfully. Exiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please choose between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}

// Function to add a new expense
void addExpense(Expense expenses[], int *count) {
    if (*count >= MAX_EXPENSES) {
        printf("\nExpense list is full! Cannot add more.\n");
        return;
    }

    Expense e;
    e.id = *count + 1;

    printf("\nEnter category (e.g., Food, Travel, Books): ");
    scanf("%49s", e.category);

    printf("Enter amount (INR): ");
    while (scanf("%f", &e.amount) != 1 || e.amount <= 0) {
        printf("Invalid amount. Enter a positive number: ");
        while (getchar() != '\n');
    }

    printf("Enter date (DD-MM-YYYY): ");
    scanf("%14s", e.date);

    expenses[*count] = e;
    (*count)++;

    printf("\nExpense added successfully! (ID: %d)\n", e.id);
}

// Function to display all recorded expenses
void viewExpenses(Expense expenses[], int count) {
    if (count == 0) {
        printf("\nNo expenses recorded yet.\n");
        return;
    }

    printf("\n----------------------------------------------------\n");
    printf("%-5s | %-15s | %-10s | %-12s\n", "ID", "Category", "Amount", "Date");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-15s | ₹%-9.2f | %-12s\n", 
               expenses[i].id, 
               expenses[i].category, 
               expenses[i].amount, 
               expenses[i].date);
    }
    printf("----------------------------------------------------\n");
}

// Function to calculate and display total expenditure
void calculateTotal(Expense expenses[], int count) {
    if (count == 0) {
        printf("\nTotal Expenses: ₹0.00\n");
        return;
    }

    float total = 0;
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
    }
    printf("\n----------------------------------------------------\n");
    printf("Total Money Spent: ₹%.2f across %d transactions.\n", total, count);
    printf("----------------------------------------------------\n");
}

// Function to filter and view expenses by category
void filterByCategory(Expense expenses[], int count) {
    if (count == 0) {
        printf("\nNo expenses recorded yet.\n");
        return;
    }

    char targetCategory[50];
    printf("\nEnter category to filter by (e.g., Food, Travel): ");
    scanf("%49s", targetCategory);

    printf("\n--- Expenses in Category: %s ---\n", targetCategory);
    printf("----------------------------------------------------\n");
    printf("%-5s | %-15s | %-10s | %-12s\n", "ID", "Category", "Amount", "Date");
    printf("----------------------------------------------------\n");

    float categoryTotal = 0;
    int foundCount = 0;

    for (int i = 0; i < count; i++) {
        if (caseInsensitiveMatch(expenses[i].category, targetCategory)) {
            printf("%-5d | %-15s | ₹%-9.2f | %-12s\n", 
                   expenses[i].id, 
                   expenses[i].category, 
                   expenses[i].amount, 
                   expenses[i].date);
            categoryTotal += expenses[i].amount;
            foundCount++;
        }
    }
    printf("----------------------------------------------------\n");
    if (foundCount > 0) {
        printf("Total spent on '%s': ₹%.2f (%d items)\n", targetCategory, categoryTotal, foundCount);
    } else {
        printf("No expenses found under category '%s'.\n", targetCategory);
    }
}

// Function to search expenses by ID or category keyword
void searchExpense(Expense expenses[], int count) {
    if (count == 0) {
        printf("\nNo expenses recorded yet.\n");
        return;
    }

    int searchChoice;
    printf("\nSearch by:\n");
    printf("1. Expense ID\n");
    printf("2. Category Keyword\n");
    printf("Enter choice (1-2): ");
    
    if (scanf("%d", &searchChoice) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    if (searchChoice == 1) {
        int targetId;
        printf("Enter Expense ID to search: ");
        scanf("%d", &targetId);

        int found = 0;
        printf("\n----------------------------------------------------\n");
        printf("%-5s | %-15s | %-10s | %-12s\n", "ID", "Category", "Amount", "Date");
        printf("----------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            if (expenses[i].id == targetId) {
                printf("%-5d | %-15s | ₹%-9.2f | %-12s\n", 
                       expenses[i].id, 
                       expenses[i].category, 
                       expenses[i].amount, 
                       expenses[i].date);
                found = 1;
                break;
            }
        }
        printf("----------------------------------------------------\n");
        if (!found) {
            printf("No expense found with ID %d.\n", targetId);
        }

    } else if (searchChoice == 2) {
        char keyword[50];
        printf("Enter keyword to search in categories: ");
        scanf("%49s", keyword);

        int foundCount = 0;
        printf("\n--- Search Results for '%s' ---\n", keyword);
        printf("----------------------------------------------------\n");
        printf("%-5s | %-15s | %-10s | %-12s\n", "ID", "Category", "Amount", "Date");
        printf("----------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            // Check if keyword is a substring of the category name
            if (strstr(expenses[i].category, keyword) != NULL) {
                printf("%-5d | %-15s | ₹%-9.2f | %-12s\n", 
                       expenses[i].id, 
                       expenses[i].category, 
                       expenses[i].amount, 
                       expenses[i].date);
                foundCount++;
            }
        }
        printf("----------------------------------------------------\n");
        printf("Found %d matching record(s).\n", foundCount);

    } else {
        printf("Invalid search option selected.\n");
    }
}

// Function to save data to a text file
void saveToFile(Expense expenses[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %.2f %s\n", 
                expenses[i].id, 
                expenses[i].category, 
                expenses[i].amount, 
                expenses[i].date);
    }

    fclose(file);
}

// Function to load data from text file on startup
void loadFromFile(Expense expenses[], int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        *count = 0;
        return;
    }

    *count = 0;
    while (fscanf(file, "%d %49s %f %14s", 
                  &expenses[*count].id, 
                  expenses[*count].category, 
                  &expenses[*count].amount, 
                  expenses[*count].date) == 4) {
        (*count)++;
        if (*count >= MAX_EXPENSES) break;
    }

    fclose(file);
}

                