/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/
#ifndef EVENT_H
#define EVENT_H

#include <stdlib.h>

struct due_date;
typedef struct due_date{
	size_t month;
	size_t date;
	size_t hour;
}due_date;

struct event;
typedef struct event {
	due_date time;
	struct event* next;
	char* activity;	
}event;


// Creates an event with given activity and time. next is by
// default set to NULL;
event* event_create(char* activity, due_date time);

// Inserts an event into its proper location
void event_insert(event* new_event, event* head);

// Remove an event from the list
void event_remove(event* target, event* head);

// Find an event with activity name
event* event_find(char* activity, event* head);

// Destroys a single event and deallocates its memory
void event_destroy(event* target);

#endif