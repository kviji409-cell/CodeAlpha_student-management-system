#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.dat"
#define NAME_LEN 50
#define COURSE_LEN 40

typedef struct {
    int id;
    char name[NAME_LEN];
    char course[COURSE_LEN];
    float marks;
} Student;

static void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static int readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) return 0;
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

static int idExists(int id) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    if (!fp) return 0;
    while (fread(&s, sizeof s, 1, fp) == 1) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

static void addStudent(void) {
    Student s;
    FILE *fp;

    printf("Student ID: ");
    if (scanf("%d", &s.id) != 1) { clearInput(); puts("Invalid ID."); return; }
    clearInput();

    if (idExists(s.id)) {
        puts("That student ID already exists.");
        return;
    }
    if (!readLine("Name: ", s.name, sizeof s.name) ||
        !readLine("Course: ", s.course, sizeof s.course)) {
        puts("Input error.");
        return;
    }
    printf("Marks (0-100): ");
    if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
        clearInput();
        puts("Invalid marks. Enter a value from 0 to 100.");
        return;
    }
    clearInput();

    fp = fopen(FILE_NAME, "ab");
    if (!fp) { perror("Could not open student file"); return; }
    if (fwrite(&s, sizeof s, 1, fp) != 1) puts("Could not save student.");
    else puts("Student added successfully.");
    fclose(fp);
}

static void displayStudents(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Student s;
    int count = 0;
    if (!fp) { puts("No student records found yet."); return; }

    printf("\n%-8s %-25s %-20s %-8s\n", "ID", "Name", "Course", "Marks");
    printf("---------------------------------------------------------------\n");
    while (fread(&s, sizeof s, 1, fp) == 1) {
        printf("%-8d %-25s %-20s %-8.2f\n", s.id, s.name, s.course, s.marks);
        count++;
    }
    if (count == 0) puts("No student records found.");
    fclose(fp);
}

static void searchStudent(void) {
    int id, found = 0;
    Student s;
    FILE *fp;

    printf("Enter student ID to search: ");
    if (scanf("%d", &id) != 1) { clearInput(); puts("Invalid ID."); return; }
    clearInput();

    fp = fopen(FILE_NAME, "rb");
    if (!fp) { puts("No student records found yet."); return; }
    while (fread(&s, sizeof s, 1, fp) == 1) {
        if (s.id == id) {
            printf("ID: %d\nName: %s\nCourse: %s\nMarks: %.2f\n",
                   s.id, s.name, s.course, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) puts("Student not found.");
}

static void updateStudent(void) {
    int id, found = 0;
    Student s;
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) { puts("No student records found yet."); return; }

    printf("Enter student ID to update: ");
    if (scanf("%d", &id) != 1) {
        clearInput(); fclose(fp); puts("Invalid ID."); return;
    }
    clearInput();

    while (fread(&s, sizeof s, 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            if (!readLine("New name: ", s.name, sizeof s.name) ||
                !readLine("New course: ", s.course, sizeof s.course)) {
                puts("Input error."); fclose(fp); return;
            }
            printf("New marks (0-100): ");
            if (scanf("%f", &s.marks) != 1 || s.marks < 0 || s.marks > 100) {
                clearInput(); fclose(fp); puts("Invalid marks."); return;
            }
            clearInput();
            fseek(fp, -(long)sizeof s, SEEK_CUR);
            if (fwrite(&s, sizeof s, 1, fp) == 1) puts("Student updated.");
            else puts("Could not update student.");
            break;
        }
    }
    fclose(fp);
    if (!found) puts("Student not found.");
}

static void deleteStudent(void) {
    int id, found = 0;
    Student s;
    FILE *in, *out;

    printf("Enter student ID to delete: ");
    if (scanf("%d", &id) != 1) { clearInput(); puts("Invalid ID."); return; }
    clearInput();

    in = fopen(FILE_NAME, "rb");
    if (!in) { puts("No student records found yet."); return; }
    out = fopen("students.tmp", "wb");
    if (!out) { fclose(in); puts("Could not create temporary file."); return; }

    while (fread(&s, sizeof s, 1, in) == 1) {
        if (s.id == id) found = 1;
        else if (fwrite(&s, sizeof s, 1, out) != 1) {
            puts("Error writing records.");
            fclose(in); fclose(out); remove("students.tmp"); return;
        }
    }
    fclose(in);
    fclose(out);

    if (found) {
        if (remove(FILE_NAME) != 0 || rename("students.tmp", FILE_NAME) != 0)
            puts("Could not replace student data file.");
        else puts("Student deleted.");
    } else {
        remove("students.tmp");
        puts("Student not found.");
    }
}

int main(void) {
    int choice;
    do {
        printf("\n=== Student Management System ===\n");
        printf("1. Add student\n2. Display all students\n3. Search student\n");
        printf("4. Update student\n5. Delete student\n0. Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            clearInput();
            puts("Please enter a number.");
            continue;
        }
        clearInput();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: puts("Goodbye."); break;
            default: puts("Invalid choice.");
        }
    } while (choice != 0);

    return 0;
}
