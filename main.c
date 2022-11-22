#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8081

void manual();

int main(int argc, char* argv[]) {
    int obj_socket;
    struct sockaddr_in address;
    if (argc == 2) {
        if (strcmp(argv[1],"server") == 0) {
            printf("%s","server running");
            int server_fd;

            int opt = 1;
            int addrlen = sizeof(address);


            // Creating socket file descriptor
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            // Forcefully attaching socket to the port
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);

            // Forcefully attaching socket to the port 8081
            if (bind(server_fd, (struct sockaddr*)&address,
                     sizeof(address))
                < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }
            if ((obj_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("%s\n","connected");

        } else {
            manual();
            return 0;
        }
    } else if (argc == 3) {
        if (strcmp(argv[1],"ip") == 0) {
            printf("%s","server running");

            struct sockaddr_in serv_addr;

            if (( obj_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
                printf ( "Socket creation error !" );
                return -1;
            }
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(PORT);

            // Converting IPv4 and IPv6 addresses from text to binary form
            if(inet_pton ( AF_INET, argv[2], &serv_addr.sin_addr) <= 0) {
                printf ( "\nInvalid address ! This IP Address is not supported !\n" );
                return -1;
            }

            if (connect( obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr )) < 0) {
                printf ( "Connection Failed : Can't establish a connection over this socket !" );

                return -1;
            }

        } else {
            manual();
            return 0;
        }

    } else {
        manual();
        return 0;
    }

    pid_t process = fork();
    if(process) {
        char message[256];
        const char* exitMsg = "exit";
        //char *line;
        //size_t len = 0;
        //ssize_t lineSize = 0;
        //free(line);
        do {
            scanf("%s",message);
            if(strcmp(message,exitMsg) == 0) {
                break;
            }
            send(obj_socket, message, strlen(message), 0);
            //free(line);


            printf("\t sent \n");


        } while (1);
        kill(process,SIGKILL);
    } else {
        char buffer[1024] = { 0 };
        do {

            read(obj_socket, buffer, 1024);

            printf("%s\n", buffer);
            for(int i = 0; i < 1024; i++) {
                buffer[i] = 0;
            }
        } while (1);
    }

    return 0;
}

void manual() {
    const char* manual = "";
    printf("%s",manual);
}