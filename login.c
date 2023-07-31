#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

 

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

 

User users[MAX_USERS];
int numUsers = 0;

 

void registerUser() {
    if (numUsers >= MAX_USERS) {
        printf("maximum number of users reached\n");
        return;
    }

 

    printf("Enter username: ");
    char username[MAX_USERNAME_LENGTH];
    scanf("%s", username);

 

    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already taken\n");
            return;
        }
    }

 

    printf("Enter password: ");
    char password[MAX_PASSWORD_LENGTH];
    scanf("%s", password);

 

    strcpy(users[numUsers].username, username);
    strcpy(users[numUsers].password, password);

 

    numUsers++;
    printf("Registration successful!\n");
}

 

int loginUser() {
    printf("Enter username: ");
    char username[MAX_USERNAME_LENGTH];
    scanf("%s", username);

 

    printf("Enter password: ");
    char password[MAX_PASSWORD_LENGTH];
    scanf("%s", password);

 

    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i;
        }
    }

 

    return -1;
}

 

int main() {
    while (1) {
        int choice;
        printf("register\n");
        printf("login\n");
        printf("exit\n");
        printf("enter the choice\n");
        scanf("%d", &choice);

 

        switch (choice) {
            case 1:
                registerUser();
                break;

 

            case 2: {
                int loggedInIndex = loginUser();
                if (loggedInIndex != -1) {
                    printf("Login successful for user: %s\n", users[loggedInIndex].username);
                } else {
                    printf("Login failed. Invalid username or password.\n");
                }
                break;
            }

 

            case 3:
                printf("Exiting the system.\n");
                exit(0);

 

            default:
                printf("Invalid choice\n");
        }
        printf("\n");
    }
    return 0;
}
