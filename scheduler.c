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

void debugList(List *list)
{
    Element *current = list->head->next;
    while (current != NULL)
    {
        if (current->appointment == NULL)
        {
            printf("Error: Invalid appointment data\n");
        }
        else if (current->next == NULL)
        {
            printf("Reached end of list\n");
        }
        else
        {
            char buff[20];
            time_t start_time = current->appointment->start;
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&start_time));
            printf("Appointment: %s\n", current->appointment->description);
            printf("Start time: %s\n", buff);
        }
        current = current->next;
    }
}

// Function to create a new list of appointments
List *createList()
{
    List *list = malloc(sizeof(List));
    list->head = malloc(sizeof(Element));
    list->tail = malloc(sizeof(Element));

    list->head->next = list->tail;
    list->tail->next = NULL;

    return list;
}

/*void clearList(List *list)
{
    // Set the current pointer to the first element of the list
    Appointment* current = list->head->next;

    // Iterate over the list and free each node
    while (current != list->tail) {
        Appointment* temp = current;
        current = current->next;
        free(temp->text);
        free(temp);
    }

    // Reset the head and tail pointers to their initial values
    list->head->next = list->tail;
    list->tail->prev = list->head;
}*/

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


/*void findElement(List list, const char *searchText)
{
}*/

bool deleteElement(List *list, const char *deleteText)
{
    Element *previous = list->head;
    Element *current = previous->next;

    while (current->next != list->tail && strcmp(current->appointment->description, deleteText) != 0)
    {
        previous = current;
        current = current->next;
    }

    if (current->next != list->tail)
    {
        previous->next = current->next;
        free(current->appointment->description);
        free(current->appointment);
        free(current);
        return true;
    }
    else
    {
        return false;
    }
}

void printAppointment(Appointment *appointment)
{
    char buff[20];
    time_t start_time = appointment->start;
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&start_time));
    printf("%s: %s\n", buff, appointment->description);
}

void printList(List *list, int day, int month, int year)
{
    printf("Appointments on %d-%02d-%02d:\n", year, month, day);
    Element *current = list->head->next;
    while (current->next != list->tail)
    {
        time_t start_time = current->appointment->start;
        struct tm *start_tm = localtime(&start_time);
        if ((start_tm->tm_mday == day && start_tm->tm_mon + 1 == month && start_tm->tm_year + 1900 == year) || (day == 0 && month == 0 && year == 0))
        {
            char buff[20];
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", start_tm);
            printf("%s:\n\t\"%s\"\n\n", buff, current->appointment->description);
        }
        current = current->next;
    }
}

int main(void)
{
    List *test_list = createList();
    insertElement(test_list, 1676363073, "First Element");
    insertElement(test_list, 1676363114, "Second Element");
    insertElement(test_list, 1676363165, "Third Element");
    insertElement(test_list, 1673880593, "Fourth Element");
    insertElement(test_list, 1452955793, "Old Event");
    insertElement(test_list, 1452955793, "Old Event 2");
    insertElement(test_list, 1452955791, "First Element");

    // debugList(test_list);
    printList(test_list, 0, 0, 0);
    // printList(test_list, 16, 0, 2016);
    // printList(test_list, 14, 2, 2023);
    printf("Response of deleteElement(): %d\n", deleteElement(test_list, "First Element"));
    printf("Response of deleteElement(): %d\n", deleteElement(test_list, "First Element"));
    printf("Response of deleteElement(): %d\n", deleteElement(test_list, "First Element"));
    printf("Response of deleteElement(): %d\n", deleteElement(test_list, "Second Element"));

    printList(test_list, 0, 0, 0);

    printAppointment(test_list->head->next->next->appointment);
}