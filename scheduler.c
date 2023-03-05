#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    time_t start;
    char *description;
} Appointment;

typedef struct Element
{
    Appointment *appointment;
    struct Element *next;
} Element;

typedef struct
{
    Element *head, *tail;
} List;


List *createList(void)
{
    List *list = malloc(sizeof(List));
    list->head = malloc(sizeof(Element));
    list->tail = malloc(sizeof(Element));

    list->head->next = list->tail;
    list->tail->next = NULL;

    return list;
}

void clearList(List *list)
{
    // Set the current pointer to the first element of the list
    Element *current = list->head->next;

    // Iterate over the list and free each node
    while (current != list->tail) {
        Element *temp = current;
        current = current->next;
        free(temp->appointment->description);
        free(temp);
    }

    // Reset the head and tail pointers to their initial values
    list->head->next = list->tail;
}

void insertElement(List *list, time_t time, const char *text)
{
    // create a new Appointment struct and assign its fields
    Appointment *new_appointment = malloc(sizeof(Appointment));
    new_appointment->start = time;
    new_appointment->description = malloc(strlen(text) + 1);
    strcpy(new_appointment->description, text);

    // create a new Element struct and assign its fields
    Element *new_element = malloc(sizeof(Element));
    new_element->appointment = new_appointment;

    // insert the new element into the list
    if (list->head->next == list->tail)
    {
        // if the list is empty, set the head and tail pointers to the new element
        list->head->next = new_element;
        new_element->next = list->tail;
    } 
    else 
    {
        // otherwise, find the correct position to insert the new element
        Element *current = list->head;
        while (current->next != list->tail && current->next->appointment->start <= time)
        {
            current = current->next;
        }
        // insert the new element after the current position and before the next position
        new_element->next = current->next;
        current->next = new_element;
    }
}

Element* findElement(List *list, const char *searchText)
{
    Element *current = list->head->next;

    while (current != list->tail)
    {
        // Compare the description with the searchText
        if(strcmp(current->appointment->description, searchText) == 0)
        {
            return current;
        }
        else
        {
            current = current->next;
        }
    }
    return NULL;
}

