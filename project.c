#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Structures
typedef struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
    struct Patient* next;
} Patient;

typedef struct Doctor {
    int id;
    char name[50];
    char department[50];
    struct Doctor* next;
} Doctor;

typedef struct Queue {
    int front, rear;
    Patient* data[MAX];
} Queue;

typedef struct Stack {
    int top;
    Patient* data[MAX];
} Stack;

// Tree for Departments
typedef struct DeptNode {
    char department[50];
    struct DeptNode* left;
    struct DeptNode* right;
} DeptNode;

// 🔧 Function prototype for insertDept (fix for error)
DeptNode* insertDept(DeptNode* root, char dept[]);

// Graph for Rooms
int roomGraph[10][10] = {0};

// Globals
Patient* patientHead = NULL;
Doctor* doctorHead = NULL;
Queue appointmentQueue = {-1, -1};
Stack dischargeStack = {.top = -1};
DeptNode* deptRoot = NULL;

// Utility functions
Patient* createPatient() {
    Patient* newP = (Patient*)malloc(sizeof(Patient));
    printf("Enter Patient ID: ");
    scanf("%d", &newP->id);
    printf("Enter Patient Name: ");
    scanf(" %[^\n]", newP->name);
    printf("Enter Age: ");
    scanf("%d", &newP->age);
    printf("Enter Disease: ");
    scanf(" %[^\n]", newP->disease);
    newP->next = NULL;
    return newP;
}

void addPatient() {
    Patient* newP = createPatient();
    newP->next = patientHead;
    patientHead = newP;
    printf("✅ Patient added successfully!\n");
}

void viewPatients() {
    Patient* temp = patientHead;
    printf("--- Patient List ---\n");
    while (temp) {
        printf("ID: %d | Name: %s | Age: %d | Disease: %s\n", temp->id, temp->name, temp->age, temp->disease);
        temp = temp->next;
    }
}

Doctor* createDoctor() {
    Doctor* newD = (Doctor*)malloc(sizeof(Doctor));
    printf("Enter Doctor ID: ");
    scanf("%d", &newD->id);
    printf("Enter Doctor Name: ");
    scanf(" %[^\n]", newD->name);
    printf("Enter Department: ");
    scanf(" %[^\n]", newD->department);
    newD->next = NULL;
    return newD;
}

void addDoctor() {
    Doctor* newD = createDoctor();
    newD->next = doctorHead;
    doctorHead = newD;
    printf("✅ Doctor added successfully!\n");
    // insert to department tree
    deptRoot = insertDept(deptRoot, newD->department);
}

void viewDoctors() {
    Doctor* temp = doctorHead;
    printf("--- Doctor List ---\n");
    while (temp) {
        printf("ID: %d | Name: %s | Department: %s\n", temp->id, temp->name, temp->department);
        temp = temp->next;
    }
}

// Queue
void enqueue(Patient* p) {
    if (appointmentQueue.rear == MAX - 1) {
        printf("❌ Appointment queue is full!\n");
        return;
    }
    if (appointmentQueue.front == -1) appointmentQueue.front = 0;
    appointmentQueue.data[++appointmentQueue.rear] = p;
    printf("✅ Appointment booked for %s\n", p->name);
}

void viewAppointments() {
    if (appointmentQueue.front == -1 || appointmentQueue.front > appointmentQueue.rear) {
        printf("No appointments.\n");
        return;
    }
    printf("--- Appointment Queue ---\n");
    for (int i = appointmentQueue.front; i <= appointmentQueue.rear; i++) {
        printf("Patient ID: %d | Name: %s\n", appointmentQueue.data[i]->id, appointmentQueue.data[i]->name);
    }
}

// Stack
void dischargePatient() {
    if (appointmentQueue.front == -1 || appointmentQueue.front > appointmentQueue.rear) {
        printf("No patients to discharge.\n");
        return;
    }
    Patient* p = appointmentQueue.data[appointmentQueue.front++];
    dischargeStack.data[++dischargeStack.top] = p;
    printf("✅ Patient %s discharged.\n", p->name);
}

void viewDischarged() {
    printf("--- Recently Discharged ---\n");
    for (int i = dischargeStack.top; i >= 0; i--) {
        printf("Name: %s\n", dischargeStack.data[i]->name);
    }
}

// Tree
DeptNode* insertDept(DeptNode* root, char dept[]) {
    if (root == NULL) {
        DeptNode* newNode = (DeptNode*)malloc(sizeof(DeptNode));
        strcpy(newNode->department, dept);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcmp(dept, root->department) < 0)
        root->left = insertDept(root->left, dept);
    else if (strcmp(dept, root->department) > 0)
        root->right = insertDept(root->right, dept);
    return root;
}

void inorder(DeptNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%s\n", root->department);
        inorder(root->right);
    }
}

// Graph
void addRoomConnection(int a, int b) {
    roomGraph[a][b] = 1;
    roomGraph[b][a] = 1;
}

void viewRoomGraph(int n) {
    printf("--- Room Connections ---\n");
    for (int i = 0; i < n; i++) {
        printf("Room %d is connected to: ", i);
        for (int j = 0; j < n; j++) {
            if (roomGraph[i][j]) printf("%d ", j);
        }
        printf("\n");
    }
}

// Main Menu
void menu() {
    int choice, a, b, roomCount;
    while (1) {
        printf("\n===DIU HOSPITAL MANAGEMENT MENU ===\n");
        printf("1. Add Patient\n");
        printf("2. View Patients\n");
        printf("3. Add Doctor\n");
        printf("4. View Doctors\n");
        printf("5. Book Appointment\n");
        printf("6. View Appointments\n");
        printf("7. Discharge Patient\n");
        printf("8. View Discharged\n");
        printf("9. View Doctor Departments\n");
        printf("10. Add Room Connection\n");
        printf("11. View Room Graph\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: addDoctor(); break;
            case 4: viewDoctors(); break;
            case 5: {
                printf("Booking for which patient ID? ");
                int id;
                scanf("%d", &id);
                Patient* temp = patientHead;
                while (temp) {
                    if (temp->id == id) {
                        enqueue(temp);
                        break;
                    }
                    temp = temp->next;
                }
                if (!temp) printf("❌ Patient not found.\n");
                break;
            }
            case 6: viewAppointments(); break;
            case 7: dischargePatient(); break;
            case 8: viewDischarged(); break;
            case 9: {
                printf("--- Departments ---\n");
                inorder(deptRoot);
                break;
            }
            case 10:
                printf("Enter two room numbers to connect (e.g. 0 1): ");
                scanf("%d %d", &a, &b);
                addRoomConnection(a, b);
                break;
            case 11:
                printf("Enter number of rooms to show: ");
                scanf("%d", &roomCount);
                viewRoomGraph(roomCount);
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
