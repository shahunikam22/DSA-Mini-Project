#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <limits>
using namespace std;

const int SUBJECTS = 5;
const int MAX_STUDENTS = 100;
const int STACK_SIZE = 100;
const int QUEUE_SIZE = 50;

const char *subjectNames[SUBJECTS] = {
    "DSA", "DBMS", "OOP", "Maths", "Digital Logic"
};

// Singly linked list node
struct Student {
    int roll;
    char name[50];
    int marks[SUBJECTS];
    Student *next;
};

// Stack entry for undo operation
struct Update {
    int roll;
    int subject;
    int oldMarks;
};

// Circular queue entry
struct Request {
    int roll;
    char type[30];
};

Student *head = NULL;
int studentCount = 0;

Update undoStack[STACK_SIZE];
int top = -1;

Request requestQueue[QUEUE_SIZE];
int front = -1, rear = -1;

// Find student by roll number
Student* findStudent(int roll) {
    Student *temp = head;
    while (temp != NULL) {
        if (temp->roll == roll)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Calculate total marks
int totalMarks(Student *s) {
    int total = 0;
    for (int i = 0; i < SUBJECTS; i++)
        total += s->marks[i];
    return total;
}

// Calculate grade and pass status
char grade(Student *s) {
    for (int i = 0; i < SUBJECTS; i++) {
        if (s->marks[i] < 35)
            return 'F';
    }
    float percentage = totalMarks(s) / (float)SUBJECTS;
    if (percentage >= 90) return 'A';
    if (percentage >= 75) return 'B';
    if (percentage >= 60) return 'C';
    if (percentage >= 50) return 'D';
    return 'E';
}

// Display one student's result
void displayStudent(Student *s) {
    cout << "\nRoll Number: " << s->roll;
    cout << "\nName: " << s->name << "\n";
    for (int i = 0; i < SUBJECTS; i++) {
        cout << subjectNames[i] << ": " << s->marks[i] << "/100\n";
    }
    int total = totalMarks(s);
    float percentage = total / (float)SUBJECTS;
    char g = grade(s);
    cout << "Total: " << total << "/500\n";
    cout << fixed << setprecision(2);
    cout << "Percentage: " << percentage << "%\n";
    cout << "Result: " << (g == 'F' ? "FAIL" : "PASS") << "\n";
    cout << "Grade: " << g << "\n";
}

// Read a valid integer
int readInteger(const char *message, int low, int high) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value && value >= low && value <= high) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) {
            cout << "\nInput ended. Exiting.\n";
            exit(0);
        }
        cout << "Invalid input! Enter a number from "
             << low << " to " << high << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Add a student
void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Student limit reached!\n";
        return;
    }
    int roll = readInteger("Enter roll number: ", 1, 1000000);
    if (findStudent(roll) != NULL) {
        cout << "Roll number already exists!\n";
        return;
    }

    Student *s = new Student;
    s->roll = roll;

    cout << "Enter student name: ";
    cin >> ws;
    cin.getline(s->name, 50);
    if (cin.fail() || strlen(s->name) == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        delete s;
        cout << "Invalid name!\n";
        return;
    }

    for (int i = 0; i < SUBJECTS; i++) {
        char prompt[80];
        snprintf(prompt, sizeof(prompt),
                 "Enter marks in %s (0-100): ", subjectNames[i]);
        s->marks[i] = readInteger(prompt, 0, 100);
    }

    s->next = head;
    head = s;
    studentCount++;
    cout << "Student added successfully!\n";
}

// Display all students
void displayAll() {
    if (head == NULL) {
        cout << "No student records found.\n";
        return;
    }
    Student *temp = head;
    while (temp != NULL) {
        displayStudent(temp);
        cout << "-------------------------\n";
        temp = temp->next;
    }
}

// Search student
void searchStudent() {
    int roll = readInteger("Enter roll number: ", 1, 1000000);
    Student *s = findStudent(roll);
    if (s == NULL)
        cout << "Student not found!\n";
    else
        displayStudent(s);
}

// Push update into stack
void pushUpdate(int roll, int subject, int oldMarks) {
    if (top == STACK_SIZE - 1) {
        // Discard the oldest history entry
        for (int i = 0; i < STACK_SIZE - 1; i++)
            undoStack[i] = undoStack[i + 1];
        top--;
    }
    top++;
    undoStack[top].roll = roll;
    undoStack[top].subject = subject;
    undoStack[top].oldMarks = oldMarks;
}

// Update marks
void updateMarks() {
    int roll = readInteger("Enter roll number: ", 1, 1000000);
    Student *s = findStudent(roll);
    if (s == NULL) {
        cout << "Student not found!\n";
        return;
    }
    cout << "\nSubjects:\n";
    for (int i = 0; i < SUBJECTS; i++)
        cout << i + 1 << ". " << subjectNames[i] << "\n";

    int subject = readInteger("Choose subject (1-5): ", 1, SUBJECTS);
    int newMarks = readInteger("Enter new marks (0-100): ", 0, 100);

    pushUpdate(roll, subject - 1, s->marks[subject - 1]);
    s->marks[subject - 1] = newMarks;
    cout << "Marks updated successfully!\n";
}

// Undo most recent marks update
void undoUpdate() {
    if (top == -1) {
        cout << "No updates available to undo.\n";
        return;
    }
    Update u = undoStack[top];
    top--;
    Student *s = findStudent(u.roll);
    if (s == NULL) {
        cout << "Student no longer exists.\n";
        return;
    }
    s->marks[u.subject] = u.oldMarks;
    cout << "Update undone successfully!\n";
    cout << "Restored " << subjectNames[u.subject]
         << " marks to " << u.oldMarks << ".\n";
}

