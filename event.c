/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

struct event {
	time_t time;
	char* activity;
	event* next;
};


event* event_create(char* activity, time_t time){}

void event_insert(event* new_event, event* head){}

void event_remove(event* target, event* head){}

event* event_find(char* activity, event* head){}

void event_destroy(event* target){}
