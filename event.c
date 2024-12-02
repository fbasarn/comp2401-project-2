#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

/* Event functions */

/**
 * Initializes an `Event` structure.
 *
 * Sets up an `Event` with the provided system, resource, status, priority, and amount.
 *
 * @param[out] event     Pointer to the `Event` to initialize.
 * @param[in]  system    Pointer to the `System` that generated the event.
 * @param[in]  resource  Pointer to the `Resource` associated with the event.
 * @param[in]  status    Status code representing the event type.
 * @param[in]  priority  Priority level of the event.
 * @param[in]  amount    Amount related to the event (e.g., resource amount).
 */
void event_init(Event *event, System *system, Resource *resource, int status, int priority, int amount) {
    event->system = system;
    event->resource = resource;
    event->status = status;
    event->priority = priority;
    event->amount = amount;
}

/* EventQueue functions */

/**
 * Initializes the `EventQueue`.
 *
 * Sets up the queue for use, initializing any necessary data (e.g., semaphores when threading).
 *
 * @param[out] queue  Pointer to the `EventQueue` to initialize.
 */
void event_queue_init(EventQueue *queue) {
    queue->head = NULL;
    queue->size = 0;
}

/**
 * Cleans up the `EventQueue`.
 *
 * Frees any memory and resources associated with the `EventQueue`.
 * 
 * @param[in,out] queue  Pointer to the `EventQueue` to clean.
 */
void event_queue_clean(EventQueue *queue) {

    EventNode *curr = queue->head;

    while(curr != NULL){
        EventNode *temp = curr;
        curr = curr->next;
        free(temp);
    }

    queue->size = 0;
}

/**
 * Pushes an `Event` onto the `EventQueue`.
 *
 * Adds the event to the queue in a thread-safe manner, maintaining priority order (highest first).
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[in]     event  Pointer to the `Event` to push onto the queue.
 */
void event_queue_push(EventQueue *queue, const Event *event) {
    

    EventNode *newNode = (EventNode*)malloc(sizeof(EventNode));
    newNode->event = *event;
    newNode->next = NULL;

    if(queue->head == NULL) queue->head = newNode;
    else if(queue->head->event.priority < newNode->event.priority){
        newNode->next = queue->head;
        queue->head = newNode;
    }else{
        EventNode *curr = queue->head;
        EventNode *temp = NULL;
        while (curr != NULL && curr->event.priority >= newNode->event.priority) {
            temp = curr;
            curr = curr->next;
        }

        temp->next = newNode;
        newNode->next = curr;
    }
    queue->size++;
}

/**
 * Pops an `Event` from the `EventQueue`.
 *
 * Removes the highest priority event from the queue in a thread-safe manner.
 *
 * @param[in,out] queue  Pointer to the `EventQueue`.
 * @param[out]    event  Pointer to the `Event` structure to store the popped event.
 * @return               Non-zero if an event was successfully popped; zero otherwise.
 */
int event_queue_pop(EventQueue *queue, Event *event) {
    //if there are any events in the queue, remove the event at the head of the queue and copy its
    //event data into the *event argument. Return a nonzero value (e.g., 1);
    //4.2. If there were no events in the queue, return 0.
    if(queue->head == NULL) return 0;
    else{
        *event = queue->head->event;
        
        EventNode *temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
        queue->size--;
        return 1;
    }
}
