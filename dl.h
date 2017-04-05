/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

#ifndef DL_H
#define DL_H
#include "event.h"

struct deadlines;
typedef struct deadlines {
	size_t size; // number of events in the linked list
	event* head; // head of the linked list
}deadlines;

// Creates a new deadlines object from a file input
deadlines* create_from_file(char* filename);

// Insert a new event into the linked list
void deadline_insert(deadlines* dl, event* input);

// Remove an event from the linked list
void deadline_remove(deadlines* dl, event* input);

// Destroys and free all memories used in a deadlines object
void deadlines_destroy(deadlines* input);

// Saves a deadlines object into a file
void write_to_file(char* filename, deadlines* dl);

// Print deadlines to stdout according to args
void deadlines_display_all(deadlines* dl);

// Set up TCP server to send out data to another machine
void deadlines_send()

void print_usage();

#endif


