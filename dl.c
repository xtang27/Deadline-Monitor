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
#include <netinet/in.h>
#include <ifaddrs.h>

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

void deadline_push(char* host, char* port, char* user_name){
	int s;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* IPv4 only */
	hints.ai_socktype = SOCK_STREAM; /* TCP */

	s = getaddrinfo(host, port, &hints, &result);
	if (s != 0) {
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
	}

	if(connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1){
        perror("connect");
        exit(2);
    }
    char buf[36];
    memset(buf, 0, 36);
    sprintf(buf, "%s\n", user_name);   
    write(sock_fd, buf, strlen(user_name)+1);
    write(sock_fd, "PUSH\n", strlen("PUSH\n"));
    FILE* file = fopen("data.bin", "r");
    char buff[1024];
	size_t c = 0;
	char read;
	ssize_t read_ret = fread(&read, 1, 1, file);
	while(read_ret != 0){
		buff[c] = read;
		c++;
		read_ret = fread(&read, 1, 1, file);
	}
	
	printf("Read %zd bytes from file\n", c);
    int write_ret = write(sock_fd, buff, c);
    //buffer[len] = '\0';

    printf("Wrote %d bytes\n", write_ret);
    printf("===\n");
    // printf("%s\n", buffer);
    fclose(file);
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);

}

void deadlines_send(char* port){
	int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    //char port[10];
    //memset(port, 0 ,10);
    //sprintf(port, "%d", port_num);
    s = getaddrinfo(NULL, port, &hints, &result);
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
    
    int required_family = AF_INET; // Change to AF_INET6 for IPv6
    struct ifaddrs *myaddrs, *ifa;
    getifaddrs(&myaddrs);
    char mhost[256], mport[256];
    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
        int family = ifa->ifa_addr->sa_family;
        if (family == required_family && ifa->ifa_addr) {
            if (0 == getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                sizeof(struct sockaddr_in6),
                                mhost, sizeof(mhost), mport, sizeof(mport)
                                 , NI_NUMERICHOST | NI_NUMERICSERV  ))
                puts(mhost);
        }
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
	while(read_ret != 0){
		buff[c] = read;
		c++;
		read_ret = fread(&read, 1, 1, file);
	}
	
	printf("Read %zd bytes from file\n", read_ret);
    int write_ret = write(client_fd, buff, c);
    //buffer[len] = '\0';

    printf("Wrote %d bytes\n", write_ret);
    printf("===\n");
    // printf("%s\n", buffer);
    fclose(file);
    //freeaddrinfo(&hints);

    // return 0;
}

void deadlines_receive(char* host, char* port, char* user_name){
	int s;
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* IPv4 only */
	hints.ai_socktype = SOCK_STREAM; /* TCP */

	s = getaddrinfo(host, port, &hints, &result);
	if (s != 0) {
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
	}

	if(connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1){
        perror("connect");
        exit(2);
    }
    FILE* file = fopen("data.bin", "w");
    size_t c = 0;
    char rr[1024];

    if(user_name == NULL){
	    ssize_t read_ret = read(sock_fd, &rr, 1024);
	    while(read_ret != 0){
	    	fwrite(&rr, read_ret, 1, file);
	    	memset(rr, 0, 1024);
	    	c += read_ret;
	    	read_ret = read(sock_fd, &rr, 1024);
	    	
	    }
	    printf("Read %zd bytes from server\n", c);
	    fclose(file);
	}else{
		char request[36];
		memset(request, 0, 36);
		sprintf(request, "%s\n", user_name);
		write(sock_fd, request, strlen(request));
		write(sock_fd, "PULL\n", strlen("PULL\n"));
		shutdown(sock_fd, SHUT_WR);
		char respond;
		read(sock_fd, &respond, 1);
		if(respond == '1'){
			ssize_t read_ret = read(sock_fd, rr, 1024);
		    while(read_ret != 0){
		    	fwrite(rr, read_ret, 1, file);
		    	memset(rr, 0, 1024);
		    	c += read_ret;
		    	read_ret = read(sock_fd, rr, 1024);
		    	
		    }
		    printf("Read %zd bytes from server\n", c);
		}else{
			printf("No data found for user: %s\n", user_name);
		}
	    fclose(file);

	}
    //freeaddrinfo(&hints);
    //freeaddrinfo(result);

}

void print_usage(){
	printf("Wrong Usage. \n");
}
