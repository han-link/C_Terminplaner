/*
Compiler command:
    gcc scheduler.c -o scheduler -Wall -pedantic-errors -g
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
    Structure to store the appointment
 */
typedef struct
{
    time_t start;
    char *description;
} Appointment;

/*
    This structure stores a list element together with a pointer to
    the successor
 */
typedef struct Element
{
    Appointment *appointment;
    struct Element *next;
} Element;

/*
    This structure stores the beginning and the end of the list
 */
typedef struct
{
    Element *head, *tail;
} List;

void debugList(List *list)
{
    Element *current = list->head->next;
    while (current != NULL) {
        if (current->appointment == NULL) {
            printf("Error: Invalid appointment data\n");
        } else {
            printf("Appointment: %s\n", current->appointment->description);
            printf("Start time: %ld\n", current->appointment->start);
        }
        current = current->next;
    }
}

// Function to create a new list of appointments
List *createList()
{
    List *list = malloc(sizeof(List));
    Element *head = malloc(sizeof(Element));
    Element *tail = malloc(sizeof(Element));

    list->head = head;
    list->tail = tail;
    head->next = tail;
    tail->next = NULL;

    return list;
}

void clearList(List list)
{
         
}

void insertElement(List *list, time_t time, const char *text)
{
    // create a new Appointment struct and assign its fields
    Appointment *new_appointment = malloc(sizeof(Appointment));
    new_appointment->start = time;
    new_appointment->description = text;

    // create a new Element struct and assign its fields
    Element *new_element = malloc(sizeof(Element));
    new_element->appointment = new_appointment;

    // insert the new element into the list
    if (list->head == NULL)
    {
        // if the list is empty, set the head and tail pointers to the new element
        new_element->next = list->tail;
        list->head->next = new_element;
    } 
    else 
    {
        // otherwise, find the correct position to insert the new element
        Element *current = list->head->next;
        while (current->next != NULL && current->next->appointment->start <= time) 
        {
            current = current->next;
        }
        // insert the new element after the current position and before the next position
        new_element->next = current->next;
        current->next = new_element;
    }
}


void /* should return a pointer */ findElement(List list, const char *searchText)
{
}

bool deleteElement(List list, const char *deleteText)
{
    return false;
}

void printAppointment(Appointment p)
{
}

void printList(List list, int day, int month, int year)
{
}

int main(void)
{
    List *test_list = createList();
    insertElement(test_list, 1676363073, "Erstes Element");
    // insertElement(test_list, 1676363114, "Zweites Element");
    // insertElement(test_list, 1676363165, "Drittes Element");
    debugList(test_list);
}