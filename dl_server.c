/**
 * Deadline-Monitor
 * CS 241 Honor: Xiao Tang
 **/

// Deadline-Monitor Server Code

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

int main(int argc, char* argv[]){
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
    s = getaddrinfo(NULL, argv[1], &hints, &result);
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

    printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));

    printf("Waiting for connection...\n");
    while(1){
	    int client_fd = accept(sock_fd, NULL, NULL);
	    printf("Connection made: client_fd=%d\n", client_fd);
	    FILE *user_input = fdopen(client_fd, "r+");
	    char *user_name = NULL;
	    size_t user_name_len = 0;
	    getline(&user_name, &user_name_len, user_input);
	    if(user_name[strlen(user_name)-1] == '\n')user_name[strlen(user_name)-1] = '\0';
	    printf("Client ID = %s\n", user_name);
	    char *request = NULL;
	    size_t request_len = 0;
	    getline(&request, &request_len, user_input);
	    if(strcmp(request, "PULL\n") == 0){
            printf("=====Sending data to user=====\n");
	    	char user_filename[36];
	    	memset(user_filename, 0, 36);
	    	sprintf(user_filename, "%s.bin", user_name);
	    	FILE *file = fopen(user_filename, "r");
            if(file == NULL){
                write(client_fd, "0", 1);
                shutdown(client_fd, SHUT_RDWR);
                close(client_fd);
            }else{
                write(client_fd, "1", 1);
                char buf[1024];
                memset(buf, 0, 1024);
                size_t bytes_read = fread(buf, 1, 1024, file);
                size_t bytes_sent = 0;
                while(bytes_read != 0){
                    ssize_t sent = 0;
                    while(sent != bytes_read){
                        sent = write(client_fd, buf + sent, bytes_read - sent);
                    }
                    bytes_sent += sent;
                    memset(buf, 0, 1024);
                    bytes_read = fread(buf, 1, 1024, file);
                }
                printf("Sent %zu bytes \n", bytes_sent);
                shutdown(client_fd, SHUT_RDWR);
                fclose(file);
                close(client_fd);
            }
	    }
        else if(strcmp(request, "PUSH\n") == 0){
            char user_filename[36];
            memset(user_filename, 0, 36);
            sprintf(user_filename, "%s.bin", user_name);
            FILE *file = fopen(user_filename, "w+");
            char buf[1024];
            size_t bytes_read = fread(buf, 1, 1024, user_input);
            while(bytes_read){
                fwrite(buf, 1, bytes_read, file);
                memset(buf, 0, 1024);
                bytes_read = read(client_fd, buf, 1024);
                if(bytes_read == -1)break;
            }
            fclose(file);
            shutdown(client_fd, SHUT_RDWR);
            close(client_fd);
        }

	}

}