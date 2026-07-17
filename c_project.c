#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ASSIGNMENTS 100
#define FILENAME "assignments.dat"

struct assignment {
    int id;
    char title[100];
    char description[200];
    int due_day;
    int due_month;
    int due_year;
    int completed;
    int deleted;
};

struct assignment assignments[MAX_ASSIGNMENTS];
int assignmentCount = 0;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadAssignments() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        assignmentCount = 0;
        return;
    }
    assignmentCount = fread(assignments, sizeof(struct assignment), MAX_ASSIGNMENTS, fp);
    fclose(fp);
}

void saveAssignments() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("\nError: Unable to save file!\n");
        return;
    }
    fwrite(assignments, sizeof(struct assignment), assignmentCount, fp);
    fclose(fp);
}

// Simple ID generation that a beginner would write
int generateUniqueId() {
    int maxId = 0;
    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].id > maxId) {
            maxId = assignments[i].id;
        }
    }
    return maxId + 1;
}

void addAssignment() {
    if (assignmentCount >= MAX_ASSIGNMENTS) {
        printf("\nError: Maximum capacity reached! Cannot add more assignments.\n");
        return;
    }
    struct assignment newAssign;
    newAssign.id = generateUniqueId();
    newAssign.completed = 0;
    newAssign.deleted = 0;

    printf("\n--- Add New Assignment ---\n");

    printf("Enter Title: ");
    if (fgets(newAssign.title, sizeof(newAssign.title), stdin) != NULL) {
        newAssign.title[strcspn(newAssign.title, "\n")] = '\0';
    }
    while (strlen(newAssign.title) == 0) {
        printf("Title cannot be empty. Enter Title: ");
        if (fgets(newAssign.title, sizeof(newAssign.title), stdin) != NULL) {
            newAssign.title[strcspn(newAssign.title, "\n")] = '\0';
        }
    }

    printf("Enter Description: ");
    if (fgets(newAssign.description, sizeof(newAssign.description), stdin) != NULL) {
        newAssign.description[strcspn(newAssign.description, "\n")] = '\0';
    }

    printf("Enter Due Day: ");
    scanf("%d", &newAssign.due_day);

    printf("Enter Due Month: ");
    scanf("%d", &newAssign.due_month);

    printf("Enter Due Year: ");
    scanf("%d", &newAssign.due_year);
    clearInputBuffer();

    assignments[assignmentCount++] = newAssign;
    saveAssignments();
    printf("\nAssignment added successfully! Assignment ID: %d\n", newAssign.id);
}

int compareDate(struct assignment a, struct assignment b) {
    if (a.due_year != b.due_year)
        return a.due_year - b.due_year;

    if (a.due_month != b.due_month)
        return a.due_month - b.due_month;

    return a.due_day - b.due_day;
}

void displayAssignments() {
    int found = 0;
    printf(".........................................................\n");
    printf("\n======================== ALL ASSIGNMENTS ==========================\n");
    printf("%-5s %-20s %-20s %-12s %-12s\n", "ID", "Title", "Description", "Due Date", "Status");
    printf("---------------------------------------------------------------------\n");

    // We sort a temporary local array instead of messing up the primary file array!
    struct assignment temp_arr[MAX_ASSIGNMENTS];
    for (int i = 0; i < assignmentCount; i++) {
        temp_arr[i] = assignments[i];
    }

    struct assignment temp;
    for (int i = 0; i < assignmentCount - 1; i++) {
        for (int j = i + 1; j < assignmentCount; j++) {
            if (temp_arr[i].deleted == 0 &&
                temp_arr[j].deleted == 0 &&
                compareDate(temp_arr[i], temp_arr[j]) > 0) {

                temp = temp_arr[i];
                temp_arr[i] = temp_arr[j];
                temp_arr[j] = temp;
            }
        }
    }

    for (int i = 0; i < assignmentCount; i++) {
        if (temp_arr[i].deleted == 0) {
            char status[20];
            strcpy(status, temp_arr[i].completed ? "Completed" : "Pending");
            printf("%-5d %-20s %-20s %02d-%02d-%d   %-12s\n",
                   temp_arr[i].id,
                   temp_arr[i].title,
                   temp_arr[i].description,
                   temp_arr[i].due_day,
                   temp_arr[i].due_month,
                   temp_arr[i].due_year,
                   status);
            found = 1;
        }
    }
    if (!found) {
        printf("No assignments found!\n");
    }
    printf("=====================================================================\n");
}

void searchAssignment() {
    int choice, id, found = 0;
    char title[100];
    printf("\n--- Search Assignment ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Title\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        printf("Enter Assignment ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        for (int i = 0; i < assignmentCount; i++) {
            if (assignments[i].id == id && assignments[i].deleted == 0) {
                printf("\n------------------ Assignment Found ------------------\n");
                printf("ID:          %d\n", assignments[i].id);
                printf("Title:       %s\n", assignments[i].title);
                printf("Description: %s\n", assignments[i].description);
                printf("Due Date:    %02d-%02d-%d\n", // Fixed missing \n here
                       assignments[i].due_day,
                       assignments[i].due_month,
                       assignments[i].due_year);
                printf("Status:      %s\n", assignments[i].completed ? "Completed" : "Pending");
                printf(".........................................................\n");
                found = 1;
                break;
            }
        }
    } else if (choice == 2) {
        printf("Enter Title to search: ");
        if (fgets(title, sizeof(title), stdin) != NULL) {
            title[strcspn(title, "\n")] = '\0';
        }
        for (int i = 0; i < assignmentCount; i++) {
            if (assignments[i].deleted == 0 && strstr(assignments[i].title, title) != NULL) {
                printf("\n------------------ Assignment Found ------------------\n");
                printf("ID:          %d\n", assignments[i].id);
                printf("Title:       %s\n", assignments[i].title);
                printf("Description: %s\n", assignments[i].description);
                printf("Due Date:    %02d-%02d-%d\n",
                       assignments[i].due_day,
                       assignments[i].due_month,
                       assignments[i].due_year);
                printf("Status:      %s\n", assignments[i].completed ? "Completed" : "Pending");
                printf(".........................................................\n");
                found = 1;
            }
        }
    } else {
        printf("Invalid choice!\n");
        return;
    }
    if (!found) {
        printf("\nNo matching assignment found!\n");
    }
}

