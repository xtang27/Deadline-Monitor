/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

// Main function of Deadline-Monitor

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "dl.h"

int main(int argc, char* argv[]){
	deadlines *dl = create_from_file("data.bin");
	int ch;
  	ch = getopt(argc, argv, "a:r:");
  	size_t mm, dd, hh;
  	switch(ch){
	  	case 'a':
	  		sscanf(optarg, "%zu/%zu/%zu", &mm, &dd, &hh);
	  		argc -= optind;
	  		argv += optind;
	  		char* event_name = calloc(1,strlen(*argv)+1);
	  		strcpy(event_name, *argv);
	  		due_date due;
	  		due.month = mm;
	  		due.date = dd;
	  		due.hour = hh;
	  		event* new_event = event_create(event_name, due);
	  		deadline_insert(dl, new_event);
	  		break;
	  	case 'r':
	  		deadline_remove(dl,event_find(optarg,dl->head));
	  		break;
	  	case -1:
	  		deadlines_display_all(dl);
	  		break;
	  	default:
	  		print_usage();
  	}
  	write_to_file("data.bin", dl);
  	deadlines_destroy(dl);
	return 0;
}