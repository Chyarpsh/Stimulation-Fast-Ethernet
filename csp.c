#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

int clientList[10];

//File pointer to open and close the output file
FILE *outfile;

//subfunction which has main logic which is under loop
void doprocessing (int sock);

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid,i,maxi;

    //if user does not give enough arguments while executing
    if (argc != 2)
    {
        printf("\n Please use : ./csp <port number> \n");
        exit(0);
    }

    //port no which user gives as argument
    portno = atoi(argv[1]);

    outfile = fopen("Communication_Switch_Output.txt", "w");

    if(outfile == NULL)
    {
        perror("Error opening output file");
        return(-1);
    }

    //setting the clientlist to -1 
    for (i = 0; i < 10; i++)
        clientList[i] = -1; 

    //creating a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    //setting server address types for binding
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //binding operation
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    // listening to the socket for starting the message sharing
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    fd_set rset;

    FD_ZERO(&rset);
    maxi = -1;

    while (1)
    {
        FD_SET(sockfd, &rset);

        //waiting for connection from any station
        int nready = select(sockfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset))
        {

            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

            if (newsockfd < 0)
            {
                perror("ERROR on accept");
                exit(1);
            }

             for (i = 0; i < 10; i++){
                if (clientList[i] < 0){
                    clientList[i] = newsockfd; 
                    break;
                }
             }

            if (i == 10)
             printf("too many clients");

            if (i > maxi)
                 maxi = i;

            //spawn a child process to handle the station frame part
            pid = fork();

            if (pid < 0)
            {
                perror("ERROR on fork");
                exit(1);
            }

            if (pid == 0)
            {
                
                close(sockfd);
                for (;;)
                {
                    //main logic
                    doprocessing(newsockfd);
                }

                exit(0);
            }
            else
            {
                close(newsockfd);
            }

        }

    } /* end of while */

    fclose(outfile);
}


void doprocessing (int sock)
{
    int n;
    char buffer[256];

    int fromStation, toStation, frameid,waittime;
    char s1[100];
    char permissionText[20] = "permission";
    char rejectText[20] = "rejected";
    char waitText[20] = "wait";

    //Read from station process
    bzero(buffer,256);
    n = read(sock,buffer,255);
    if (n < 0)
    {
        exit(1);
    }

    if(strstr(buffer,permissionText) != NULL)
    {
        //writing to file and socket for permission
        sscanf(buffer, "%s %d", &s1, &fromStation);
        fprintf(outfile,"Receive request from SP %d \n",fromStation);
        n = write(sock,"Accepted",8);

        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }
        
        fprintf(outfile,"Send positive reply to SP %d \n",fromStation);

    }else if(strstr(buffer,waitText) != NULL){

        sscanf(buffer, "%s %d %d", &s1, &waittime,&fromStation);
        printf("Waiting for SP %d", fromStation);
    }
    else
    {
        sscanf(buffer, "%d %d %d", &frameid, &fromStation,&toStation);
        fprintf(outfile,"Receive data frame from SP %d (to SP %d) \n",fromStation,toStation);
        bzero(buffer,256);
        strncpy(buffer, "Frame Received", 256);
        n = write(sock,buffer,256);

        fprintf(outfile,"Forward data Frame (from SP %d) to SP %d \n",fromStation,toStation);
    }
}