void updateAssignment() {
    int id, found = 0;
    printf("\n--- Update Assignment ---\n");
    printf("Enter Assignment ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].id == id && assignments[i].deleted == 0) {
            found = 1;
            printf("\nCurrent Details:\n");
            printf("Title:       %s\n", assignments[i].title);
            printf("Description: %s\n", assignments[i].description);
            printf("Due Date:    %02d-%02d-%d\n",
                   assignments[i].due_day,
                   assignments[i].due_month,
                   assignments[i].due_year);

            printf("\n--- Enter New Details (press Enter to keep current) ---\n");
            char newTitle[100], newDesc[200], inputStr[50];

            printf("New Title [%s]: ", assignments[i].title);
            if (fgets(newTitle, sizeof(newTitle), stdin) != NULL) {
                newTitle[strcspn(newTitle, "\n")] = '\0';
            }
            if (strlen(newTitle) > 0) {
                strcpy(assignments[i].title, newTitle);
            }

            printf("New Description [%s]: ", assignments[i].description);
            if (fgets(newDesc, sizeof(newDesc), stdin) != NULL) {
                newDesc[strcspn(newDesc, "\n")] = '\0';
            }
            if (strlen(newDesc) > 0) {
                strcpy(assignments[i].description, newDesc);
            }

            // Begginer-friendly safe numerical input parsing via string check:
            printf("New Due Day [%d]: ", assignments[i].due_day);
            if (fgets(inputStr, sizeof(inputStr), stdin) != NULL && inputStr[0] != '\n') {
                assignments[i].due_day = atoi(inputStr);
            }

            printf("New Due Month [%d]: ", assignments[i].due_month);
            if (fgets(inputStr, sizeof(inputStr), stdin) != NULL && inputStr[0] != '\n') {
                assignments[i].due_month = atoi(inputStr);
            }

            printf("New Due Year [%d]: ", assignments[i].due_year);
            if (fgets(inputStr, sizeof(inputStr), stdin) != NULL && inputStr[0] != '\n') {
                assignments[i].due_year = atoi(inputStr);
            }

            saveAssignments();
            printf("\nAssignment updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("\nAssignment not found!\n");
    }
}

void deleteAssignment() {
    int id, found = 0;
    printf("\n--- Delete Assignment ---\n");
    printf("Enter Assignment ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].id == id && assignments[i].deleted == 0) {
            char confirm;
            printf("Are you sure you want to delete assignment '%s'? (y/n): ", assignments[i].title);
            scanf("%c", &confirm);
            clearInputBuffer();
            if (confirm == 'y' || confirm == 'Y') {
                assignments[i].deleted = 1;
                saveAssignments();
                printf("\nAssignment deleted successfully!\n");
            } else {
                printf("\nDeletion cancelled!\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("\nAssignment not found or already deleted!\n");
    }
}

void markCompleted() {
    int id, found = 0;
    printf("\n--- Mark Assignment as Completed ---\n");
    printf("Enter Assignment ID: ");
    scanf("%d", &id);
    clearInputBuffer();
    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].id == id && assignments[i].deleted == 0) {
            if (assignments[i].completed == 1) {
                printf("\nAssignment is already marked as completed!\n");
            } else {
                assignments[i].completed = 1;
                saveAssignments();
                printf("\nAssignment marked as completed!\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("\nAssignment not found!\n");
    }
}

void displayMenu() {
    printf("\n");
    printf("======================================================\n");
    printf("               ASSIGNMENT MANAGEMENT SYSTEM           \n");
    printf("======================================================\n");
    printf("  1. Add Assignment\n");
    printf("  2. Display Assignments\n");
    printf("  3. Search Assignment\n");
    printf("  4. Update Assignment\n");
    printf("  5. Delete Assignment\n");
    printf("  6. Mark Completed\n");
    printf("  7. Exit\n");
    printf("=======================================================\n");

    int activeCount = 0;
    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].deleted == 0) {
            activeCount++;
        }
    }
    printf("  Current Assignments: %d / %d\n", activeCount, MAX_ASSIGNMENTS);
    printf("..............................................................\n");
    printf("  Enter your choice: ");
}

int main() {
    int choice;
    loadAssignments();
    while (1) {
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: addAssignment(); break;
            case 2: displayAssignments(); break;
            case 3: searchAssignment(); break;
            case 4: updateAssignment(); break;
            case 5: deleteAssignment(); break;
            case 6: markCompleted(); break;
            case 7:
                printf("\nThank you for using Assignment Management System!\n");
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}
