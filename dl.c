/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/
struct deadlines {
	size_t size; // number of events in the linked list
	event* head; // head of the linked list
};

deadlines* create_from_file(char* filename){}

void insert_event(event* input){}

void remove_event(event* input){}

void deadlines_destroy(deadlines* input){}

void write_to_file(char* filename, deadlines* dl){}

void deadlines_display(char* args, deadlines* dl){}
