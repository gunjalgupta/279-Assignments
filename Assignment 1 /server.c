// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

#define PORT 80

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    pid_t currentProcess;
    struct passwd* nobodyUser;
    int return_setuid_value;

    

    // Show ASLR
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // valread = read( new_socket , buffer, 1024);
    // printf("%s\n",buffer );
    // send(new_socket , hello , strlen(hello) , 0 );
    // printf("Hello message sent\n");
    // return 0;



    // Splitting reading data from client.sending sata to client and creating the socket

    currentProcess = fork();

    //Check if child process is currently being executed
    if(currentProcess==0){
        printf("%s","Inside Child Process \n");

        printf("Current child: %d \n", getuid());

        // dropping the privilege
        if((nobodyUser=getpwnam("nobody"))==NULL){
            printf("Cannot find nobody user details \n"); 
        }

        //Dropping the privileges of the child to that of Nobody so that it does not have sudo privileges
        return_setuid_value = setuid(nobodyUser->pw_uid);

        //Succesfully dropped privilege
        if(return_setuid_value==0)
        {
            printf("Dropped the child privileges \n");
            printf("After dropping privileges, Child Id: %d \n", getuid());
            valread = read(new_socket, buffer, 1024);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello message sent \n");
        }

        //If privilege drop is unsuccesfull
        else{
            printf("Privilege drop is unsuccesfull. Exiting \n %d ", return_setuid_value);
            return 0;
        }

    }
    //If parent process is still running
    else if(currentProcess>0){
        wait(NULL);
        printf("Parent is still running\n");
    }

    else{
        printf("Forking failed");
    }

    return 0;



//     if(pid==0){
//         printf("%s","Child Process");
//         printf("\n");
//         int res = setuid(65534);
//         printf("\nThe new user id of this process is : ");
//         printf("%d\n",getuid());
//         printf("%d\n",res);
        
//        if (listen(server_fd, 3) < 0)
//         {
//             perror("listen");
//             exit(EXIT_FAILURE);
//         }
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
//                            (socklen_t*)&addrlen))<0)
//         {
//             perror("accept");
//             exit(EXIT_FAILURE);
//         }
//         valread = read( new_socket , buffer, 1024);
//         printf("%s\n",buffer );
//         send(new_socket , hello , strlen(hello) , 0 );
//         printf("Hello message sent\n");
//         return 0;

//     }
//     else
//     {   
//         pid_t mpid = wait(NULL);
//         printf("%d",mpid);
//         printf("\n");
//         printf("%s","Master Process");
//         printf("\n");
//         printf("%d\n", getuid());
       
//     }
 }


