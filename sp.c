#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n,station_num,waitcount = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    FILE *input_file;
    FILE *outfile;

    char buffer[256];
    char bufferFile[256];

    if (argc != 4)
    {
        fprintf(stderr,"Usage: ./sp <server name> <port> <station number> \n");
        exit(0);
    }

    portno = atoi(argv[2]);

    //creating a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //station number given as argument while executing
    station_num = atoi(argv[3]);

    if (station_num < 0)
    {
        fprintf(stderr,"ERROR, Input station number\n");
        exit(0);
    }

    //setting server address types for binding
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //connecting to the server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    //Individual log file creation for each SP
    bzero(bufferFile,256);
    sprintf(bufferFile, "Station_Process_Output_SP_%d.txt", station_num);
    outfile = fopen(bufferFile , "w");

    if(outfile == NULL)
	{
		perror("Error opening output file");
		return(-1);
    }

    //open the input file based on the station number
    switch(station_num)
    {
    case 1:
        input_file  = fopen("station_process1.txt", "r");
        break;
    case 2:
        input_file  = fopen("station_process2.txt", "r");
        break;
    case 3:
        input_file  = fopen("station_process3.txt", "r");
        break;
    case 4:
        input_file  = fopen("station_process4.txt", "r");
        break;
    case 5:
        input_file  = fopen("station_process5.txt", "r");
        break;
    case 6:
        input_file  = fopen("station_process6.txt", "r");
        break;
    case 7:
        input_file  = fopen("station_process7.txt", "r");
        break;
    case 8:
        input_file  = fopen("station_process8.txt", "r");
        break;
    case 9:
        input_file  = fopen("station_process9.txt", "r");
        break;
    case 10:
        input_file  = fopen("station_process10.txt", "r");
        break;
    }

    //check if input file is opened without any errors.
    if(input_file == NULL)
    {
        perror("Error opening input file");
        return(-1);
    }

    fd_set rset;

    FD_ZERO(&rset);

    bzero(bufferFile,256);

    while(fgets(bufferFile, sizeof bufferFile, input_file) != NULL)
    {
        FD_SET(sockfd, &rset);

        int frameid, tostation,waitreq;
        char s1[100], s2[100], s3[100], s4[100];
        //Asking permission
        fprintf(outfile,"Send request to CSP to send data %s",bufferFile);

        if(strstr(bufferFile,"Wait") != NULL){
            sscanf(bufferFile, "%s %d", s1, &waitreq);
            waitcount = waitreq;
            bzero(bufferFile,256);
            sprintf(bufferFile, "wait %d %d", waitreq, station_num);

        }else{
            sscanf(bufferFile, "%s %d %s %s %d", s1, &frameid, s2, s3,&tostation);
            bzero(bufferFile,256);
            sprintf(bufferFile, "%d %d %d", frameid, station_num, tostation);
        }

        bzero(buffer,256);
        sprintf(buffer, "%s %d", "permission", station_num);

        //writing to socket
        n = write(sockfd, buffer, strlen(buffer));

        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }


        bzero(buffer,256);
        if((n = read(sockfd, buffer, 255)) > 0)
        {
            if(strstr(buffer,"Accepted") != NULL)
            {
                fprintf(outfile,"Receive positive reply (permission) from CSP to send data frame %d to SP %d \n",frameid,tostation);
                n = write(sockfd, bufferFile, strlen(bufferFile));

                if (n < 0)
                {
                    perror("ERROR writing from socket");
                    exit(1);
                }

                fprintf(outfile,"Send (via CSP) data frame %d to SP %d \n",frameid,tostation);
            }
            else if(strstr(buffer,"rejected") != NULL)
            {
                fprintf(outfile,"Retransmit request to CSP to send data %s \n",bufferFile);

            }
        }

        bzero(buffer,256);
        bzero(bufferFile,256);
        FD_CLR(sockfd, &rset);
    }

    fclose(input_file);
    fclose(outfile);


    close(sockfd);

    return 0;
}
