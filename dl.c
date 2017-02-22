/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/
struct deadlines {
	size_t size; // number of events in the linked list
	event* head; // head of the linked list
};

deadlines* create_from_file(char* filename){

}

void deadline_insert(deadlines* dl, event* input){
	event_insert(input, dl->head);
	(dl->size)++;
}

void deadline_remove(deadlines* dl, event* input){
	event_remove(input, dl->head);
	(dl->size)--;
}

void deadlines_destroy(deadlines* input){
	while(head != NULL){
		event* temp = head;
		head = head->next;
		event_destroy(temp);
		size--;
	}
}

void write_to_file(char* filename, deadlines* dl){

}

void deadlines_display_all(deadlines* dl){
	event* pos = dl->head;
	int i = 1;
	while(pos != NULL){
		printf("%d: %s, %zu/%zu %zu:00, \n", i, pos->activity, pos->time.month, pos->time.date, pos->time.hour);
		i++;
	}
}

void print_usage(){
	printf("//TODO\n");
}
