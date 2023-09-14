# **Appointment Scheduler Program**

## **Overview**
The C-program is designed to manage a list of appointments. It offers the following functionalities:
- Display all appointments for the current day.
- Display all appointments for a specific day, as entered by the user.
- Show the entire list of appointments.
- Add a new appointment.
- Search for an appointment in the list.
- Delete an appointment from the list.
- Delete the entire list of appointments.
- Exit the program.

The list of appointments is saved to a file before the program is closed. Upon starting the program, this file is read, and any past appointments are automatically deleted. Subsequently, all appointments for the current day are displayed.

## **Implementation Details**
- Appointments are stored using a C-structure named `Appointment`.
- Appointments are stored in a linked list sorted by their time.
- Two C-structures, `Element` and `List`, are used to store the linked list.
- Several C-functions are implemented to manage and display the linked list, such as `createList`, `clearList`, `insertElement`, `findElement`, `deleteElement`, `printAppointment`, and `printList`.
- A `menu` function displays eight menu options and reads input from the console.
- The `main` function reads the file and saves it in a linked list. Expired appointments are omitted. If the filename is not provided as a command-line argument, "termine.txt" is used by default.

## **Usage Instructions**
1. Compile the program using the GCC-Compiler with the options `-Wall` and `-pedantic-errors`.
   ```bash
    gcc -Wall -pedantic-errors -o scheduler.out  scheduler.c
   ```
3. Run the program. If you have a specific file you want to use, provide it as a command-line argument. If not, the program will default to using "termine.txt".
4. Use the menu to interact with the program and manage your appointments.
