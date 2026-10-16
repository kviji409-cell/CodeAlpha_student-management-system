Student Management System
A simple command-line Student Management System written in C. It lets you add, display, search, update, and delete student records, with data persisted to a binary file (students.dat).
Features
Add a new student (ID, name, course, marks)
Display all students in a formatted table
Search for a student by ID
Update a student's name, course, and marks
Delete a student by ID
Duplicate ID prevention
Input validation (marks must be 0–100)
Persistent storage via binary file (students.dat)
Project Structure
student-management/
├── task3_student_management.c   # Main source file
├── Makefile                     # Build configuration
├── .gitignore                   # Files to exclude from Git
└── README.md                    # This file
Requirements
GCC (or any C99-compatible compiler)
Linux / macOS / Windows (with MinGW or WSL)
Build & Run
Using Make
make
./student_mgmt
Manual Compilation
gcc -Wall -Wextra -std=c99 -o student_mgmt task3_student_management.c
./student_mgmt
On Windows (MinGW)
gcc -Wall -Wextra -std=c99 -o student_mgmt.exe task3_student_management.c
student_mgmt.exe
Usage
When you run the program, you will see this menu:
=== Student Management System ===
1. Add student
2. Display all students
3. Search student
4. Update student
5. Delete student
0. Exit
Choose:
Example Session
Choose: 1
Student ID: 101
Name: Alice Johnson
Course: Computer Science
Marks (0-100): 88.5
Student added successfully.

Choose: 2

ID       Name                      Course               Marks
---------------------------------------------------------------
101      Alice Johnson             Computer Science     88.50
Data Storage
Student records are stored in students.dat in the same directory as the executable. The file uses a raw binary format (fixed-size structs). Do not commit this file to Git — it is listed in .gitignore.
Limitations
No sorting or pagination for the display view
Binary file format is not portable across platforms with different struct padding
Single-file design; can be extended with separate header/source files for larger projects
License
This project is released under the MIT License.
