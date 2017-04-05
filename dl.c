/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "dl.h"

deadlines* create_from_file(char* filename){
	FILE *file = fopen(filename, "r");
	if(!file){
		perror("fail to open");
		return NULL;
	}
	deadlines *dl = malloc(sizeof(deadlines));
	dl->size = 0;
	dl->head = NULL;
	//event* tail = NULL;
	while(1){
		event *e = malloc(sizeof(event));
		size_t rc = fread(&e->time, sizeof(size_t), 3,file);
		if(rc < 3){
			free(e);
			return dl;
		}
		char buff[256];
		size_t c = 0;
		char read;
		fread(&read, 1, 1, file);
		while(read != '\0'){
			buff[c] = read;
			c++;
			fread(&read, 1, 1, file);
		}
		buff[c] = '\0';
		c++;
		char *act = malloc(c);
		e->activity = act;

		strncpy(act, buff,c);
		deadline_insert(dl, e);
	}

}

void deadline_insert(deadlines* dl, event* input){
	if(dl->head == NULL)dl->head = input;
	else if(earlier(input, dl->head)){
		input->next = dl->head;
		dl->head = input;
	}
	else{
	event_insert(input, dl->head);
	}
	(dl->size)++;
}

void deadline_remove(deadlines* dl, event* input){
	if(strcmp(input->activity, (dl->head)->activity)==0){
		event_destroy(dl->head);
		dl->head = NULL;
	}else{
	event_remove(input, dl->head);
	}
	(dl->size)--;
}

void deadlines_destroy(deadlines* dl){
	while(dl->head != NULL){
		event* temp = dl->head;
		dl->head = dl->head->next;
		event_destroy(temp);
		dl->size--;
	}
}

void write_to_file(char* filename, deadlines* dl){
		FILE* file = fopen(filename, "w");
		event* e = dl->head;
		while(e != NULL){
			fwrite(e, sizeof(struct due_date), 1, file);
			fwrite(e->activity, 1, strlen(e->activity), file);
			fwrite("\0",1,1,file);
			e = e->next;
		}
		fclose(file);

}

void deadlines_display_all(deadlines* dl){
	event* pos = dl->head;
	int i = 1;
	while(pos != NULL){
		printf("%d: %s, %zu/%zu %zu:00. \n", i, pos->activity, pos->time.month, pos->time.date, pos->time.hour);
		i++;
		pos = pos->next;
	}
	return;
}

void deadlines_send(){
	int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, "1234", &hints, &result);
    if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(1);
    }
    int optval = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(sock_fd, 10) != 0) {
        perror("listen()");
        exit(1);
    }
    
    struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
    printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));

    printf("Waiting for connection...\n");
    int client_fd = accept(sock_fd, NULL, NULL);
    printf("Connection made: client_fd=%d\n", client_fd);

    FILE *file = fopen("data.bin", "r");
    if(!file){
		perror("fail to open data.bin");
		// return NULL;
	}
	char buff[1024];
	size_t c = 0;
	char read;
	ssize_t read_ret = fread(&read, 1, 1, file);
	while(read != 0){
		buff[c] = read;
		c++;
		read_ret = fread(&read, 1, 1, file);
	}
	printf("Read %zd chars from file\n", read_ret);
    int write_ret = write(client_fd, buff, c);
    //buffer[len] = '\0';

    printf("Wrote %d chars\n", write_ret);
    printf("===\n");
    // printf("%s\n", buffer);
    fclose(file);
    freeaddrinfo(&hints);

    // return 0;
}

void print_usage(){
	printf("Wrong Usage. \n");
}
