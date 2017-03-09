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
	while(1){
		char* cmd = NULL;
		size_t length = 0;
		int read = 0;
		read = getline(&cmd, &length, stdin);
		cmd[strlen(cmd)] = '\0';
		if(strcmp(cmd, "exit\n") == 0){
			deadlines_destroy(dl);
			exit(0);
		}
		else if(strcmp(cmd, "show\n") == 0){
			deadlines_display_all(dl);
		}
		else if(strcmp(strtok(cmd, " "),"add")){
			size_t mm = atoi(strtok(NULL, " "));
			size_t dd = atoi(strtok(NULL, " "));
			size_t hh = atoi(strtok(NULL, " "));
			char* act = strtok(NULL, " ");
			struct due_date d;
			d.month = mm;
			d.date = dd;
			d.hour = hh;
			deadline_insert(dl, event_create(act, d));
		}
		else if(strcmp(strtok(cmd, " "),"rm")){
			deadline_remove(dl, event_find(strtok(NULL, " "), dl->head));
		}
		else{
			print_usage();
		}
	}
	return 0;
}