// Delete student
void deleteStudent() {
    int roll = readInteger("Enter roll number to delete: ", 1, 1000000);
    Student *temp = head;
    Student *prev = NULL;
    while (temp != NULL && temp->roll != roll) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        cout << "Student not found!\n";
        return;
    }
    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;
    delete temp;
    studentCount--;

    // Remove undo entries belonging to the deleted student
    int newTop = -1;
    for (int i = 0; i <= top; i++) {
        if (undoStack[i].roll != roll)
            undoStack[++newTop] = undoStack[i];
    }
    top = newTop;

    cout << "Student deleted successfully!\n";
}

// Class statistics
void classStatistics() {
    if (head == NULL) {
        cout << "No student records available.\n";
        return;
    }
    int totalStudents = 0;
    int classTotal = 0;
    int passCount = 0;
    Student *temp = head;
    Student *topper = head;

    while (temp != NULL) {
        int total = totalMarks(temp);
        classTotal += total;
        totalStudents++;
        if (grade(temp) != 'F')
            passCount++;
        if (total > totalMarks(topper))
            topper = temp;
        temp = temp->next;
    }

    cout << "\nTotal students: " << totalStudents;
    cout << "\nClass average percentage: "
         << fixed << setprecision(2)
         << classTotal / (float)(totalStudents * SUBJECTS) << "%";
    cout << "\nStudents passed: " << passCount;
    cout << "\nStudents failed: " << totalStudents - passCount;
    cout << "\nClass topper: " << topper->name;
    cout << "\nTopper's total: " << totalMarks(topper) << "/500\n";
}

// Merit list using array and bubble sort
void displayMeritList() {
    if (head == NULL) {
        cout << "No student records available.\n";
        return;
    }
    Student *arr[MAX_STUDENTS];
    int n = 0;
    Student *temp = head;
    while (temp != NULL && n < MAX_STUDENTS) {
        arr[n++] = temp;
        temp = temp->next;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (totalMarks(arr[j]) < totalMarks(arr[j + 1])) {
                Student *t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }

    cout << "\n----------- MERIT LIST -----------\n";
    cout << left << setw(8) << "Rank"
         << setw(12) << "Roll"
         << setw(22) << "Name"
         << setw(10) << "Total"
         << "Percentage\n";
    for (int i = 0; i < n; i++) {
        cout << left << setw(8) << i + 1
             << setw(12) << arr[i]->roll
             << setw(22) << arr[i]->name
             << setw(10) << totalMarks(arr[i])
             << fixed << setprecision(2)
             << totalMarks(arr[i]) / (float)SUBJECTS << "%\n";
    }
}

// Add request to circular queue
void addRequest() {
    int roll = readInteger("Enter student roll number: ", 1, 1000000);
    if (findStudent(roll) == NULL) {
        cout << "Student not found!\n";
        return;
    }

    char type[30];
    cout << "Enter request type (Marksheet/Verification): ";
    cin >> ws;
    cin.getline(type, 30);
    if (cin.fail() || strlen(type) == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid or too long request type!\n";
        return;
    }

    int next = (rear + 1) % QUEUE_SIZE;
    if (front != -1 && next == front) {
        cout << "Request queue is full!\n";
        return;
    }
    if (front == -1)
        front = 0;
    rear = next;
    requestQueue[rear].roll = roll;
    strcpy(requestQueue[rear].type, type);
    cout << "Request added to queue!\n";
}

// Process the next request
void processRequest() {
    if (front == -1) {
        cout << "No pending requests.\n";
        return;
    }
    Request r = requestQueue[front];
    cout << "\nProcessing request for roll number " << r.roll << "\n";
    cout << "Request type: " << r.type << "\n";

    if (front == rear)
        front = rear = -1;
    else
        front = (front + 1) % QUEUE_SIZE;
    cout << "Request processed successfully!\n";
}

// Display pending requests
void displayRequests() {
    if (front == -1) {
        cout << "No pending requests.\n";
        return;
    }
    cout << "\nPending requests (FIFO order):\n";
    int i = front;
    while (true) {
        cout << "Roll: " << requestQueue[i].roll
             << " | Type: " << requestQueue[i].type << "\n";
        if (i == rear) break;
        i = (i + 1) % QUEUE_SIZE;
    }
}

// Free linked-list memory
void cleanup() {
    while (head != NULL) {
        Student *temp = head;
        head = head->next;
        delete temp;
    }
    studentCount = 0;
}

int main() {
    int choice;
    do {
        cout << "\n===== STUDENT MARKS MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Marks\n";
        cout << "5. Delete Student\n";
        cout << "6. Class Statistics\n";
        cout << "7. Display Merit List\n";
        cout << "8. Undo Last Marks Update\n";
        cout << "9. Add Result Request\n";
        cout << "10. Process Next Request\n";
        cout << "11. Display Pending Requests\n";
        cout << "0. Exit\n";
        choice = readInteger("Enter your choice: ", 0, 11);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateMarks(); break;
            case 5: deleteStudent(); break;
            case 6: classStatistics(); break;
            case 7: displayMeritList(); break;
            case 8: undoUpdate(); break;
            case 9: addRequest(); break;
            case 10: processRequest(); break;
            case 11: displayRequests(); break;
            case 0:
                cleanup();
                cout << "Exiting program...\n";
                break;
        }
    } while (choice != 0);

    return 0;
}