#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONE_LEN 15
#define EMAIL_LEN 50

typedef struct {
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void addContact();
void deleteContact();
void updateContact();
void searchContact();
void displayContacts();
int isDuplicate(const char *value, int checkPhone);
void sanitizeInput(char *input);
void saveContactsToFile();
void loadContactsFromFile();

int main() {
    loadContactsFromFile(); // Load contacts from file when the program starts

    int choice;
    while (1) {
        printf("\n------------------- Contact Management -------------------\n");
        printf("1. Add a contact\n2. Delete a contact\n3. Update a contact\n4. Search a contact\n5. Display all contacts\n6. Exit the system\n");
        printf("---------------------------------------------------------\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
            case 1: addContact(); break;
            case 2: deleteContact(); break;
            case 3: updateContact(); break;
            case 4: searchContact(); break;
            case 5: displayContacts(); break;
            case 6: 
                printf("Exiting from the system...\n");
                saveContactsToFile(); // Save contacts before exiting
                return 0;
            default: printf("Invalid choice!\n");
        }
    }
}

void sanitizeInput(char *input) {
    input[strcspn(input, "\n")] = '\0';
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Contact list is full!\n");
        return;
    }

    Contact newContact;
    
    printf("Enter The Name: ");
    fgets(newContact.name, NAME_LEN, stdin);
    sanitizeInput(newContact.name);

    while (isDuplicate(newContact.name, 0)) {
        printf("Name already exists, enter a different name: ");
        fgets(newContact.name, NAME_LEN, stdin);
        sanitizeInput(newContact.name);
    }

    printf("Enter The Phone Number: ");
    fgets(newContact.phone, PHONE_LEN, stdin);
    sanitizeInput(newContact.phone);
    
    while (isDuplicate(newContact.phone, 1)) {
        printf("Phone already exists, try again: ");
        fgets(newContact.phone, PHONE_LEN, stdin);
        sanitizeInput(newContact.phone);
    }

    printf("Enter The Email: ");
    fgets(newContact.email, EMAIL_LEN, stdin);
    sanitizeInput(newContact.email);
    
    while (isDuplicate(newContact.email, 0)) {
        printf("Email already used, enter a different email: ");
        fgets(newContact.email, EMAIL_LEN, stdin);
        sanitizeInput(newContact.email);
    }

    contacts[contactCount++] = newContact;
    printf("Contact added!\n");
    
    saveContactsToFile(); // Save contacts to file after adding
}

void deleteContact() {
    char name[NAME_LEN];
    printf("Enter name of the contact you want to delete: ");
    fgets(name, NAME_LEN, stdin);
    sanitizeInput(name);

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            contacts[i] = contacts[--contactCount];
            printf("Contact deleted!\n");
            
            saveContactsToFile(); // Save contacts to file after deletion
            return;
        }
    }
    printf("Contact not found in the system!\n");
}

void updateContact() {
    char name[NAME_LEN];
    printf("Enter name of the contact you want to update: ");
    fgets(name, NAME_LEN, stdin);
    sanitizeInput(name);

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            char newPhone[PHONE_LEN], newEmail[EMAIL_LEN];
            printf("Enter new phone (leave blank to keep): ");
            fgets(newPhone, PHONE_LEN, stdin);
            sanitizeInput(newPhone);
            if (strlen(newPhone) > 0) {
                while (isDuplicate(newPhone, 1)) {
                    printf("Phone already exists, try again: ");
                    fgets(newPhone, PHONE_LEN, stdin);
                    sanitizeInput(newPhone);
                }
                strcpy(contacts[i].phone, newPhone);
            }

            printf("Enter new email (leave blank to keep): ");
            fgets(newEmail, EMAIL_LEN, stdin);
            sanitizeInput(newEmail);
            if (strlen(newEmail) > 0) {
                while (isDuplicate(newEmail, 0)) {
                    printf("Email already exists, try again: ");
                    fgets(newEmail, EMAIL_LEN, stdin);
                    sanitizeInput(newEmail);
                }
                strcpy(contacts[i].email, newEmail);
            }

            printf("Contact updated!\n");
            
            saveContactsToFile(); // Save contacts to file after updating
            return;
        }
    }
    printf("Contact not found in the system!\n");
}

void searchContact() {
    char name[NAME_LEN];
    printf("Enter name of the contact you want to search: ");
    fgets(name, NAME_LEN, stdin);
    sanitizeInput(name);

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            printf("Name: %s\nPhone: %s\nEmail: %s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
            return;
        }
    }
    printf("Contact not found in the system!\n");
}

void displayContacts() {
    if (contactCount == 0) {
        printf("No contacts available.\n");
        return;
    }
    printf("\n--- Contact List ---\n");
    for (int i = 0; i < contactCount; i++) {
        printf("%d. Name: %s | Phone: %s | Email: %s\n", i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
    }
}

int isDuplicate(const char *value, int checkPhone) {
    for (int i = 0; i < contactCount; i++) {
        if (checkPhone) {
            if (strcmp(contacts[i].phone, value) == 0) return 1;
        } else {
            if (strcmp(contacts[i].name, value) == 0 || strcmp(contacts[i].email, value) == 0) return 1;
        }
    }
    return 0;
}

void saveContactsToFile() {
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL) {
        printf("Error opening file for saving contacts!\n");
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s,%s,%s\n", contacts[i].name, contacts[i].phone, contacts[i].email);
    }

    fclose(file);
    printf("Contacts saved to file successfully.\n");
}

void loadContactsFromFile() {
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL) {
        printf("No existing contact file found, starting fresh.\n");
        return;
    }

    contactCount = 0;
    while (fscanf(file, "%[^,],%[^,],%[^\n]\n", contacts[contactCount].name, contacts[contactCount].phone, contacts[contactCount].email) == 3) {
        contactCount++;
    }

    fclose(file);
    printf("Contacts loaded from file successfully.\n");
}
