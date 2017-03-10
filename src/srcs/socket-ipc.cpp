/*
 * Progarm Name: socket-ipc.cpp
 * Created Time: 2017-03-09 08:54:05
 * Last modified: 2017-03-09 08:54:40
 * @author: minphone.linails linails@foxmail.com 
 */

#include "socket-ipc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using std::cout;
using std::endl;

int server_tcp_echo(int argc, char **argv)
{
    printf("------ server-tcp-echo ------\n");

    int serv_sock;
    int client_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

    socklen_t clnt_addr_size;

    char message[512] = "Hello World - echo!";



    printf("\n");
    printf("Print argv[] List :\n{\n");
    for(int i=0; i<argc; i++){
        printf("    argv[%d] : %s\n", i, argv[i]);
    }
    printf("}\n\n");



    if(2 != argc){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    /******************************************************************************/

    /* Step 1 */
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == serv_sock){
        printf("socket() error !!!\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
    serv_addr.sin_port          = htons(atoi(argv[1]));

    /* Step 2 */
    if(-1 == bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))){
        printf("bind() error !!!\n");
        exit(1);
    }

    /* Step 3 */
    if(-1 == listen(serv_sock, 5)){
        printf("listen() error !!!\n");
        exit(1);
    }

    for(int i=0; i<6; i++){
        printf("for i = %d\n", i);
        /* Step 4 */
        clnt_addr_size  = sizeof(client_addr);
        client_sock     = accept(serv_sock, (struct sockaddr*)&client_addr, &clnt_addr_size);
        if(-1 == client_sock){
            printf("accept() error\n");
            exit(1);
        }

        /******************************************************************************/

        printf("Client : %d -> port : %#x\n",
                client_sock, 
                client_addr.sin_port);

        int str_len = 0;
        while(0 != (str_len = read(client_sock, message, sizeof(message)))){
            write(client_sock, message, str_len);
            message[str_len] = '\0';

            printf("Message To client : %s -> client : %d -> port : %#x\n",
                    message, 
                    client_sock, 
                    client_addr.sin_port);
        }

        /* Close all fd */
        close(client_sock);
    }

    close(serv_sock);

    return 0;
}


