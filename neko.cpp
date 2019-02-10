#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

using namespace std;

int main(int agrc, char* argv[])
{
    char in_buff, out_buff;
    char * in_buff_p = &in_buff;
    char * out_buff_p = &out_buff;
    pid_t chpid_1, chpid_2;
    int sock_fd, new_sock_fd, port, w=1, r=1, n, stat;
    socklen_t client_addr_s;
    struct sockaddr_in server_addr, client_addr;
    if ((sock_fd=socket(AF_INET, SOCK_STREAM, 0))<0){
        cout << "Opening socket fail\n";
        exit(1);
    }
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    if (strcmp(argv[1],"-l")==0){
        port=atoi(argv[2]);
        server_addr.sin_port=htons(port);
        server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        if ((bind(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)))==-1){
            perror(strerror(errno));
            cout << "Can't bind socket-address\n";
            exit(1);
        }
        listen(sock_fd, 5);
        client_addr_s=sizeof(client_addr);
        if ((new_sock_fd=accept(sock_fd, (struct sockaddr *) &client_addr, &client_addr_s))<0){
            cout << "Can't accept connection";
            exit(1);
        }
    }
    else {
        inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
        port=atoi(argv[2]);
        server_addr.sin_port=htons(port);
        //server_addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
        stat=connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
        if (stat<0){
            cout << "Cant connect\n";
            exit(1);
        }
        new_sock_fd=sock_fd;
    }
    chpid_1=fork();
    if (chpid_1!=0){
        while(w!=0){
            bzero(in_buff_p,1);
            w=recv(new_sock_fd, in_buff_p, 1, 0);
            cout << in_buff;
            //cout << n;
        }
    }
    if (chpid_1==0){
        while(r!=0){
            bzero(out_buff_p,1);
            r=read(0, out_buff_p, 1);
            //n=write(new_sock_fd, out_buff_p, 1);
            n=send(new_sock_fd, out_buff_p, 1, 0);
            if (n<0){
                cout << "Cant write to socket\n";
            }
        }
    }
}
