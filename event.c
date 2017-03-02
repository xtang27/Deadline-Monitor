/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

#include "event.h"
#include <string.h>
#include <stdio.h>

event* event_create(char* activity, due_date time){
	event* new_event = malloc(sizeof(char*) + sizeof(due_date) + sizeof(event*));
	new_event->time.month = time.month;
	new_event->time.date = time.date;
	new_event->time.hour = time.hour;
	new_event->next = NULL;
	new_event->activity = malloc(strlen(activity)+1);
	strcpy(new_event->activity, activity);
	return new_event;
}

size_t earlier(event* a, event* b){
	if(a->time.month > b->time.month){
		return 0;
	}
	if(a->time.date > b->time.date){
		return 0;
	}
	if(a->time.hour > b->time.hour){
		return 0;
	}
	return 1;
}

void event_insert(event* new_event, event* head){
	event* prev = NULL;
	event* position = head;
	if(head == NULL){
		head = new_event;
	}
	while(earlier(position, new_event) && position->next != NULL){
		prev = position;
		position = position->next;
	}
	if(prev == NULL){
		new_event->next = head;
		head = new_event;
	}else if(position->next == NULL && earlier(position, head)){
		position->next = new_event;
	}else{
		prev->next = new_event;
		new_event->next = position;
	}
}

void event_remove(event* target, event* head){
	if(target == head){
		head = head->next;
		event_destroy(target);
	}else{
		event* position = head;
		while(position->next != target){
			position = position->next;
		}
		position->next = target->next;
		event_destroy(target);
	}
}

event* event_find(char* activity, event* head){
	event* position = head;
	while(position != NULL){
		if(strcmp(position->activity, activity) == 0){
			return position;
		}
	}
	return position;
}

void event_destroy(event* target){
	free(target->activity);
	free(target);
	target = NULL;
}
