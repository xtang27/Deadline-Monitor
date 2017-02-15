/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

struct deadlines;
typedef struct deadlines deadlines;

// Creates a new deadlines object from a file input
deadlines* create_from_file(char* filename);

// Insert a new event into the linked list
void insert_event(event* input);

// Remove an event from the linked list
void remove_event(event* input);

// Destroys and free all memories used in a deadlines object
void deadlines_destroy(deadlines* input);

// Saves a deadlines object into a file
void write_to_file(char* filename, deadlines* dl);

// Print deadlines to stdout according to args
void deadlines_display(char* args, deadlines* dl);


