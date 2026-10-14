#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include <unistd.h>
#include <signal.h>
#include "list.h"

#define PORT 9001
#define ACK "ACK: "

// Global pointers for graceful shutdown
list_t *mylist = NULL;
int servSockD = -1;
int clientSocket = -1;

void sig_handler(int sig) {
    if (mylist != NULL) {
        list_free(mylist);
        mylist = NULL;
    }
    if (clientSocket >= 0) close(clientSocket);
    if (servSockD >= 0) close(servSockD);
    exit(0);
}

int main(int argc, char const* argv[]) 
{ 
    signal(SIGINT, sig_handler);

    int val, idx;
    char buf[1024];
    char sbuf[1024];
    char* token;

    // create server socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // allow immediate reuse of the port
    int opt = 1;
    setsockopt(servSockD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // define server address 
    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 

    // bind socket to the specified IP and port 
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(1);
    }

    // listen for incoming connections 
    if (listen(servSockD, 1) < 0) {
        perror("Listen failed");
        close(servSockD);
        exit(1);
    }

    // allocate linked list
    mylist = list_alloc();

    while (1) {
        // accept client connection
        clientSocket = accept(servSockD, NULL, NULL); 
        if (clientSocket < 0) {
            continue;
        }

        while (1) {
            memset(buf, '\0', sizeof(buf));
            memset(sbuf, '\0', sizeof(sbuf));

            int bytes_read = recv(clientSocket, buf, sizeof(buf) - 1, 0);
            if (bytes_read <= 0) {
                // Client disconnected
                break;
            }

            // Remove trailing newline if present
            buf[strcspn(buf, "\r\n")] = '\0';

            token = strtok(buf, " ");
            if (token == NULL) {
                continue;
            }

            if (strcmp(token, "exit") == 0) {
                list_free(mylist);
                mylist = NULL;
                close(clientSocket);
                close(servSockD);
                exit(0);
            } 
            else if (strcmp(token, "get_length") == 0) {
                val = list_length(mylist);
                sprintf(sbuf, "Length = %d", val);
            } 
            else if (strcmp(token, "add_front") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    val = atoi(token);
                    list_add_to_front(mylist, val);
                    sprintf(sbuf, "%s%d", ACK, val);
                }
            } 
            else if (strcmp(token, "add_back") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    val = atoi(token);
                    list_add_to_back(mylist, val);
                    sprintf(sbuf, "%s%d", ACK, val);
                }
            } 
            else if (strcmp(token, "add_position") == 0) {
                token = strtok(NULL, " ");
                idx = (token != NULL) ? atoi(token) : -1;
                token = strtok(NULL, " ");
                val = (token != NULL) ? atoi(token) : 0;
                list_add_at_index(mylist, val, idx);
                sprintf(sbuf, "%s%d", ACK, val);
            } 
            else if (strcmp(token, "remove_front") == 0) {
                val = list_remove_from_front(mylist);
                sprintf(sbuf, "%d", val);
            } 
            else if (strcmp(token, "remove_back") == 0) {
                val = list_remove_from_back(mylist);
                sprintf(sbuf, "%d", val);
            } 
            else if (strcmp(token, "remove_position") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    idx = atoi(token);
                    val = list_remove_at_index(mylist, idx);
                    sprintf(sbuf, "%d", val);
                }
            } 
            else if (strcmp(token, "get") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    idx = atoi(token);
                    val = list_get_elem_at(mylist, idx);
                    sprintf(sbuf, "%d", val);
                }
            } 
            else if (strcmp(token, "print") == 0) {
                char *str = listToString(mylist);
                if (str != NULL) {
                    snprintf(sbuf, sizeof(sbuf), "%s", str);
                    free(str);
                }
            }

            // Send response back to the client
            send(clientSocket, sbuf, strlen(sbuf) + 1, 0);
        }

        close(clientSocket);
        clientSocket = -1;
    }

    return 0; 
}