bool deleteElement(List *list, const char *deleteText)
{
    // store the previous element because element->next need to be defined new
    Element *previous = list->head;
    Element *current = previous->next;

    while (current->next != list->tail)
    {
        if(strcmp(current->appointment->description, deleteText) == 0)
        {
            // Connect perviouse with next element
            previous->next = current->next;
            free(current->appointment->description);
            free(current->appointment);
            free(current);
            return true;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
    return false;
}

void printAppointment(Appointment *appointment)
{
    char buff[20];
    time_t start_time = appointment->start;
    strftime(buff, 20, "%Y-%m-%d", localtime(&start_time));
    printf("Appointment:\n%s\n%s\n", buff, appointment->description);
}

void printList(List *list, int day, int month, int year)
{
    if(day == 0 && month == 0 && year == 0)
    {
        printf("All Apponintments:\n");
    }
    else
    {
        printf("\nAppointments on %d-%02d-%02d:\n\n", year, month, day);
    }
    Element *current = list->head->next;
    while (current != list->tail)
    {
        time_t start_time = current->appointment->start;
        struct tm *start_tm = localtime(&start_time);
        if ((start_tm->tm_mday == day && start_tm->tm_mon + 1 == month && start_tm->tm_year + 1900 == year) || (day == 0 && month == 0 && year == 0))
        {
            char buff[20];
            strftime(buff, 20, "%Y-%m-%d", localtime(&start_time));
            printf("%s:\n%s\n", buff, current->appointment->description);
        }
        current = current->next;
    }
}

int readInt() {
    char input[20];
    fgets(input, 20, stdin);
    int number = strtol(input, NULL, 10);
    return number;
}

void writeListToFile(List *list, const char *filename)
{
    FILE *file = fopen(filename, "w");

    Element *current = list->head->next;
    while (current != list->tail)
    {
        Appointment *appointment = current->appointment;
        fprintf(file, "%ld\n%s\n", (long)appointment->start, appointment->description);
        current = current->next;
    }

    fclose(file);

    // Free the memory of the list
    current = list->head->next;
    while (current != list->tail) {
        Element *next = current->next;
        free(current->appointment->description);
        free(current->appointment);
        free(current);
        current = next;
    }
    free(list->head);
    free(list->tail);
}

time_t unix_time_to_time_t(const char* unix_time_str)
{
    // Convert string to long integer
    long unix_time = strtol(unix_time_str, NULL, 10);

    // Create time_t object from Unix timestamp
    time_t time = (time_t)unix_time;

    return time;
}

void menu(List *list, char* filename) {
    int option;
    while (true)
    {
        printf("****************** MENU ******************\n");
        printf("1. Show all appointments today\n");
        printf("2. Show all appointments on a specific day\n");
        printf("3. Show all appointments\n");
        printf("4. Insert a appointment\n");
        printf("5. Search for a appointment\n");
        printf("6. Delete a appointment\n");
        printf("7. Delete the list\n");
        printf("8. Exit\n");
        printf("Enter your choice:\n");
        fflush(stdout);

        option =  readInt();

        switch (option)
        {
            case 1: // Show all appointments today
            {
                time_t current_time = time(NULL);
                struct tm *now_tm = localtime(&current_time);
                printList(list, now_tm->tm_mday, now_tm->tm_mon + 1, now_tm->tm_year + 1900);
                break;
            }
            case 2: // Show all appointments on a specific day
            {
                int year, month, day;

                printf("Enter the year YYYY: \n");
                year = readInt();

                printf("Enter the month mm: \n");\
                month = readInt();
                printf("Enter the day: \n");
                day = readInt();

                printList(list, day, month, year);
                break;

            }
            case 3: // Show all appointments
                printList(list, 0, 0, 0);
                break;
            case 4: // Insert a appointment
            {
                int year, month, day;
                char appointment_description[256];
                struct tm appointment_time = {0};

                printf("Enter the month year YYYY: \n");\
                year = readInt();
                printf("Enter the month mm: \n");\
                month = readInt();
                printf("Enter the day dd: \n");
                day = readInt();

                appointment_time.tm_year = year - 1900;
                appointment_time.tm_mon = month -1;
                appointment_time.tm_mday = day;

                printf("Enter appointment_description:\n");
                fgets(appointment_description, 256, stdin);

                // Remove newline character from input
                appointment_description[strcspn(appointment_description, "\n")] = 0;

                time_t appointment_timestamp = mktime(&appointment_time);
                // Every element is inserted at 12:00:00
                insertElement(list, appointment_timestamp + 43200, appointment_description);

                printf("Appointment added successfully!\n");

                break;
            }
            case 5: // Search for an appointment
            {
                char searchText[256];
                printf("Enter appointment description:\n");
                fgets(searchText, 256, stdin);
                searchText[strcspn(searchText, "\n")] = 0;
                Element *element = findElement(list, searchText);
                if(element != NULL)
                {
                    printAppointment(element->appointment);
                }
                else
                {
                    printf("No matching appointment found\n");
                }
                break;
            }
            case 6: // Delete an appointment
            {
                char deleteText[256];
                printf("Enter appointment description:\n");
                fgets(deleteText, 256, stdin);
                deleteText[strcspn(deleteText, "\n")] = 0;

                if(deleteElement(list, deleteText) == true)
                {
                    printf("Element was deleted successfull\n");
                }
                else
                {
                    printf("No matching appointment found\n");
                }
                break;
            }
            case 7:// Delete the list
                clearList(list);
                break;
            case 8: // Exit
                writeListToFile(list,filename);
                exit(0);
            default:
                fprintf(stderr, "Error: This is no option in the menu\n");
                fflush(stderr);
        }
    }
}

time_t start_of_day() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return mktime(tm);
}

int main(void)
{
    FILE *textFile;
    char filename[256];
    const char DEFAULT_FILENAME[256] = "termine.txt";
    List *appointment_list = createList();

    printf("Enter filename or just hit enter for default name:\n");

    fgets(filename, 256, stdin);

    if(strlen(filename) > 1)
    {
        filename[strcspn(filename, "\n")] = 0;
    }
    else
    {
        strcpy(filename, DEFAULT_FILENAME);
    }

    printf("Filname: %s\n", filename);
    textFile = fopen(filename, "r");


    if (textFile != NULL)
    {
        char time_str[256], description_str[256];

        while (fgets(time_str, 256, textFile) && fgets(description_str, 256, textFile))
        {
            time_t today = start_of_day();
            time_t appointmentStart = unix_time_to_time_t(time_str);
            if(appointmentStart > today)
            {
                description_str[strcspn(description_str, "\n")] = 0;
                insertElement(appointment_list,appointmentStart,description_str);
            }
        }
        printf("File was read\n");
        fclose(textFile);
    }
    else
    {
        printf("failed to open the file\n");
    }
    time_t current_time = time(NULL);
    struct tm *now_tm = localtime(&current_time);
    printList(appointment_list, now_tm->tm_mday, now_tm->tm_mon + 1, now_tm->tm_year + 1900);
    menu(appointment_list, filename);

    return 0;
}
